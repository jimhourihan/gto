//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _GelGtoNURBS_h_
#define _GelGtoNURBS_h_

#include <GelGto/Object.h>

namespace GelGto {

class NURBS : public Object
{
public:
    NURBS( const std::string &name, 
           const unsigned int protocolVersion,
           Gto::Reader *reader );
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
        SURFACE_VRANGE_P
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

    virtual void doneReading( ReaderPhase rp );

protected:
    void *degreeData();
    void *knotsUData( size_t knotsUSize );
    void *knotsVData( size_t knotsVSize );
    void *rangeUData();
    void *rangeVData();
    void *weightsData( size_t weightsSize );
    void *positionsRefData( size_t positionsRefSize );
    void *positionsOpenData( size_t positionsOpenSize );
    void *positionsCloseData( size_t positionsCloseSize );

protected:
    virtual void internalDeclareRi(GelatoAPI *_rend) const;

    int m_degreeU;
    int m_degreeV;
    float *m_knotsU;
    size_t m_knotsUSize;
    float *m_knotsV;
    size_t m_knotsVSize;
    float m_minU;
    float m_maxU;
    float m_minV;
    float m_maxV;

    float *m_positionsRef;
    size_t m_positionsRefSize;
    
    float *m_positionsOpen;
    size_t m_positionsOpenSize;
    
    float *m_positionsClose;
    size_t m_positionsCloseSize;

    float *m_weights;
    size_t m_weightsSize;

    float *m_homogPosRef;
    float *m_homogPosOpen;
    float *m_homogPosClose;
};

} // End namespace GelGto

#endif
