//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __GTOINTRANSFORM_H__
#define __GTOINTRANSFORM_H__

#include <maya/MObject.h>
#include "GtoInObject.h"

#include <string>
#include <sys/types.h>

namespace GtoIOPlugin {

class Transform : public Object
{
public:
    Transform( const std::string &name,
               const std::string &protocol,
               const unsigned int protocolVersion );
    virtual ~Transform();
  
    virtual void declareMaya();
};

} // End namespace GtoIOPlugin

#endif    // End #ifdef __GTOINTRANSFORM_H__
