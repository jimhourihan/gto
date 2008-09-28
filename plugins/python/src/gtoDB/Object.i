// *****************************************************************************

struct Object
{
    Object(const std::string& n, const std::string& p, unsigned int v)
        : name(n), protocol(p), protocolVersion(v) {}
    ~Object();

    std::string     name;
    std::string     protocol;
    unsigned int    protocolVersion;
};
#ifdef SWIGPYTHON
%extend Object {

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


    long __len__()
    {
        return long($self->components.size());
    }


    long __cmp__(PyObject *other)
    {
        Object *otherObj = NULL;
        PYOBJECT_CAST(Object, other, otherObj);
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
                    return CAST_PYOBJECT(c, Component);
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
            return CAST_PYOBJECT($self->components[index], Component);
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
                PyObject *co = CAST_PYOBJECT(c, Component);
                PyTuple_SetItem(tuple, tupleIndex, co);
            }
            return tuple;
        }
        throw std::invalid_argument("GTO Object Invalid index type");
    }


    void __setitem__(PyObject *key, PyObject *value)
    {
        Component *comp = NULL;
        PYOBJECT_CAST(Component, value, comp);

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

    
    Components values()
    {
        return $self->components;
    }


    void append(PyObject *value)
    {
        Component *comp = NULL;
        PYOBJECT_CAST(Component, value, comp);

        for(size_t i = 0; i < $self->components.size(); ++i)
        {
            if( comp->name == $self->components[i]->name )
            {
                throw std::invalid_argument("GTO Object duplicate component name");
            }
        }
        Py_INCREF(value);
        $self->components.push_back(comp);
    }


    void extend(PyObject *value)
    {
        if(PySequence_Check(value) && (! PyString_Check(value)))
        {
            for(size_t i = 0; i < PySequence_Length(value); ++i )
            {
                PyObject *pyComp = PySequence_GetItem(value, i);
                Component *comp = NULL;
                PYOBJECT_CAST(Component, pyComp, comp);

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

%template(ObjectVector) std::vector<Object *>;
typedef std::vector<std::Object*> Objects;

