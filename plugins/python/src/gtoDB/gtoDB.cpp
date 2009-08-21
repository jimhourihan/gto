//
//  Copyright (c) 2009, Tweak Software
//  All rights reserved.
// 
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//     * Redistributions of source code must retain the above
//       copyright notice, this list of conditions and the following
//       disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials
//       provided with the distribution.
//
//     * Neither the name of the Tweak Software nor the names of its
//       contributors may be used to endorse or promote products
//       derived from this software without specific prior written
//       permission.
// 
//  THIS SOFTWARE IS PROVIDED BY Tweak Software ''AS IS'' AND ANY EXPRESS
//  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL Tweak Software BE LIABLE FOR
//  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
//  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
//  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
//

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
