//
//  Copyright (c) 2009, Tweak Software
//  All rights reserved.
// 
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//     * Redistributions of source code must retain the above
//       copyright notice, this list of conditions and the following
//       disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials
//       provided with the distribution.
//
//     * Neither the name of the Tweak Software nor the names of its
//       contributors may be used to endorse or promote products
//       derived from this software without specific prior written
//       permission.
// 
//  THIS SOFTWARE IS PROVIDED BY Tweak Software ''AS IS'' AND ANY EXPRESS
//  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL Tweak Software BE LIABLE FOR
//  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
//  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
//  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
//

#ifndef _RiGtoObject_h_
#define _RiGtoObject_h_

#include <string>
#include <sys/types.h>

namespace RiGto {

class Reader;

// Useful type
enum ReaderPhase
{
    READER_REF,
    READER_OPEN,
    READER_CLOSE
}; 

// An object is a named thing with an open & close transform.
// It provides a virtual method for writing rib as well.
class Object
{
public:
    Object( const std::string &name, 
            const unsigned int protocolVersion,
            Reader *reader );
    virtual ~Object();

    const std::string &name() const { return m_name; }

    //**************************************************************************
    // READER STUFF
    enum
    {
        OBJECT_C = 1,
        NEXT_C
    };

    enum
    {
        OBJECT_GLOBALMATRIX_P = 1,
        NEXT_P
    };
    
    virtual void *component( const std::string &name,
                             ReaderPhase rp ) const;
    
    virtual void *property( const std::string &name,
                            void *componentData,
                            ReaderPhase rp ) const;

    virtual void *data( void *componentData,
                        void *propertyData,
                        size_t numItems,
                        size_t itemWidth,
                        size_t numBytes,
                        ReaderPhase rp );

    virtual void dataRead( void *componentData,
                           void *propertyData,
                           ReaderPhase rp );

    // Done reading everything. Default does nothing.
    virtual void doneReading( ReaderPhase rp );

    //**************************************************************************
    // RENDERMAN OUTPUT
    void declareRi() const;

protected:
    void *transformRefData();
    void *transformOpenData();
    void *transformCloseData();
    
protected:
    // Handle to reader so we can use stringFromId(), etc...
    Reader *m_reader;

    // Derived classes should override this
    // in order to declare their own Ri.
    virtual void internalDeclareRi() const {}
    
    std::string m_name;
    unsigned int m_protocolVersion;
    float *m_transformRef;
    float *m_transformOpen;
    float *m_transformClose;
    int m_numInstances;
};

} // End namespace RiGto

#endif

