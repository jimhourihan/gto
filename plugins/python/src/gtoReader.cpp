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

#include "gtoReader.h"
#include "gtoHeader.h"
#include <TwkGeom/GTOReader.h>
#include <python2.2/Python.h>
#include <vector>

namespace PyGto {
using namespace std;

// *****************************************************************************
// We start with a few utility functions...
// *****************************************************************************

// *****************************************************************************
// Returns the C++ Gto::Reader instance if a file is open, otherwise NULL
Reader *readerIfOpen( PyInstanceObject *self )
{
    assert( self != NULL );

    gtoReader_PyObject *reader =
            (gtoReader_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__readerEngine" );
    if( ! reader )
    {
        PyErr_SetString( gtoError(), "Fatal internal error: no readerEngine!" );
        return NULL;
    }
    if( ! reader->m_reader )
    {
        PyErr_SetString( gtoError(), "Fatal internal error: "
                                     "no Gto::Reader instance!" );
        return NULL;
    }
    if( ! reader->m_isOpen )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }

    return reader->m_reader;
}

// *****************************************************************************
// Properly deallocate the instance-specific stuff-holder object
void gtoReader_PyObject_dealloc( PyObject *self )
{
    assert( self != NULL );
    gtoReader_PyObject *grSelf = (gtoReader_PyObject *)self;
    if( grSelf->m_reader )
    {
        delete grSelf->m_reader;
    }
    PyObject_DEL( self );
}

// *****************************************************************************
// Converts from gto's enum-based types to their string representation
static const char *typeAsString( int type )
{
    switch( type )
    {
    case Gto::Int: return "int";
    case Gto::Float: return "float";
    case Gto::Double: return "double";
    case Gto::Half: return "half";
    case Gto::String: return "string";
    case Gto::Boolean: return "bool";
    case Gto::Short: return "short";
    case Gto::Byte: return "byte";
    }
    return "unknown";
}



// *****************************************************************************
// The next several functions implement the methods on our derived C++ 
// Gto::Reader class.  They get called by Gto::Reader::open(), and their main 
// purpose is to convert their arguments into Python objects and then call 
// their equivalent methods on the Python gto.Reader class.
// *****************************************************************************

// *****************************************************************************
Reader::Reader( PyObject *callingInstance, unsigned int mode ) :
    m_callingInstance( callingInstance ),
    Gto::Reader::Reader( mode )
{
    // Nothing
}

// *****************************************************************************
Request Reader::object( const std::string &name,
                        const std::string &protocol,
                        unsigned int protocolVersion,
                        const Gto::Reader::ObjectInfo &info )
{
    assert( m_callingInstance != NULL );

    // Build the Python equivalent of the Gto::ObjectInfo struct
    PyObject *oi = newObjectInfo( this, info );

    PyObject *returnValue = NULL;
    returnValue = PyObject_CallMethod( m_callingInstance, "object",
                                       "ssiO", name.c_str(),                
                                              protocol.c_str(),             
                                              protocolVersion,              
                                              oi );         
    
    if( returnValue == NULL )
    {
        // Invalid parameters, stop the reader and let Python do a stack trace
        fail();
        return Request( false );
    }
    
    if( PyObject_IsTrue( returnValue ) )
    {
        Py_INCREF( returnValue );
        return Request( true, (void*)returnValue );
    }
    return Request( false );
}

// *****************************************************************************
Request Reader::component( const std::string &name,
                           const std::string &interp,
                           const Gto::Reader::ComponentInfo &info)
{
    assert( m_callingInstance != NULL );

    // Build the Python equivalent of the Gto::ComponentInfo struct
    PyObject *ci = newComponentInfo( this, info );

    PyObject *returnValue = NULL;
    // Try calling the component method
    returnValue = PyObject_CallMethod( m_callingInstance, 
                                       "component",
                                       "ssO", name.c_str(),
                                              interp.c_str(),
                                              ci );

    if( returnValue == NULL )
    {
        // Invalid parameters, stop the reader and let Python do a stack trace
        fail();
        return Request( false );
    }

    if( PyObject_IsTrue( returnValue ) )
    {
        Py_INCREF( returnValue );
        return Request( true, (void*)returnValue );
    }
    return Request( false );
}


// *****************************************************************************
Request Reader::property( const std::string &name,
                          const std::string &interp,
                          const Gto::Reader::PropertyInfo &info )
{
    assert( m_callingInstance != NULL );

    // Build the Python equivalent of the Gto::PropertyInfo struct
    PyObject *pi = newPropertyInfo( this, info );

    PyObject *returnValue = NULL;
    // Try calling the property method with the propInfo tuple
    returnValue = PyObject_CallMethod( m_callingInstance, 
                                       "property",
                                       "ssO", name.c_str(),
                                              interp.c_str(),
                                              pi );

    if( returnValue == NULL )
    {
        // Invalid parameters, stop the reader and let Python do a stack trace
        fail();
        return Request( false );
    }

    if( PyObject_IsTrue( returnValue ) )
    {
        Py_INCREF( returnValue );
        return Request( true, (void*)returnValue );
    }
    return Request( false, NULL );
}

// *****************************************************************************
// Note that this method does not call any overloaded Python method, it
// is here simply to allocate space for Gto::Reader.
void *Reader::data( const PropertyInfo &pinfo, size_t bytes )
{
    assert( m_callingInstance != NULL );
    switch( pinfo.type )
    {
    case Gto::Int:
    case Gto::String:
        m_tmpIntData.resize( pinfo.size * pinfo.width );
        return (void *)&m_tmpIntData.front();
    case Gto::Float:
        m_tmpFloatData.resize( pinfo.size * pinfo.width );
        return (void *)&m_tmpFloatData.front();
    case Gto::Double:
        m_tmpDoubleData.resize( pinfo.size * pinfo.width );
        return (void *)&m_tmpDoubleData.front();
    case Gto::Byte:
        m_tmpCharData.resize( pinfo.size * pinfo.width );
        return (void *)&m_tmpCharData.front();
    case Gto::Short:
        m_tmpShortData.resize( pinfo.size * pinfo.width );
        return (void *)&m_tmpShortData.front();
    default:
        PyErr_Format( gtoError(), "Unsupported data type: %s",
                      typeAsString( pinfo.type ) );
        fail();
    }
    return NULL;
}

// *****************************************************************************
void Reader::dataRead( const PropertyInfo &pinfo )
{
    assert( m_callingInstance != NULL );

    // Build a tuple out of the data for this property
    PyObject *dataTuple = PyTuple_New( pinfo.size );
    for( size_t i = 0; i < pinfo.size; ++i )
    {
        if( pinfo.width == 1 )
        {
            switch( pinfo.type )
            {
            case Gto::String:
                PyTuple_SetItem( dataTuple, i, 
                                 PyString_FromString( 
                                    stringFromId( m_tmpIntData[i] ).c_str() ) );
                break;
            case Gto::Int:
                PyTuple_SetItem( dataTuple, i, 
                                 PyInt_FromLong( m_tmpIntData[i] ) );
                break;
            case Gto::Float:
                PyTuple_SetItem( dataTuple, i,
                                 PyFloat_FromDouble( m_tmpFloatData[i] ) ); 
                break;
            case Gto::Double:
                PyTuple_SetItem( dataTuple, i,
                                 PyFloat_FromDouble( m_tmpDoubleData[i] ) ); 
                break;
            case Gto::Byte:
                PyTuple_SetItem( dataTuple, i, 
                                 PyInt_FromLong( m_tmpCharData[i] ) ); 
                break;
            case Gto::Short:
                PyTuple_SetItem( dataTuple, i, 
                                 PyInt_FromLong( m_tmpShortData[i] ) ); 
                break;
            }
        }
        else
        {
            PyObject *subTuple = PyTuple_New( pinfo.width );
            for( size_t w = 0; w < pinfo.width; w++ )
            {
                size_t di = ( i * pinfo.width ) + w;
                switch( pinfo.type )
                {
                case Gto::String:
                    PyTuple_SetItem( subTuple, w, 
                                     PyString_FromString( 
                                        stringFromId( m_tmpIntData[di] ).c_str() ) );
                    break;
                case Gto::Int:
                    PyTuple_SetItem( subTuple, w, 
                                     PyInt_FromLong( m_tmpIntData[di] ) );
                    break;
                case Gto::Float:
                    PyTuple_SetItem( subTuple, w,
                                     PyFloat_FromDouble( m_tmpFloatData[di] ) ); 
                    break;
                case Gto::Double:
                    PyTuple_SetItem( subTuple, w,
                                     PyFloat_FromDouble( m_tmpDoubleData[di] ) ); 
                    break;
                case Gto::Byte:
                    PyTuple_SetItem( subTuple, w, 
                                     PyInt_FromLong( m_tmpCharData[di] ) ); 
                    break;
                case Gto::Short:
                    PyTuple_SetItem( subTuple, w, 
                                     PyInt_FromLong( m_tmpShortData[di] ) ); 
                    break;
                }
            }
            PyTuple_SetItem( dataTuple, i, subTuple );
            Py_INCREF( subTuple );
        }
    }
    Py_INCREF( dataTuple );

    // Clear any temporary space we used
    m_tmpFloatData.clear();
    m_tmpFloatData.clear();
    m_tmpDoubleData.clear();
    m_tmpIntData.clear();
    m_tmpShortData.clear();
    m_tmpCharData.clear();

    assert( dataTuple != NULL );

    // Build the Python equivalent of the Gto::PropertyInfo struct
    PyObject *pi = newPropertyInfo( this, pinfo );

    PyObject *returnValue = NULL;
    returnValue = PyObject_CallMethod( m_callingInstance, "dataRead",
                                 "sOO", 
                                 stringFromId( pinfo.name ).c_str(),
                                 dataTuple,
                                 pi );
    // The data method was not properly overridden...
    if( returnValue == NULL )
    {
        // Invalid parameters, stop the reader and let Python do a stack trace
        fail();
    }
}

// *****************************************************************************
// The rest of this file is dedicated to creating and implementing the Python
// gto.Reader class and its methods.  Note that in each of gto's methods, the 
// first argument is PyObject *_self.  For some reason unbeknownst to me, this
// pointer is always NULL.  The _actual_ self pointer is the first argument
// in the PyObject *args tuple.  Go figure.
// *****************************************************************************

// *****************************************************************************
// Implements the gto.Reader( [mode] ) constructor
PyObject *gtoReader_init( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self = NULL;
    int mode = Gto::Reader::None;

    if( ! PyArg_ParseTuple( args, "O|i:gtoReader_init", &self, &mode ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    // Create a new Python object to hold the pointer to the C++ reader
    // instance and add it to this Python instance's dictionary
    gtoReader_PyObject *reader = PyObject_NEW( gtoReader_PyObject,
                                               &gtoReader_PyObjectType );

    assert( reader != NULL );    
    PyDict_SetItemString( self->in_dict, "__readerEngine", (PyObject *)reader );


    reader->m_reader = new Reader( (PyObject *)self, mode );
    if( ! reader->m_reader  )
    {
        PyErr_Format( gtoError(), "Unable to create instance of Gto::Reader.  "
                                  "Bad parameters?" );
        return NULL;
    }
    
    reader->m_isOpen = false;

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements gto.Reader.open( filename )
PyObject *gtoReader_open( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;
    char *filename;
    if( ! PyArg_ParseTuple( args, "Os:gtoReader_open", &self, &filename ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    gtoReader_PyObject *reader =
            (gtoReader_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__readerEngine" );
    if( reader == NULL )
    {
        PyErr_Format( gtoError(),
         "The open() method was called before the constructor.  If your\n"
         "           derived Reader class has an __init__ method, you need to\n"
         "           call gto.Reader.__init__() at the end of it." );
        return NULL;
    }

    // We set isOpen _before_ calling open, because it is the open method that
    // calls all our other crap.  If we don't do it now, we don't get another
    // chance until it's all over.
    reader->m_isOpen = true;
    if( ! reader->m_reader->open( filename ) )
    {
        // Something went wrong.  If the error was in the Python world, 
        // the error message should already be set.  If not, there was a
        // problem in the C++ world, so set the error message now.
        if( ! PyErr_Occurred() )
        {
            PyErr_Format( gtoError(), "Unable to open %s: %s", filename,
                                      reader->m_reader->why().c_str() );
        }
        reader->m_isOpen = false;
        return NULL;
    }

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements gto.Reader.close()
PyObject *gtoReader_close( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;
    if( ! PyArg_ParseTuple( args, "O:gtoReader_close", &self ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    gtoReader_PyObject *reader =
            (gtoReader_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__readerEngine" );
    if( reader == NULL 
        || reader->m_reader == NULL 
        || reader->m_isOpen == false )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }

    reader->m_reader->close();
    reader->m_isOpen = false;

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements a default 
//      gto.Reader.object( name, protocol, protocolVersion, ObjectInfo )
PyObject *gtoReader_object( PyObject *_self, PyObject *args )
{
    PyObject *self = NULL;
    char *name;
    char *protocol;
    int protocolVersion;
    PyObject *objInfo;

    if( ! PyArg_ParseTuple( args, "OssiO:gtoReader_object", 
                            &self, &name, &protocol, &protocolVersion,
                            &objInfo ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

#ifdef DEBUG
    cout << "object \"" << name << "\" protocol \""  << protocol << "\"";
    cout << endl;
#endif

    // Assume we want all the objects in the file
    return PyInt_FromLong( 1 );
}

// *****************************************************************************
// Implements a default gto.Reader.component( name, interp, ComponentInfo )
PyObject *gtoReader_component( PyObject *_self, PyObject *args )
{
    PyObject *self;
    char *name;
    char *interp;
    PyObject *compInfo;

    if( ! PyArg_ParseTuple( args, "OssO:gtoReader_component", 
                            &self, &name, &interp, &compInfo ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

#ifdef DEBUG
    cout << "\tcomponent \"" << name << "\"" << endl;
#endif

    // Assume we want all the components in the file
    return PyInt_FromLong( 1 );
}

// *****************************************************************************
// Implements a default gto.Reader.property( name, interp, PropertyInfo )
PyObject *gtoReader_property( PyObject *_self, PyObject *args )
{
    PyObject *self;
    char *name;
    char *interp;
    PyObject *propInfo;

    if( ! PyArg_ParseTuple( args, "OssO:gtoReader_property", 
                            &self, &name, &interp, &propInfo ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

#ifdef DEBUG
    cout << "\t\tproperty " << name << endl;
#endif
         
    // Assume we want all the properties in the file
    return PyInt_FromLong( 1 );
}

// *****************************************************************************
// Implements a default gto.Reader.dataRead( propertyInfo )
PyObject *gtoReader_dataRead( PyObject *_self, PyObject *args )
{
    PyObject *self;
    char *name;
    PyObject *dataTuple;
    PyObject *propInfo;

    if( ! PyArg_ParseTuple( args, "OsOO:gtoReader_dataRead", 
                            &self, &name, &dataTuple, &propInfo ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

#ifdef DEBUG
    cout << "data " << name << endl;
#endif
     
    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
PyObject *gtoReader_stringFromId( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;
    int id;

    if( ! PyArg_ParseTuple( args, "Oi:gtoReader_stringFromId", 
                            &self, &id ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    Reader *reader = readerIfOpen( self );
    if( reader == NULL )
    {
        return NULL;
    }

    PyObject *str = PyString_FromString( reader->stringFromId( id ).c_str() );

    Py_INCREF( str );
    return str;
}


} // End namespace PyGto

