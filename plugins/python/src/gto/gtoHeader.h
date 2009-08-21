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
PyObject *ObjectInfo_repr( PyObject *_self, PyObject *args );
PyObject *ComponentInfo_init( PyObject *_self, PyObject *args );
PyObject *ComponentInfo_repr( PyObject *_self, PyObject *args );
PyObject *PropertyInfo_init( PyObject *_self, PyObject *args );
PyObject *PropertyInfo_repr( PyObject *_self, PyObject *args );

// *****************************************************************************
// Table of methods available in the ObjectInfo class
static PyMethodDef ObjectInfoMethods[] = 
{
    {"__init__", ObjectInfo_init, METH_VARARGS, 
                "class constructor"},
    {"__repr__", ObjectInfo_repr, METH_VARARGS, NULL },
    {NULL}
};

// *****************************************************************************
// Table of methods available in the ComponentInfo class
static PyMethodDef ComponentInfoMethods[] = 
{
    {"__init__", ComponentInfo_init, METH_VARARGS, 
                "class constructor"},
    {"__repr__", ComponentInfo_repr, METH_VARARGS, NULL },
    {NULL}
};

// *****************************************************************************
// Table of methods available in the PropertyInfo class
static PyMethodDef PropertyInfoMethods[] = 
{
    {"__init__", PropertyInfo_init, METH_VARARGS, 
                "class constructor"},
    {"__repr__", PropertyInfo_repr, METH_VARARGS, NULL },
    {NULL}
};


}  //  End namespace PyGto

#endif    // End #ifdef __GTOHEADER_H__


