//******************************************************************************
// Copyright (c) 2003 Tweak Films. 
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

#ifndef __Gto__Writer__h__
#define __Gto__Writer__h__
#include <Gto/Header.h>
#include <Gto/Utilities.h>
#include <assert.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace Gto {

//
//  class Gto::Writer
//
//  Typically, you'll make a write on the stack and call its
//  functions. This class is not intended to be inherited from.
//

class Writer
{
public:
    typedef std::map<std::string, int>      StringMap;
    typedef std::vector<std::string>        StringVector;
    typedef std::vector<ComponentHeader>    Components;
    typedef std::vector<PropertyHeader>     Properties;
    typedef std::vector<ObjectHeader>       Objects;

    Writer();
    Writer(std::ostream&);
    ~Writer();

    //
    //  Optional open function which takes a filename
    //

    bool            open(const char* filename);

    //
    //  Close stream if applicable.
    //

    void            close();

    //
    //  Each object in the file has both a name and protocol. The
    //  protocol is a user defined string which tells software how to
    //  interpret the object data. Examples are: "catmull-clark",
    //  "polygon", "loop", "NURBS", "particle".
    //

    void            beginObject(const char *name, 
                                const char *protocol,
                                unsigned int protocolVersion);

    //
    //  beginComponent() -- declare a component. This can only be
    //  called after begin() and before end()
    //

    void            beginComponent(const char* name, unsigned int flags=0);

    //
    //  delcare a property of a component
    //

    void            property(const char* name,
                             Gto::DataType,
                             size_t numElements,
                             size_t partsPerElement=1);

    void            endComponent();

    //
    //  End object
    //

    void            endObject();

    //
    //  String table entries --- if you have a string property, you
    //  need to add all the strings. Its ok to add them multiple
    //  times.
    //

    void            intern(const char*);
    void            intern(const std::string&);

    int             lookup(const char*) const;
    int             lookup(const std::string&) const;

    //
    //  Finish of declaration section
    //

    void            beginData();

    //
    //  data -- these must be called in the same order as the
    //  declaration section above. One of the propertyData..()
    //  functions should be called once for each property declared.
    //  The additional arguments are for sanity checking -- if you
    //  supply them, the writer will check to see that the expected
    //  data matches.
    //

    void            propertyDataRaw(const void* data,
                                    const char *propertyName=0,
                                    int size=0, 
                                    int width=0);
    
    template<typename T>
    void            propertyData(const T *data, 
                                 const char *propertyName=0,
                                 int size=0, 
                                 int width=0);

    template<typename T>
    void            propertyData(const std::vector<T>& data, 
                                 const char *propertyName=0,
                                 int size=0, 
                                 int width=0);

    template<class T>
    void            propertyDataInContainer(const T &container,
                                            const char *propertyName=0,
                                            int size=0, 
                                            int width=0);

    void            endData();

    //
    //  Previously declared property data
    //

    const Properties& properties() const { return m_properties; }

private:
    void            init(std::ostream*);
    void            constructStringTable();
    void            writeHead();

    void            write(const char *, size_t);

    bool            propertySanityCheck(const char*, int, int);

private:
    std::ostream*   m_out;
    void*           m_gzfile;
    Objects         m_objects;
    Components      m_components;
    Properties      m_properties;
    StringVector    m_names;
    StringMap       m_strings;
    bool            m_needsClosing;
    bool            m_error;
    bool            m_tableFinished;
    std::string     m_outName;
    size_t          m_currentProperty;
};

template<typename T>
void Writer::propertyData(const T *data, 
                          const char *propertyName, 
                          int size, 
                          int width)
{
    size_t p = m_currentProperty++;
    size_t n = m_properties[p].size * m_properties[p].width;
    assert(dataSize(m_properties[p].type) == sizeof(T));

    if (propertySanityCheck(propertyName, size, width))
    {
        write((const char*)data, sizeof(T) * n);
    }
}

template<class T>
void Writer::propertyData(const std::vector<T>& container,
                          const char *propertyName, 
                          int size, 
                          int width)
{
    //
    //  Assumes vector implementation uses contiguous
    //  storage. Currently there are no common implementations for
    //  which this is not true and the upcoming ANSI C++ revision will
    //  make this a requirement of the implementation.
    //

    propertyData(&container.front(), propertyName, size, width);
}


template<class T>
void Writer::propertyDataInContainer(const T &container,
                                     const char *propertyName, 
                                     int size, 
                                     int width)
{
    typedef typename T::value_type value_type;
    typedef typename T::const_iterator iterator;

    std::vector<value_type> data(container.size());
    std::copy(container.begin(), container.end(), data.begin());
    propertyData(data, propertyName, size, width);
}

} // Gto

#endif // __Gto__Writer__h__
