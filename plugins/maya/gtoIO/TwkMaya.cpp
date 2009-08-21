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

#include <math.h>

#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>

#include "TwkMaya.h"

namespace GtoIOPlugin {

// *****************************************************************************
void TwkMayaStatusCheck(MStatus& status, 
                        const char* plug,
                        const char* file, 
                        size_t line)
{
    if (!status)
    {
        fprintf(stderr, "FAIL: %s, %s:%d %s\n", 
                plug, file, line,
                status.errorString().asChar());
    }
}


// *****************************************************************************
MVector
GetVectorAttr(MObject node, const char* attr)
{
    MStatus status;
    MFnDependencyNode dn( node, &status );                          STATUS;
    MPlug plug = dn.findPlug( attr, true &status );                 STATUS;

    double x = 0;
    double y = 0;
    double z = 0;
    if( plug.isCompound() )
    {
        MPlug plug_x = plug.child(0);
        MPlug plug_y = plug.child(1);
        MPlug plug_z = plug.child(2);

        plug_x.getValue( x );
        plug_y.getValue( y );
        plug_z.getValue( z );
    }
    
    return MVector( x, y, z );
}


// *****************************************************************************
char
GetEnumAttr(MObject node, const char* attr)
{
    MStatus status;
    MFnDependencyNode dn( node, &status );                          STATUS;
    MPlug plug = dn.findPlug( attr, true &status );                 STATUS;
    
    char value = 0;
    status = plug.getValue( value );                                STATUS;
    
    return value;
}


// *****************************************************************************
void convert( const MFnAnimCurve &in, GtoAnimCurve &out )
{
    MStatus status;

    out.name( in.name().asChar() );

    GtoAnimCurve::Flags flags = GtoAnimCurve::FLAG_NONE;
    if( in.isWeighted() ) flags = (GtoAnimCurve::Flags)(flags | GtoAnimCurve::FLAG_WEIGHTED);
    if( in.isStatic() ) flags = (GtoAnimCurve::Flags)(flags | GtoAnimCurve::FLAG_STATIC);
    out.flags( flags );

    out.preInfinity( (GtoAnimCurve::InfinityType)in.preInfinityType() );
    out.postInfinity( (GtoAnimCurve::InfinityType)in.postInfinityType() );

    GtoAnimCurve::Keys &keys = out.keys();
    for( size_t i = 0; i < in.numKeys(); ++i )
    {
        GtoAnimCurve::Key k;
       
        k.time = in.time( i, &status ).as( MTime::kSeconds );       STATUS;
        k.value = in.value( i, &status );                           STATUS;

        float tx;
        float ty;
        status = in.getTangent( i, tx, ty, true );                  STATUS;
        k.inTanX = tx;
        k.inTanY = ty;

        status = in.getTangent( i, tx, ty, false );                 STATUS;
        k.outTanX = tx;
        k.outTanY = ty;

        keys.push_back( k );
    }
}

#ifndef MM_TO_INCH
#define MM_TO_INCH 0.03937
#endif

// *****************************************************************************
static void computeViewingFrustum( double window_aspect,
                                   double &left,
                                   double &right,
                                   double &bottom,
                                   double &top,
                                   MFnCamera &cam )
{
    double film_aspect = cam.aspectRatio();
    double aperture_x = cam.horizontalFilmAperture();
    double aperture_y = cam.verticalFilmAperture();
    double offset_x = cam.horizontalFilmOffset();
    double offset_y = cam.verticalFilmOffset();
    double focal_to_near = cam.nearClippingPlane()
                           / (cam.focalLength() * MM_TO_INCH);

    focal_to_near *= cam.cameraScale();

    double scale_x = 1.0;
    double scale_y = 1.0;
    double translate_x = 0.0;
    double translate_y = 0.0;

    switch( cam.filmFit() )
    {
    case MFnCamera::kFillFilmFit:
        if( window_aspect < film_aspect )
        {
            scale_x = window_aspect / film_aspect;
        }
        else
        {
            scale_y = film_aspect / window_aspect;
        }
        break;

    case MFnCamera::kHorizontalFilmFit:
        scale_y = film_aspect / window_aspect;
        if( scale_y > 1.0 )
        {
            translate_y = cam.filmFitOffset()
                          * ( aperture_y - (aperture_y * scale_y) ) / 2.0;
        }
        break;

    case MFnCamera::kVerticalFilmFit:
        scale_x = window_aspect / film_aspect;
        if( scale_x > 1.0 )
        {
            translate_x = cam.filmFitOffset()
                          * ( aperture_x - (aperture_x * scale_x) ) / 2.0;
        }
        break;

    case MFnCamera::kOverscanFilmFit:
        if( window_aspect < film_aspect )
        {
            scale_y = film_aspect / window_aspect;
        }
        else
        {
            scale_x = window_aspect / film_aspect;
        }
        break;

    case MFnCamera::kInvalid:
        break; // handle all cases to avoid compiler warnings
    }

    left   = focal_to_near * (-.5*aperture_x*scale_x+offset_x+translate_x);
    right  = focal_to_near * ( .5*aperture_x*scale_x+offset_x+translate_x);
    bottom = focal_to_near * (-.5*aperture_y*scale_y+offset_y+translate_y);
    top    = focal_to_near * ( .5*aperture_y*scale_y+offset_y+translate_y);
}


// *****************************************************************************
static void portFieldOfView( int port_width, int port_height,
                             double &horizontal,
                             double &vertical,
                             MFnCamera &fnCamera )
{
    double left, right, bottom, top;
    double aspect = (double)port_width / (double)port_height;
    computeViewingFrustum( aspect, left, right, bottom, top, fnCamera );

    double neardb = fnCamera.nearClippingPlane();
    horizontal = atan( ((right - left) * 0.5) / neardb ) * 2.0;
    vertical = atan( ((top - bottom) * 0.5) / neardb ) * 2.0;
}


// *****************************************************************************
static double getCameraInfo( MFnCamera &cam, int width, int height,
                             bool ignoreFilmGate )
{
    double fov_ratio = 1.0;

    // Resoultion can change if camera film-gate clips image
    // so we must keep camera width/height separate from render
    // globals width/height.
    //
    int cam_width = width;
    int cam_height = height;

    // If we are using a film-gate then we may need to
    // adjust the resolution to simulate the 'letter-boxed'
    // effect.
    //
    if( cam.filmFit() == MFnCamera::kHorizontalFilmFit )
    {
        if( ! ignoreFilmGate )
        {
            int new_height = (int)( cam_width
                                    / ( cam.horizontalFilmAperture()
                                        / cam.verticalFilmAperture() ) );
            if( new_height < cam_height )
            {
                cam_height = new_height;
            }
        }

        double hfov = 0.0;
        double vfov = 0.0;
        portFieldOfView( cam_width, cam_height, hfov, vfov, cam );
        fov_ratio = hfov / vfov;
    }
    else if( cam.filmFit() == MFnCamera::kVerticalFilmFit )
    {
        int new_width = (int)( cam_height
                               / ( cam.verticalFilmAperture()
                                   / cam.horizontalFilmAperture() ) );
        double hfov = 0.0;
        double vfov = 0.0;
        // case 1 : film-gate smaller than resolution
        //          film-gate on
        if( (new_width < cam_width) && (!ignoreFilmGate) )
        {
            cam_width = new_width;
            fov_ratio = 1.0;
        }

        // case 2 : film-gate smaller than resolution
        //          film-gate off
        else if( (new_width < cam_width) && (ignoreFilmGate) )
        {
            portFieldOfView( new_width, cam_height, hfov, vfov, cam );
            fov_ratio = hfov / vfov;
        }

        // case 3 : film-gate larger than resolution
        //          film-gate on
        else if( !ignoreFilmGate )
        {
            portFieldOfView( new_width, cam_height, hfov, vfov, cam );
            fov_ratio = hfov / vfov;
        }

        // case 4 : film-gate larger than resolution
        //          film-gate off
        else if( ignoreFilmGate )
        {
            portFieldOfView( new_width, cam_height, hfov, vfov, cam );
            fov_ratio = hfov / vfov;
        }
    }
    else if( cam.filmFit() == MFnCamera::kOverscanFilmFit ) 
    {
        int new_height = (int) (cam_width /
            (cam.horizontalFilmAperture() /
             cam.verticalFilmAperture()));
        int new_width = (int) (cam_height /
            (cam.verticalFilmAperture() /
             cam.horizontalFilmAperture()));

        if( new_width < cam_width )
        {
            if( !ignoreFilmGate )
            {
                cam_width = new_width;
                fov_ratio = 1.0;
            }
            else
            {
                double hfov, vfov;
                portFieldOfView( new_width, cam_height, hfov, vfov, cam );
                fov_ratio = hfov/vfov;
            }
        }
        else
        {
            if( !ignoreFilmGate )
            {
                cam_height = new_height;
            }

            double hfov, vfov;
            portFieldOfView( cam_width, cam_height, hfov, vfov, cam );
            fov_ratio = hfov/vfov;
        }
    }
    else if( cam.filmFit() == MFnCamera::kFillFilmFit )
    {
        int new_width = (int) (cam_height /
            (cam.verticalFilmAperture() /
             cam.horizontalFilmAperture()));
        double hfov, vfov;

        if( new_width >= cam_width )
        {
            portFieldOfView( new_width, cam_height, hfov, vfov, cam );
            fov_ratio = hfov / vfov;
        }
        else
        {
            portFieldOfView( cam_width, cam_height, hfov, vfov, cam );
            fov_ratio = hfov / vfov;
        }
    }
    return fov_ratio;
}

// *****************************************************************************
double RIBCameraFOV( MFnCamera &cam )
{
    MStatus status;
    int width = 0;
    int height = 0;
    bool ignoreFilmGate = false;

    // Get the render globals node
    //
    MSelectionList renderGlobalsList;
    renderGlobalsList.add( "defaultRenderGlobals" );
    MObject renderGlobalsNode;
    if( renderGlobalsList.length() <= 0 ) abort(); // TODO: Better error check

    renderGlobalsList.getDependNode( 0, renderGlobalsNode );
    MFnDependencyNode fnRenderGlobals( renderGlobalsNode );

    // Find the resolution node and get the width and height
    //
    MPlugArray connectedPlugs;
    MPlug resPlug = fnRenderGlobals.findPlug( "resolution" );
    resPlug.connectedTo( connectedPlugs,
                         true,  // asDestination
                         false, // asSource
                         &status );

    // Must be length 1 or we would have fan-in
    //
    if ( status && connectedPlugs.length() == 1 ) {
        MObject resNode = connectedPlugs[0].node( &status );
        if ( status ) {
            MFnDependencyNode fnRes( resNode );
            MPlug resWidth = fnRes.findPlug( "width" );
            MPlug resHeight = fnRes.findPlug( "height" );
            short res_width, res_height;
            resWidth.getValue( res_width );
            resHeight.getValue( res_height );
            width = (uint)res_width;
            height = (uint)res_height;
        }
    }
    MPlug filmGatePlug = fnRenderGlobals.findPlug( "ignoreFilmGate" );
    filmGatePlug.getValue( ignoreFilmGate );

    double fov_ratio = getCameraInfo( cam, width, height, ignoreFilmGate );

    return cam.horizontalFieldOfView() / fov_ratio;
}

// *****************************************************************************
CameraFrustum::CameraFrustum( double fovy, double aspect,
                              double nearPlane, double farPlane )
{
    m_top = nearPlane * tan( fovy / 2.0 );
    m_bottom = -m_top;
    m_right = (m_top - m_bottom) * aspect / 2.0;
    m_left = -m_right;
    m_near = nearPlane;
    m_far = farPlane;
    m_ortho = false;
}

// *****************************************************************************
CameraFrustum::CameraFrustum( double left, double right,
                              double bottom,  double top,
                              double nearPlane, double farPlane,
                              bool ortho )
{
    m_left   = left;
    m_right  = right;
    m_bottom = bottom;
    m_top    = top;
    m_near   = nearPlane;
    m_far    = farPlane;
    m_ortho  = ortho;
}

// *****************************************************************************
double CameraFrustum::fovy() const
{
    return atan2(m_top, m_near) - atan2(m_bottom, m_near);
}

// *****************************************************************************
double CameraFrustum::aspect() const
{
    double w = m_right - m_left;
    double h = m_top - m_bottom;

    return w != 0.0 ? (w/h) : 0.0;
}


// *****************************************************************************
double degToRad( double d )
{
    return d * 0.0174532925199433; // PI / 180
    
}

}  //  End namespace GtoIOPlugin
