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


// *****************************************************************************

%ignore GtoDB::m_reader; // Python doesn't need access to this

%define GTODB_DOCSTRING
"Represents a single GTO file which may contain any number of objects.

This class can operate as either a dictionary or a list:
    
    object  = gtoDB[0]
    objects = gtoDB[0:3]
    object  = gtoDB['pSphere1']

    gtoDB[0] = object
    gtoDB[0:3] = objects

   if 'pSphere1' in gtoDB:   
       del gtoDB['pSphere1']

Most standard list/dictionary methods are supported.

If a GtoDB is deleted, it will delete all the Objects it contains.
"
%enddef
%feature("autodoc", GTODB_DOCSTRING);
class GtoDB {

public:

    enum FileType
    {
        BinaryGTO,
        CompressedGTO,
        TextGTO
    };

    %feature("autodoc","1");
    GtoDB();
    GtoDB(std::string filename);

%define GTODB_READ_DOCSTRING
"If called multiple times, this function will merge data
into the existing database.  This is equivalent to the command-
line 'gtomerge' tool.
"
%enddef
    %feature("docstring", GTODB_READ_DOCSTRING);
    void read(std::string filename);
    %feature("docstring", "");

    void write(std::string filename, FileType mode=CompressedGTO);

public:
    // Must be public so swig class extensions can access it
    Gto::RawDataBaseReader m_reader;

#ifdef SWIGPYTHON
%extend {

    %feature("autodoc", "Returns the number of Objects in this database");
    long __len__()
    {
        RawDataBase *db = $self->m_reader.dataBase();
        return long(db->objects.size());
    }


    %feature("autodoc", "0");
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
                    return CAST_C_TO_PYOBJECT(o, Object);
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
            return CAST_C_TO_PYOBJECT(db->objects[index], Object);
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
                PyObject *po = CAST_C_TO_PYOBJECT(o, Object);
                PyTuple_SetItem(tuple, tupleIndex, po);
            }
            return tuple;
        }
        throw std::invalid_argument("GTO DB Invalid index type");
    }


    %feature("autodoc", "0");
    void __setitem__(PyObject *key, PyObject *value)
    {
        RawDataBase *db = $self->m_reader.dataBase();

        Object *obj = NULL;
        CAST_PYOBJECT_TO_C(Object, value, obj);

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


    %feature("autodoc", "0");
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


    %feature("autodoc", "0");
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


    %feature("autodoc", "0");
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

    
    %feature("autodoc", "0");
    Objects values()
    {
        RawDataBase *db = $self->m_reader.dataBase();
        return db->objects;
    }


    %feature("autodoc", "0");
    PyObject *items()
    {
        RawDataBase *db = $self->m_reader.dataBase();

        PyObject *tuple = PyTuple_New(db->objects.size());
        for(size_t i = 0; i < db->objects.size(); ++i)
        {
            PyObject *subtuple = PyTuple_New(2);
            PyObject *name = PyString_FromString(db->objects[i]->name.c_str());
            PyObject *po = CAST_C_TO_PYOBJECT(db->objects[i], Object);
            PyTuple_SetItem(subtuple, 0, name);
            PyTuple_SetItem(subtuple, 1, po);
            PyTuple_SetItem(tuple, i, subtuple);
        }
        return tuple;
    }


    %feature("autodoc", "0");
    void append(PyObject *item)
    {
        RawDataBase *db = $self->m_reader.dataBase();

        Object *obj = NULL;
        CAST_PYOBJECT_TO_C(Object, item, obj);

        for(size_t i = 0; i < db->objects.size(); ++i)
        {
            if( obj->name == db->objects[i]->name )
            {
                throw std::invalid_argument("GTO DB duplicate object name");
            }
        }
        Py_INCREF(item);
        db->objects.push_back(obj);
    }


    %feature("autodoc", "0");
    void extend(PyObject *sequence)
    {
        if(PySequence_Check(sequence) && (! PyString_Check(sequence)))
        {
            RawDataBase *db = $self->m_reader.dataBase();
            for(size_t i = 0; i < PySequence_Length(sequence); ++i )
            {
                PyObject *pyObj = PySequence_GetItem(sequence, i);
                Object *obj = NULL;
                CAST_PYOBJECT_TO_C(Object, pyObj, obj);

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

    %feature("autodoc", "0");
    void clear()
    {
        RawDataBase *db = $self->m_reader.dataBase();
        db->objects.clear();
        db->strings.clear();
    }


    // This doesn't work for some reason.  Always returns
    // an empty list.  Seems to be busted in Gto::RawData.
#if 0
    %feature("autodoc", "Returns the GTO string table.");
    const Strings &strings()
    {
        RawDataBase *db = $self->m_reader.dataBase();
        return db->strings;
    }
#endif
};

#endif // #ifdef SWIGPYTHON

}; // End class GtoDB


