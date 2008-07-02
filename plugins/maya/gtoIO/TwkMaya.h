//******************************************************************************
// Copyright (c) 2001-2007 Tweak Inc. All rights reserved.
//******************************************************************************

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
