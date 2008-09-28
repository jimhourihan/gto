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

#include "GtoDB.h"

using namespace Gto;

// Begin verbatim (copied from gtomerge/main.cpp)

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
    

// *****************************************************************************
GtoDB::GtoDB()
{
}


// *****************************************************************************
GtoDB::GtoDB(std::string filename)
{
    read(filename);
}


// *****************************************************************************
GtoDB::~GtoDB()
{
    // Nothing
}


// *****************************************************************************
void GtoDB::read(std::string filename)
{
    RawDataBaseReader mergeReader;
    mergeReader.open(filename.c_str());
    objectMerge(m_reader.dataBase(), mergeReader.dataBase());
}


// *****************************************************************************
void GtoDB::write(std::string filename, FileType mode)
{
    RawDataBaseWriter writer;
//     if(m_reader.dataBase()->objects.empty())
//     {
//         throw std::runtime_error("No objects to write");
//     }
    writer.write(filename.c_str(), *m_reader.dataBase(), 
                 (Writer::FileType)(mode));
}
