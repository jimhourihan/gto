
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

