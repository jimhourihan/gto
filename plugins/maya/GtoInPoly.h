//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __GTOINPOLY_H__
#define __GTOINPOLY_H__

#include <maya/MObject.h>
#include "GtoInObject.h"

namespace GtoIOPlugin {

typedef Gto::Reader::Request Request;
typedef Gto::Reader::StringTable StringTable;

class Poly : public Object
{
public:
    Poly( const std::string &name, 
          const std::string &protocol, 
          const unsigned int protocolVersion );
    virtual ~Poly();

 
    //**************************************************************************
    // READER STUFF
    enum Poly_Components
    {
        POINTS_C = Object::NEXT_C,
        ELEMENTS_C,
        INDICES_C,
        MAPPINGS_C,
        NORMALS_C
    };

    enum Poly_Properties
    {
        POINTS_POSITION_P = Object::NEXT_P,
        ELEMENTS_SIZE_P,
        INDICES_VERTEX_P,
        INDICES_ST_P,
        INDICES_NORMAL_P,
        MAPPINGS_ST_P,
        NORMALS_NORMAL_P
    };

    virtual Request component( const std::string &name ) const;

    virtual Request property( const std::string &name,
                              void *componentData ) const;

    virtual void *data( const PropertyInfo &pinfo, 
                        size_t bytes,
                        void *componentData,
                        void *propertyData );

    virtual void dataRead( const PropertyInfo &pinfo,
                           void *componentData,
                           void *propertyData,
                           const StringTable &strings );

    virtual void declareMaya();

protected:
    void setNumVerts( const unsigned short *numVerts, size_t numVertsSize );
    void setIndices( const int *indices, size_t indicesSize );
    void setPositionsRef( const float *positionsRef, size_t positionsRefSize );

    void setStValues( const float *stValues, size_t stValuesSize );
    void setStIndices( const int *stIndices, size_t stIndicesSize );

    void setNormalValues( const float *normValues, size_t normValuesSize );
    void setNormalIndices( const int *normIndices, size_t normIndicesSize );

protected:

    int *m_numVerts;
    size_t m_numVertsSize;

    int *m_indices;
    size_t m_indicesSize;
    
    float *m_positionsRef;
    size_t m_positionsSize;

    float *m_stValues;
    size_t m_stValuesSize;
    
    int *m_stIndices;
    size_t m_stIndicesSize;

    int *m_normalIndices;
    size_t m_normalIndicesSize;

    float *m_normalValues;
    size_t m_normalValuesSize;
    
    std::vector<unsigned short> m_tmpShortData;
};

} // End namespace GtoIOPlugin

#endif    // End #ifdef __GTOINPOLY_H__
