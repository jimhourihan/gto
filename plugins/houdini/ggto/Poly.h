//******************************************************************************
// Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __POLY_H__
#define __POLY_H__

#include "Object.h"
#include <TwkMath/TwkMathVec3.h>
#include <vector>
#include <GU/GU_Detail.h>
#include <GEO/GEO_PrimPoly.h>
#include <GEO/GEO_PointList.h>

namespace HGto {

class Poly : public Object {
public:
    Poly( std::string name );
    virtual ~Poly();

    virtual void writeHeader( Gto::Writer &writer );
    virtual void writeData( Gto::Writer &writer );

    void addFace( const GEO_PrimPoly *prim );
    
    void getVertices( const GEO_PointList &points );
    
    void declareHoudini( GU_Detail &gdp );
    
    std::vector<TwkMath::Vec3f> &positions() { return m_pointsPositions; }
    std::vector<char> &elementsType() { return m_elementsType; }
    std::vector<short> &elementsSize() { return m_elementsSize; }
    std::vector<int> &indicesVertex() { return m_indicesVertex; }
    int &smoothingMethod() { return m_smoothingMethod; }
    

protected:
    std::vector<TwkMath::Vec3f> m_pointsPositions;
    std::vector<char> m_elementsType;
    std::vector<short> m_elementsSize;
    std::vector<int> m_indicesVertex;

    int m_smoothingMethod;
};

}  //  End namespace HGto

#endif    // End #ifdef __POLY_H__


