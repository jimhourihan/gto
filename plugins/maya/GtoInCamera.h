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


#ifndef __GTOINCAMERA_H__
#define __GTOINCAMERA_H__

#include <string>
#include <map>

#include <maya/MObject.h>
#include <maya/MMatrix.h>

#include "GtoInObject.h"


namespace GtoIOPlugin {

class Camera : public Object
{
public:
    Camera( const std::string &name,
            const std::string &protocol,
            const unsigned int protocolVersion );
    virtual ~Camera();

    //**************************************************************************
    // READER STUFF
    enum
    {
        CAMERA_C = Object::NEXT_C,
        FRUSTUM_C
    };

    enum
    {
        CAMERA_SHUTTER_P = Object::NEXT_P,
        CAMERA_BACKGROUND_P,
        FRUSTUM_LEFT_P,
        FRUSTUM_RIGHT_P,
        FRUSTUM_BOTTOM_P,
        FRUSTUM_TOP_P,
        FRUSTUM_NEAR_P,
        FRUSTUM_FAR_P,
        FRUSTUM_ORTHO_P
    };

    virtual Request component( const std::string &name ) const;

    virtual Request property( const std::string &name,
                              void *componentData ) const;

    virtual void *data( const PropertyInfo &pinfo, 
                        size_t bytes,
                        void *componentData,
                        void *propertyData );
  
    virtual void declareMaya();

private:

    float m_shutter;
    float m_background[3];
    float m_frustumLeft;
    float m_frustumRight;
    float m_frustumBottom;
    float m_frustumTop;
    float m_frustumNear;
    float m_frustumFar;
    int m_ortho;       
};

} // End namespace GtoIOPlugin


#endif    // End #ifdef __GTOINCAMERA_H__
