//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************
#ifndef __GTOREADER_H__
#define __GTOREADER_H__

#include "gtomodule.h"
#include <Gto/Reader.h>
#include <vector>

namespace PyGto {

static char readerDocString[] = 
"\n"
"The basic theory of operation is similar to how the C++ Gto::Reader class\n"
"works.  In your own code, you would create a class that is derived from\n"
"the gtoReader class defined in this module.  You should implement the\n"
"'object', 'component', 'property', and 'data' methods.  See 'example.py'\n"
"for method definitions, and Gto/Header.h for *info struct definitions.\n"
"\n"
"There are a couple of major differences from the C++ implementation to\n"
"be aware of:\n"
"\n"
"1. The return value for the object, component, and property methods should\n"
"   evaluate to True if you want to read that property, or false/None if\n"
"   you don't.  There is currently no way to return a pointer/object and\n"
"   have it passed back to you when the data function is called.  It doesn't\n"
"   really make sense to do this in Python anyway.\n"
"\n"
"2. Only the dataRead function is implemented, as the data function doesn't\n"
"   make sense in Python.  As each property is read, a python tuple is\n"
"   automatically created and passed to dataRead.  Note that the prototype \n"
"   is slightly different here than in the Gto::Reader class:  I added the \n"
"   'name' argument for convenience sake.\n"
"\n";

typedef Gto::Reader::Request Request;

//
// To use the Gto::Reader, we need to derive a simple class from it:
//
class Reader : public Gto::Reader
{
public:
    Reader( PyObject *callingInstance ) : 
        Gto::Reader(), m_callingInstance( callingInstance ) {}

    virtual Request object( const std::string& name,
                            const std::string& protocol,
                            unsigned int protocolVersion,
                            const Gto::Reader::ObjectInfo &header );

    virtual Request component( const std::string& name,
                               const Gto::Reader::ComponentInfo &header );

    virtual Request property( const std::string& name,
                              const Gto::Reader::PropertyInfo &header );

    virtual void *data( const PropertyInfo &pinfo, size_t bytes );
    virtual void dataRead( const PropertyInfo &pinfo );

private:
    PyObject *buildInfoDict( const PropertyInfo& info );

    // This is a handle to the instance of the Python class
    // (eg. "class myGtoReader( gtoReader ): ...") which is using this
    // instance of the C++ "Reader : public Gto::Reader" class.
    PyObject *m_callingInstance;

    std::vector<float> m_tmpFloatData;
    std::vector<double> m_tmpDoubleData;
    std::vector<int> m_tmpIntData;
    std::vector<unsigned short> m_tmpShortData;
    std::vector<unsigned char> m_tmpCharData;

};

// Function prototypes required for the table below
void gtoReader_PyObject_dealloc( PyObject *self );
PyObject *gtoReader_init( PyObject *_self, PyObject *args );
PyObject *gtoReader_open( PyObject *_self, PyObject *args );
PyObject *gtoReader_object( PyObject *_self, PyObject *args );
PyObject *gtoReader_component( PyObject *_self, PyObject *args );
PyObject *gtoReader_property( PyObject *_self, PyObject *args );
PyObject *gtoReader_data( PyObject *_self, PyObject *args );
PyObject *gtoReader_stringFromId( PyObject *_self, PyObject *args );


// *****************************************************************************
// We need to create a Python object that gets added to 
// <readerInstance>.__dict__ to hold some instance-specific stuff
typedef struct
{
    PyObject_HEAD
    
    // C++ Reader instance to use
    Gto::Reader *m_reader;
    
} gtoReader_PyObject;

static PyTypeObject gtoReader_PyObjectType =
{
    PyObject_HEAD_INIT( &PyType_Type )
    0,                                      // ob_size
    "gtoReaderEngine",                      // tp_name
    sizeof(gtoReader_PyObject),             // tp_basicsize
    0,                                      // tp_itemsize
    PyGto::gtoReader_PyObject_dealloc,      // tp_dealloc
};



// *****************************************************************************
// Table of methods available in the gtoReader base class
static PyMethodDef gtoReaderMethods[] = 
{
    {"__init__", gtoReader_init, METH_VARARGS, 
                "class constructor"},
    {"open", gtoReader_open, METH_VARARGS,
                "open( string filename )"},
    {"object", gtoReader_object, METH_VARARGS,
                "object( string name, string protocol, unsigned int "
                "protocolVersion, ObjectInfo header )"},
    {"component", gtoReader_component, METH_VARARGS,
                "component( string name, ComponentInfo header )"},
    {"property", gtoReader_property, METH_VARARGS,
                "property( string name, PropertyInfo header )"},
    {"data", gtoReader_data, METH_VARARGS,
                "data( string name, int size, int type, "
                "int width, tuple data )"},
    {"stringFromId", gtoReader_stringFromId, METH_VARARGS,
                "stringFromId( int i )"},
    {NULL},
};

}; // End namespace PyGto

#endif    // End #ifdef __GTOREADER_H__
