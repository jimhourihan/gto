//
// Copyright (C) 2004 Tweak Films
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
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

