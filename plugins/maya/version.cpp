//******************************************************************************
// Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
//******************************************************************************

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
