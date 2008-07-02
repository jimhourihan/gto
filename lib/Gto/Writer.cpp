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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "Writer.h"
#include "Utilities.h"
#include <fstream>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdexcept>

#define GTO_DEBUG 0

#ifdef GTO_SUPPORT_ZIP
#include <zlib.h>
#endif

#ifdef WIN32
#define snprintf _snprintf
#endif

namespace Gto {
using namespace std;


Writer::Writer() 
    : m_out(0), 
      m_gzfile(0),
      m_needsClosing(false), 
      m_error(false),
      m_tableFinished(false),
      m_currentProperty(0),
      m_type(CompressedGTO),
      m_endDataCalled(false),
      m_beginDataCalled(false),
      m_objectActive(false),
      m_componentActive(false)
{
    init(0);
}

Writer::Writer(ostream &o) 
    : m_out(0), 
      m_gzfile(0),
      m_needsClosing(true), 
      m_error(false), 
      m_tableFinished(false),
      m_currentProperty(0),
      m_type(CompressedGTO),
      m_endDataCalled(false),
      m_beginDataCalled(false),
      m_objectActive(false),
      m_componentActive(false)
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
Writer::open(const char* filename, FileType type)
{
    if (m_out || m_gzfile) return false;

    m_outName = filename;
    m_type    = type;

#ifndef GTO_SUPPORT_ZIP
    if (type == CompressedGTO) type = BinaryGTO;
#endif

    if (type == BinaryGTO || type == TextGTO)
    {
        if (type == BinaryGTO)
        {
            m_out = new ofstream(filename, ios::out|ios::binary);
        }
        else
        {
            m_out = new ofstream(filename, ios::out);
        }

        if (!(*m_out))
        {
            m_out    = 0;
            m_error  = true;
            return false;
        }
    }
#ifdef GTO_SUPPORT_ZIP
    else if (type == CompressedGTO)
    {
        m_gzfile = gzopen(filename, "wb");

        if (!m_gzfile)
        {
            m_gzfile = 0;
            m_error  = true;
            return false;
        }
    }
#endif

    m_needsClosing = true;
    m_error = false;
    return true;
}

void
Writer::close()
{
    if (m_beginDataCalled && !m_endDataCalled) 
    {
        //
        //  Should this be an error? We can gracefully finish like
        //  this, but its not how you're supposed to use the class.
        //

        cout << "WARNING: Gto::Writer::close() -- you forgot to call endData()"
             << endl;

        endData();
    }

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

    if (m_type == TextGTO)
    {
        writeFormatted("GTOa (%d)\n\n", GTO_VERSION);
    }
    else
    {
        writeHead();
    }

    m_beginDataCalled = true;
}

void
Writer::endData()
{
    if (m_type == TextGTO)
    {
        writeText("    }\n}\n");
    }

    m_endDataCalled = true;
}

void
Writer::intern(const char* s)
{
    if (m_tableFinished)
    {
        throw std::runtime_error("Gto::Writer::intern(): Unable to intern "
                                 "strings after string table is finished");
    }

    m_strings[s] = 0;
}

void
Writer::intern(const string& s)
{
    if (m_tableFinished)
    {
        throw std::runtime_error("Gto::Writer::intern(): Unable to intern "
                                 "strings after string table is finished");
    }

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

std::string
Writer::lookup(int n) const
{
    if (n < 0 || (size_t)n >= m_names.size()) return "*bad-lookup*";
    return m_names[n];
}

void
Writer::beginObject(const char* name, 
                    const char *protocol, 
                    unsigned int version)
{
    if (m_objectActive)
    {
        throw std::runtime_error("ERROR: Gto::Writer::beginObject() -- "
                                 "you forgot to call endObject()");
    }

    if (m_componentActive)
    {
        throw std::runtime_error("ERROR: Gto::Writer::beginObject() -- "
                                 "beginComponent() still active");
    }

    m_names.push_back(name);
    m_names.push_back(protocol);

    ObjectHeader header;
    memset(&header, 0, sizeof(ObjectHeader));
    header.numComponents   = 0;
    header.name            = m_names.size() - 2;
    header.protocolName    = m_names.size() - 1;
    header.protocolVersion = version;

    m_objects.push_back(header);
    m_objectActive = true;
}

void
Writer::beginComponent(const char* name, unsigned int flags)
{
    if (!m_objectActive)
    {
        throw std::runtime_error("ERROR: Gto::Writer::beginComponent() -- "
                                 "you forgot to call beginObject()");
    }

    if (m_componentActive)
    {
        throw std::runtime_error("ERROR: Gto::Writer::beginComponent() -- "
                                 "you forgot to call endComponent()");
    }

    m_names.push_back(name);
    m_objects.back().numComponents++;
    ComponentHeader header;
    memset(&header, 0, sizeof(ComponentHeader));
    
    header.numProperties  = 0;
    header.flags          = flags;
    header.name           = m_names.size() - 1;
    header.pad            = 0;

    m_names.push_back("");
    header.interpretation = m_names.size() - 1;

    m_components.push_back(header);
    m_componentActive = true;
}

void
Writer::beginComponent(const char* name, 
                       const char* interp,
                       unsigned int flags)
{
    if (!m_objectActive)
    {
        throw std::runtime_error("ERROR: Gto::Writer::beginComponent() -- "
                                 "you forgot to call beginObject()");
    }

    if (m_componentActive)
    {
        throw std::runtime_error("ERROR: Gto::Writer::beginComponent() -- "
                                 "you forgot to call endComponent()");
    }

    m_names.push_back(name);
    m_objects.back().numComponents++;
    ComponentHeader header;
    memset(&header, 0, sizeof(ComponentHeader));
    
    header.numProperties = 0;
    header.flags         = flags;
    header.name          = m_names.size() - 1;
    header.pad           = 0;

    if (!interp) interp = "";
    m_names.push_back(interp);
    header.interpretation = m_names.size() - 1;

    m_components.push_back(header);
    m_componentActive = true;
}

void
Writer::endComponent()
{
    m_componentActive = false;
}

void
Writer::endObject()
{
    m_objectActive = false;
}

void
Writer::property(const char* name,
                 DataType type,
                 size_t numElements,
                 size_t partsPerElement,
                 const char *interp)
{
    if (!m_objectActive || !m_componentActive)
    {
        throw std::runtime_error("ERROR: Gto::Writer::property() -- "
                                 "no active component or object");
    }

    m_names.push_back(name);
    m_components.back().numProperties++;

    PropertyHeader header;
    memset(&header, 0, sizeof(PropertyHeader));
    header.size   = numElements;
    header.type   = type;
    header.width  = partsPerElement;
    header.name   = m_names.size() - 1;
    header.pad    = 0;

    if (!interp) interp = "";
    m_names.push_back(interp);
    header.interpretation = m_names.size() - 1;

    m_properties.push_back(header);

    if (m_type == TextGTO)
    {
        PropertyPath ppath(m_objects.size() - 1,
                           m_components.size() - 1);
        m_propertyMap[m_properties.size() - 1] = ppath;
    }
}


void
Writer::constructStringTable()
{
    intern( "(Gto::Writer compiled " __DATE__ 
            " " __TIME__ 
            ", $Id: Writer.cpp,v 1.37 2008/04/11 23:25:24 src Exp $)" );

    for (size_t i=0; i < m_names.size(); i++)
    {
        intern(m_names[i]);
    }

    //
    //  Assign numbers -- note, this is a workaround for the fact that
    //  set does not implement operator-() for its
    //  iterators. Otherwise, we could use a set instead of a map.
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
    //  Find all the name ids for the header structs
    //

    for (size_t o=0, c=0, p=0, n=0; o < m_objects.size(); o++)
    {
        ObjectHeader &oh = m_objects[o];
        oh.name = m_strings[m_names[n++]];
        oh.protocolName = m_strings[m_names[n++]];

        for (size_t i=0; i < oh.numComponents; i++, c++)
        {
            ComponentHeader &ch = m_components[c];
            ch.name = m_strings[m_names[n++]];
            ch.interpretation = m_strings[m_names[n++]];

            for (size_t q=0; q < ch.numProperties; q++, p++)
            {
                PropertyHeader &ph = m_properties[p];
                ph.name = m_strings[m_names[n++]];
                ph.interpretation = m_strings[m_names[n++]];
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

static bool gto_isalnum(const string& str)
{
    bool allnumbers = true;

    for (size_t i=0, s=str.size(); i < s; i++)
    {
        int c = str[i];

        if (!isalnum(c) && c != '_')
        {
            return false;
        }

        if (!isdigit(c)) allnumbers = false;
    }

    return !allnumbers;
}

void
Writer::writeMaybeQuotedString(const string& str)
{
    static const char* keywords[] = {
        "float", "double", "half", "bool", "int",
        "short", "byte", "as", "GTOa", "string", 0 };

    for (const char** k = keywords; *k; k++)
    {
        if (str == *k)
        {
            writeQuotedString(str);
            return;
        }
    }
    
    if (gto_isalnum(str))
    {
        writeText(str);
    }
    else
    {
        writeQuotedString(str);
    }
}

void
Writer::writeQuotedString(const string& str)
{
    writeFormatted("\"");
    static const char delim = '"';

    for (size_t i=0; i < str.size(); i++)
    {
	char c = str[i];

	if (c == 0)
	{
            write("");
	}
	else if (iscntrl(c))
	{
	    switch (c)
	    {
	      case '\n': writeFormatted("\\n"); break;
	      case '\b': writeFormatted("\\b"); break;
	      case '\r': writeFormatted("\\r"); break;
	      case '\t': writeFormatted("\\t"); break;
	      default:
                  {
                      char temp[41];
                      temp[40] = 0;
                      snprintf(temp, 40, "\\%o", int(c));
                      writeFormatted(temp);
                  }
		  break;
	    }
	}
	else if (c == delim)
	{
            writeFormatted("\\%c", delim);
	}
        else if (c & 0x80)
        {
            // UTF-8
        }
	else
	{
            write(&c, sizeof(char));
	}
    }

    writeFormatted("\"");
}

void
Writer::writeText(const std::string& s)
{
    if (m_out)
    {
        (*m_out) << s;
    }
#ifdef GTO_SUPPORT_ZIP
    else if (m_gzfile)
    {
        gzwrite(m_gzfile, (void*)s.c_str(), s.size());
    }
#endif
}

void
Writer::writeFormatted(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    char* m;
    /* AJG - windows vsnprintf hacko */
    m = (char*)(malloc(1024*10*sizeof(char)));
    // vasprintf(&m, format, ap);
    vsprintf(m, format, ap);
    write(m, strlen(m));
    free(m);
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

    for (size_t i=0; i < m_objects.size(); i++)
    {
        ObjectHeader &o = m_objects[i];
        write(&o, sizeof(ObjectHeader));
    }

    for (size_t i=0; i < m_components.size(); i++)
    {
        ComponentHeader &c = m_components[i];
        write(&c, sizeof(ComponentHeader));
    }

    for (size_t i=0; i < m_properties.size(); i++)
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

    if (size > 0 && (size_t) size != m_properties[p].size)
    {
        std::cerr << "ERROR: Gto::Writer: propertyData expected data of size "
                  << m_properties[p].size << " but got data of size "
                  << size << " instead while writing property '"
                  << m_names[m_properties[p].name] << "'" << std::endl;
        m_error = true;
        return false;
    }

    if (width > 0 && (size_t) width != m_properties[p].width)
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
    if (!m_beginDataCalled) beginData();

    size_t                p     = m_currentProperty++;
    const PropertyHeader& info  = m_properties[p];
    size_t                n     = info.size * info.width;
    size_t                ds    = dataSize(info.type);
    char*                 bdata = (char*)data;

    if (propertySanityCheck(propertyName, size, width))
    {
        if (m_type == TextGTO)
        {
            PropertyPath p0 = p == 0 ? PropertyPath() : m_propertyMap[p-1];
            PropertyPath p1 = m_propertyMap[p];

            if (p0.objectIndex != p1.objectIndex)
            {
                if (p != 0)
                {
                    writeFormatted("    }\n}\n\n");
                }

                const ObjectHeader& o = m_objects[p1.objectIndex];
                writeMaybeQuotedString(lookup(o.name));
                writeFormatted(" : ");
                writeMaybeQuotedString(lookup(o.protocolName));
                writeFormatted(" (%d)\n{\n", o.protocolVersion);
            }

            if (p0.componentIndex != p1.componentIndex)
            {
                if (p != 0 && p0.objectIndex == p1.objectIndex)
                {
                    writeFormatted("    }\n\n");
                }

                const ComponentHeader& c = m_components[p1.componentIndex];
                writeText("    ");
                writeMaybeQuotedString(lookup(c.name));
                writeFormatted("\n    {\n");
            }

            writeText("        ");
            writeText(typeName(Gto::DataType(info.type)));
            if (info.width > 1) writeFormatted("[%d]", info.width);
            writeText(" ");
            writeMaybeQuotedString(lookup(info.name));

            if (info.interpretation)
            {
                writeText(" as ");
                writeMaybeQuotedString(lookup(info.interpretation));
            }

            writeText(" =");

            if (n == 0) writeText(" [ ]");
            if (n > 1) writeText(" [");

            for (size_t i = 0; i < n; i++)
            {
                if (info.width > 1)
                {
                    if (i % info.width == 0)
                    {
                        if (i) writeFormatted(" ]");
                        writeFormatted(" [");
                    }
                }

                if (isNumber(DataType(info.type)))
                {
                    Number num = asNumber(bdata + (i * ds), 
                                          DataType(info.type));

                    if (num.type == Int)
                    {
                        writeFormatted(" %d", num._int);
                    }
                    else
                    {
                        writeFormatted(" %g", double(num._double));
                    }
                }
                else
                {
                    char* s = bdata + (i*ds);
                    writeText(" ");
                    writeQuotedString(lookup(*(int*)s));
                }
            }

            if (n > 0 && info.width > 1) writeText(" ]");
            if (n > 1) writeText(" ]");

            writeText("\n");
        }
        else
        {
            write(data, dataSize(m_properties[p].type) * n);
        }
    }
}


} // Gto

#ifdef _MSC_VER
#undef snprintf
#endif
