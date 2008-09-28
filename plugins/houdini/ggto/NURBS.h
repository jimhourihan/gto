//******************************************************************************
// Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __NURBS_H__
#define __NURBS_H__

#include "Object.h"
#include <TwkMath/TwkMathVec3.h>
#include <TwkMath/TwkMathVec2.h>
#include <vector>
#include <GEO/GEO_PrimNURBSurf.h>
#include <GEO/GEO_PointList.h>

namespace HGto {

class NURBS : public Object {
public:
    NURBS( const GEO_PrimNURBSurf *nurbsSrf );
    virtual ~NURBS();

    virtual void writeHeader( Gto::Writer &writer );
    virtual void writeData( Gto::Writer &writer );

    void getVertices( const GEO_PointList &points );
    
    static std::string newName();

protected:
    const GEO_PrimNURBSurf *m_hNurbsSurf;
        
    std::vector<TwkMath::Vec4f> m_pointsPositions;
    TwkMath::Vec2f m_degree;

    std::vector<float> m_uKnots;
    std::vector<float> m_vKnots;

    std::vector<float> m_uRange;
    std::vector<float> m_vRange;

private:
    static int m_nurbsObjNumber;
};


}  //  End namespace HGto

#endif    // End #ifdef __NURBS_H__


