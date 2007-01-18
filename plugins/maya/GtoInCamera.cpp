//
// Copyright (C) 2003 Tweak Films
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
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
