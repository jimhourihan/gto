//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __GTOMODULE_H__
#define __GTOMODULE_H__

#include <python2.2/Python.h>

namespace PyGto {

// *****************************************************************************
// Just returns a pointer to the module-wide g_gtoError object
PyObject *gtoError();

}; // End namespace pyGto

#endif    // End #ifdef __GTOMODULE_H__
