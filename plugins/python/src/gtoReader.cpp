//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#include "gtoReader.h"
#include "gtoHeader.h"
#include <Python.h>
#include <vector>

namespace PyGto {
using namespace std;

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
    case Gto::Int:
        return "int";
        break;
    case Gto::Float:
        return "float";
        break;
    case Gto::Double:
        return "double";
        break;
    case Gto::Half:
        return "half";
        break;
    case Gto::String:
        return "string";
        break;
    case Gto::Boolean:
        return "bool";
        break;
    case Gto::Short:
        return "short";
        break;
    case Gto::Byte:
        return "byte";
        break;
    }
    return "unknown";
}

// *****************************************************************************
// The next several functions implement the methods on our derived Reader 
// class.  They get called by Gto::Reader::open(), and their main purpose is 
// to convert their arguments into Python objects and then call their 
// equivalent methods on the Python gtoReader class.
// *****************************************************************************

// *****************************************************************************
Request Reader::object( const std::string &name,
                        const std::string &protocol,
                        unsigned int protocolVersion,
                        const Gto::Reader::ObjectInfo &info )
{
    assert( m_callingInstance != NULL );

    // Build the Python equivalent of the Gto::ObjectInfo struct
    PyObject *oi = newObjectInfo( info );

    PyObject *returnValue = NULL;
    returnValue = PyObject_CallMethod( m_callingInstance, "object",
                                       "ssiO", name.c_str(),                
                                              protocol.c_str(),             
                                              protocolVersion,              
                                              oi );         
    
    if( returnValue == NULL )
    {
        // Invalid parameters, let Python do a stack trace
        PyErr_Print();
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
                           const Gto::Reader::ComponentInfo &info)
{
    assert( m_callingInstance != NULL );

    // Build the Python equivalent of the Gto::ComponentInfo struct
    PyObject *ci = newComponentInfo( info );

    PyObject *returnValue = NULL;
    // Try calling the component method 
    returnValue = PyObject_CallMethod( m_callingInstance, 
                                       "component",                   
                                       "sO", name.c_str(),           
                                             ci );

    // If attempt failed, the component method wasn't properly overridden
    if( returnValue == NULL )
    {
        // Invalid parameters, let Python do a stack trace
        PyErr_Print();
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
                          const Gto::Reader::PropertyInfo &info )
{
    assert( m_callingInstance != NULL );

    // Build the Python equivalent of the Gto::PropertyInfo struct
    PyObject *pi = newPropertyInfo( info );

    PyObject *returnValue = NULL;
    // Try calling the property method with the propInfo tuple
    returnValue = PyObject_CallMethod( m_callingInstance, 
                                       "property",
                                       "sO", name.c_str(),
                                             pi );

    // If attempt failed, the property method was not properly overridden
    if( returnValue == NULL )
    {
        // Invalid parameters, let Python do a stack trace
        PyErr_Print();
        return Request( false, NULL );
    }

    if( PyObject_IsTrue( returnValue ) )
    {
        Py_INCREF( returnValue );
        return Request( true, (void*)returnValue );
    }
    return Request( false, NULL );
}

// *****************************************************************************
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
    PyObject *pi = newPropertyInfo( pinfo );

    PyObject *returnValue = NULL;
    returnValue = PyObject_CallMethod( m_callingInstance, "dataRead",
                                 "sOO", 
                                 stringFromId( pinfo.name ).c_str(),
                                 dataTuple,
                                 pi );

    // The data method was not properly overridden...
    if( returnValue == NULL )
    {
        // Invalid parameters, let Python do a stack trace
        PyErr_Print();
    }
}

// *****************************************************************************
// The rest of this file is dedicated to creating and implementing the Python
// gtoReader class and its methods.  Note that in each of gto's methods, the 
// first argument is PyObject *_self.  For some reason unbeknownst to me, this
// pointer is always NULL.  The _actual_ self pointer is the first argument
// in the PyObject *args tuple.  Go figure.
// *****************************************************************************

// *****************************************************************************
// Class constructor. Does nothing, but is required anyway
PyObject *gtoReader_init( PyObject *_self, PyObject *args )
{
    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements gtoReader.open( filename )
PyObject *gtoReader_open( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;
    char *filename;
    if( ! PyArg_ParseTuple( args, "Os:gtoReader_open", &self, &filename ) )
    {
        // Invalid parameters, let Python do a stack trace
        PyErr_Print();
        return NULL;
    }

    // Create a new python reader object and add it to this instance's
    // dictionary
    gtoReader_PyObject *reader = PyObject_NEW( gtoReader_PyObject,
                                               &gtoReader_PyObjectType );
    PyDict_SetItemString( self->in_dict, "__reader", (PyObject *)reader );


    reader->m_reader = new Reader( (PyObject *)self );

    if( ! reader->m_reader->open( filename ) )
    {
        PyErr_Format( gtoError(), "Unable to open specified file: %s",
                      filename );
        return NULL;
    }

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements a default 
//      gtoReader.object( name, protocol, protocolVersion, numComponents )
PyObject *gtoReader_object( PyObject *_self, PyObject *args )
{
    PyObject *self = NULL;
    char *name;
    char *protocol;
    int protocolVersion;
    int numComponents;

    if( ! PyArg_ParseTuple( args, "Ossii:gtoReader_object", 
                            &self, &name, &protocol, &protocolVersion,
                            &numComponents ) )
    {
        // Invalid parameters, let Python do a stack trace
        PyErr_Print();
        return NULL;
    }

    // Just dump the info to cout
    cout << "object \"" << name << "\" protocol \""  << protocol << "\", "
         << numComponents << " components";
    cout << endl;

    // Assume we want all the objects in the file
    return PyInt_FromLong( 1 );
}

// *****************************************************************************
// Implements a default gtoReader.component( name, numProperties, flags )
PyObject *gtoReader_component( PyObject *_self, PyObject *args )
{
    PyObject *self;
    char *name;
    int numProperties;
    int flags;
    PyObject *objInfo;

    if( ! PyArg_ParseTuple( args, "Osii|O:gtoReader_component", 
                            &self, &name, &numProperties, 
                            &flags, &objInfo ) )
    {
        // Invalid parameters, let Python do a stack trace
        PyErr_Print();
        return NULL;
    }

    // Just dump the info to cout
    cout << "component \"" << name << "\", " << numProperties
         << " properties";
    if( flags & Gto::Transposed )
    {
        cout << " transposed";
    }
    cout << endl;
         
    // Assume we want all the components in the file
    return PyInt_FromLong( 1 );
}

// *****************************************************************************
// Implements a default gtoReader.property( name, size, type, width )
PyObject *gtoReader_property( PyObject *_self, PyObject *args )
{
    PyObject *self;
    char *name;
    int size;
    int type;
    int width;
    PyObject *propInfo;

    if( ! PyArg_ParseTuple( args, "Osiii|O:gtoReader_property", 
                            &self, &name, &size, &type, &width,
                            &propInfo ) )
    {
        // Invalid parameters, let Python do a stack trace
        PyErr_Print();
        return NULL;
    }

    // Just dump the info to cout
    cout << "property " << typeAsString( type );
    if( width != 1 )
    {
        cout << "[" << width << "]";
    }
    cout << " \"" << name << "\" " << size << " elements" 
         << endl;
         
    // Assume we want all the properties in the file
    return PyInt_FromLong( 1 );
}

// *****************************************************************************
// Implements a default gtoReader.data( name, size, type, width, data )
PyObject *gtoReader_data( PyObject *_self, PyObject *args )
{
    PyObject *self;
    char *name;
    int size;
    int type;
    int width;
    PyObject *data;

    if( ! PyArg_ParseTuple( args, "OsiiiO:gtoReader_data", 
                            &self, &name, &size,
                            &type, &width, &data ) )
    {
        // Invalid parameters, let Python do a stack trace
        PyErr_Print();
        return NULL;
    }

    // Dump the fact that we have data, but not the data itself, to cout.
    // User will have to override this function in a derived class to actually
    // do something with the data.
    cout << "data " << typeAsString( type );
    if( width != 1 )
    {
        cout << "[" << width << "]";
    }
    cout << " \"" << name << "\" " << size << " elements" 
         << endl;
         
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
        PyErr_Print();
        return NULL;
    }

    // Get a handle to our Gto::Writer instance
    gtoReader_PyObject *reader =
            (gtoReader_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__reader" );
    if( reader == NULL )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }
    assert( reader->m_reader != NULL );

    PyObject *str = PyString_FromString( 
                    reader->m_reader->stringFromId( id ).c_str() );

    Py_INCREF( str );
    return str;
}



} // End namespace PyGto

