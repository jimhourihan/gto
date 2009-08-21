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
"the gto.Reader class defined in this module.  You should implement the\n"
"'object', 'component', 'property', and 'dataRead' methods.\n"
"\n"
"There are a couple of major differences from the C++ implementation to\n"
"be aware of:\n"
"\n"
"1. The return value for the object, component, and property methods should\n"
"   evaluate to True if you want to read that property, or false/None if\n"
"   you don't.  There is no way to return a pointer/object and have it \n"
"   passed back to you when the data function is called.  It doesn't really\n"
"   make sense to do this in Python anyway.\n"
"\n"
"2. Only the dataRead method is implemented, as the data method doesn't\n"
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
    Reader( PyObject *callingInstance, unsigned int mode = None );

    virtual Request object( const std::string &name,
                            const std::string &protocol,
                            unsigned int protocolVersion,
                            const Gto::Reader::ObjectInfo &header );

    virtual Request component( const std::string &name,
                               const std::string &interp,
                               const Gto::Reader::ComponentInfo &header );

    virtual Request property( const std::string &name,
                              const std::string &interp,
                              const Gto::Reader::PropertyInfo &header );

    virtual void *data( const PropertyInfo &pinfo, size_t bytes );
    virtual void dataRead( const PropertyInfo &pinfo );

private:
    // This is a handle to the instance of the Python class
    // (eg. "class myGtoReader( gto.Reader ): ...") which is using this
    // instance of the C++ "Reader : public Gto::Reader" class.
    PyObject *m_callingInstance;

    // Containers for the C++ Gto::Reader class to put data into before it is
    // converted into Python objects
    std::vector<float> m_tmpFloatData;
    std::vector<double> m_tmpDoubleData;
    std::vector<int> m_tmpIntData;
    std::vector<unsigned short> m_tmpShortData;
    std::vector<unsigned char> m_tmpCharData;

};


// *****************************************************************************
// We need to create a Python object that gets added to 
// <readerInstance>.__dict__ to hold some instance-specific stuff
typedef struct
{
    PyObject_HEAD
    
    // C++ Reader instance to use
    Reader *m_reader;
    
    // flags for dumbassness checking
    bool m_isOpen;
    
} gtoReader_PyObject;

void gtoReader_PyObject_dealloc( PyObject *self );
static PyTypeObject gtoReader_PyObjectType =
{
    PyObject_HEAD_INIT( &PyType_Type )
    0,                                      // ob_size
    "gtoReaderEngine",                      // tp_name
    sizeof(gtoReader_PyObject),             // tp_basicsize
    0,                                      // tp_itemsize
    PyGto::gtoReader_PyObject_dealloc,      // tp_dealloc
};


// Function prototypes required for the table below
PyObject *gtoReader_init( PyObject *_self, PyObject *args );
PyObject *gtoReader_open( PyObject *_self, PyObject *args );
PyObject *gtoReader_fail( PyObject *_self, PyObject *args );
PyObject *gtoReader_why( PyObject *_self, PyObject *args );
PyObject *gtoReader_close( PyObject *_self, PyObject *args );
PyObject *gtoReader_object( PyObject *_self, PyObject *args );
PyObject *gtoReader_component( PyObject *_self, PyObject *args );
PyObject *gtoReader_property( PyObject *_self, PyObject *args );
PyObject *gtoReader_dataRead( PyObject *_self, PyObject *args );
PyObject *gtoReader_stringFromId( PyObject *_self, PyObject *args );
PyObject *gtoReader_stringTable( PyObject *_self, PyObject *args );
PyObject *gtoReader_isSwapped( PyObject *_self, PyObject *args );
PyObject *gtoReader_objects( PyObject *_self, PyObject *args );
PyObject *gtoReader_accessObject( PyObject *_self, PyObject *args );
PyObject *gtoReader_components( PyObject *_self, PyObject *args );
PyObject *gtoReader_properties( PyObject *_self, PyObject *args );


// *****************************************************************************
// Table of methods available in the gto.Reader base class
static PyMethodDef gtoReaderMethods[] = 
{
    {"__init__", gtoReader_init, METH_VARARGS, 
                "class constructor"},
    {"open", gtoReader_open, METH_VARARGS,
                "open( string filename )"},
    {"fail", gtoReader_fail, METH_VARARGS,
                "fail( string why )"},
    {"why", gtoReader_why, METH_VARARGS,
                "why()"},
    {"close", gtoReader_close, METH_VARARGS,
                "close()"},
    {"object", gtoReader_object, METH_VARARGS,
                "object( string name, string protocol, unsigned int "
                "protocolVersion, ObjectInfo header )"},
    {"component", gtoReader_component, METH_VARARGS,
                "component( string name, ComponentInfo header )"},
    {"property", gtoReader_property, METH_VARARGS,
                "property( string name, PropertyInfo header )"},
    {"dataRead", gtoReader_dataRead, METH_VARARGS,
                "dataRead( PropertyInfo pinfo )"},
    {"stringFromId", gtoReader_stringFromId, METH_VARARGS,
                "stringFromId( int i )"},
    {"stringTable", gtoReader_stringTable, METH_VARARGS,
                "stringTable()"},
    {"isSwapped", gtoReader_isSwapped, METH_VARARGS,
                "isSwapped()"},
    {"objects", gtoReader_objects, METH_VARARGS,
                "objects()"},
    {"accessObject", gtoReader_accessObject, METH_VARARGS,
                "accessObject( objectInfo )"},
    {"components", gtoReader_components, METH_VARARGS,
                "components()"},
    {"properties", gtoReader_properties, METH_VARARGS,
                "properties()"},

    {NULL},
};

}; // End namespace PyGto

#endif    // End #ifdef __GTOREADER_H__
