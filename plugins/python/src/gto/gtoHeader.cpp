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

#include "gtoHeader.h"

namespace PyGto {
using namespace std;

// *****************************************************************************
//          ObjectInfo Class
// *****************************************************************************

// *****************************************************************************
// Implements ObjectInfo.__init__(self)
PyObject *ObjectInfo_init( PyObject *_self, PyObject *args )
{
    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements ObjectInfo.__repr__(self)
PyObject *ObjectInfo_repr( PyObject *_self, PyObject *args )
{
    PyObject *self;

    if( ! PyArg_ParseTuple( args, "O", &self ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    PyObject *name = PyObject_GetAttrString( self, "name" );
    if( name == NULL )
    {
        PyObject *reprStr = 
                        PyString_FromString( "<INVALID ObjectInfo object>" );
        Py_INCREF( reprStr );
        return reprStr;
    }
    PyObject *reprStr = PyString_FromFormat( "<ObjectInfo: '%s'>", 
                                             PyString_AsString( name ) );
    Py_INCREF( reprStr );
    return reprStr;
}

// *****************************************************************************
PyObject *newObjectInfo( Gto::Reader *reader, 
                         const Gto::Reader::ObjectInfo &oi )
{
    PyObject *module = PyImport_AddModule( "gto" );
    PyObject *moduleDict = PyModule_GetDict( module );
            
    PyObject *classObj = PyDict_GetItemString( moduleDict, "ObjectInfo" );

    PyObject *args = Py_BuildValue( "()" ); // Empty tuple
    PyObject *objInfo = PyInstance_New( classObj, args, NULL );
    
    PyObject_SetAttr( objInfo, 
                      PyString_FromString( "name" ),
                      PyString_FromString( 
                            reader->stringFromId( oi.name ).c_str() ) );

    PyObject_SetAttr( objInfo, 
                      PyString_FromString( "protocolName" ),
                      PyString_FromString( 
                            reader->stringFromId( oi.protocolName ).c_str() ) );

    PyObject_SetAttr( objInfo, 
                      PyString_FromString( "protocolVersion" ),
                      PyInt_FromLong( oi.protocolVersion ) );

    PyObject_SetAttr( objInfo, 
                      PyString_FromString( "numComponents" ),
                      PyInt_FromLong( oi.numComponents ) );

    PyObject_SetAttr( objInfo, 
                      PyString_FromString( "pad" ),
                      PyInt_FromLong( oi.pad ) );

    // Since Gto::Reader::accessObject() __requires__ that the objectInfo
    // reference given to it be from the reader's cache, not just a copy
    // with the same information, we store it here.
    PyObject_SetAttr( objInfo, 
                      PyString_FromString( "__objInfoPtr" ),
                      PyCObject_FromVoidPtr( (void *)&oi, NULL ) );

    Py_INCREF( objInfo );
    return objInfo;
}


// *****************************************************************************
//          ComponentInfo Class
// *****************************************************************************

// *****************************************************************************
// Implements ComponentInfo.__repr__(self)
PyObject *ComponentInfo_init( PyObject *_self, PyObject *args )
{
    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements ComponentInfo.__repr__(self)
PyObject *ComponentInfo_repr( PyObject *_self, PyObject *args )
{
    PyObject *self;

    if( ! PyArg_ParseTuple( args, "O", &self ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    PyObject *name = PyObject_GetAttrString( self, "name" );
    if( name == NULL )
    {
        PyObject *reprStr = 
                        PyString_FromString( "<INVALID ComponentInfo object>" );
        Py_INCREF( reprStr );
        return reprStr;
    }
    PyObject *reprStr = PyString_FromFormat( "<ComponentInfo: '%s'>", 
                                             PyString_AsString( name ) );
    Py_INCREF( reprStr );
    return reprStr;
}

// *****************************************************************************
PyObject *newComponentInfo( Gto::Reader *reader,
                            const Gto::Reader::ComponentInfo &ci )
{
    PyObject *module = PyImport_AddModule( "gto" );
    PyObject *moduleDict = PyModule_GetDict( module );
            
    PyObject *classObj = PyDict_GetItemString( moduleDict, "ComponentInfo" );

    PyObject *args = Py_BuildValue( "()" ); // Empty tuple
    PyObject *compInfo = PyInstance_New( classObj, args, NULL );
    
    PyObject_SetAttr( compInfo, 
                      PyString_FromString( "name" ),
                      PyString_FromString( 
                            reader->stringFromId( ci.name ).c_str() ) );

    PyObject_SetAttr( compInfo, 
                      PyString_FromString( "numProperties" ),
                      PyInt_FromLong( ci.numProperties ) );
    
    PyObject_SetAttr( compInfo, 
                      PyString_FromString( "flags" ),
                      PyInt_FromLong( ci.flags ) );
    
    PyObject_SetAttr( compInfo, 
                      PyString_FromString( "interpretation" ),
                      PyString_FromString( 
                          reader->stringFromId( ci.interpretation ).c_str() ) );

    PyObject_SetAttr( compInfo, 
                      PyString_FromString( "pad" ),
                      PyInt_FromLong( ci.pad ) );

    PyObject_SetAttr( compInfo, 
                      PyString_FromString( "object" ),
                      newObjectInfo( reader, (*ci.object) ) );

    Py_INCREF( compInfo );
    return compInfo;
}


// *****************************************************************************
//          PropertyInfo Class
// *****************************************************************************

// *****************************************************************************
// Implements PropertyInfo.__init__(self)
PyObject *PropertyInfo_init( PyObject *_self, PyObject *args )
{
    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements PropertyInfo.__repr__(self)
PyObject *PropertyInfo_repr( PyObject *_self, PyObject *args )
{
    PyObject *self;

    if( ! PyArg_ParseTuple( args, "O", &self ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    PyObject *name = PyObject_GetAttrString( self, "name" );
    if( name == NULL )
    {
        PyObject *reprStr = 
                         PyString_FromString( "<INVALID PropertyInfo object>" );
        Py_INCREF( reprStr );
        return reprStr;
    }
    PyObject *reprStr = PyString_FromFormat( "<PropertyInfo: '%s'>", 
                                             PyString_AsString( name ) );
    Py_INCREF( reprStr );
    return reprStr;
}

// *****************************************************************************
PyObject *newPropertyInfo( Gto::Reader *reader,
                           const Gto::Reader::PropertyInfo &pi )
{
    PyObject *module = PyImport_AddModule( "gto" );
    PyObject *moduleDict = PyModule_GetDict( module );
            
    PyObject *classObj = PyDict_GetItemString( moduleDict, "PropertyInfo" );

    PyObject *args = Py_BuildValue( "()" ); // Empty tuple
    PyObject *propInfo = PyInstance_New( classObj, args, NULL );
    
    PyObject_SetAttr( propInfo, 
                      PyString_FromString( "name" ),
                      PyString_FromString( 
                            reader->stringFromId( pi.name ).c_str() ) );

    PyObject_SetAttr( propInfo, 
                      PyString_FromString( "size" ),
                      PyInt_FromLong( pi.size ) );
    
    PyObject_SetAttr( propInfo, 
                      PyString_FromString( "type" ),
                      PyInt_FromLong( pi.type ) );
    
    PyObject_SetAttr( propInfo, 
                      PyString_FromString( "width" ),
                      PyInt_FromLong( pi.width ) );

    PyObject_SetAttr( propInfo, 
                      PyString_FromString( "interpretation" ),
                      PyString_FromString( 
                          reader->stringFromId( pi.interpretation ).c_str() ) );

    PyObject_SetAttr( propInfo, 
                      PyString_FromString( "pad" ),
                      PyInt_FromLong( pi.pad ) );

    PyObject_SetAttr( propInfo, 
                      PyString_FromString( "component" ),
                      newComponentInfo( reader, (*pi.component) ) );

    Py_INCREF( propInfo );
    return propInfo;
}


}  //  End namespace PyGto
