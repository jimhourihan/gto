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
#include <Gto/RawData.h>
#include <iostream>

namespace Gto {
using namespace std;

Property::Property(const std::string& n, 
                   Gto::DataType t, 
                   size_t s, 
                   size_t w, 
                   bool allocate)
    : name(n), type(t), size(s), width(w), voidData(0)
{
    if (allocate)
    {
        if (t == String)
        {
            stringData = new string[w * s];
        }
        else
        {
            voidData = new char[dataSize(t) * w * s];
        }
    }
}

Property::~Property()
{
    if (type == Gto::String)
    {
        delete [] stringData;
    }
    else
    {
        delete [] (char*)voidData;
    }
}

//----------------------------------------------------------------------

Component::~Component()
{
    for (int i=0; i < properties.size(); i++)
    {
	delete properties[i];
    }
}

//----------------------------------------------------------------------

Object::~Object()
{
    for (int i=0; i < components.size(); i++)
    {
	delete components[i];
    }
}

//----------------------------------------------------------------------

RawDataBase::~RawDataBase()
{
    for (int i=0; i < objects.size(); i++)
    {
	delete objects[i];
    }
}

//----------------------------------------------------------------------

RawDataBaseReader::RawDataBaseReader() : Reader(false)
{
    m_dataBase = new RawDataBase;
}

RawDataBaseReader::~RawDataBaseReader() 
{
}

bool
RawDataBaseReader::open(const char *filename)
{
    if (Reader::open(filename))
    {
        m_dataBase->strings = stringTable();
        return true;
    }
    else
    {
        return false;
    }
}

bool
RawDataBaseReader::open(std::istream& in, const char *name)
{
    if (Reader::open(in, name))
    {
        m_dataBase->strings = stringTable();
        return true;
    }
    else
    {
        return false;
    }
}

Reader::Request
RawDataBaseReader::object(const string& name, 
			  const string& protocol, 
			  unsigned int protocolVersion,
			  const ObjectInfo& info) 
{
    Object *o = new Object(name, protocol, protocolVersion);
    m_dataBase->objects.push_back(o);
    return Request(true, o);
}

Reader::Request
RawDataBaseReader::component(const string& name, const ComponentInfo& info) 
{ 
    Object *o    = reinterpret_cast<Object*>(info.object->objectData);
    Component *c = new Component(name, info.flags);
    o->components.push_back(c);
    return Request(true, c);
}

Reader::Request
RawDataBaseReader::property(const string& name, const PropertyInfo& info) 
{ 
    Object *o    = reinterpret_cast<Object*>(info.component->object->objectData);
    Component *c = reinterpret_cast<Component*>(info.component->componentData);
    Property *p  = new Property(name, (DataType)info.type, info.size, info.width);
    c->properties.push_back(p);
    return Request(true, p);
}

void* 
RawDataBaseReader::data(const PropertyInfo& info, size_t bytes)
{
    Object *o    = reinterpret_cast<Object*>(info.component->object->objectData);
    Component *c = reinterpret_cast<Component*>(info.component->componentData);
    Property *p  = reinterpret_cast<Property*>(info.propertyData);

    p->voidData = new char[bytes];
    return p->voidData;
}

void
RawDataBaseReader::dataRead(const PropertyInfo& info)
{
    Object *o    = reinterpret_cast<Object*>(info.component->object->objectData);
    Component *c = reinterpret_cast<Component*>(info.component->componentData);
    Property *p  = reinterpret_cast<Property*>(info.propertyData);

    if (p->type == Gto::String)
    {
        int* ints = p->int32Data;
        size_t numItems = p->size * p->width;
	p->stringData = new string[numItems];

	for (int i=0; i < numItems; i++)
	{
	    p->stringData[i] = stringTable()[ints[i]];
	}

        delete [] (char*)ints;
    }
}

//----------------------------------------------------------------------


void
RawDataBaseWriter::writeProperty(bool header, const Property *property)
{
    if (header)
    {
	m_writer.property(property->name.c_str(),
			  property->type,
			  property->size,
			  property->width);

	if (property->type == Gto::String)
	{
	    int numItems = property->size * property->width;
	    const string* data = property->stringData;

	    for (int i=0; i < numItems; i++)
	    {
		m_writer.intern(data[i]);
	    }
	}
    }
    else
    {
	switch (property->type)
	{
	  case Gto::Float:
	      m_writer.propertyData(property->floatData);
	      break;
	  case Gto::Double:
	      m_writer.propertyData(property->doubleData);
	      break;
	  case Gto::Short:
	      m_writer.propertyData(property->uint16Data);
	      break;
	  case Gto::Int:
	      m_writer.propertyData(property->int32Data);
	      break;
	  case Gto::Byte:
	      m_writer.propertyData(property->uint8Data);
	      break;
	  case Gto::Half:
	  case Gto::Boolean:
	  default:
	      abort();    // not implemented;
	  case Gto::String:
	      {
		  size_t numItems = property->size * property->width;
		  vector<int> data(numItems);

		  for (int i=0; i < numItems; i++)
		  {
                      int stringId = m_writer.lookup(property->stringData[i]);

                      if (stringId == -1)
                      {
                          cerr << "WARNING: writer detected bogus string id in "
                               << property->name << endl;
                          stringId = 0;
                      }

		      data[i] = stringId;
		  }

		  m_writer.propertyData(&data.front());
	      }
	      break;
	}
    }
}

void
RawDataBaseWriter::writeComponent(bool header, const Component *component)
{
    const Properties &props = component->properties;

    if (props.size())
    {
        m_writer.beginComponent(component->name.c_str());

        for (int i=0; i < props.size(); i++)
        {
            if (const Property *p = props[i])
            {
                writeProperty(header, p);
            }
        }

        m_writer.endComponent();
    }
}

bool
RawDataBaseWriter::write(const char *filename, const RawDataBase& db)
{
    if (!m_writer.open(filename))
    {
	return false;
    }

    for (int i=0; i < db.strings.size(); i++)
    {
        m_writer.intern(db.strings[i]);
    }

    for (int i=0; i < db.objects.size(); i++)
    {
	if (db.objects[i])
	{
	    const Object &o = *db.objects[i];
	    unsigned int version = o.protocolVersion;

	    m_writer.beginObject(o.name.c_str(), 
				 o.protocol.c_str(),
				 o.protocolVersion);

	    const Components &components = o.components;

	    for (int q=0; q < components.size(); q++)
	    {
		writeComponent(true, components[q]);
	    }

	    m_writer.endObject();
	}
    }

    m_writer.beginData();

    for (int i=0; i < db.objects.size(); i++)
    {
	if (db.objects[i])
	{
	    const Object &o = *db.objects[i];
	    const Components &components = o.components;

	    for (int q=0; q < components.size(); q++)
	    {
		writeComponent(false, components[q]);
	    }
	}
    }

    m_writer.endData();
    return true;
}


} // namespace Gto
