
//******************************************************************************
// Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __VERSION_H__
#define __VERSION_H__

// Check for correct compiler version
#ifndef WIN32
    #if ! ( ( __GNUC__ == 2 )  \
          && ( __GNUC_MINOR__ == 96 ) \
          && ( __GNUC_PATCHLEVEL__ == 0 ) )
    #error ------------------------------------------
    #error This plugin must be compiled with gcc 2.96
    #error ------------------------------------------
    #endif
#else
    // Work around stupid microsoft bugs
    #pragma warning (disable : 4786)
    #define for if(0);else for
    #define snprintf _snprintf
#endif

namespace GtoIOPlugin {

#define GTOIO_MAJOR_VERSION 1
#define GTOIO_MINOR_VERSION 17

const char *versionString();

}  //  End namespace GtoIOPlugin

#endif    // End #ifdef __VERSION_H__

