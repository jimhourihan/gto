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

#include "Reader.h"
#include "Utilities.h"
#include <fstream>
#include <assert.h>
#ifdef GTO_SUPPORT_ZIP
#include <zlib.h>
#endif

#if ( __GNUC__ == 2 )
    #define IOS_CUR ios::cur
    #define IOS_BEG ios::beg
#else
    #define IOS_CUR ios_base::cur
    #define IOS_BEG ios_base::beg
#endif

namespace Gto {
using namespace std;

Reader::Reader(unsigned int mode) 
    : m_in(0), 
      m_gzfile(0), 
      m_gzrval(0), 
      m_error(0), 
      m_needsClosing(false),
      m_mode(mode)
{
}

Reader::~Reader()
{
    close();
}

bool
Reader::open(istream& i, const char *name)
{
    if (m_in || m_gzfile) close();

    m_in            = &i;
    m_needsClosing  = false;
    m_inName        = name;
    m_error         = 0;

    return read();
}

bool
Reader::open(const char *filename)
{
    if (m_in) return false;

    m_inName = filename;
    size_t i = m_inName.find(".gz", 0, 3);

    if (i == (strlen(filename) - 3))
    {
#ifdef GTO_SUPPORT_ZIP
        m_gzfile = gzopen(filename, "rb");
        
        if (!m_gzfile)
        {
            m_error = 1;
            m_why = "file open failed";
            return false;
        }
#else
        m_error = 1;
        m_why = "this library was not compiled with zlib support";
        return false;
#endif
    }
    else
    {
#ifdef GTO_SUPPORT_ZIP
        m_gzfile = gzopen(filename, "rb");

        if (!m_gzfile)
        {
            //
            //  Try .gz version before giving up completely
            //

            std::string temp(filename);
            temp += ".gz";
            return open(temp.c_str());
        }
#else
        m_in = new ifstream(filename, ios::in|ios::binary);
        
        if ( !(*m_in) )
        {
            m_in = 0;
            m_error = 1;
            m_why = "stream failed to open";
            return false;
        }
#endif
    }

    m_needsClosing = true;
    m_error = 0;
    return read();
}

void
Reader::close()
{
    if (m_needsClosing) 
    {
        delete m_in;
        m_in = 0;
#ifdef GTO_SUPPORT_ZIP

        if (m_gzfile) 
        {
            gzclose(m_gzfile);
            m_gzfile = 0;
        }
#endif
    }
}

void Reader::header(const Header&) {}
Reader::Request Reader::object(const string&, const string&, unsigned int,
                    const ObjectInfo &) { return Request(true); }
Reader::Request Reader::component(const string&, const ComponentInfo &) { return Request(true); }
Reader::Request Reader::property(const string&, const PropertyInfo &) { return Request(true); }
void* Reader::data(const PropertyInfo&, size_t) { return 0; }
void Reader::dataRead(const PropertyInfo&) {}

static void
swapWords(void *data, size_t size)
{
    struct bytes { char c[4]; };

    bytes* ip = reinterpret_cast<bytes*>(data);

    for (int i=0; i<size; i++)
    {
        bytes temp = ip[i];
        ip[i].c[0] = temp.c[3];
        ip[i].c[1] = temp.c[2];
        ip[i].c[2] = temp.c[1];
        ip[i].c[3] = temp.c[0];
    }
}

static void
swapShorts(void *data, size_t size)
{
    struct bytes { char c[2]; };

    bytes* ip = reinterpret_cast<bytes*>(data);

    for (int i=0; i<size; i++)
    {
        bytes temp = ip[i];
        ip[i].c[0] = temp.c[1];
        ip[i].c[1] = temp.c[0];
    }
}

void
Reader::readHeader()
{
    read((char*)&m_header, sizeof(Header));
    if (m_error) return;

    m_swapped = false;

    if (m_header.magic == Header::Cigam)
    {
        m_swapped = true;
        swapWords(&m_header, sizeof(m_header) / sizeof(int));
    }
    else if (m_header.magic != Header::Magic)
    {
        m_error = 2;
        m_why = "bad magic number";
        return;
    }

    if (m_header.version != GTO_VERSION)
    {
        m_error = 1;
        m_why = "version mismatch";
        cerr << "ERROR: Gto::Reader: gto file version == " 
             << m_header.version << ", but I can only read v"
             << GTO_VERSION << " files\n";
        return;
    }

    header(m_header);
}

void
Reader::readStringTable()
{
    for (int i=0; i < m_header.numStrings; i++)
    {
        string s;
        char c;

        for (get(c); c && notEOF(); get(c))
        {
            s.push_back(c);
        }

        m_strings.push_back(s);
    }
}

void
Reader::readObjects()
{
    int coffset = 0;

    for (int i=0; i < m_header.numObjects; i++)
    {
        ObjectInfo o;
        read((char*)&o, sizeof(ObjectHeader));
        if (m_error) return;

        if (m_swapped) swapWords(&o, sizeof(ObjectHeader) / sizeof(int));

        stringFromId(o.name);
        stringFromId(o.protocolName);
        o.coffset = coffset;
        coffset += o.numComponents;

        if (m_error)
        {
            o.requested = false;
            return;
        }
        else if (!(m_mode & RandomAccess))
        {
            Request r = object( stringFromId(o.name), 
                                stringFromId(o.protocolName), 
                                o.protocolVersion,
                                o);

            o.requested  = r.m_want;
            o.objectData = r.m_data;
        }

        m_objects.push_back(o);
    }
}


void
Reader::readComponents()
{
    int poffset = 0;

    for (int i=0; i < m_objects.size(); i++)
    {
        const ObjectInfo &o = m_objects[i];

        for (int q=0; q < o.numComponents; q++)
        {
            ComponentInfo c;
            read((char*)&c, sizeof(ComponentHeader));
            if (m_error) return;

            if (m_swapped) swapWords(&c, sizeof(ComponentHeader) / sizeof(int));

            c.object = &o;
            c.poffset = poffset;
            poffset += c.numProperties;

            if (o.requested && !(m_mode & RandomAccess))
            {
                stringFromId(c.name);
                if (m_error) return;

                Request r = component(stringFromId(c.name), c);
                c.requested = r.m_want;
                c.componentData = r.m_data;
            }
            else
            {
                c.requested = false;
            }

            m_components.push_back(c);
        }
    }
}

void
Reader::readProperties()
{
    for (int i=0; i < m_components.size(); i++)
    {
        const ComponentInfo &c = m_components[i];

        for (int q=0; q < c.numProperties; q++)
        {
            PropertyInfo p;
            
            read((char*)&p, sizeof(PropertyHeader));
            if (m_error) return;

            if (m_swapped) 
            {
                swapWords(&p, sizeof(PropertyHeader) / sizeof(int));
            }
            
            p.component = &c;

            if (c.requested && !(m_mode & RandomAccess))
            {
                stringFromId(p.name);
                if (m_error) return;

                Request r = property(stringFromId(p.name), p);
                p.requested = r.m_want;
                p.propertyData = r.m_data;
            }
            else
            {
                p.requested = false;
            }

            m_properties.push_back(p);
        }
    }
}

bool
Reader::accessObject(ObjectInfo& o)
{
    Request r = object( stringFromId(o.name), 
                        stringFromId(o.protocolName), 
                        o.protocolVersion,
                        o);
    
    o.requested  = r.m_want;
    o.objectData = r.m_data;

    if (o.requested)
    {
        for (int q=0; q < o.numComponents; q++)
        {
            assert( (o.coffset+q) < m_components.size() );
            ComponentInfo& c = m_components[o.coffset + q];
            const std::string &nme = stringFromId( c.name );
            Request        r = component( nme, c);
            c.requested      = r.m_want;
            c.componentData  = r.m_data;

            if (c.requested)
            {
                for (int j=0; j < c.numProperties; j++)
                {
                    PropertyInfo& p = m_properties[c.poffset + j];
                    Request       r = property(stringFromId(p.name), p);
                    p.requested     = r.m_want;
                    p.propertyData  = r.m_data;

                    if (p.requested)
                    {
                        seekTo(p.offset);
                        readProperty(p);
                    }
                }
            }
        }
    }

    return true;
}

bool
Reader::read()
{
    readHeader();           if (m_error) return false; 
    readStringTable();      if (m_error) return false;
    readObjects();          if (m_error) return false;
    readComponents();       if (m_error) return false;
    readProperties();       if (m_error) return false;

    if (m_mode & HeaderOnly)
    {
        return true;
    }

    int propIndex = 0;

    for (int i=0; i < m_components.size(); i++)
    {
        ComponentInfo &comp = m_components[i];

        if (comp.flags & Gto::Transposed)
        {
            cerr << "ERROR: Transposed data for '"
                 << stringFromId( comp.object->name ) << "."
                 << stringFromId( comp.name ) 
                 << "' is currently unsupported." << endl;
            abort();
        }
        else
        {
            for (int q=propIndex; 
                 q < (propIndex + comp.numProperties); 
                 q++)
            {
                if (!readProperty(m_properties[q]))
                {
                    return false;
                }
            }
        }

        propIndex += comp.numProperties;
    }

    return true;
}

bool
Reader::readProperty(PropertyInfo& prop)
{
    size_t num   = prop.size * prop.width;
    size_t bytes = num * dataSize(prop.type);
    char* buffer = 0;

    //
    //  Cache the offset pointer
    //

    prop.offset = tell();
    bool readok = false;

    if (prop.requested)
    {
        if (buffer = (char*)data(prop, bytes))
        {
            read(buffer, bytes);
            readok = true;
        }
        else
        {
            seekForward(bytes);
        }
    }
    else
    {
        seekForward(bytes);
    }

    if (m_error) return false;

    if (readok)
    {
        if (m_swapped)
        {
            switch (prop.type)
            {
              case Gto::Int: 
              case Gto::String:
              case Gto::Float: 
                  swapWords(buffer, num);
                  break;
                          
              case Gto::Short:
              case Gto::Half: 
                  swapShorts(buffer, num);
                  break;
                          
              case Gto::Double: 
                  swapWords(buffer, num * 2);
                  break;
                          
              case Gto::Byte:
              case Gto::Boolean: 
                  break;
            }
        }

        dataRead(prop);
    }


    return true;
}

bool
Reader::notEOF()
{
    if (m_in) 
    {
        return (*m_in);
    }
#ifdef GTO_SUPPORT_ZIP
    else if (m_gzfile)
    {
        return m_gzrval != -1;
    }
#endif

    return false;
}

void
Reader::read(char *buffer, size_t size)
{
    if (m_in)
    {
        m_in->read(buffer,size);

#ifndef PLATFORM_DARWIN
        if (m_in->fail())
        {
            std::cerr << "ERROR: Gto::Reader: Failed to read gto file: '";
            std::cerr << m_inName << "': " << std::endl;
            memset( buffer, 0, size );
            m_error = 1;
            m_why = "stream fail";
        }
#endif
    }

#ifdef GTO_SUPPORT_ZIP
    else if (m_gzfile)
    {
        if (gzread(m_gzfile, buffer, size) != size)
        {
            int zError = 0;
            std::cerr << "ERROR: Gto::Reader: Failed to read gto file: ";
            std::cerr << gzerror( m_gzfile, &zError );
            std::cerr << std::endl;
            memset( buffer, 0, size );
            m_error = 1;
            m_why = "gzread fail";
        }
    }
#endif
}

void
Reader::get(char &c)
{
    if (m_in)
    {
        m_in->get(c);
    }
#ifdef GTO_SUPPORT_ZIP
    else if (m_gzfile)
    {
        m_gzrval = gzgetc(m_gzfile);
        c = char(m_gzrval);
    }
#endif
}

const std::string& Reader::stringFromId(unsigned int i)
{
    static std::string empty( "" );
    if (i < 0 || i >= m_strings.size())
    {
        std::cerr << "WARNING: Gto::Reader: Malformed gto file: ";
        std::cerr << "invalid string index" << std::endl;
        m_error = 1;
        m_why = "malformed file, invalid string index";
        return empty;
    }

    return m_strings[i];
}

void Reader::seekForward(size_t bytes)
{
    if (m_in)
    {
        m_in->seekg(bytes, IOS_CUR);
    }
#ifdef GTO_SUPPORT_ZIP
    else
    {
        gzseek(m_gzfile, bytes, SEEK_CUR);
    }
#endif
}

void Reader::seekTo(size_t bytes)
{
    if (m_in)
    {
        m_in->seekg(bytes, IOS_BEG);
    }
#ifdef GTO_SUPPORT_ZIP
    else
    {
        gzseek(m_gzfile, bytes, SEEK_SET);
    }
#endif
}

int Reader::tell()
{
    if (m_in)
    {
        return m_in->tellg();
    }
#ifdef GTO_SUPPORT_ZIP
    else
    {
        return gztell(m_gzfile);
    }
#endif
}

} // Gto
