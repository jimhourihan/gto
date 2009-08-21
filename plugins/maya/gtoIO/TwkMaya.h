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

#ifndef __TWKMAYA_H__
#define __TWKMAYA_H__

#include "GtoAnimCurve.h"
#include <maya/MStatus.h>
#include <maya/MVector.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MFnCamera.h>

namespace GtoIOPlugin {

#ifndef TWK_PLUG_NAME
#define TWK_PLUG_NAME "GtoIO Plugin"
#endif

void TwkMayaStatusCheck(MStatus&, const char*, const char*, size_t);
#define CHECK(S) TwkMayaStatusCheck(S, TWK_PLUG_NAME, __FILE__, __LINE__)
#define STATUS CHECK(status)

MVector         GetVectorAttr(MObject node, const char* attr);
char            GetEnumAttr(MObject node, const char* attr);
void convert( const MFnAnimCurve &in, GtoAnimCurve &out );

double degToRad( double d );

double RIBCameraFOV( MFnCamera &cam );

class CameraFrustum
{
public:
    CameraFrustum( double fovy, double aspect,
                   double nearPlane, double farPlane );

    CameraFrustum( double left, double right,
                   double bottom,  double top,
                   double nearPlane, double farPlane,
                   bool ortho = false);
    
    double fovy() const;
    double aspect() const;

    double left() const { return m_left; }
    double right() const { return m_right; }
    double bottom() const { return m_bottom; }
    double top() const { return m_top; }
    double nearPlane() const { return m_near; }
    double farPlane() const { return m_far; }

private:
    double m_left;
    double m_right;
    double m_bottom;
    double m_top;
    double m_near;
    double m_far;
    bool m_ortho;
};

}  //  End namespace GtoIOPlugin


#endif    // End #ifdef __TWKMAYA_H__
