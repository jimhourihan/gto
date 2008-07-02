//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

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
