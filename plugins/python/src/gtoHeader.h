//
// Copyright (C) 2004 Tweak Films
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

#ifndef __GTOHEADER_H__
#define __GTOHEADER_H__
#include "gtomodule.h"
#include <Gto/Reader.h>

// This file defines three Python classes which are used by the Python 
// gto.Reader class to pass around information about Objects, Components, and
// Properties.  They are analagous to the C++ Gto::Reader::ObjectInfo, 
// Gto::Reader::ComponentInfo, and Gto::Reader::PropertyInfo structs.
//
// Conveniently, the __getattr__ method is defined by Python so we don't have
// to worry about it.  Thus, for example, we can refer to an instance of
// PropertyInfo from Python like so:  pinfo.size 


namespace PyGto {

// *****************************************************************************
// C++ Reader helper function prototypes
PyObject *newObjectInfo( Gto::Reader *reader, 
                         const Gto::Reader::ObjectInfo &oi );

PyObject *newComponentInfo( Gto::Reader *reader, 
                            const Gto::Reader::ComponentInfo &ci );

PyObject *newPropertyInfo( Gto::Reader *reader, 
                           const Gto::Reader::PropertyInfo &pi );

// *****************************************************************************
// Python method prototypes

PyObject *ObjectInfo_init( PyObject *_self, PyObject *args );
PyObject *ComponentInfo_init( PyObject *_self, PyObject *args );
PyObject *PropertyInfo_init( PyObject *_self, PyObject *args );

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


