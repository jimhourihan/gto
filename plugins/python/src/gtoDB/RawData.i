#ifdef SWIGPYTHON

// *****************************************************************************
%template(StringVector) std::vector<std::string>;
typedef std::vector<std::string> Strings;

%{  // Begin verbatim

void propertyMerge(Component *out, Component *in)
{
    for (size_t i=0; i < in->properties.size(); i++)
    {
        Property *p = in->properties[i];
        bool found = false;

        for (size_t q=0; q < out->properties.size(); q++)
        {
            if (out->properties[q]->name == p->name)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            out->properties.push_back(p);
            in->properties.erase(in->properties.begin() + i);
            i--;
        }
    }
}

void componentMerge(Object *out, Object *in)
{
    for (size_t i=0; i < in->components.size(); i++)
    {
        Component *c = in->components[i];
        bool found = false;

        for (size_t q=0; q < out->components.size(); q++)
        {
            if (out->components[q]->name == c->name)
            {
                found = true;
                propertyMerge(out->components[q], c);
                break;
            }
        }

        if (!found)
        {
            out->components.push_back(c);
            in->components.erase(in->components.begin() + i);
            i--;
        }
    }
}

void objectMerge(RawDataBase *out, RawDataBase *in)
{
    for (size_t i=0; i < in->objects.size(); i++)
    {
        Object *o = in->objects[i];
        bool found = false;

        for (size_t q=0; q < out->objects.size(); q++)
        {
            if (out->objects[q]->name == o->name)
            {
                found = true;
                componentMerge(out->objects[q], o);
                break;
            }
        }

        if (!found)
        {
            out->objects.push_back(o);
            in->objects.erase(in->objects.begin() + i);
            i--;
        }
    }
}
    
    
%} // End verbatim


%ignore GtoDB::m_reader; // Python doesn't need access to this

