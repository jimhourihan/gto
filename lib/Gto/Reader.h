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
#ifndef __Gto__Reader__h__
#define __Gto__Reader__h__
#include <string>
#include <iostream>
#include <string>
#include <Gto/Header.h>
#include <vector>

namespace Gto {

//
//  class Reader
//
//  Reads a GTO file as a byte stream or with random access. If compiled
//  with GTO_SUPPORT_ZIP defined, it can read gzipped gto files directly.
//

class Reader
{
public:

    //
    //  Types
    //

    struct ObjectInfo : ObjectHeader
    {
        void*               objectData; // whatever was returned from object()

    private:
        int                 coffset;
        bool                requested;
        friend class Reader;
    };

    struct ComponentInfo : ComponentHeader
    {
        void*               componentData; // return value of component()
        const ObjectInfo*   object;

    private:
        int                 poffset;
        bool                requested;
        friend class Reader;
    };

    struct PropertyInfo : PropertyHeader
    {
        void*                propertyData;
        int                  offset;    // file offset
        const ComponentInfo* component;

    private:
        bool                 requested;
        friend class Reader;
    };

    typedef std::vector<std::string>   StringTable;
    typedef std::vector<ObjectInfo>    Objects;
    typedef std::vector<ComponentInfo> Components;
    typedef std::vector<PropertyInfo>  Properties;

    //
    //  The open modes:
    //
    //  None: the file is read as a stream with default API behavior.
    //
    //  HeaderOnly: only the header information will be read. This is
    //  equivalent to requesting none of the data, but it will short
    //  circuit the reader into finishing successfully without seeking
    //  through the file.
    //
    //  RandomAccess: the file should be prepared for random access: no
    //  header functions or data will be called until you specifically ask
    //  for an object by name -- then they will called as if the file
    //  contained only that data. You can do this as many times as you
    //  want using the accessObject() function.
    //

    enum ReadMode
    {
        None             = 0,
        HeaderOnly       = 1 << 0,
        RandomAccess     = 1 << 1,
    };

    Reader(unsigned int mode = None);
    virtual ~Reader();

    //
    //  Open takes the path to the .gto file. If the GTO_SUPPORT_ZIP
    //  is enabled then open will automatically attempt to find a
    //  gziped version of the file if the file does not exist.
    //
    //  If the mode is RandomAccess, then open will return having only read
    //  the header information.
    //

    virtual bool        open(const char *filename);
    virtual bool        open(std::istream&, const char *name);
    void                close();

    //
    //  If it failed. why() will return a description.
    //
    void                fail( std::string why = "" );

    const std::string&  why() const { return m_why; }

    const std::string&  stringFromId(unsigned int i);
    const StringTable&  stringTable() { return m_strings; }

    bool                isSwapped() const { return m_swapped; }

    //
    //  This function is called right after the file header is read.
    //

    virtual void        header(const Header&);

    //
    //  RandomAccess functions. When the file is openned as RandomAccess,
    //  you can get ahold of an ObjectInfo and pass it to this function to
    //  have the reader seek directly.
    //
    //  accessObject() requires that the reference passed in be from the
    //  reader's object cache, not a copy.
    //
    //  The components() and properties() functions are there to make it
    //  easy to take parts for later retrieval with accessObject()
    //

    Objects&            objects() { return m_objects; }
    bool                accessObject(ObjectInfo&);

    Components&         components() { return m_components; }
    Properties&         properties() { return m_properties; }

    //
    //  These are used to declare a component or property. The
    //  functions are called expecting the return value to be non-zero
    //  (or your own pointer) if the reader should try and read the
    //  data associated with the component or property -- the default
    //  is non-zero.
    //

    struct Request
    {
        Request() : m_want(true), m_data(0) {}
        Request(bool want, void* data = 0)
          : m_want( want ), m_data( data ) {}

    private:
        bool    m_want;
        void*   m_data;
        friend class Reader;
    };

    //
    //  Version 3 API
    //

    virtual Request     object(const std::string& name,
                               const std::string& protocol,
                               unsigned int protocolVersion,
                               const ObjectInfo &header);


    virtual Request     component(const std::string& name,
                                  const std::string& interp,
                                  const ComponentInfo &header);


    virtual Request     property(const std::string& name,
                                 const std::string& interp,
                                 const PropertyInfo &header);

    //
    //  Version 2 API still works, if the version 3 API (above) is not
    //  overriden. New applications should not use these functions.
    //

    virtual Request     component(const std::string& name,
                                  const ComponentInfo &header);

    virtual Request     property(const std::string& name,
                                 const PropertyInfo &header);

    //
    //  Data should return a pointer to a region of memory large
    //  enough to hold the data that will be read for
    //  PropertyInfo. The bytes parameter tells you how much memory
    //  (in bytes) will be required
    //

    virtual void*       data(const PropertyInfo&, size_t bytes);

    //
    //  dataRead() (read "data red") is called after the data is
    //  succesfully read (so after the data() function is called)
    //

    virtual void        dataRead(const PropertyInfo&);

private:
    bool                read();
    void                readHeader();
    void                readStringTable();
    void                readObjects();
    void                readComponents();
    void                readProperties();
    bool                readProperty(PropertyInfo&);

    void                read(char *, size_t);
    void                get(char &);
    bool                notEOF();
    void                seekForward(size_t);
    int                 tell();
    void                seekTo(size_t);

private:
    Header              m_header;
    Objects             m_objects;
    Components          m_components;
    Properties          m_properties;
    StringTable         m_strings;
    std::istream*       m_in;
    void*               m_gzfile;
    int                 m_gzrval;
    std::string         m_inName;
    bool                m_needsClosing;
    bool                m_error;
    std::string         m_why;
    bool                m_swapped;
    unsigned int        m_mode;
};

} // Gto

#endif // __Gto__Reader__h__
