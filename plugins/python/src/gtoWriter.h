//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __GTOWRITER_H__
#define __GTOWRITER_H__

#include "gtomodule.h"
#include <Gto/Writer.h>

namespace PyGto {

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

// *****************************************************************************
// We need to create a Python object that gets added to 
// <writerInstance>.__dict__ to hold some instance-specific stuff
typedef struct
{
    PyObject_HEAD
    
    // C++ Writer instance to use
    Gto::Writer *m_writer;
    
    // property currently being written
    unsigned int m_propCount;
    bool m_beginDataCalled;
    
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
                "intern()"},
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
