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

//
// This is the gto module for Python 2.2.1.  It may or may not work with other
// versions.
//

#include "gtomodule.h"
#include "gtoHeader.h"
#include "gtoReader.h"
#include "gtoWriter.h"

namespace PyGto {

// A python exception object
static PyObject *g_gtoError = NULL;

// *****************************************************************************
// Just returns a pointer to the module-wide g_gtoError object
PyObject *gtoError()
{
    return g_gtoError;
}

// *****************************************************************************
// Returns the Python type name of an object as a string
const char *PyTypeName( PyObject *object )
{
    // Figure out the class name (as a string)
    PyObject *itemClass = PyObject_GetAttr( object,
                                PyString_FromString( "__class__" ) );
    assert( itemClass != NULL );
    PyObject *itemClassName = PyObject_GetAttr( itemClass,
                            PyString_FromString( "__name__" ) );
    assert( itemClassName != NULL );

    return PyString_AsString( itemClassName );
}

} // End namespace PyGto


// This module has no module-scope methods
static PyMethodDef ModuleMethods[] = 
{
    { NULL }
};

// *****************************************************************************
// Adds a class to the module dictionary, and return the classDef
static PyObject *defineClass( PyObject *moduleDict, 
                              char *classNameStr, 
                              PyMethodDef *classMethods, 
                              char *docString = NULL )
{
    PyObject *classDict = NULL;
    PyObject *className = NULL;
    PyObject *classDef = NULL;

    classDict = PyDict_New();
    className = PyString_FromString( classNameStr );

    if( docString )
    {
        PyDict_SetItemString( classDict, "__doc__", 
                 PyString_FromString( docString ) );
    }

    // Add methods to the class
    for( PyMethodDef *def = classMethods;
         def->ml_name != NULL; 
         def++ )
    {
        PyObject *func = PyCFunction_New( def, NULL );
        PyObject *method = PyMethod_New( func, NULL, classDef );
        PyDict_SetItemString( classDict, def->ml_name, method );
        Py_DECREF( func );
        Py_DECREF( method );
    }

    classDef = PyClass_New( NULL, classDict, className );
    
    assert( classDict != NULL );
    assert( className != NULL );
    assert( classDef != NULL );
    
    PyDict_SetItemString( moduleDict, classNameStr, classDef );
    Py_DECREF( classDict );
    Py_DECREF( className );
    
    return classDef;
}

// *****************************************************************************
static void defineConstants( PyObject *moduleDict )
{
    PyDict_SetItemString( moduleDict, "__doc__", 
        PyString_FromString( "gto I/O module  v3.01\n"
                             "(c) 2003 Tweak Films\n"
                             "Compiled on "
                             __DATE__ " at " __TIME__ ) );

    PyDict_SetItemString( moduleDict, "TRANSPOSED", 
        PyInt_FromLong( Gto::Transposed ) );

    PyDict_SetItemString( moduleDict, "MATRIX", 
        PyInt_FromLong( Gto::Matrix ) );

    PyDict_SetItemString( moduleDict, "INT", 
        PyInt_FromLong( Gto::Int ) );

    PyDict_SetItemString( moduleDict, "FLOAT", 
        PyInt_FromLong( Gto::Float ) );

    PyDict_SetItemString( moduleDict, "DOUBLE", 
        PyInt_FromLong( Gto::Double ) );

    PyDict_SetItemString( moduleDict, "HALF", 
        PyInt_FromLong( Gto::Half ) );

    PyDict_SetItemString( moduleDict, "STRING", 
        PyInt_FromLong( Gto::String ) );

    PyDict_SetItemString( moduleDict, "BOOLEAN", 
        PyInt_FromLong( Gto::Boolean ) );

    PyDict_SetItemString( moduleDict, "SHORT", 
        PyInt_FromLong( Gto::Short ) );

    PyDict_SetItemString( moduleDict, "BYTE", 
        PyInt_FromLong( Gto::Byte ) );

    PyDict_SetItemString( moduleDict, "GTO_VERSION",
        PyInt_FromLong( GTO_VERSION ) );
}

// *****************************************************************************
// This function is called by Python when the module is imported
extern "C" void initgto()
{
    // Create a new gto module object
    PyObject *module = Py_InitModule( "gto", ModuleMethods );
    PyObject *moduleDict = PyModule_GetDict( module );
    
    // Create the exception and add it to the module
    PyGto::g_gtoError = PyErr_NewException( "gto.Error", NULL, NULL );
    PyDict_SetItemString( moduleDict, "Error", PyGto::g_gtoError );

    // Add 'constants' to the module
    defineConstants( moduleDict );

    // Create info classes
    defineClass( moduleDict, "ObjectInfo", PyGto::ObjectInfoMethods );
    defineClass( moduleDict, "ComponentInfo", PyGto::ComponentInfoMethods );
    defineClass( moduleDict, "PropertyInfo", PyGto::PropertyInfoMethods );

    // Create the Reader class
    PyObject *readerClass = defineClass( moduleDict, 
                                         "Reader", 
                                         PyGto::gtoReaderMethods,
                                         PyGto::readerDocString );
    PyClassObject *readerClassObj = (PyClassObject *)( readerClass );

    // Add a couple of Reader-specific constants
    PyDict_SetItemString( readerClassObj->cl_dict, "NONE", 
                          PyInt_FromLong( Gto::Reader::None ) );
    PyDict_SetItemString( readerClassObj->cl_dict, "HEADERONLY", 
                          PyInt_FromLong( Gto::Reader::HeaderOnly ) );
    PyDict_SetItemString( readerClassObj->cl_dict, "RANDOMACCESS", 
                          PyInt_FromLong( Gto::Reader::RandomAccess ) );
    PyDict_SetItemString( readerClassObj->cl_dict, "BINARYONLY", 
                          PyInt_FromLong( Gto::Reader::BinaryOnly ) );
    PyDict_SetItemString( readerClassObj->cl_dict, "TEXTONLY", 
                          PyInt_FromLong( Gto::Reader::TextOnly ) );


    // Create the Writer class
    PyObject *writerClass = defineClass( moduleDict, 
                                         "Writer", 
                                         PyGto::gtoWriterMethods,
                                         PyGto::writerDocString );
    PyClassObject *writerClassObj = (PyClassObject *)( writerClass );

    // Add a couple of Writer-specific constants
    PyDict_SetItemString( writerClassObj->cl_dict, "BINARYGTO", 
                          PyInt_FromLong( Gto::Writer::BinaryGTO ) );
    PyDict_SetItemString( writerClassObj->cl_dict, "COMPRESSEDGTO", 
                          PyInt_FromLong( Gto::Writer::CompressedGTO ) );
    PyDict_SetItemString( writerClassObj->cl_dict, "TEXTGTO", 
                          PyInt_FromLong( Gto::Writer::TextGTO ) );

}
