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
#include <stdio.h>

#ifndef __USER__
#define __USER__ ""
#endif

namespace GtoIOPlugin {

const char *versionString()
{
    static char versionString[128];
    snprintf( versionString, 128, 
              "(GtoIO for Maya, Version: %d.%d   %s %s    %s)", 
                GTOIO_MAJOR_VERSION, GTOIO_MINOR_VERSION, 
                __DATE__, __TIME__, __USER__ );    
    return versionString;    
}

}  //  End namespace GtoIOPlugin
