//******************************************************************************
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
//******************************************************************************

#ifdef SWIGPYTHON

%{  // Begin verbatim

#include <stdexcept>
#include <Gto/Header.h>
#include <Gto/Reader.h>
#include <Gto/RawData.h>
#include "GtoDB.h"

using namespace Gto;


// *****************************************************************************
//
// Utility function, not exposed to Python.  Build a python representation of
// one item in a given property.  If property width is > 1, result will be
// a tuple.
//
PyObject *gtoDB_getItem(const Property* prop, long index)
{
    if(prop->width == 1)
    {
        switch(prop->type)
        {
        case Gto::String:
            return PyString_FromString(prop->stringData[index].c_str());
        case Gto::Int:
            return PyInt_FromLong(prop->int32Data[index]);
        case Gto::Float:
            return PyFloat_FromDouble(prop->floatData[index]); 
        case Gto::Double:
            return PyFloat_FromDouble(prop->doubleData[index]); 
        case Gto::Byte:
            return PyInt_FromLong(prop->uint8Data[index]); 
        case Gto::Short:
            return PyInt_FromLong(prop->uint16Data[index]); 
        default:
            PyErr_SetString(PyExc_IndexError, "Unhandled GTO data type");
            return NULL;
        }
    }
    else
    {
        PyObject *tuple = PyTuple_New(prop->width);
        for( size_t w = 0; w < prop->width; ++w )
        {
            const size_t di = ( index * prop->width ) + w;
            PyObject *item = NULL;
            switch(prop->type)
            {
            case Gto::String:
                item = PyString_FromString(prop->stringData[di].c_str());
                break;
            case Gto::Int:
                item = PyInt_FromLong(prop->int32Data[di]);
                break;
            case Gto::Float:
                item = PyFloat_FromDouble(prop->floatData[di]);
                break;
            case Gto::Double:
                item = PyFloat_FromDouble(prop->doubleData[di]);
                break;
            case Gto::Byte:
                item = PyInt_FromLong(prop->uint8Data[di]);
                break;
            case Gto::Short:
                item = PyInt_FromLong(prop->uint16Data[di]);
                break;
            default:
                PyErr_SetString(PyExc_IndexError, "Unhandled GTO data type");
                return NULL;
            }
            PyTuple_SetItem(tuple, w, item);
        }
        return tuple;
    }

    throw std::runtime_error("GTO Property unknown failure");
}


// *****************************************************************************
//
// Utility function, not exposed to Python.  Build a python representation of
// a slice of items in a given property.  Always returns a tuple.
//
PyObject *gtoDB_getSlice(const Property* prop, long start, long stop, 
                   long step, long size)
{
    PyObject *tuple = PyTuple_New(size);
    for(long dataIndex = start, tupleIndex = 0; 
        dataIndex < stop; 
        tupleIndex++, dataIndex += step)
    {
        PyObject *item = gtoDB_getItem(prop, dataIndex);
        PyTuple_SetItem(tuple, tupleIndex, item);
    }
    return tuple;
}


// *****************************************************************************
//
// Utility function, not exposed to Python.  Set one data item in the given
// property.  Number of items in the value must match the property's width.
//
void setItem(const Property* prop, long index, PyObject *value)
{
    if(PySequence_Check(value) && (! PyString_Check(value)))
    {
        const size_t valueSize = PySequence_Size(value);
        if(valueSize != prop->width)
        {
            throw std::invalid_argument("GTO Property width mismatch");
        }
        for( size_t w = 0; w < prop->width; ++w )
        {
            const size_t di = ( index * prop->width ) + w;
            switch(prop->type)
            {
            case Gto::String:
                prop->stringData[di] = PyString_AsString(PySequence_GetItem(value, w));
                break;
            case Gto::Int:
                prop->int32Data[di] = PyInt_AsLong(PySequence_GetItem(value, w));
                break;
            case Gto::Float:
                prop->floatData[di] = PyFloat_AsDouble(PySequence_GetItem(value, w));
                break;
            case Gto::Double:
                prop->doubleData[di] = PyFloat_AsDouble(PySequence_GetItem(value, w));
                break;
            case Gto::Byte:
                prop->uint8Data[di] = PyInt_AsLong(PySequence_GetItem(value, w));
                break;
            case Gto::Short:
                prop->uint16Data[di] = PyInt_AsLong(PySequence_GetItem(value, w));
                break;
            default:
                throw std::invalid_argument("Unhandled GTO data type");
            }
        }
    }
    else if (prop->width == 1)
    {
        switch(prop->type)
        {
        case Gto::String:
            prop->stringData[index] = PyString_AsString(value);
            break;
        case Gto::Int:
            prop->int32Data[index] = PyInt_AsLong(value);
            break;
        case Gto::Float:
            prop->floatData[index] = PyFloat_AsDouble(value);
            break;
        case Gto::Double:
            prop->doubleData[index] = PyFloat_AsDouble(value);
            break;
        case Gto::Byte:
            prop->uint8Data[index] = PyInt_AsLong(value);
            break;
        case Gto::Short:
            prop->uint16Data[index] = PyInt_AsLong(value);
            break;
        default:
            throw std::invalid_argument("Unhandled GTO data type");
        }
    }
    else
    {
        throw std::invalid_argument("GTO Property unknown failure");
    }
}


// *****************************************************************************
//
// Utility function, not exposed to Python.  Set a slice of values on the given
// property.  Number of items in the value must match the size of the slice.
//
void setSlice(const Property* prop, long start, long stop, 
                   long step, long size, PyObject *value)
{
    if(PySequence_Check(value) && (! PyString_Check(value)))
    {
        const size_t valueSize = PySequence_Size(value);
        if(valueSize != size)
        {
            throw std::invalid_argument("GTO Property value count mismatch");
        }
        for(size_t dataIndex = start, valueIndex = 0; 
                   dataIndex < stop; dataIndex += step, ++valueIndex )
        {
            setItem(prop, dataIndex, PySequence_GetItem(value, valueIndex));
        }
    }
    else if( size == 1 )
    {
        setItem(prop, start, value);
    }
}


// *****************************************************************************
//
// Utility function, not exposed to Python.  Choose a GTO data type from the 
// given Python value.
//
inline Gto::DataType gtoTypeFromPyObject(PyObject *o)
{
    if( PyString_Check(o) )
    {
        return Gto::String;
    }
    else if( PyInt_Check(o) || PyLong_Check(o) || PyBool_Check(o) )
    {
        return Gto::Int;
    }
    else if( PyFloat_Check(o) )
    {
        return Gto::Float;
    }
    else
    {
        return Gto::ErrorType;
    }
}


// *****************************************************************************
//
// Utility function, not exposed to Python.  Create a new GTO Property object,
// infering the type, size, and width from the given Python data.
//
Property* createPropertyFromPyObject(const std::string name, 
                                     const std::string interp, 
                                     PyObject *data)
{
    size_t size = 1;
    size_t width = 1;
    Gto::DataType type = Gto::ErrorType;
    if( PySequence_Check(data) && (! PyString_Check(data)) )
    {
        size = PySequence_Length(data);
        if( size == 0 )
        {
            throw std::invalid_argument("GTO cannot infer data type from empty sequence");
        }
        PyObject *firstItem = PySequence_GetItem(data, 0);
        if( PySequence_Check(firstItem) && (! PyString_Check(firstItem)) )
        {
            width = PySequence_Length(firstItem);
            type = gtoTypeFromPyObject(PySequence_GetItem(firstItem, 0));
        }
        else
        {
            type = gtoTypeFromPyObject(PySequence_GetItem(data, 0));
        }
    }
    else
    {
        type = gtoTypeFromPyObject(data);
    }
    if( type == Gto::ErrorType)
    {
        throw std::invalid_argument("GTO cannot store data of this type");
    }
    Property *prop = new Property( name, interp, type, size, width, true );
    setSlice(prop, 0, size, 1, size, data);
    return prop;
}


%}  // End verbatim

