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
#include "version.h"

#include <iostream>
#include <maya/MMatrix.h>
#include <maya/MFnTransform.h>
#include <maya/MFnCamera.h>
#include <maya/MPlug.h>

#include "TwkMaya.h"
#include "GtoInCamera.h"

namespace GtoIOPlugin {

using namespace std;

//******************************************************************************
Camera::Camera( const std::string &name, 
                const std::string &protocol,
                const unsigned int protocolVersion )
  : Object( name, protocol, protocolVersion )
{
    m_background[0] = 0.0f;
    m_background[1] = 0.0f;
    m_background[2] = 0.0f;
}

//******************************************************************************
Camera::~Camera()
{
}


//******************************************************************************
Request Camera::component( const std::string &name ) const
{
    if( name == "camera" )
    {
        return Request( true, (void *)CAMERA_C );
    }
    else if( name == "frustum" )
    {
        return Request( true, (void *)FRUSTUM_C );
    }

    // Superclass
    return Object::component( name );
}


//******************************************************************************
Request Camera::property( const std::string &name,
                          void *componentData ) const
{
    if( (( int )componentData) == CAMERA_C )
    {
        if( name == "shutter" )
        {
            return Request( true, ( void * )CAMERA_SHUTTER_P );
        }
        else if( name == "background" )
        {
            return Request( true, ( void * )CAMERA_BACKGROUND_P );
        }
    }
    else if( (( int )componentData ) == FRUSTUM_C )
    {
        if( name == "left" )
        {
            return Request( true, ( void * )FRUSTUM_LEFT_P );
        }
        else if( name == "right" )
        {
            return Request( true, ( void * )FRUSTUM_RIGHT_P );
        }
        else if( name == "bottom" )
        {
            return Request( true, ( void * )FRUSTUM_BOTTOM_P );
        }
        else if( name == "top" )
        {
            return Request( true, ( void * )FRUSTUM_TOP_P );
        }
        else if( name == "near" )
        {
            return Request( true, ( void * )FRUSTUM_NEAR_P );
        }
        else if( name == "far" )
        {
            return Request( true, ( void * )FRUSTUM_FAR_P );
        }
        else if( name == "ortho" )
        {
            return Request( true, ( void * )FRUSTUM_ORTHO_P );
        }
    }

    // Superclass
    return Object::property( name, componentData );
}


// *****************************************************************************
void *Camera::data( const PropertyInfo &pinfo, 
                   size_t bytes,
                   void *componentData,
                   void *propertyData )
{
    if( ( (int)propertyData ) == CAMERA_SHUTTER_P )
    {
        return (void *)&m_shutter;
    }
    else if( (int)propertyData == CAMERA_BACKGROUND_P )
    {
        return (void *)m_background;
    }
    else if( (int)propertyData == FRUSTUM_LEFT_P )
    {
        return (void *)&m_frustumLeft;
    }
    else if( (int)propertyData == FRUSTUM_RIGHT_P )
    {
        return (void *)&m_frustumRight;
    }
    else if( (int)propertyData == FRUSTUM_BOTTOM_P )
    {
        return (void *)&m_frustumBottom;
    }
    else if( (int)propertyData == FRUSTUM_TOP_P )
    {
        return (void *)&m_frustumTop;
    }
    else if( (int)propertyData == FRUSTUM_NEAR_P )
    {
        return (void *)&m_frustumNear;
    }
    else if( (int)propertyData == FRUSTUM_FAR_P )
    {
        return (void *)&m_frustumFar;
    }
    else if( (int)propertyData == FRUSTUM_ORTHO_P )
    {
        return (void *)&m_ortho;
    }

    // Superclass
    return Object::data( pinfo, bytes, componentData, propertyData );    
}


// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
//******************************************************************************
void Camera::declareMaya()
{
    MStatus status;
    MFnCamera camera;

    CameraFrustum frustum( m_frustumLeft, m_frustumRight,
                           m_frustumBottom, m_frustumTop,
                           m_frustumNear, m_frustumFar, m_ortho );

    // camera.create() will create an empty transform node
    // and return it to us
    m_mayaParentObject = camera.create( &status );
    CHECK_MSTATUS( status );
    
    status = camera.setNearClippingPlane( frustum.nearPlane() );
    CHECK_MSTATUS( status );
    status = camera.setFarClippingPlane( frustum.farPlane() );
    CHECK_MSTATUS( status );
    status = camera.setAspectRatio( frustum.aspect() );
    CHECK_MSTATUS( status );
    status = camera.setVerticalFieldOfView( frustum.fovy() );
    CHECK_MSTATUS( status );
    status = camera.setShutterAngle( degToRad( m_shutter ) );
    CHECK_MSTATUS( status );
    status = camera.setFilmFit( MFnCamera::kFillFilmFit );
    CHECK_MSTATUS( status );    

    // Now use that transform node we got earlier to store a handle to
    // the real geometry node.
    MFnDagNode parentDN( m_mayaParentObject );
    m_mayaObject = parentDN.child( 0 );

    MFnDependencyNode dn( m_mayaObject );

    MPlug bgColorPlugR = dn.findPlug( "backgroundColorR", &status );
    CHECK_MSTATUS( status );
    MPlug bgColorPlugG = dn.findPlug( "backgroundColorG", &status );
    CHECK_MSTATUS( status );
    MPlug bgColorPlugB = dn.findPlug( "backgroundColorB", &status );
    CHECK_MSTATUS( status );

    bgColorPlugR.setValue( m_background[0] );
    bgColorPlugG.setValue( m_background[1] );
    bgColorPlugB.setValue( m_background[2] );

    // Set misc stuff...
    setName();
}


} // End namespace GtoIOPlugin
