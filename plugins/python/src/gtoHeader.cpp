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

#include "gtoHeader.h"

namespace PyGto {
using namespace std;

// *****************************************************************************
//          ObjectInfo Class
// *****************************************************************************

// *****************************************************************************
PyObject *ObjectInfo_init( PyObject *_self, PyObject *args )
{
    Py_INCREF( Py_None );
    return Py_None;
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

    Py_INCREF( objInfo );
    return objInfo;
}


// *****************************************************************************
//          ComponentInfo Class
// *****************************************************************************

// *****************************************************************************
PyObject *ComponentInfo_init( PyObject *_self, PyObject *args )
{
    Py_INCREF( Py_None );
    return Py_None;
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
PyObject *PropertyInfo_init( PyObject *_self, PyObject *args )
{
    Py_INCREF( Py_None );
    return Py_None;
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
