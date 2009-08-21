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

#ifndef _GelGtoObject_h_
#define _GelGtoObject_h_

#include <vector>

#include <sys/types.h>
#include <Gto/Reader.h>

#include "gelatoapi.h"

namespace GelGto {

typedef Gto::Reader::PropertyInfo PropertyInfo;

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
            Gto::Reader *reader );
    virtual ~Object();

    const std::string &name() const { return m_name; }

    //**************************************************************************
    // READER STUFF
    enum
    {
        OBJECT_C = 1,
        MAPPINGS_C,
        CHANNELS_C,
        NEXT_C
    };

    enum
    {
        OBJECT_GLOBALMATRIX_P = 1,
        OBJECT_VISIBILITY_P,
        MAPPINGS_P,
        CHANNELS_P,
        NEXT_P
    };
    
    virtual void *component( const std::string &name,
                             ReaderPhase rp ) const;
    
    virtual void *property( const std::string &name,
                            void *componentData,
                            ReaderPhase rp,
                            const PropertyInfo &pinfo ) const;

    virtual void *data( void *componentData,
                        void *propertyData,
                        size_t numItems,
                        size_t itemWidth,
                        size_t numBytes,
                        ReaderPhase rp,
                        const PropertyInfo &pinfo );

    virtual void dataRead( void *componentData,
                           void *propertyData,
                           ReaderPhase rp,
                           const PropertyInfo &pinfo );

    // Done reading everything. Default does nothing.
    virtual void doneReading( ReaderPhase rp );

    //**************************************************************************
    // RENDERMAN OUTPUT
    void declareRi(GelatoAPI *_rend) const;

protected:
    void *transformRefData();
    void *transformOpenData();
    void *transformCloseData();
    
    void *additionalTokenData( const PropertyInfo &pinfo, size_t numBytes );

    void *visibilityData();
    
protected:
    // Handle to reader so we can use stringFromId(), etc...
    Gto::Reader *m_reader;

    // Derived classes should override this
    // in order to declare their own Ri.
    virtual void internalDeclareRi(GelatoAPI *_rend) const {}
    
    std::string m_name;
    unsigned int m_protocolVersion;
    float *m_transformRef;
    float *m_transformOpen;
    float *m_transformClose;
    int m_numInstances;
    char *m_visibility;
    bool m_nomotion;
    
    float *m_textureCoordinates;
    
    // Used when gathering additional mappings/channels data
    char *m_currentAddtionalData;
    std::vector<char *> m_additionalData;
    std::vector<char *> m_additionalTokens;
    std::vector<char *> m_additionalValuePointers;
};

inline void copyTransposed( const float *srcMat, float *dstMat )
{
    dstMat[0] = srcMat[0];
    dstMat[1] = srcMat[4];
    dstMat[2] = srcMat[8];
    dstMat[3] = srcMat[12];
    dstMat[4] = srcMat[1];
    dstMat[5] = srcMat[5];
    dstMat[6] = srcMat[9];
    dstMat[7] = srcMat[13];
    dstMat[8] = srcMat[2];
    dstMat[9] = srcMat[6];
    dstMat[10] = srcMat[10];
    dstMat[11] = srcMat[14];
    dstMat[12] = srcMat[3];
    dstMat[13] = srcMat[7];
    dstMat[14] = srcMat[11];
    dstMat[15] = srcMat[15];
}


} // End namespace GelGto

#endif
