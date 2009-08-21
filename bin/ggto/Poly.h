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

#ifndef __POLY_H__
#define __POLY_H__

#include "Object.h"
#include <vector>
#include <UT/UT_Vector.h>
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
    
    std::vector<UT_Vector3> &positions() { return m_pointsPositions; }
    std::vector<char> &elementsType() { return m_elementsType; }
    std::vector<short> &elementsSize() { return m_elementsSize; }
    std::vector<int> &indicesVertex() { return m_indicesVertex; }
    int &smoothingMethod() { return m_smoothingMethod; }
    

protected:

    std::vector<UT_Vector3> m_pointsPositions;
    std::vector<char> m_elementsType;
    std::vector<short> m_elementsSize;
    std::vector<int> m_indicesVertex;

    int m_smoothingMethod;
};

}  //  End namespace HGto

#endif    // End #ifdef __POLY_H__


