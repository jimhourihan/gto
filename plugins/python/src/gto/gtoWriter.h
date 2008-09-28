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
