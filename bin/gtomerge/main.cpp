//******************************************************************************
// Copyright (c) 2002 Tweak Inc. 
// All rights reserved.
//******************************************************************************
#include <Gto/RawData.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

using namespace Gto;
using namespace std;

//----------------------------------------------------------------------

void
propertyMerge(Component *out, Component *in)
{
    for (int i=0; i < in->properties.size(); i++)
    {
	Property *p = in->properties[i];
	bool found = false;

	for (int q=0; q < out->properties.size(); q++)
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
    for (int i=0; i < in->components.size(); i++)
    {
	Component *c = in->components[i];
	bool found = false;

	for (int q=0; q < out->components.size(); q++)
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
objectMerge(RawDataBase *out, RawDataBase *in)
{
    for (int i=0; i < in->objects.size(); i++)
    {
	Object *o = in->objects[i];
	bool found = false;

	for (int q=0; q < out->objects.size(); q++)
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

void usage()
{
    cout << "USAGE: "
         << "gtomerge -o outfile.gto infile1.gto infile2.gto ...\n";
    exit(-1);
}

int main(int argc, char *argv[])
{
    vector<string> inputFiles;
    char *outFile	= 0;
    int replace = 0;
    RawDataBase outObjects;

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

    for (int i=0; i < inputFiles.size(); i++)
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
	    objectMerge(&outObjects, inObjects);
	}

	delete reader.dataBase();
    }

    RawDataBaseWriter writer;

    if (!writer.write(outFile, outObjects))
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
