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

#ifndef __NURBS_H__
#define __NURBS_H__

#include "Object.h"
#include <UT/UT_Vector2.h>
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
        
    std::vector<UT_Vector4> m_pointsPositions;
    UT_Vector2 m_degree;

    std::vector<float> m_uKnots;
    std::vector<float> m_vKnots;

    std::vector<float> m_uRange;
    std::vector<float> m_vRange;

private:
    static int m_nurbsObjNumber;
};


}  //  End namespace HGto

#endif    // End #ifdef __NURBS_H__


