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

#ifndef _GelGtoPoly_h_
#define _GelGtoPoly_h_

#include <GelGto/Object.h>

namespace GelGto {

class Poly : public Object
{
public:
    Poly( const std::string &name, 
          const unsigned int protocolVersion,
          Gto::Reader *reader );
    virtual ~Poly();

    //**************************************************************************
    // READER STUFF
    enum
    {
        POINTS_C = Object::NEXT_C,
        ELEMENTS_C,
        INDICES_C,
        MAPPINGS_C,
        SMOOTHING_C,
        NORMALS_C
    };

    enum
    {
        POINTS_POSITION_P = Object::NEXT_P,
        ELEMENTS_SIZE_P,
        INDICES_VERTEX_P,
        INDICES_ST_P,
        INDICES_NORMAL_P,
        MAPPINGS_ST_P,
        NORMALS_NORMAL_P,
        SMOOTHING_METHOD_P
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

    virtual void doneReading( ReaderPhase rp );

protected:
    void *numVertsData( size_t numVertsSize );
    void *indicesData( size_t indicesSize );
    void *positionsRefData( size_t positionsRefSize );
    void *positionsOpenData( size_t positionsOpenSize );
    void *positionsCloseData( size_t positionsCloseSize );
    void *stValuesData( size_t stvSize );
    void *stIndicesData( size_t stiSize );
    void *normalValuesRefData( size_t normValuesSize );
    void *normalValuesOpenData( size_t normValuesSize );
    void *normalValuesCloseData( size_t normValuesSize );
    void *normalIndicesRefData( size_t normIndicesSize );
    void *normalIndicesOpenData( size_t normIndicesSize );
    void *normalIndicesCloseData( size_t normIndicesSize );
    
protected:
    virtual void internalDeclareRi(GelatoAPI *_rend) const;
    
    unsigned short *m_numVerts;
    int *m_numVertsInt;
    size_t m_numVertsSize;

    int *m_indices;
    size_t m_indicesSize;
    
    float *m_positionsRef;
    float *m_positionsOpen;
    float *m_positionsClose;
    size_t m_positionsSize;

    float *m_stValues;
    size_t m_stValuesSize;
    
    int *m_stIndices;
    size_t m_stIndicesSize;

    int *m_normalIndicesRef;
    int *m_normalIndicesOpen;
    int *m_normalIndicesClose;
    size_t m_normalIndicesSizeRef;
    size_t m_normalIndicesSizeOpen;
    size_t m_normalIndicesSizeClose;

    float *m_normalValuesRef;
    float *m_normalValuesOpen;
    float *m_normalValuesClose;

    size_t m_normalValuesSizeRef;
    size_t m_normalValuesSizeOpen;
    size_t m_normalValuesSizeClose;
    
    int m_smoothingMethodRef;
    int m_smoothingMethodOpen;
    int m_smoothingMethodClose;
};

} // End namespace GelGto

#endif
