//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _GtoInNURBS_h_
#define _GtoInNURBS_h_

#include <maya/MObject.h>
#include "GtoInObject.h"
#include <Gto/Reader.h>

namespace GtoIOPlugin {

typedef Gto::Reader::Request Request;
typedef Gto::Reader::StringTable StringTable;

class NURBS : public Object
{
public:
    NURBS( const std::string &name, 
           const std::string &protocol, 
           const unsigned int protocolVersion );
    virtual ~NURBS();

    //**************************************************************************
    // READER STUFF
    enum
    {
        POINTS_C = Object::NEXT_C,
        SURFACE_C
    };

    enum
    {
        POINTS_POSITION_P = Object::NEXT_P,
        POINTS_WEIGHT_P,
        SURFACE_DEGREE_P,
        SURFACE_UKNOTS_P,
        SURFACE_VKNOTS_P,
        SURFACE_URANGE_P,
        SURFACE_VRANGE_P,
        SURFACE_UFORM_P,
        SURFACE_VFORM_P
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
    void setWeights( const float *weights, size_t weightsSize );
    void setPositionsRef( const float *positionsRef,
                          size_t positionsRefSize );

protected:
    int m_degree[2];       // U, V
    int m_form[2];         // U, V
    float m_Urange[2];     // min, max
    float m_Vrange[2];     // min, max
    float *m_knotsU;
    size_t m_knotsUSize;
    float *m_knotsV;
    size_t m_knotsVSize;

    float *m_positionsRef;
    size_t m_positionsSize;

    std::vector<float> m_tmpFloatData;
};

} // End namespace GtoIOPlugin

#endif
