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

#include <sstream>
#include "gtoWriter.h"

namespace PyGto {
using namespace std;

// *****************************************************************************
// We start with a few utility functions...
// *****************************************************************************

// *****************************************************************************
// Properly deallocate the instance-specific stuff-holder object
void gtoWriter_PyObject_dealloc( PyObject *self )
{
    assert( self != NULL );
    gtoWriter_PyObject *gwSelf = (gtoWriter_PyObject *)self;
    delete gwSelf->m_writer;
    delete gwSelf->m_propertyNames;    
    PyObject_DEL( self );
}

// *****************************************************************************
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

// *****************************************************************************
// Flatten a tuple or list into a C array of any type using the converter
// supplied.  'start' is used internally for recursive purposes.  Returns
// the number of items in the C array.
template<typename T> int flatten( PyObject *object,
                                  T *data,
                                  int maxItems,
                                  const char *expectedTypeStr,
                                  T (*converter)(PyObject *),
                                  bool start = true )
{
    static int pos;
    if( start )
    {
        pos = 0;
    }

    // If we come across a class instance, do we know what to do with it?
    if( PyInstance_Check( object ) )
    {
        string classname( PyTypeName( object ) );

        // If we know what it is, convert it to something useful
        if( classname == "mat3" || classname == "mat4" )
        {
            // mat3 and mat4 convert easily to a list
            PyObject *tmp = PyObject_GetAttrString( object, "mlist" );
            Py_INCREF( tmp );
            object = tmp;

        }
        else if( classname == "vec3" || classname == "vec4" )
        {
            // vec3 and vec4 have no handy .toList() method, so we have
            // to do it the 'hard way'...
            PyObject *tmp;
            classname == "vec3" ? tmp = PyTuple_New(3) : tmp = PyTuple_New(4);

            PyObject *x = PyObject_GetAttrString( object, "x" );
            Py_INCREF( x );
            PyObject *y = PyObject_GetAttrString( object, "y" );
            Py_INCREF( y );
            PyObject *z = PyObject_GetAttrString( object, "z" );
            Py_INCREF( z );
            PyTuple_SetItem( tmp, 0, x );
            PyTuple_SetItem( tmp, 1, y );
            PyTuple_SetItem( tmp, 2, z );
            if( classname == "vec4" )
            {
                PyObject *w = PyObject_GetAttrString( object, "w" );
                Py_INCREF( w );
                PyTuple_SetItem( tmp, 3, w );
            }
            object = tmp;
        }
        else if( classname == "quat" )
        {
            // quat has no handy .toList() method either...
            PyObject *tmp = PyTuple_New(4);

            PyObject *w = PyObject_GetAttrString( object, "w" );
            Py_INCREF( w );
            PyObject *x = PyObject_GetAttrString( object, "x" );
            Py_INCREF( x );
            PyObject *y = PyObject_GetAttrString( object, "y" );
            Py_INCREF( y );
            PyObject *z = PyObject_GetAttrString( object, "z" );
            Py_INCREF( z );
            PyTuple_SetItem( tmp, 0, w );
            PyTuple_SetItem( tmp, 1, x );
            PyTuple_SetItem( tmp, 2, y );
            PyTuple_SetItem( tmp, 3, z );
            object = tmp;
        }
        else
        {
            // Otherwise, barf on it
            PyErr_Format( gtoError(), "Can't handle '%s' class data directly."
                                      "  Convert it to a tuple or list first.",
                            classname.c_str() );
            return -1;
        }
    }

    // Put atoms directly into the buffer, and recurse on more complex types
    for( int i = 0; i < PySequence_Size( object ); ++i )
    {
        PyObject *item = PySequence_GetItem( object, i );
        if( PyTuple_Check( item )
           || PyList_Check( item )
           || PyInstance_Check( item ) )
        {
            flatten( item, data, maxItems, expectedTypeStr, converter, false );
        }
        else
        {
            // Add the atom to the buffer and move on
            data[pos] = converter( item );
            if( PyErr_Occurred() )
            {
                if( ! PyErr_ExceptionMatches( PyExc_TypeError ) )
                {
                    // This is something other than a type error, so
                    // this will cause a Python traceback later...
                    return -1;
                }
                // Data of a type not handled by the converter
                PyErr_Format( gtoError(), "Expected data of type '%s', but "
                                          "got '%s'", expectedTypeStr,
                                                      PyTypeName( item ) );
                return -1;
            }
            pos++;
            if( pos > maxItems )
            {
                return pos;
            }
        }
    }

    return pos;
}



// *****************************************************************************
// The next several functions implement the methods on the Python gto.Writer
// class.
// *****************************************************************************

// *****************************************************************************
// gto.Writer class constructor. Does nothing, but is required anyway
PyObject *gtoWriter_init( PyObject *_self, PyObject *args )
{
    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements gto.Writer.open( filename )
PyObject *gtoWriter_open( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;
    char *filename;
    int compress = true;

    if( ! PyArg_ParseTuple( args, "Os|i:gtoWriter_open", &self, &filename,
                                                         &compress ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    // Create a new python writer object and add it to this instance's
    // dictionary
    gtoWriter_PyObject *writer = PyObject_NEW( gtoWriter_PyObject,
                                               &gtoWriter_PyObjectType );
    writer->m_writer = new Gto::Writer();
    writer->m_propCount = 0;
    writer->m_beginDataCalled = false;
    writer->m_objectDef = false;
    writer->m_componentDef = false;
    writer->m_propertyNames = new vector<string>;

    PyDict_SetItemString( self->in_dict, "__writerEngine", (PyObject *)writer );

    // Ask the writer to open the given file
    if( ! writer->m_writer->open( filename, compress ) )
    {
        PyErr_Format( gtoError(), "Unable to open specified file: %s",
                      filename );
        return NULL;
    }

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements gto.Writer.close()
PyObject *gtoWriter_close( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;
    if( ! PyArg_ParseTuple( args, "O:gtoWriter_close", &self ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    // Get a handle to our Gto::Writer instance
    gtoWriter_PyObject *writer =
            (gtoWriter_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__writerEngine" );
    if( writer == NULL )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }
    assert( writer->m_writer != NULL );

    // Close the file
    writer->m_writer->close();

    // Remove the writer from the class dictionary
    PyDict_DelItemString( self->in_dict, "__writerEngine" );
    Py_DECREF( writer );

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements gto.Writer.beginObject( name, protocol, protocolVersion )
PyObject *gtoWriter_beginObject( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;
    char *name;
    char *protocol;
    unsigned int protocolVersion;

    if( ! PyArg_ParseTuple( args, "Ossi:gtoWriter_beginObject",
                            &self, &name, &protocol, &protocolVersion ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    // Get a handle to our Gto::Writer instance
    gtoWriter_PyObject *writer =
            (gtoWriter_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__writerEngine" );
    if( writer == NULL )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }
    assert( writer->m_writer != NULL );

    // Check for dumbassness
    if( writer->m_objectDef == true )
    {
        PyErr_SetString( gtoError(), "Can't nest object declarations" );
        return NULL;
    }
    if( writer->m_beginDataCalled == true )
    {
        PyErr_SetString( gtoError(), "Once beginData is called, no new "
                                     "objects can be declared" );
        return NULL;
    }

    // Make it so
    writer->m_writer->beginObject( name, protocol, protocolVersion );
    writer->m_objectDef = true;

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements gto.Writer.endObject()
PyObject *gtoWriter_endObject( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;

    if( ! PyArg_ParseTuple( args, "O:gtoWriter_endObject",
                            &self ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    // Get a handle to our Gto::Writer instance
    gtoWriter_PyObject *writer =
            (gtoWriter_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__writerEngine" );
    if( writer == NULL )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }
    assert( writer->m_writer != NULL );

    // Check for dumbassness
    if( writer->m_objectDef == false )
    {
        PyErr_SetString( gtoError(), "endObject called before beginObject" );
        return NULL;
    }

    // Make it so
    writer->m_writer->endObject();
    writer->m_objectDef = false;

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements gto.Writer.beginComponent( name, interp, flags )
PyObject *gtoWriter_beginComponent( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;
    char *name;
    char *interp = "";
    int flags = 0;

    // Try GTOv2 prototype first...
    if( ! PyArg_ParseTuple( args, "Os|i:gtoWriter_beginComponent",
                            &self, &name, &flags ) )
    {
        PyErr_Clear();
        // If that doesn't work, try the GTOv3 prototype
        if( ! PyArg_ParseTuple( args, "Oss|i:gtoWriter_beginComponent",
                                &self, &name, &interp, &flags ) )
        {
            // Invalid parameters, let Python do a stack trace
            return NULL;
        }
    }

    // Get a handle to our Gto::Writer instance
    gtoWriter_PyObject *writer =
            (gtoWriter_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__writerEngine" );
    if( writer == NULL )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }
    assert( writer->m_writer != NULL );

    // Check for dumbassness
    if( writer->m_objectDef == false )
    {
        PyErr_SetString( gtoError(), "Components can only exist inside object "
                                     "blocks" );
        return NULL;
    }
    if( writer->m_componentDef == true )
    {
        PyErr_SetString( gtoError(), "Can't nest component declarations" );
        return NULL;
    }

    // Make it so
    writer->m_writer->beginComponent( name, interp, flags );
    writer->m_componentDef = true;

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements gto.Writer.endComponent()
PyObject *gtoWriter_endComponent( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;

    if( ! PyArg_ParseTuple( args, "O:gtoWriter_endComponent",
                            &self ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    // Get a handle to our Gto::Writer instance
    gtoWriter_PyObject *writer =
            (gtoWriter_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__writerEngine" );
    if( writer == NULL )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }
    assert( writer->m_writer != NULL );

    // Check for dumbassness
    if( writer->m_componentDef == false )
    {
        PyErr_SetString( gtoError(), "endComponent called before "
                                     "beginComponent" );
        return NULL;
    }

    // Make it so
    writer->m_writer->endComponent();
    writer->m_componentDef = false;

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// Implements gto.Writer.property( name, type, numElements, width, interp )
PyObject *gtoWriter_property( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;
    char *name;
    int type;
    int numElements;
    int width = 1;
    char *interp = "";
    
    if( ! PyArg_ParseTuple( args, "Osii|is:gtoWriter_property",
                            &self, &name, &type, &numElements, 
                            &width, &interp ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    // Get a handle to our Gto::Writer instance
    gtoWriter_PyObject *writer =
            (gtoWriter_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__writerEngine" );
    if( writer == NULL )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }
    assert( writer->m_writer != NULL );

    // Check for dumbassness    
    if( writer->m_objectDef == false || writer->m_componentDef == false )
    {
        PyErr_SetString( gtoError(), "Properties can only exist inside "
                                     "object/component blocks" );
        return NULL;
    }

    // Store name for later dumbassness checking in propertyData()
    writer->m_propertyNames->push_back( name );

    // Make it so
    writer->m_writer->property( name,
                                (Gto::DataType)type,
                                numElements, 
                                width,
                                interp );

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// implements gto.Writer.intern( string | tuple | list )
PyObject *gtoWriter_intern( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;
    PyObject *data;

    if( ! PyArg_ParseTuple( args, "OO:gtoWriter_intern",
                            &self, &data ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    // Get a handle to our Gto::Writer instance
    gtoWriter_PyObject *writer =
            (gtoWriter_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__writerEngine" );
    if( writer == NULL )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }
    assert( writer->m_writer != NULL );


    // Handle a single string
    if( PyString_Check( data ) )
    {
        char *str = PyString_AsString( data );
        writer->m_writer->intern( str );
    }
    // Handle a bunch of strings all at once
    else if( PySequence_Check( data ) )
    {
        for( int i = 0; i < PySequence_Size( data ); ++i )
        {
            PyObject *pstr = PySequence_GetItem( data, i );
            if( ! PyString_Check( pstr ) )
            {
                PyErr_SetString( gtoError(), "Non-string in sequence" );
                return NULL;
            }
            char *str = PyString_AsString( pstr );
            writer->m_writer->intern( str );
        }
    }
    // We can't handle what we were given
    else
    {
        PyErr_SetString( gtoError(), "intern requires a string or a "
                                     "sequence of strings" );
        return NULL;
    }

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// implements gto.Writer.lookup( string )
PyObject *gtoWriter_lookup( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;
    char *str;

    if( ! PyArg_ParseTuple( args, "Os:gtoWriter_lookup",
                            &self, &str ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    // Get a handle to our Gto::Writer instance
    gtoWriter_PyObject *writer =
            (gtoWriter_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__writerEngine" );
    if( writer == NULL )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }
    assert( writer->m_writer != NULL );

    // Check for dumbassness
    if( writer->m_beginDataCalled == false )
    {
        PyErr_SetString( gtoError(), "lookup() cannot be used until "
                                     "beginData() is called" );
        return NULL;
    }

    // Make it so
    PyObject *strId_PyObj = PyInt_FromLong( writer->m_writer->lookup( str ) );

    Py_INCREF( strId_PyObj );
    return strId_PyObj;
}

// *****************************************************************************
// implements gto.Writer.beginData()
PyObject *gtoWriter_beginData( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;

    if( ! PyArg_ParseTuple( args, "O:gtoWriter_beginData",
                            &self ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    // Get a handle to our Gto::Writer instance
    gtoWriter_PyObject *writer =
            (gtoWriter_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__writerEngine" );
    if( writer == NULL )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }
    assert( writer->m_writer != NULL );

    // Check for dumbassness
    if( writer->m_writer->properties().size() == 0 )
    {
        PyErr_SetString( gtoError(), "There are no properties to write" );
        return NULL;
    }

    // Make it so
    writer->m_writer->beginData();
    writer->m_beginDataCalled = true;

    Py_INCREF( Py_None );
    return Py_None;
}

// *****************************************************************************
// implements gto.Writer.endData()
PyObject *gtoWriter_endData( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;

    if( ! PyArg_ParseTuple( args, "O:gtoWriter_endData",
                            &self ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    // Get a handle to our Gto::Writer instance
    gtoWriter_PyObject *writer =
            (gtoWriter_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__writerEngine" );
    if( writer == NULL )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }
    assert( writer->m_writer != NULL );

    // Check for dumbassness
    if( writer->m_beginDataCalled == false )
    {
        PyErr_SetString( gtoError(), "endData called before beginData" );
        return NULL;
    }

    // Make it so
    writer->m_writer->endData();

    Py_INCREF( Py_None );
    return Py_None;
}


// *****************************************************************************
// implements gto.Writer.propertyData( data )
PyObject *gtoWriter_propertyData( PyObject *_self, PyObject *args )
{
    PyInstanceObject *self;
    PyObject *rawdata;

    if( ! PyArg_ParseTuple( args, "OO:gtoWriter_propertyData",
                            &self, &rawdata ) )
    {
        // Invalid parameters, let Python do a stack trace
        return NULL;
    }

    // Get a handle to our Gto::Writer instance
    gtoWriter_PyObject *writer =
            (gtoWriter_PyObject *)PyDict_GetItemString( self->in_dict,
                                                        "__writerEngine" );
    if( writer == NULL )
    {
        PyErr_SetString( gtoError(), "no file is open." );
        return NULL;
    }
    assert( writer->m_writer != NULL );

    // Check for dumbassness
    if( ! writer->m_beginDataCalled )
    {
        PyErr_SetString( gtoError(), "propertyData called before beginData" );
        return NULL;
    }

    // If we're handed a single value, tuple-ize it for the code below
    if( PyInt_Check( rawdata )
        || PyFloat_Check( rawdata )
        || PyString_Check( rawdata ) )
    {
        PyObject *tmp = PyTuple_New( 1 );
        PyTuple_SetItem( tmp, 0, rawdata );
        Py_DECREF( rawdata );
        rawdata = tmp;
    }

    // Get a handle to the property definition for the current property
    // and do some sanity checking
    Gto::PropertyHeader prop;
    prop = writer->m_writer->properties()[writer->m_propCount];
    if( writer->m_propCount >= writer->m_writer->properties().size() )
    {
        PyErr_SetString( gtoError(), "Undeclared data." );
        return NULL;
    }
    
    const char *currentPropName = (*writer->m_propertyNames)[writer->m_propCount].c_str();

    // Determine how many elements we have in the data
    int dataSize = prop.size * prop.width;

    // Write that data!
    if( prop.type == Gto::Int )
    {
        int data[dataSize];
        int numItems = flatten( rawdata, data, dataSize, "int", PyInt_AsInt );
        if( PyErr_Occurred() )
        {
            return NULL;
        }
        if( numItems != dataSize )
        {
            PyErr_Format( gtoError(), "Property '%s' was declared as having %d"
                          " x %d values, but %d values were given for writing",
                          currentPropName, prop.size, prop.width,
                          numItems );
            return NULL;
        }
        writer->m_writer->propertyData( data );
        writer->m_propCount++;

        Py_INCREF( Py_None );
        return Py_None;
    }

    if( prop.type == Gto::Float )
    {
        float data[dataSize];
        int numItems = flatten( rawdata, data, dataSize, "float", 
                                PyFloat_AsFloat );
        if( PyErr_Occurred() )
        {
            return NULL;
        }
        if( numItems != dataSize )
        {
            PyErr_Format( gtoError(), "Property '%s' was declared as having %d"
                          " x %d values, but %d values were given for writing",
                          currentPropName, prop.size, prop.width,
                          numItems );
            return NULL;
        }
        writer->m_writer->propertyData( data );
        writer->m_propCount++;

        Py_INCREF( Py_None );
        return Py_None;
    }
    if( prop.type == Gto::Double )
    {
        double data[dataSize];
        int numItems = flatten( rawdata, data, dataSize, "double",
                                PyFloat_AsDouble );
        if( PyErr_Occurred() )
        {
            return NULL;
        }
        if( numItems != dataSize )
        {
            PyErr_Format( gtoError(), "Property '%s' was declared as having %d"
                          " x %d values, but %d values were given for writing",
                          currentPropName, prop.size, prop.width,
                          numItems );
            return NULL;
        }
        writer->m_writer->propertyData( data );
        writer->m_propCount++;
        Py_INCREF( Py_None );
        return Py_None;
    }
    if( prop.type == Gto::Short )
    {
        unsigned short data[dataSize];
        int numItems = flatten( rawdata, data, dataSize, "short",
                                PyInt_AsShort );
        if( PyErr_Occurred() )
        {
            return NULL;
        }
        if( numItems != dataSize )
        {
            PyErr_Format( gtoError(), "Property '%s' was declared as having %d"
                          " x %d values, but %d values were given for writing",
                          currentPropName, prop.size, prop.width,
                          numItems );
            return NULL;
        }
        writer->m_writer->propertyData( data );
        writer->m_propCount++;
        Py_INCREF( Py_None );
        return Py_None;
    }
    if( prop.type == Gto::Byte )
    {
        unsigned char data[dataSize];
        int numItems = flatten( rawdata, data, dataSize, "byte",
                                PyInt_AsByte );
        if( PyErr_Occurred() )
        {
            return NULL;
        }
        if( numItems != dataSize )
        {
            PyErr_Format( gtoError(), "Property '%s' was declared as having %d"
                          " x %d values, but %d values were given for writing",
                          currentPropName, prop.size, prop.width,
                          numItems );
            return NULL;
        }
        writer->m_writer->propertyData( data );
        writer->m_propCount++;
        Py_INCREF( Py_None );
        return Py_None;
    }
    if( prop.type == Gto::String )
    {
        char *strings[dataSize];
        int numItems = flatten( rawdata, strings, dataSize, "string",
                                PyString_AsString );
        if( PyErr_Occurred() )
        {
            return NULL;
        }
        if( numItems != dataSize )
        {
            PyErr_Format( gtoError(), "Property '%s' was declared as having %d"
                          " x %d values, but %d values were given for writing",
                          currentPropName, prop.size, prop.width,
                          numItems );
            return NULL;
        }

        int data[dataSize];
        for( int i = 0; i < numItems; ++i )
        {
            data[i] = writer->m_writer->lookup( strings[i] );
            if( data[i] == -1 )
            {
                PyErr_Format( gtoError(),
                       "'%s' needs to be \"interned\" before it can "
                       "be used as data in property #%d",
                       strings[i], writer->m_propCount );
                return NULL;
            }
        }
        writer->m_writer->propertyData( data );
        Py_INCREF( Py_None );
        writer->m_propCount++;
        return Py_None;
    }

    PyErr_Format( gtoError(), "Undefined property type: %d  in property '%s'",
                     prop.type, currentPropName );
    return NULL;
}


}; // End namespace PyGto
