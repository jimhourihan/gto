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

%define OBJECT_DOCSTRING
"Represents a single GTO Object which may contain any number of 
components.  Information about the object itself is had via
the attributes name, protocol, and protocolVersion.

This class can operate as either a dictionary or a list:

   component  = object[0]
   component  = object['points'] 
   components = object[0:2]
   numComponents = len(object)

   if 'points' in object:   
       del object['points']

Most standard list/dictionary methods are supported.

If an Object is deleted, it will delete all the Components it contains."
%enddef
%feature("autodoc", OBJECT_DOCSTRING);
struct Object
{
    %feature("autodoc", "1");
    Object(const std::string& name, const std::string& protocol, unsigned int protocolVersion)
        : name(name), protocol(protocol), protocolVersion(protocolVersion) {}
    ~Object();

    std::string     name;
    std::string     protocol;
    unsigned int    protocolVersion;

#ifdef SWIGPYTHON
%extend {

    %feature("autodoc", "0");
    char *__repr__() {
        static char tmp[256];
        snprintf(tmp, 256, "<GTO Object '%s' protocol '%s' v%d at %p>", 
                 $self->name.c_str(),
                 $self->protocol.c_str(),
                 $self->protocolVersion,
                 $self
                );
        return tmp;
    }


    %feature("autodoc", "Returns the number of Components in this object");
    long __len__()
    {
        return long($self->components.size());
    }


    %feature("autodoc", "Compares object names *only*");
    long __cmp__(PyObject *other)
    {
        Object *otherObj = NULL;
        CAST_PYOBJECT_TO_C(Object, other, otherObj);
        if($self == otherObj)
        {
            return 0;
        }
        else if(otherObj->name > $self->name)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    %feature("autodoc", "");


    %feature("autodoc", "0");
    PyObject *__getitem__(PyObject *key)
    {
        if( PyString_Check (key) )
        {
            const char *keystr = PyString_AS_STRING(key);
            for(size_t i = 0; i < $self->components.size(); ++i )
            {
                Gto::Component *c = $self->components[i];
                if( c->name == keystr )
                {
                    return CAST_C_TO_PYOBJECT(c, Component);
                }
            }
            throw std::out_of_range("GTO Object KeyError");
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = $self->components.size() - index - 2;
            }
            if(index < 0 || index >= $self->components.size())
            {
                throw std::out_of_range("GTO Object index out of range");
            }
            return CAST_C_TO_PYOBJECT($self->components[index], Component);
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, 
                                              $self->components.size(), 
                                              &start, &stop, &step, &slicelength);

            std::cerr << "start: " << start << std::endl;
            std::cerr << "stop: " << stop << std::endl;
            std::cerr << "step: " << step << std::endl;
            std::cerr << "slicelen: " << slicelength << std::endl;

            if(result != 0)
            {
                PyErr_SetString(PyExc_IndexError, "GTO Object invalid slice");
                return NULL;
            }
            PyObject *tuple = PyTuple_New(slicelength);
            for(size_t dataIndex = start, tupleIndex = 0;
                dataIndex < stop; dataIndex += step, ++tupleIndex)
            {
                Component *c = $self->components[dataIndex];
                PyObject *co = CAST_C_TO_PYOBJECT(c, Component);
                PyTuple_SetItem(tuple, tupleIndex, co);
            }
            return tuple;
        }
        throw std::invalid_argument("GTO Object Invalid index type");
    }


    %feature("autodoc", "0");
    void __setitem__(PyObject *key, PyObject *value)
    {
        Component *comp = NULL;
        CAST_PYOBJECT_TO_C(Component, value, comp);

        if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = $self->components.size() - index - 2;
            }
            if(index < 0 || index >= $self->components.size())
            {
                throw std::out_of_range("GTO Object index out of range");
            }
            for(size_t i = 0; i < $self->components.size(); ++i)
            {
                if($self->components[i]->name == comp->name)
                {
                    throw std::invalid_argument("GTO Object duplicate component name");
                }
            }
            $self->components[index] = comp;
            Py_INCREF(value);
        }
        else
        {
            throw std::invalid_argument("GTO Object Invalid index type");
        }
    }


    %feature("autodoc", "0");
    void __delitem__(PyObject *key)
    {
        if( PyString_Check(key) )
        {
            const char *keystr = PyString_AS_STRING(key);
            Components::iterator citer = $self->components.begin();
            for(; citer != $self->components.end(); ++citer)
            {
                if( (*citer)->name == keystr )
                {
                    $self->components.erase(citer);
                    return;
                }
            }
            throw std::out_of_range("GTO Object KeyError");
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = $self->components.size() - index - 2;
            }
            if(index < 0 || index >= $self->components.size())
            {
                throw std::out_of_range("GTO Object index out of range");
            }
            $self->components.erase($self->components.begin() + index);
            return;
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, 
                                              $self->components.size(), &start,
                                              &stop, &step, &slicelength);
            if(result != 0)
            {
                throw std::invalid_argument("GTO Object Invalid slice");
            }
            size_t numErased = 0;
            for( size_t i = start; i < stop; i += step )
            {
                $self->components.erase($self->components.begin() + i - numErased);
                ++numErased;
            }
            return;
        }
        throw std::invalid_argument("GTO Object Invalid index type");
    }


    %feature("autodoc", "0");
    bool __contains__(PyObject *item)
    {
        if( PyString_Check(item) )
        {
            const char *itemstr = PyString_AS_STRING(item);
            for(size_t i = 0; i < $self->components.size(); ++i )
            {
                if( $self->components[i]->name == itemstr )
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
        PyObject *tuple = PyTuple_New($self->components.size());
        for(size_t i = 0; i < $self->components.size(); ++i)
        {
            PyObject *name = PyString_FromString($self->components[i]->name.c_str());
            PyTuple_SetItem(tuple, i, name);
        }
        return tuple;
    }

    
    %feature("autodoc", "0");
    Components values()
    {
        return $self->components;
    }


    %feature("autodoc", "0");
    void append(PyObject *item)
    {
        Component *comp = NULL;
        CAST_PYOBJECT_TO_C(Component, item, comp);

        for(size_t i = 0; i < $self->components.size(); ++i)
        {
            if( comp->name == $self->components[i]->name )
            {
                throw std::invalid_argument("GTO Object duplicate component name");
            }
        }
        Py_INCREF(item);
        $self->components.push_back(comp);
    }


    %feature("autodoc", "0");
    void extend(PyObject *sequence)
    {
        if(PySequence_Check(sequence) && (! PyString_Check(sequence)))
        {
            for(size_t i = 0; i < PySequence_Length(sequence); ++i )
            {
                PyObject *pyComp = PySequence_GetItem(sequence, i);
                Component *comp = NULL;
                CAST_PYOBJECT_TO_C(Component, pyComp, comp);

                for(size_t i = 0; i < $self->components.size(); ++i)
                {
                    if( comp->name == $self->components[i]->name )
                    {
                        throw std::invalid_argument("GTO Object duplicate component name");
                    }
                }
                Py_INCREF(pyComp);
                $self->components.push_back(comp);
            }
        }
        else
        {
            throw std::invalid_argument("GTO Object invalid sequence");
        }
    }

   
}   //  End %extend Object

#endif // SWIGPYTHON

}; // End struct Object
