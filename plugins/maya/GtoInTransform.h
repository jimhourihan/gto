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
