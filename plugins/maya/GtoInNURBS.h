//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _GtoInNURBS_h_
#define _GtoInNURBS_h_

#include <maya/MObject.h>
#include "GtoInObject.h"

namespace GtoIOPlugin {

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

    virtual void *component( const std::string &name ) const;

    virtual void *property( const std::string &name,
                            void *componentData ) const;

    virtual void data( void *componentData,
                       void *propertyData,
                       const float *items,
                       size_t numItems,
                       size_t width);
    
    virtual void data( void *componentData,
                       void *propertyData,
                       const int *items,
                       size_t numItems,
                       size_t width);

    virtual void declareMaya();

protected:
    void setDegree( int degreeU, int degreeV );
    void setKnotsU( const float *knotsU, size_t knotsUSize );
    void setKnotsV( const float *knotsV, size_t knotsVSize );
    void setRangeU( float minU, float maxU );
    void setRangeV( float minV, float maxV );
    void setFormU( int formU );
    void setFormV( int formV );
    void setWeights( const float *weights, size_t weightsSize );
    void setPositionsRef( const float *positionsRef,
                          size_t positionsRefSize );

protected:
    int m_degreeU;
    int m_degreeV;
    int m_formU;
    int m_formV;
    float *m_knotsU;
    size_t m_knotsUSize;
    float *m_knotsV;
    size_t m_knotsVSize;
    float m_minU;
    float m_maxU;
    float m_minV;
    float m_maxV;

    float *m_positionsRef;
    size_t m_positionsSize;
};

} // End namespace GtoIOPlugin

#endif
