//******************************************************************************
// Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __GTOHEADER_H__
#define __GTOHEADER_H__

#include <Python.h>
#include <Gto/Reader.h>
#include "gtomodule.h"

namespace PyGto {

// *****************************************************************************
// Function prototypes

PyObject *ObjectInfo_init( PyObject *_self, PyObject *args );
PyObject *ComponentInfo_init( PyObject *_self, PyObject *args );
PyObject *PropertyInfo_init( PyObject *_self, PyObject *args );

PyObject *newObjectInfo( const Gto::Reader::ObjectInfo &oi );
PyObject *newComponentInfo( const Gto::Reader::ComponentInfo &ci );
PyObject *newPropertyInfo( const Gto::Reader::PropertyInfo &pi );

// *****************************************************************************
// Table of methods available in the ObjectInfo class
static PyMethodDef ObjectInfoMethods[] = 
{
    {"__init__", ObjectInfo_init, METH_VARARGS, 
                "class constructor"},
    {NULL}
};

// *****************************************************************************
// Table of methods available in the ComponentInfo class
static PyMethodDef ComponentInfoMethods[] = 
{
    {"__init__", ComponentInfo_init, METH_VARARGS, 
                "class constructor"},
    {NULL}
};

// *****************************************************************************
// Table of methods available in the PropertyInfo class
static PyMethodDef PropertyInfoMethods[] = 
{
    {"__init__", PropertyInfo_init, METH_VARARGS, 
                "class constructor"},
    {NULL}
};


}  //  End namespace PyGto

#endif    // End #ifdef __GTOHEADER_H__


