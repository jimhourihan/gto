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

%define COMPONENT_DOCSTRING
"Represents a single GTO Component which may contain any number of 
properties.  Information about the object itself is had via
the attributes name, interp, and flags.

This class can operate as either a dictionary or a list:

   property   = component[0]
   property   = component['position'] 
   properties = component[0:2]
   numProperties = len(component)

   if 'position' in component:   
       del component['position']

Most standard list/dictionary methods are supported.

If a Component is deleted, it will delete all the Properties it contains."
%enddef
%feature("autodoc", COMPONENT_DOCSTRING);
struct Component
{
    %feature("autodoc", "1");
    Component(const std::string& name, const std::string& interp, int flags) 
               : name(name), interp(interp), flags(flags) {}
    Component(const std::string& name, int flags) : name(name), flags(flags) {}

    std::string     name;
    std::string     interp;
    unsigned short  flags;

#ifdef SWIGPYTHON
%extend {

    Component(const std::string name, const std::string interp)
    {
        return new Component(name, interp, 0);
    }


    Component(const std::string name)
    {
        return new Component(name, "", 0);
    }


    %feature("autodoc", "0");
    char *__repr__() 
    {
        static char tmp[256];
        snprintf(tmp, 256, "<GTO Component '%s' at %p>", 
                 $self->name.c_str(),
                 $self
                );
        return tmp;
    }


    %feature("autodoc", "Returns the number of Properties in this component");
    long __len__()
    {
        return long($self->properties.size());
    }


    %feature("autodoc", "Compares component names *only*");
    long __cmp__(PyObject *other)
    {
        Component *otherComp = NULL;
        CAST_PYOBJECT_TO_C(Component, other, otherComp);
        if($self == otherComp)
        {
            return 0;
        }
        else if(otherComp->name > $self->name)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }


    %feature("autodoc", "0");
    PyObject *__getitem__(PyObject *key)
    {
        if( PyString_Check(key) )
        {
            const char *keystr = PyString_AS_STRING(key);
            for(size_t i = 0; i < $self->properties.size(); ++i )
            {
                Gto::Property *p = $self->properties[i];
                if( p->name == keystr )
                {
                    return CAST_C_TO_PYOBJECT(p, Property);
                }
            }
            throw std::out_of_range("GTO Component KeyError");
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = $self->properties.size() - index - 2;
            }
            if(index < 0 || index >= $self->properties.size())
            {
                throw std::out_of_range("GTO Component index out of range");
            }
            return CAST_C_TO_PYOBJECT($self->properties[index], Property);
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, 
                                              $self->properties.size(), 
                                              &start, &stop, &step, &slicelength);
            if(result != 0)
            {
                PyErr_SetString(PyExc_IndexError, "GTO Component invalid slice");
                return NULL;
            }
            PyObject *tuple = PyTuple_New(slicelength);
            for(size_t dataIndex = start, tupleIndex = 0;
                dataIndex < stop; dataIndex += step, ++tupleIndex)
            {
                Property *p = $self->properties[dataIndex];
                PyObject *po = CAST_C_TO_PYOBJECT(p, Property);
                PyTuple_SetItem(tuple, tupleIndex, po);
            }
            return tuple;
        }
        throw std::invalid_argument("GTO Component Invalid index type");
    }


    %feature("autodoc", "0");
    void __setitem__(PyObject *key, PyObject *value)
    {
        Property *prop = NULL;
        CAST_PYOBJECT_TO_C(Property, value, prop);

        if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = $self->properties.size() - index - 2;
            }
            if(index < 0 || index >= $self->properties.size())
            {
                throw std::out_of_range("GTO Component index out of range");
            }
            for(size_t i = 0; i < $self->properties.size(); ++i)
            {
                if($self->properties[i]->name == prop->name)
                {
                    throw std::invalid_argument("GTO Component duplicate property name");
                }
            }
            $self->properties[index] = prop;
            Py_INCREF(value);
        }
        else
        {
            throw std::invalid_argument("GTO Component Invalid index type");
        }
    }


    %feature("autodoc", "0");
    void __delitem__(PyObject *key)
    {
        if( PyString_Check(key) )
        {
            const char *keystr = PyString_AS_STRING(key);
            Properties::iterator piter = $self->properties.begin();
            for(; piter != $self->properties.end(); ++piter)
            {
                if( (*piter)->name == keystr )
                {
                    $self->properties.erase(piter);
                    return;
                }
            }
            throw std::out_of_range("GTO Component KeyError");
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = $self->properties.size() - index - 2;
            }
            if(index < 0 || index >= $self->properties.size())
            {
                throw std::out_of_range("GTO Component index out of range");
            }
            $self->properties.erase($self->properties.begin() + index);
            return;
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, 
                                              $self->properties.size(), &start,
                                              &stop, &step, &slicelength);
            if(result != 0)
            {
                throw std::invalid_argument("GTO Component Invalid slice");
            }
            size_t numErased = 0;
            for( size_t i = start; i < stop; i += step )
            {
                $self->properties.erase($self->properties.begin() + i - numErased);
                ++numErased;
            }
            return;
        }
        throw std::invalid_argument("GTO Component Invalid index type");
    }


    %feature("autodoc", "0");
    bool __contains__(PyObject *item)
    {
        if( PyString_Check(item) )
        {
            const char *itemstr = PyString_AS_STRING(item);
            for(size_t i = 0; i < $self->properties.size(); ++i )
            {
                if( $self->properties[i]->name == itemstr )
                {
                    return true;
                }
            }
        }
        return false;
    }


    %feature("autodoc", "0");
    PyObject *keys()
    {
        PyObject *tuple = PyTuple_New($self->properties.size());
        for(size_t i = 0; i < $self->properties.size(); ++i)
        {
            PyObject *name = PyString_FromString($self->properties[i]->name.c_str());
            PyTuple_SetItem(tuple, i, name);
        }
        return tuple;
    }


    Properties values()
    {
        return $self->properties;
    }


    %feature("autodoc", "0");
    PyObject *items()
    {
        PyObject *tuple = PyTuple_New($self->properties.size());
        for(size_t i = 0; i < $self->properties.size(); ++i)
        {
            PyObject *subtuple = PyTuple_New(2);
            PyObject *name = PyString_FromString($self->properties[i]->name.c_str());
            PyObject *pyProp = CAST_C_TO_PYOBJECT($self->properties[i], Property);
            PyTuple_SetItem(subtuple, 0, name);
            PyTuple_SetItem(subtuple, 1, pyProp);
            PyTuple_SetItem(tuple, i, subtuple);
        }
        return tuple;
    }


    %feature("autodoc", "0");
    void append(PyObject *item)
    {
        Property *prop = NULL;
        CAST_PYOBJECT_TO_C(Property, item, prop);

        for(size_t i = 0; i < $self->properties.size(); ++i)
        {
            if( prop->name == $self->properties[i]->name )
            {
                throw std::invalid_argument("GTO Component duplicate property name");
            }
        }
        Py_INCREF(item);
        $self->properties.push_back(prop);
    }


    %feature("autodoc", "0");
    void extend(PyObject *sequence)
    {
        if(PySequence_Check(sequence) && (! PyString_Check(sequence)))
        {
            for(size_t i = 0; i < PySequence_Length(sequence); ++i )
            {
                PyObject *pyProp = PySequence_GetItem(sequence, i);
                Property *prop = NULL;
                CAST_PYOBJECT_TO_C(Property, pyProp, prop);

                for(size_t i = 0; i < $self->properties.size(); ++i)
                {
                    if( prop->name == $self->properties[i]->name )
                    {
                        throw std::invalid_argument("GTO Component duplicate property name");
                    }
                }
                Py_INCREF(pyProp);
                $self->properties.push_back(prop);
            }
        }
        else
        {
            throw std::invalid_argument("GTO Component invalid sequence");
        }
    }

} //  End %extend Component

#endif // SWIGPYTHON

}; // End struct Component
