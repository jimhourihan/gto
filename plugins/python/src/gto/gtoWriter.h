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

#ifndef __GTOWRITER_H__
#define __GTOWRITER_H__

#include "gtomodule.h"
#include <Gto/Writer.h>
#include <string>
#include <vector>

namespace PyGto {

static char writerDocString[] = 
"\n"
"The Writer class is designed as an API to a state machine. You indicate\n"
"a conceptual hierarchy to the file and then all the data. The writer\n"
"handles generating the string table, the header information, etc.\n"
"\n";


// *****************************************************************************
// Function prototypes

void gtoWriter_PyObject_dealloc( PyObject *self );
PyObject *gtoWriter_init( PyObject *_self, PyObject *args );
PyObject *gtoWriter_open( PyObject *_self, PyObject *args );
PyObject *gtoWriter_close( PyObject *_self, PyObject *args );
PyObject *gtoWriter_beginObject( PyObject *_self, PyObject *args );
PyObject *gtoWriter_endObject( PyObject *_self, PyObject *args );
PyObject *gtoWriter_beginComponent( PyObject *_self, PyObject *args );
PyObject *gtoWriter_endComponent( PyObject *_self, PyObject *args );
PyObject *gtoWriter_property( PyObject *_self, PyObject *args );
PyObject *gtoWriter_beginData( PyObject *_self, PyObject *args );
PyObject *gtoWriter_endData( PyObject *_self, PyObject *args );
PyObject *gtoWriter_propertyData( PyObject *_self, PyObject *args );
PyObject *gtoWriter_intern( PyObject *_self, PyObject *args );
PyObject *gtoWriter_lookup( PyObject *_self, PyObject *args );

// *****************************************************************************
// We need to create a Python object that gets added to 
// <writerInstance>.__dict__ to hold some instance-specific stuff
typedef struct
{
    PyObject_HEAD
    
    // C++ Writer instance to use
    Gto::Writer *m_writer;
    
    // property currently being written
    int m_propCount;
    
    // For graceful sanity checking...
    bool m_beginDataCalled;
    bool m_objectDef;
    bool m_componentDef;
    std::vector<std::string> *m_propertyNames;

} gtoWriter_PyObject;

static PyTypeObject gtoWriter_PyObjectType =
{
    PyObject_HEAD_INIT( &PyType_Type )
    0,                                      // ob_size
    "gtoWriterEngine",                      // tp_name
    sizeof(gtoWriter_PyObject),             // tp_basicsize
    0,                                      // tp_itemsize
    PyGto::gtoWriter_PyObject_dealloc,      // tp_dealloc
};

// *****************************************************************************
// Table of methods available in the gtoWriter class
static PyMethodDef gtoWriterMethods[] = 
{
    {"__init__", gtoWriter_init, METH_VARARGS, 
                "class constructor"},
    {"open", gtoWriter_open, METH_VARARGS,
                "open( filename )"},
    {"close", gtoWriter_close, METH_VARARGS,
                "close()"},
    {"beginObject", gtoWriter_beginObject, METH_VARARGS,
                "beginObject( string name, string protocol )"},
    {"endObject", gtoWriter_endObject, METH_VARARGS,
                "endObject()"},
    {"beginComponent", gtoWriter_beginComponent, METH_VARARGS,
                "beginComponent( string name, transposed = false )"},
    {"endComponent", gtoWriter_endComponent, METH_VARARGS,
                "endComponent()"},
    {"property", gtoWriter_property, METH_VARARGS,
                "property( string name, int type, int numElements,"
                " int partsPerElement  )"},
    {"intern", gtoWriter_intern, METH_VARARGS,
                "intern(string)"},
    {"lookup", gtoWriter_lookup, METH_VARARGS,
                "lookup(string)"},
    {"beginData", gtoWriter_beginData, METH_VARARGS,
                "beginData()"},
    {"propertyData", gtoWriter_propertyData, METH_VARARGS,
                "propertyData( tuple data )"},
    {"endData", gtoWriter_endData, METH_VARARGS,
                "endData()"},
    {NULL}
};

// *****************************************************************************
// Since Python can't convert directly to int...
inline int PyInt_AsInt( PyObject *p )
{
    return (int)PyInt_AsLong( p );
}

// *****************************************************************************
// Since Python can't convert directly to float...
inline float PyFloat_AsFloat( PyObject *p )
{
    return (float)PyFloat_AsDouble( p );
}

// *****************************************************************************
// Since Python can't convert directly to short...
inline unsigned short PyInt_AsShort( PyObject *p )
{
    return (unsigned short)PyInt_AsLong( p );
}

// *****************************************************************************
// Since Python can't convert directly to char...
inline unsigned char PyInt_AsByte( PyObject *p )
{
    return (unsigned char)PyInt_AsLong( p );
}

}; // End namespace PyGto


#endif    // End #ifdef __GTOWRITER_H__
