// *****************************************************************************

struct Component
{
    Component(const std::string& n, const std::string& i, int f) 
               : name(n), interp(i), flags(f) {}
    Component(const std::string& n, int f) : name(n), flags(f) {}


    std::string     name;
    std::string     interp;
    unsigned short  flags;
};
#ifdef SWIGPYTHON
%extend Component {

    Component(const std::string n, const std::string i)
    {
        return new Component(n, i, 0);
    }


    Component(const std::string& n)
    {
        return new Component(n, "", 0);
    }


    char *__repr__() 
    {
        static char tmp[256];
        snprintf(tmp, 256, "<GTO Component '%s' at %p>", 
                 $self->name.c_str(),
                 $self
                );
        return tmp;
    }


    long __len__()
    {
        return long($self->properties.size());
    }


    long __cmp__(PyObject *other)
    {
        Component *otherComp = NULL;
        PYOBJECT_CAST(Component, other, otherComp);
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
                    return CAST_PYOBJECT(p, Property);
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
            return CAST_PYOBJECT($self->properties[index], Property);
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
                PyObject *po = CAST_PYOBJECT(p, Property);
                PyTuple_SetItem(tuple, tupleIndex, po);
            }
            return tuple;
        }
        throw std::invalid_argument("GTO Component Invalid index type");
    }


    void __setitem__(PyObject *key, PyObject *value)
    {
        Property *prop = NULL;
        PYOBJECT_CAST(Property, value, prop);

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


    PyObject *items()
    {
        PyObject *tuple = PyTuple_New($self->properties.size());
        for(size_t i = 0; i < $self->properties.size(); ++i)
        {
            PyObject *subtuple = PyTuple_New(2);
            PyObject *name = PyString_FromString($self->properties[i]->name.c_str());
            PyObject *pyProp = CAST_PYOBJECT($self->properties[i], Property);
            PyTuple_SetItem(subtuple, 0, name);
            PyTuple_SetItem(subtuple, 1, pyProp);
            PyTuple_SetItem(tuple, i, subtuple);
        }
        return tuple;
    }


    void append(PyObject *value)
    {
        Property *prop = NULL;
        PYOBJECT_CAST(Property, value, prop);

        for(size_t i = 0; i < $self->properties.size(); ++i)
        {
            if( prop->name == $self->properties[i]->name )
            {
                throw std::invalid_argument("GTO Component duplicate property name");
            }
        }
        Py_INCREF(value);
        $self->properties.push_back(prop);
    }


    void extend(PyObject *value)
    {
        if(PySequence_Check(value) && (! PyString_Check(value)))
        {
            for(size_t i = 0; i < PySequence_Length(value); ++i )
            {
                PyObject *pyProp = PySequence_GetItem(value, i);
                Property *prop = NULL;
                PYOBJECT_CAST(Property, pyProp, prop);

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


typedef std::vector<Component*> Components;
%template(ComponentVector) std::vector<Component *>;