#endif // #ifdef SWIGPYTHON

// *****************************************************************************
// *****************************************************************************
//
//  Python representation of the Property class from RawData.h
//
// *****************************************************************************
// *****************************************************************************

%define PROPERTY_DOCSTRING
"Represents a single GTO Property and the data it contains.  Property
data can be accessed like so:

    property()       # Returns data in its native form (int, float, str, tuple)
    property[i]      # Returns a single item of the property's data
    property[i:j]    # Returns a slice of the property's data

Information about the Property itself can be had via the attributes name, interp,
size, width, and type.).  A Property's name and interp string can be changed, 
but the size, width, and data type cannot.  Property data can be changed:
        
    property[0] = 'hello'
    property[0:2] = (0.1, 0.2)

"
%enddef
%feature("autodoc", PROPERTY_DOCSTRING);
struct Property
{
    // Because we always want to allocate memory when creating Properties
    // from Python, we don't expose the standard constructors here.  Python-
    // specific ones are created via %extend below.

    std::string     name;
    std::string     interp;
    %immutable;
    size_t          size;  // Can't dynamically change a Property's size
    size_t          width; // Can't dynamically change a Property's width
    Gto::DataType   type;  // Can't dynamically change a Property's data type

#ifdef SWIGPYTHON
%extend {

    %feature("autodoc", "1");
    Property(const std::string name, const std::string interp, 
             Gto::DataType type, size_t size, size_t width=1)
    {
        if( type >= NumberOfDataTypes )
        {
            throw std::invalid_argument("GTO Property invalid data type");
        }
        return new Property( name, interp, type, size, width, true ); 
    }


    Property(const std::string name, Gto::DataType type, size_t size, 
             size_t width=1) 
    {
        if( type >= NumberOfDataTypes )
        {
            throw std::invalid_argument("GTO Property invalid data type");
        }
        return new Property( name, type, size, width, true ); 
    }

%define PROPERTY_CONTSTRUCTOR_DOCSTRING
"

Variants of the contstructor that accept a PyObject parameter will auto-
detect the data type, size, and width parameters for the new Property based
on the type and structure of the Python object, and set the property data
accordingly.  For example:

    Property('a', 'hello')          => type=string, size=1, width=1
    Property('b', ('hello', 'bye')  => type=string, size=2, width=1
    Property('c', 3.14159)          => type=float, size=1, width=1
    Property('d', (1, 2))           => type=int, size=2, width=1
    Property('e', ((1, 2), (3, 4))) => type=int, size=2, width=2
    Property('f', range(10))        => type=int, size=10, width=1
"
%enddef 

    %feature("docstring", PROPERTY_CONTSTRUCTOR_DOCSTRING);
    Property(const std::string name, const std::string interp, PyObject *data) 
    {
        return createPropertyFromPyObject(name, interp, data);
    }


    Property(const std::string name, PyObject *data) 
    {
        return createPropertyFromPyObject(name, "", data);
    }
    %feature("docstring", "");


    %feature("autodoc", "0");
    char *__repr__()
    {
        static const char typeStr[][16] = { "int", "float", "double", "half",
                                            "string", "boolean", "short", 
                                            "byte" };
        static char tmp[256];
        snprintf(tmp, 256, "<GTO Property %s[%u][%u] '%s' at %p>", 
                 typeStr[$self->type], 
                 $self->width,
                 $self->size, 
                 $self->name.c_str(),
                 $self
                );
        return tmp;
    }


    %feature("autodoc", "0");
    long __cmp__(PyObject *other)
    {
        Property *otherProp = NULL;
        CAST_PYOBJECT_TO_C(Property, other, otherProp);
        if($self == otherProp)
        {
            return 0;
        }
        else if(otherProp->name > $self->name)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }

    
    %feature("autodoc", "Equivalent to property.size");
    long __len__()
    {
        return long($self->size);
    }


    %feature("autodoc", "Returns all property data in its native form (int, float, str, tuple)");
    PyObject *__call__()
    {
        if( $self->size == 1 )
        {
            return gtoDB_getItem($self, 0);
        }
        else
        {
            return gtoDB_getSlice($self, 0, $self->size, 1, $self->size);
        }
    }


    %feature("autodoc", "0");
    PyObject *__getitem__(PyObject *key)
    {
        if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, $self->size, 
                                              &start, &stop, &step, &slicelength);
            if(result != 0)
            {
                PyErr_SetString(PyExc_IndexError, "GTO Property invalid slice");
                return NULL;
            }
            return gtoDB_getSlice($self, start, stop, step, slicelength);
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = $self->size - index - 2;
            }
            if(index < 0 || index >= $self->size)
            {
                PyErr_SetString(PyExc_IndexError, "GTO Property index out of range");
                return NULL;
            }
            return gtoDB_getItem($self, index);
        }
        else if( ($self->size > 1) && PyTuple_Check(key) && (PyTuple_Size(key) == 2) )
        {
            PyObject* xObj = PyTuple_GetItem(key, 0);
            PyObject* yObj = PyTuple_GetItem(key, 1);
            if(PyInt_Check(xObj) && PyInt_Check(yObj))
            {
                long xindex = PyInt_AS_LONG(xObj);
                long yindex = PyInt_AS_LONG(yObj);
                if(xindex < 0)
                {
                    xindex = $self->size - xindex - 2;
                }
                if(xindex < 0 || xindex >= $self->size)
                {
                    PyErr_SetString(PyExc_IndexError, "GTO Property index out of range");
                    return NULL;
                }

                if(yindex < 0)
                {
                    yindex = $self->width - yindex - 2;
                }
                if(yindex < 0 || yindex >= $self->width)
                {
                    PyErr_SetString(PyExc_IndexError, "GTO Property index out of range");
                    return NULL;
                }
                PyObject *tuple = gtoDB_getItem($self, xindex);
                assert(PyTuple_Check(tuple));
                assert(yindex < PyTuple_Size(tuple));

                PyObject *result = PyTuple_GetItem(tuple, yindex);
                Py_INCREF(result);
                return result;
            }
        }

