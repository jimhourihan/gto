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

#include "Writer.h"
#include "Utilities.h"
#include <fstream>
#include <assert.h>

#define GTO_DEBUG 0

#ifdef GTO_SUPPORT_ZIP
#include <zlib.h>
#endif

namespace Gto {
using namespace std;


Writer::Writer() 
    : m_out(0), m_gzfile(0), 
      m_needsClosing(false), m_error(false), m_tableFinished(false)
{
    init(0);
}

Writer::Writer(ostream &o) 
    : m_out(0), m_gzfile(0),
      m_needsClosing(true), m_error(false), m_tableFinished(false)
{
    init(&o);
}

Writer::~Writer()
{
    close();
}

void
Writer::init(ostream *o)
{
    m_out = o;
}

bool
Writer::open(const char* filename, bool compress)
{
    if (m_out || m_gzfile) return false;

    m_outName = filename;

#ifdef GTO_SUPPORT_ZIP
    if (compress)
    {
        m_gzfile = gzopen(filename, "wb");

        if (!m_gzfile)
        {
            m_gzfile = 0;
            m_error  = true;
            return false;
        }
    }
    else
    {
#endif
        m_out = new ofstream(filename, ios::out|ios::binary);

        if (!(*m_out))
        {
            m_out    = 0;
            m_error  = true;
            return false;
        }
#ifdef GTO_SUPPORT_ZIP
    }
#endif

    m_needsClosing = true;
    m_error = false;
    return true;
}

void
Writer::close()
{
    if (m_out && m_needsClosing)
    {
        delete m_out;
    }
#ifdef GTO_SUPPORT_ZIP
    else if (m_gzfile && m_needsClosing)
    {
        gzclose(m_gzfile);
    }

    m_gzfile = 0;
#endif
    m_out    = 0;
}


void
Writer::beginData()
{
    m_currentProperty = 0;
    constructStringTable();
    writeHead();
}

void
Writer::endData()
{
}

void
Writer::intern(const char* s)
{
    assert(!m_tableFinished);
    m_strings[s] = 0;
}

void
Writer::intern(const string& s)
{
    assert(!m_tableFinished);
    m_strings[s] = 0;
}

int
Writer::lookup(const char* s) const
{
    if (m_tableFinished)
    {
        StringMap::const_iterator i = m_strings.find(s);

        if (i == m_strings.end())
        {
            return -1;
        }
        else
        {
            return i->second;
        }
    }
    else
    {
        return -1;
    }
}

int
Writer::lookup(const string& s) const
{
    if (m_tableFinished)
    {
        StringMap::const_iterator i = m_strings.find(s);

        if (i == m_strings.end())
        {
            return -1;
        }
        else
        {
            return i->second;
        }
    }
    else
    {
        return -1;
    }
}

void
Writer::beginObject(const char* name, 
                    const char *protocol, 
                    unsigned int version)
{
    m_names.push_back(name);
    m_names.push_back(protocol);

    ObjectHeader header;
    header.numComponents   = 0;
    header.name            = m_names.size() - 2;
    header.protocolName    = m_names.size() -1;
    header.protocolVersion = version;

    m_objects.push_back(header);
}

void
Writer::beginComponent(const char* name, unsigned int flags)
{
    m_names.push_back(name);
    m_objects.back().numComponents++;
    ComponentHeader header;
    
    header.numProperties = 0;
    header.flags = flags;
    header.name = m_names.size() - 1;

    m_components.push_back(header);
}

void
Writer::endComponent()
{
}

void
Writer::endObject()
{
}

void
Writer::property(const char* name,
                 DataType type,
                 size_t numElements,
                 size_t partsPerElement)
{
    m_names.push_back(name);
    m_components.back().numProperties++;

    PropertyHeader header;
    header.size   = numElements;
    header.type   = type;
    header.width  = partsPerElement;
    header.name   = m_names.size() - 1;

    m_properties.push_back(header);
}


void
Writer::constructStringTable()
{
    intern( "(Gto::Writer compiled " __DATE__ 
            " " __TIME__ 
            ", $Id: Writer.cpp,v 1.2 2004/04/26 17:13:07 jimh Exp $)" );

    for (int i=0; i < m_names.size(); i++)
    {
        intern(m_names[i]);
    }

    //
    //        Assign numbers -- note, this is a workaround for the fact that
    //        set does not implement operator-() for its
    //        iterators. Otherwise, we could use a set instead of a map.
    //

    int count = 0;
    size_t bytes = 0;

    for (StringMap::iterator i = m_strings.begin();
         i != m_strings.end(); 
         ++i)
    {
        (*i).second = count++;
        bytes += ((*i).first.size()+1) * sizeof(char);
    }

    //
    //        Find all the name ids for the header structs
    //

    for (int o=0, c=0, p=0, n=0; o < m_objects.size(); o++)
    {
        ObjectHeader &oh = m_objects[o];
        oh.name = m_strings[m_names[n++]];
        oh.protocolName = m_strings[m_names[n++]];

        for (int i=0; i < oh.numComponents; i++, c++)
        {
            ComponentHeader &ch = m_components[c];
            ch.name = m_strings[m_names[n++]];

            for (int q=0; q < ch.numProperties; q++, p++)
            {
                PropertyHeader &ph = m_properties[p];
                ph.name = m_strings[m_names[n++]];
            }
        }
    }

    //
    //  Reformat the m_names to do inverse lookups for debugging
    //

    m_names.resize(m_strings.size());

    for (StringMap::iterator i = m_strings.begin();
         i != m_strings.end();
         ++i)
    {
        m_names[i->second] = i->first;
    }

    m_tableFinished = true;
}

void
Writer::write(const void* p, size_t s)
{
    if( s == 0 ) return;
    if (m_out)
    {
        m_out->write((const char*)p, s);
    }
#ifdef GTO_SUPPORT_ZIP
    else if (m_gzfile)
    {
        gzwrite(m_gzfile, (void*)p, s);
    }
#endif
}

void
Writer::write(const std::string& s)
{
    if (m_out)
    {
        (*m_out) << s;
        m_out->put(0);
    }
#ifdef GTO_SUPPORT_ZIP
    else if (m_gzfile)
    {
        gzwrite(m_gzfile, (void*)s.c_str(), s.size() + 1);
    }
#endif
}

void
Writer::flush()
{
    if (m_out) (*m_out) << std::flush;
}

void
Writer::writeHead()
{
    assert(m_objects.size() > 0);
    assert(m_components.size() > 0);
    assert(m_properties.size() > 0);

    Header header;
    header.magic         = GTO_MAGIC;
    header.numObjects    = m_objects.size();
    header.numStrings    = m_strings.size();
    header.version       = GTO_VERSION;
    header.flags         = 0;

    write(&header, sizeof(Header));

    for (StringMap::iterator i = m_strings.begin();
         i != m_strings.end(); 
         ++i)
    {
        write(i->first);
    }

    for (int i=0; i < m_objects.size(); i++)
    {
        ObjectHeader &o = m_objects[i];
        write(&o, sizeof(ObjectHeader));
    }

    for (int i=0; i < m_components.size(); i++)
    {
        ComponentHeader &c = m_components[i];
        write(&c, sizeof(ComponentHeader));
    }

    for (int i=0; i < m_properties.size(); i++)
    {
        PropertyHeader &p = m_properties[i];
        write(&p, sizeof(PropertyHeader));
    }

    flush();
}

bool
Writer::propertySanityCheck(const char *propertyName, int size, int width)
{
    if (!propertyName) return true;
    size_t p = m_currentProperty - 1;

    if (propertyName != NULL && propertyName != m_names[m_properties[p].name])
    {
        std::cerr << "ERROR: Gto::Writer: propertyData expected '"
                  << m_names[m_properties[p].name] << "' but got data for '"
                  << propertyName << "' instead." << std::endl;
        m_error = true;
        return false;
    }

    if (size > 0 && size != m_properties[p].size)
    {
        std::cerr << "ERROR: Gto::Writer: propertyData expected data of size "
                  << m_properties[p].size << " but got data of size "
                  << size << " instead while writing property '"
                  << m_names[m_properties[p].name] << "'" << std::endl;
        m_error = true;
        return false;
    }

    if (width > 0 && width != m_properties[p].width)
    {
        std::cerr << "ERROR: Gto::Writer: propertyData expected data of width "
                  << m_properties[p].width << " but got data of width "
                  << width << " instead while writing property '"
                  << m_names[m_properties[p].name] << "'" << std::endl;
        m_error = true;
        return false;
    }
    
    return true;
}

void 
Writer::propertyDataRaw(const void* data,
                        const char *propertyName, 
                        int size, 
                        int width)
{
    size_t p = m_currentProperty++;
    size_t n = m_properties[p].size * m_properties[p].width;

    if (propertySanityCheck(propertyName, size, width))
    {
        write(data, dataSize(m_properties[p].type) * n);
    }
}


} // Gto
