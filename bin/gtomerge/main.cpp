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
#include <Gto/RawData.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

using namespace Gto;
using namespace std;


// *****************************************************************************
const char *stripNamePrefix( const std::string &name, const char *prefix )
{
    if( ! prefix )
    {
        return name.c_str();
    }
    if( name.substr( 0, strlen( prefix ) ) == prefix )
    {
        return &(name[strlen(prefix)]);
    }
    return name.c_str();
}

//----------------------------------------------------------------------

void
propertyMerge(Component *out, Component *in)
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

void
componentMerge(Object *out, Object *in)
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

void
objectMerge(RawDataBase *out, RawDataBase *in, const char *stripPrefix)
{
    if( stripPrefix )
    {
        for (size_t i=0; i < in->objects.size(); i++)
        {
            Object *o = in->objects[i];
            bool found = false;

            std::string mungedNameIn( stripNamePrefix( o->name, stripPrefix ) );

            for (size_t q=0; q < out->objects.size(); q++)
            {
                std::string mungedNameOut( stripNamePrefix( out->objects[q]->name, stripPrefix ) );
                if (mungedNameOut == mungedNameIn)
                {
                    found = true;
                    componentMerge(out->objects[q], o);
                    break;
                }
            }

            if (!found)
            {
                o->name = mungedNameIn;
                out->objects.push_back(o);
                in->objects.erase(in->objects.begin() + i);
                i--;
            }
        }
    }
    else
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
}

void usage()
{
    cout << "gtomerge [OPTIONS] -o OUTFILE INFILE1 INFILE2 ..." << endl
         << "Merge GTO property data" << endl
         << endl
         << "-t             output as text GTO" << endl
         << "-nc            force uncompressed GTO" << endl
         << "-sp PREFIX     strip prefix" << endl
         << endl;
    
    exit(-1);
}

int main(int argc, char *argv[])
{
    vector<string> inputFiles;
    char *outFile = NULL;
    RawDataBase outObjects;
    char *stripPrefix = NULL;
    int text = 0;
    int nocompress = 0;

    for (int i=1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-o"))
        {
            i++;

            if (i < argc)
            {
                outFile = argv[i];
            }
        }
        else if (!strcmp(argv[i], "-sp"))
        {
            i++;

            if (i < argc)
            {
                stripPrefix = argv[i];
            }
        }
        else if (!strcmp(argv[i], "-t"))
        {
            text = 1;
        }
        else if (!strcmp(argv[i], "-nc"))
        {
            nocompress = 1;
        }
        else if (*argv[i] == '-')
        {
            usage();
        }
        else
        {
            inputFiles.push_back(argv[i]);
        }
    }

    if (!inputFiles.size() || !outFile)
    {
        cout << "no infile or outfile specified.\n" << flush;
        cout << endl;
        usage();
    }

    for (size_t i=0; i < inputFiles.size(); i++)
    {
        RawDataBaseReader reader;
        cout << "Reading input file " << inputFiles[i] << "..." << endl;

        if (!reader.open(inputFiles[i].c_str()))
        {
            cerr << "ERROR: unable to read file " << inputFiles[i].c_str()
                 << endl;
            exit(-1);
        }
        else
        {
            RawDataBase *inObjects = reader.dataBase();
            objectMerge(&outObjects, inObjects, stripPrefix);
        }
    }

    RawDataBaseWriter writer;
    Writer::FileType type = Writer::CompressedGTO;
    if (nocompress) type = Writer::BinaryGTO;
    if (text) type = Writer::TextGTO;

    if (!writer.write(outFile, outObjects, type))
    {
        cerr << "ERROR: unable to write file " << outFile
             << endl;
        exit(-1);
    }
    else
    {
        cout << "Wrote file " << outFile << endl;
    }

    return 0;
}
