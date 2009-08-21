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
#include <algorithm>
#include <vector>
#include <fnmatch.h>
#include <sys/types.h>

#ifdef WIN32
#include <pcreposix.h>
#else
#include <regex.h>
#endif

using namespace Gto;
using namespace std;

bool verbose    = false;
bool glob       = true;
bool text       = false;
bool nocompress = false;

regex_t excludeRegex;
regex_t includeRegex;

struct FullProperty
{
    string     name;
    string     interp;
    Object*    object;
    Component* component;
    Property*  property;
};

typedef vector<FullProperty> FullProperties;

struct Empty_p
{
    bool operator() (Object* o) { return o->components.empty(); }
};

void gather(RawDataBase* db, FullProperties& all)
{
    for (size_t i=0; i < db->objects.size(); i++)
    {
        Object* o = db->objects[i];

        for (size_t j=0; j < o->components.size(); j++)
        {
            Component* c = o->components[j];

            for (size_t q=0; q < c->properties.size(); q++)
            {
                Property* p = c->properties[q];
        
                FullProperty fp;
                fp.name = o->name;
                fp.name += ".";
                fp.name += c->name;
                fp.name += ".";
                fp.name += p->name;
                fp.object = o;
                fp.component = c;
                fp.property = p;
                fp.interp = p->interp;
        
                all.push_back(fp);
            }
        }
    }
}

void filter(RawDataBase* db,
            FullProperties& properties, 
            const char *include, 
            const char *exclude)
{
    for (size_t i=0; i < properties.size(); i++)
    {
        const FullProperty &fp = properties[i];
        bool imatch = false;
        bool ematch = false;

        if (include)
        {
            bool matched;

            if (glob)
            {
                matched = !fnmatch(include, fp.name.c_str(), 0);
            }
            else
            {
                matched = !regexec(&includeRegex, fp.name.c_str(), 0, 0, 0);
            }

            if (verbose && matched)
            {
                cout << "gtomerge: include pattern matched "
                     << fp.name << endl;
            }

            if (matched) imatch = true;
        }

        if (exclude)
        {
            bool matched;

            if (glob)
            {
                matched = !fnmatch(exclude, fp.name.c_str(), 0);
            }
            else
            {
                matched = !regexec(&excludeRegex, fp.name.c_str(), 0, 0, 0);
            }

            if (verbose && matched)
            {
                cout << "gtomerge: exclude pattern matched "
                     << fp.name << endl;
            }

            if (matched) ematch = true;
        }

        if (include && imatch && exclude && ematch)
        {
            cout << "gtomerge: including " << fp.name 
                 << " despite matching include and exclude pattern"
                 << endl;
        }
        else if ((include && !exclude && !imatch) || 
                 (!include && exclude && ematch) ||
                 (include && !imatch && exclude && ematch))
        {
            Object *o = fp.object;
            Component *c = fp.component;
            Property *p = fp.property;

            c->properties.erase( find(c->properties.begin(), 
                                      c->properties.end(), 
                                      p) );

            if (c->properties.empty())
            {
                o->components.erase( find(o->components.begin(),
                                          o->components.end(),
                                          c) );

                if (o->components.empty())
                {
                    db->objects.erase( find(db->objects.begin(),
                                            db->objects.end(),
                                            o) );
                }
            }
        }
    }

    //
    //  Do a clean up pass to handle "empty" input data as well
    //

    size_t n = db->objects.size();

    db->objects.erase(remove_if(db->objects.begin(),
                                db->objects.end(),
                                Empty_p()),
                      db->objects.end());

    if (verbose && n != db->objects.size())
    {
        cout << "gtofilter: removed " << (n - db->objects.size())
             << " empty objects from input"
             << endl;
    }
}

void usage()
{
    cout << "USAGE: "
         << "gtofilter [options] -o outfile.gto infile.gto\n"
         << "-nc                    force uncompressed file output\n"
         << "-t                     output text format\n"
         << "-regex                 use basic posix regular expressions\n"
         << "-glob                  use glob regular expressions (default)\n"
         << "-ie/--include regex    inclusion regex\n"
         << "-ee/--exclude regex    exclusion regex\n"
         << "-v                     verbose\n"
         << endl;
    
    exit(-1);
}
            
int main(int argc, char *argv[])
{
    char *inFile = 0;
    char *outFile = 0;
    char *includeExpr = 0;
    char *excludeExpr = 0;

    for (int i=1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-o"))
        {
            i++;
            if (i >= argc) usage();
            outFile = argv[i];
        }
        else if (!strcmp(argv[i], "-ie") || !strcmp(argv[i], "--include"))
        {
            i++;
            if (i >= argc) usage();
            includeExpr = argv[i];
        }
        else if (!strcmp(argv[i], "-ee") || !strcmp(argv[i], "--exclude"))
        {
            i++;
            if (i >= argc) usage();
            excludeExpr = argv[i];
        }
        else if (!strcmp(argv[i], "-v"))
        {
            verbose = true;
        }
        else if (!strcmp(argv[i], "-nc"))
        {
            nocompress = true;
        }
        else if (!strcmp(argv[i], "-t"))
        {
            text = true;
        }
        else if (!strcmp(argv[i], "-glob"))
        {
            glob = true;
        }
        else if (!strcmp(argv[i], "-regex"))
        {
            glob = false;
        }
        else if (*argv[i] == '-')
        {
            usage();
        }
        else
        {
            if (inFile) usage();
            inFile = argv[i];
        }
    }

    if (!inFile || !outFile)
    {
	cout << "no infile or outfile specified.\n" << flush;
        cout << endl;
        usage();
    }

    if (!glob)
    {
        if (excludeExpr)
        {
            if (int err = regcomp(&excludeRegex, excludeExpr, REG_NOSUB))
            {
                char temp[256];
                regerror(err, &excludeRegex, temp, 256);
                cerr << "ERROR: " << temp << endl;
                exit(-1);
            }
        }

        if (includeExpr)
        {
            if (int err = regcomp(&includeRegex, includeExpr, REG_NOSUB))
            {
                char temp[256];
                regerror(err, &includeRegex, temp, 256);
                cerr << "ERROR: " << temp << endl;
                exit(-1);
            }
        }
    }

    RawDataBaseReader reader;
    cout << "Reading input file " << inFile << "..." << endl;

    if (!reader.open(inFile))
    {
        cerr << "ERROR: unable to read file " << inFile
             << endl;
        exit(-1);
    }

    RawDataBase *db = reader.dataBase();
    FullProperties allProperties;
    gather(db, allProperties);
    filter(db, allProperties, includeExpr, excludeExpr);

    if (db->objects.empty())
    {
        cerr << "ERROR: everything was excluded" << endl;
        exit(-1);
    }

    RawDataBaseWriter writer;
    Writer::FileType type = Writer::CompressedGTO;
    if (nocompress) type = Writer::BinaryGTO;
    if (text) type = Writer::TextGTO;

    if (!writer.write(outFile, *db, type))
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