        PyErr_SetString(PyExc_IndexError, "GTO Property Invalid index type");
        return NULL;
    }


    %feature("autodoc", "0");
    void __setitem__(PyObject *key, PyObject *value)
    {
        if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = $self->size - index - 2;
            }
            if(index < 0 || index >= $self->size)
            {
                throw std::out_of_range("GTO Property index out of range");
            }
            setItem($self, index, value);
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, $self->size, 
                                              &start, &stop, &step, &slicelength);
            if(result != 0)
            {
                throw std::out_of_range("GTO Property invalid slice");
            }
            setSlice($self, start, stop, step, slicelength, value);
        }
        else
        {
            throw std::invalid_argument("GTO Property Invalid index type");
        }
    }


    // This function is big and annoying, and is not strictly necessary, since
    // Python will use __getitem__ to search for values if we don't provide it.
    // However, doing it ourselves here is MUCH, MUCH faster.
    %feature("autodoc", "0");
    bool __contains__(PyObject *item)
    {
        if(PyString_Check(item) && ($self->type == Gto::String))
        {
            const char *itemstr = PyString_AS_STRING(item);
            for(size_t i = 0; i < $self->size; ++i )
            {
                if( $self->stringData[i] == itemstr )
                {
                    return true;
                }
            }
        }
        else if(PyInt_Check(item))
        {
            if( $self->type == Gto::Int )
            {
                const int32 itemVal = PyInt_AS_LONG(item);
                for(size_t i = 0; i < $self->size; ++i )
                {
                    if( self->int32Data[i] == itemVal ) return true;
                }
            }
            else if( $self->type == Gto::Short )
            {
                const uint16 itemVal = PyInt_AS_LONG(item);
                for(size_t i = 0; i < $self->size; ++i )
                {
                    if( self->uint16Data[i] == itemVal ) return true;
                }
            }
            else if( $self->type == Gto::Byte )
            {
                const uint16 itemVal = PyInt_AS_LONG(item);
                for(size_t i = 0; i < $self->size; ++i )
                {
                    if( self->uint8Data[i] == itemVal ) return true;
                }
            }
        }
        else if(PyFloat_Check(item))
        {
            if( $self->type == Gto::Float )
            {
                const float itemVal = PyFloat_AS_DOUBLE(item);
                for(size_t i = 0; i < $self->size; ++i )
                {
                    if( self->floatData[i] == itemVal ) return true;
                }
            }
            else if( $self->type == Gto::Double )
            {
                const double itemVal = PyFloat_AS_DOUBLE(item);
                for(size_t i = 0; i < $self->size; ++i )
                {
                    if( self->doubleData[i] == itemVal ) return true;
                }
            }
//             else if( $self->type == Gto::Half )
//             {
//                 const half itemVal = PyFloat_AS_DOUBLE(item);
//                 for(size_t i = 0; i < $self->size; ++i )
//                 {
//                     if( self->halfData[i] == itemVal ) return true;
//                 }
//             }
        }
        else if(PySequence_Check(item) && (! PyString_Check(item)))
        {
            if(PySequence_Length(item) != $self->width) return false;
            if( $self->type == Gto::String )
            {
                for(size_t i = 0; i < $self->size; ++i )
                {
                    bool match = true;
                    for( size_t w = 0; w < $self->width; ++w )
                    {
                        const size_t di = ( i * $self->width ) + w;
                        const char *v = PyString_AsString(PySequence_GetItem(item, w));
                        if( $self->stringData[di] != v )
                        {
                            match = false;
                            break;
                        }
                    }
                    if( match ) return true;
                }
            }
            else if( $self->type == Gto::Int )
            {
                for(size_t i = 0; i < $self->size; ++i )
                {
                    bool match = true;
                    for( size_t w = 0; w < $self->width; ++w )
                    {
                        const size_t di = ( i * $self->width ) + w;
                        const int32 v = PyInt_AsLong(PySequence_GetItem(item, w));
                        if( $self->int32Data[di] != v )
                        {
                            match = false;
                            break;
                        }
                    }
                    if( match ) return true;
                }
            }
            else if( $self->type == Gto::Short )
            {
                for(size_t i = 0; i < $self->size; ++i )
                {
                    bool match = true;
                    for( size_t w = 0; w < $self->width; ++w )
                    {
                        const size_t di = ( i * $self->width ) + w;
                        const uint16 v = PyInt_AsLong(PySequence_GetItem(item, w));
                        if( $self->uint16Data[di] != v )
                        {
                            match = false;
                            break;
                        }
                    }
                    if( match ) return true;
                }
            }
            else if( $self->type == Gto::Byte )
            {
                for(size_t i = 0; i < $self->size; ++i )
                {
                    bool match = true;
                    for( size_t w = 0; w < $self->width; ++w )
                    {
                        const size_t di = ( i * $self->width ) + w;
                        const uint8 v = PyInt_AsLong(PySequence_GetItem(item, w));
                        if( $self->uint8Data[di] != v )
                        {
                            match = false;
                            break;
                        }
                    }
                    if( match ) return true;
                }
            }
            else if( $self->type == Gto::Float )
            {
                for(size_t i = 0; i < $self->size; ++i )
                {
                    bool match = true;
                    for( size_t w = 0; w < $self->width; ++w )
                    {
                        const size_t di = ( i * $self->width ) + w;
                        const float v = PyFloat_AsDouble(PySequence_GetItem(item, w));
                        if( $self->floatData[di] != v )
                        {
                            match = false;
                            break;
                        }
                    }
                    if( match ) return true;
                }
            }
            else if( $self->type == Gto::Double )
            {
                for(size_t i = 0; i < $self->size; ++i )
                {
                    bool match = true;
                    for( size_t w = 0; w < $self->width; ++w )
                    {
                        const size_t di = ( i * $self->width ) + w;
                        const double v = PyFloat_AsDouble(PySequence_GetItem(item, w));
                        if( $self->floatData[di] != v )
                        {
                            match = false;
                            break;
                        }
                    }
                    if( match ) return true;
                }
            }

        }

        return false;
    }

};   // End %extend

#endif // SWIGPYTHON

};  // End struct Property
