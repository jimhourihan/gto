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

//
// This is the gto module for Python 2.2.1.  It may or may not work with other
// versions.  This module defines two class: gto.Reader and gto.Writer
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
//     Py_DECREF( classDef );
    
    return classDef;
}

// *****************************************************************************
static void defineConstants( PyObject *moduleDict )
{
    PyDict_SetItemString( moduleDict, "__doc__", 
        PyString_FromString( "gto I/O module  v3.00\n"
                             "(c) 2003 Tweak Films\n"
                             "Compiled on "
                             __DATE__ " at " __TIME__ ) );

    PyDict_SetItemString( moduleDict, "Transposed", 
        PyInt_FromLong( Gto::Transposed ) );

    PyDict_SetItemString( moduleDict, "Matrix", 
        PyInt_FromLong( Gto::Matrix ) );

    PyDict_SetItemString( moduleDict, "Int", 
        PyInt_FromLong( Gto::Int ) );

    PyDict_SetItemString( moduleDict, "Float", 
        PyInt_FromLong( Gto::Float ) );

    PyDict_SetItemString( moduleDict, "Double", 
        PyInt_FromLong( Gto::Double ) );

    PyDict_SetItemString( moduleDict, "Half", 
        PyInt_FromLong( Gto::Half ) );

    PyDict_SetItemString( moduleDict, "String", 
        PyInt_FromLong( Gto::String ) );

    PyDict_SetItemString( moduleDict, "Boolean", 
        PyInt_FromLong( Gto::Boolean ) );

    PyDict_SetItemString( moduleDict, "Short", 
        PyInt_FromLong( Gto::Short ) );

    PyDict_SetItemString( moduleDict, "Byte", 
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
    PyGto::g_gtoError = PyErr_NewException( "gto.error", NULL, NULL );
    PyDict_SetItemString( moduleDict, "error", PyGto::g_gtoError );

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
    PyDict_SetItemString( readerClassObj->cl_dict, "None", 
                          PyInt_FromLong( Gto::Reader::None ) );
    PyDict_SetItemString( readerClassObj->cl_dict, "HeaderOnly", 
                          PyInt_FromLong( Gto::Reader::HeaderOnly ) );
    PyDict_SetItemString( readerClassObj->cl_dict, "RandomAccess", 
                          PyInt_FromLong( Gto::Reader::RandomAccess ) );
    

    // Create the Writer class
    defineClass( moduleDict, "Writer", PyGto::gtoWriterMethods );
}

