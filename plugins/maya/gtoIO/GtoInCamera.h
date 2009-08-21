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