%inline %{

class GtoDB {
public:
    
    enum FileType
    {
        BinaryGTO,
        CompressedGTO,
        TextGTO
    };
        
    GtoDB() {}
    GtoDB(const char *filename) { read(filename); }

    void read(const char *filename)
    {
        RawDataBaseReader mergeReader;
        mergeReader.open(filename);
        objectMerge(m_reader.dataBase(), mergeReader.dataBase());
    }
    

    void write(const char *filename, FileType mode=CompressedGTO)
    {
        RawDataBaseWriter writer;
        if(m_reader.dataBase()->objects.empty())
        {
            throw std::runtime_error("No objects to write");
        }
        writer.write(filename, *m_reader.dataBase(), 
                     (Writer::FileType)(mode));
    }

public:
    // Must be public so swig class extensions can access it
    RawDataBaseReader m_reader;
};
%} // End %inline
%extend GtoDB {

    long __len__()
    {
        RawDataBase *db = $self->m_reader.dataBase();
        return long(db->objects.size());
    }


    PyObject *__getitem__(PyObject *key)
    {
        RawDataBase *db = $self->m_reader.dataBase();

        if( PyString_Check(key) )
        {
            const char *keystr = PyString_AS_STRING(key);
            for(size_t i = 0; i < db->objects.size(); ++i )
            {
                Gto::Object *o = db->objects[i];
                if( o->name == keystr )
                {
                    return CAST_PYOBJECT(o, Object);
                }
            }
            throw std::out_of_range("GTO DB KeyError");
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = db->objects.size() - index - 2;
            }
            if(index < 0 || index >= db->objects.size())
            {
                throw std::out_of_range("GTO DB index out of range");
            }
            return CAST_PYOBJECT(db->objects[index], Object);
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, db->objects.size(), 
                                              &start, &stop, &step, &slicelength);
            if(result != 0)
            {
                PyErr_SetString(PyExc_IndexError, "GTO DB invalid slice");
                return NULL;
            }
            PyObject *tuple = PyTuple_New(slicelength);
            for(size_t dataIndex = start, tupleIndex = 0;
                dataIndex < stop; dataIndex += step, ++tupleIndex)
            {
                Object *o = db->objects[dataIndex];
                PyObject *po = CAST_PYOBJECT(o, Object);
                PyTuple_SetItem(tuple, tupleIndex, po);
            }
            return tuple;
        }
        throw std::invalid_argument("GTO DB Invalid index type");
    }


    void __setitem__(PyObject *key, PyObject *value)
    {
        RawDataBase *db = $self->m_reader.dataBase();

        Object *obj = NULL;
        PYOBJECT_CAST(Object, value, obj);

        if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = db->objects.size() - index - 2;
            }
            if(index < 0 || index >= db->objects.size())
            {
                throw std::out_of_range("GTO DB index out of range");
            }
            for(size_t i = 0; i < db->objects.size(); ++i)
            {
                if(db->objects[i]->name == obj->name)
                {
                    throw std::invalid_argument("GTO DB duplicate object name");
                }
            }
            db->objects[index] = obj;
            Py_INCREF(value);
        }
        else
        {
            throw std::invalid_argument("GTO DB Invalid index type");
        }
    }


    void __delitem__(PyObject *key)
    {
        RawDataBase *db = $self->m_reader.dataBase();

        if( PyString_Check(key) )
        {
            const char *keystr = PyString_AS_STRING(key);
            Objects::iterator oiter = db->objects.begin();
            for(; oiter != db->objects.end(); ++oiter)
            {
                if( (*oiter)->name == keystr )
                {
                    db->objects.erase(oiter);
                    return;
                }
            }
            throw std::out_of_range("GTO DB KeyError");
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = db->objects.size() - index - 2;
            }
            if(index < 0 || index >= db->objects.size())
            {
                throw std::out_of_range("GTO DB index out of range");
            }
            db->objects.erase(db->objects.begin() + index);
            return;
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, 
                                              db->objects.size(), &start,
                                              &stop, &step, &slicelength);
            if(result != 0)
            {
                throw std::invalid_argument("GTO DB Invalid slice");
            }
            size_t numErased = 0;
            for( size_t i = start; i < stop; i += step )
            {
                db->objects.erase(db->objects.begin() + i - numErased);
                ++numErased;
            }
            return;
        }
        throw std::invalid_argument("GTO DB Invalid index type");
    }


    bool __contains__(PyObject *item)
    {
        RawDataBase *db = $self->m_reader.dataBase();

        if( PyString_Check(item) )
        {
            const char *itemstr = PyString_AS_STRING(item);
            for(size_t i = 0; i < db->objects.size(); ++i )
            {
                if( db->objects[i]->name == itemstr )
                {
                    return true;
                }
            }
        }
        return false;
    }


    PyObject *keys()
    {
        RawDataBase *db = $self->m_reader.dataBase();

        PyObject *tuple = PyTuple_New(db->objects.size());
        for(size_t i = 0; i < db->objects.size(); ++i)
        {
            PyObject *name = PyString_FromString(db->objects[i]->name.c_str());
            PyTuple_SetItem(tuple, i, name);
        }
        return tuple;
    }

    
    Objects values()
    {
        RawDataBase *db = $self->m_reader.dataBase();
        return db->objects;
    }


    PyObject *items()
    {
        RawDataBase *db = $self->m_reader.dataBase();

        PyObject *tuple = PyTuple_New(db->objects.size());
        for(size_t i = 0; i < db->objects.size(); ++i)
        {
            PyObject *subtuple = PyTuple_New(2);
            PyObject *name = PyString_FromString(db->objects[i]->name.c_str());
            PyObject *po = CAST_PYOBJECT(db->objects[i], Object);
            PyTuple_SetItem(subtuple, 0, name);
            PyTuple_SetItem(subtuple, 1, po);
            PyTuple_SetItem(tuple, i, subtuple);
        }
        return tuple;
    }


    void append(PyObject *value)
    {
        RawDataBase *db = $self->m_reader.dataBase();

        Object *obj = NULL;
        PYOBJECT_CAST(Object, value, obj);

        for(size_t i = 0; i < db->objects.size(); ++i)
        {
            if( obj->name == db->objects[i]->name )
            {
                throw std::invalid_argument("GTO DB duplicate object name");
            }
        }
        Py_INCREF(value);
        db->objects.push_back(obj);
    }


    void extend(PyObject *value)
    {
        if(PySequence_Check(value) && (! PyString_Check(value)))
        {
            RawDataBase *db = $self->m_reader.dataBase();
            for(size_t i = 0; i < PySequence_Length(value); ++i )
            {
                PyObject *pyObj = PySequence_GetItem(value, i);
                Object *obj = NULL;
                PYOBJECT_CAST(Object, pyObj, obj);

                for(size_t i = 0; i < db->objects.size(); ++i)
                {
                    if( obj->name == db->objects[i]->name )
                    {
                        throw std::invalid_argument("GtoDB duplicate object name");
                    }
                }
                Py_INCREF(pyObj);
                db->objects.push_back(obj);
            }
        }
        else
        {
            throw std::invalid_argument("GtoDB invalid sequence");
        }
    }


    void dumpInfo()
    {
        RawDataBase *db = $self->m_reader.dataBase();

        Objects::iterator oiter = db->objects.begin();
        for(; oiter != db->objects.end(); ++oiter)
        {
            Object *o = (*oiter);
            printf("object \"%s\" protocol \"%s\" v%d\n", o->name.c_str(), 
                        o->protocol.c_str(), o->protocolVersion);
            Components::iterator citer = o->components.begin();
            for(; citer != o->components.end(); ++citer)
            {
                Component *c = (*citer);
                printf("  component \"%s\"\n", c->name.c_str());
                Properties::iterator piter = c->properties.begin();
                for(; piter != c->properties.end(); ++piter)
                {
                    Property *p = (*piter);
                    const char *typestr = "UNKNOWN";
                    switch (p->type)
                    {
                    case Gto::Int: typestr = "int"; break;
                    case Gto::Float: typestr = "float"; break;
                    case Gto::Double: typestr = "double"; break;
                    case Gto::Half: typestr = "half"; break;
                    case Gto::String: typestr = "string"; break;
                    case Gto::Boolean: typestr = "boolean"; break;
                    case Gto::Short: typestr = "short"; break;
                    case Gto::Byte: typestr = "byte"; break;
                    }
                    printf("    property %s[%u][%u] \"%s\"\n", 
                            typestr, p->width, p->size, p->name.c_str());
                }
            }
        }
    }  //  End void dumpInfo()...
};


#endif // #ifdef SWIGPYTHON
