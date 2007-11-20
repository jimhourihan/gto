#ifndef __Gto__RawData__h__
#define __Gto__RawData__h__
//
// Copyright (C) 2003  Tweak Films
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
#include <Gto/Header.h>
#include <Gto/Reader.h>
#include <Gto/Writer.h>
#include <list>
#include <string>

namespace Gto {

//
//  These classes implement a "raw" database of Gto data. Its mostly
//  useful for basic gto munging. The data from the gto file is read
//  into a hierarchy of arrays. This structure is *not* recommended
//  for use beyond very simple applications. It will also be useful as
//  an example of how the Reader/Writer are intended to be used.
//

//----------------------------------------------------------------------

struct Property
{
    Property(const std::string& n,
             const std::string& i,
             Gto::DataType t,
             size_t s,
             size_t w,
             bool allocate=false);

    Property(const std::string& n,
             Gto::DataType t,
             size_t s,
             size_t w,
             bool allocate=false);

    ~Property();

    std::string     name;
    std::string     interp;
    Gto::DataType   type;
    size_t          size;
    size_t          width;

    union
    {
        float*          floatData;
        double*         doubleData;
        int32*          int32Data;
        uint16*         uint16Data;
        uint8*          uint8Data;
        std::string*    stringData;

        void*           voidData;
    };
};

typedef std::vector<Property*> Properties;

//----------------------------------------------------------------------

struct Component
{
    Component(const std::string& n, 
              const std::string& i, 
              uint16 f) : name(n), interp(i), flags(f) {}
    Component(const std::string& n, uint16 f) : name(n), flags(f) {}

    ~Component();

    std::string     name;
    std::string     interp;
    uint16          flags;
    Properties      properties;
};

typedef std::vector<Component*> Components;

//----------------------------------------------------------------------

struct Object
{
    Object(const std::string& n, const std::string& p, unsigned int v)
        : name(n), protocol(p), protocolVersion(v) {}
    ~Object();

    std::string     name;
    std::string     protocol;
    unsigned int    protocolVersion;
    Components      components;
};

typedef std::vector<Object*> Objects;

//----------------------------------------------------------------------

typedef std::vector<std::string> Strings;

struct RawDataBase
{
    ~RawDataBase();

    Objects         objects;
    Strings         strings;
};

//----------------------------------------------------------------------

//
//  class RawDataBaseReader
//
//  Call open() with a filename (from the base class) to have it read
//  the data. This class initializes the Reader base class to not own
//  the data. This is a bit unusual. Normally you'd copy the data the
//  reader gives you; but in this case, since its raw anyway we'll
//  just take the data and keep it.
//
//  You need to delete the RawDataBase that's returned from the dataBase()
//  function. The RawDataBaseReader will not delete it.
//

class RawDataBaseReader : public Reader
{
public:
    explicit RawDataBaseReader(unsigned int mode = None);
    virtual ~RawDataBaseReader();

    virtual bool        open(const char *filename);
    virtual bool        open(std::istream&, const char *name);

    RawDataBase*        dataBase() { return m_dataBase; }

protected:
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

    virtual void*       data(const PropertyInfo&, size_t bytes);
    virtual void        dataRead(const PropertyInfo&);

protected:
    RawDataBase*        m_dataBase;
};


//----------------------------------------------------------------------

//
//  class RawDataBaseWriter
//
//  The uses a Writer class (instead of being one).
//

class RawDataBaseWriter
{
public:
    RawDataBaseWriter() : m_writer() {}

    bool            write(const char *filename, const RawDataBase&, 
                          Writer::FileType type=Writer::CompressedGTO);

    bool            write(const char *f, const RawDataBase& db, bool c)
                    { return write(f, db, c ? Writer::CompressedGTO : Writer::BinaryGTO); }
    
    void            close() { m_writer.close(); }
    
private:
    void            writeComponent(bool header, const Component*);
    void            writeProperty(bool header, const Property*);

private:
    Writer          m_writer;
};

} // namespace Gto

#endif // __Gto__RawData__h__
