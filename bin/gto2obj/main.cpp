//
//  Copyright (c) 2003 Tweak Films
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as
//  published by the Free Software Foundation; either version 2 of the
//  License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
//  02111-1307, USA.
//
#include <Gto/RawData.h>
#include <Gto/Protocols.h>
#include <WFObj/Reader.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace Gto;
using namespace std;

//----------------------------------------------------------------------

class ObjReader : public WFObj::Reader
{
public:
    ObjReader(RawDataBase* db, const char* protocol)
        : m_db(db), m_protocol(protocol) {}
    virtual ~ObjReader() {}

    virtual void finished();
    virtual void v(float,float,float);
    virtual void vt(float,float);
    virtual void vt(float,float,float);
    virtual void vn(float, float, float);
    virtual void f(const VertexIndices&,
                   const NormalIndices&,
                   const TextureIndices&);

private:
    RawDataBase*        m_db;
    const char*         m_protocol;
    vector<float>       m_vertices;
    vector<float>       m_normals;
    vector<float>       m_sts;
    vector<short>       m_sizes;
    vector<char>        m_types;
    vector<int>         m_indices;
    vector<int>         m_nindices;
    vector<int>         m_stindices;
};

void
ObjReader::v(float x, float y, float z)
{
    m_vertices.push_back(x);
    m_vertices.push_back(y);
    m_vertices.push_back(z);
}

void
ObjReader::vt(float x, float y)
{
    m_sts.push_back(x);
    m_sts.push_back(y);
}

void ObjReader::vt(float x, float y, float z) { vt(x, y); }

void
ObjReader::vn(float x, float y, float z)
{
    m_normals.push_back(x);
    m_normals.push_back(y);
    m_normals.push_back(z);
}

void
ObjReader::f(const VertexIndices& vi,
             const NormalIndices& ni,
             const TextureIndices& ti)
{
    m_sizes.push_back(vi.size());
    m_types.push_back(GTO_POLYGON_POLYGON);

    copy(vi.begin(), vi.end(), back_inserter(m_indices));
    copy(ni.begin(), ni.end(), back_inserter(m_nindices));
    copy(ti.begin(), ti.end(), back_inserter(m_stindices));
}

void
ObjReader::finished()
{
    //
    //  Version 2 of GTO_PROTOCOL_POLYGON
    //

    Object* o = new Object("obj", m_protocol, 2);
    m_db->objects.push_back(o);

    Components& c = o->components;
    c.push_back(new Component(GTO_COMPONENT_POINTS, Gto::Matrix));
    c.push_back(new Component(GTO_COMPONENT_NORMALS, 0));
    c.push_back(new Component(GTO_COMPONENT_MAPPINGS, 0));
    c.push_back(new Component(GTO_COMPONENT_ELEMENTS, Gto::Matrix));
    c.push_back(new Component(GTO_COMPONENT_INDICES, Gto::Matrix));
    c.push_back(new Component(GTO_COMPONENT_SMOOTHING, 0));

    //
    //  Points properties
    //

    Property* p;

    p = new Property(GTO_PROPERTY_POSITION,
                     GTO_INTERPRET_COORDINATE,
                     Float, m_vertices.size() / 3, 3, true);
    copy(m_vertices.begin(), m_vertices.end(), p->floatData);
    c[0]->properties.push_back(p);

    //
    //  Normals properties
    //

    if (!m_normals.empty())
    {
        p = new Property(GTO_PROPERTY_NORMAL,
                         GTO_INTERPRET_NORMAL,
                         Float, m_normals.size() / 3, 3, true);
        copy(m_normals.begin(), m_normals.end(), p->floatData);
        c[1]->properties.push_back(p);
    }

    //
    //  Mappings properties
    //

    if (!m_sts.empty())
    {
        p = new Property(GTO_PROPERTY_ST,
                         GTO_INTERPRET_COORDINATE,
                         Float, m_sts.size() / 2, 2, true);
        copy(m_sts.begin(), m_sts.end(), p->floatData);
        c[2]->properties.push_back(p);
    }

    //
    //  Elements properties
    //

    p = new Property(GTO_PROPERTY_SIZE,
                     GTO_INTERPRET_SIZE,
                     Short, m_sizes.size(), 1, true);
    copy(m_sizes.begin(), m_sizes.end(), p->uint16Data);
    c[3]->properties.push_back(p);

    p = new Property(GTO_PROPERTY_TYPE, Byte, m_types.size(), 1, true);
    copy(m_types.begin(), m_types.end(), p->uint8Data);
    c[3]->properties.push_back(p);

    //
    //  Indices properties
    //

    p = new Property(GTO_PROPERTY_VERTEX,
                     GTO_INTERPRET_INDICES,
                     Int, m_indices.size(), 1, true);
    copy(m_indices.begin(), m_indices.end(), p->int32Data);
    c[4]->properties.push_back(p);

    if (!m_nindices.empty())
    {
        p = new Property(GTO_PROPERTY_NORMAL,
                         GTO_INTERPRET_INDICES,
                         Int, m_nindices.size(), 1, true);
        copy(m_nindices.begin(), m_nindices.end(), p->int32Data);
        c[4]->properties.push_back(p);
    }

    if (!m_stindices.empty())
    {
        p = new Property(GTO_PROPERTY_ST,
                         GTO_INTERPRET_INDICES,
                         Int, m_stindices.size(), 1, true);
        copy(m_stindices.begin(), m_stindices.end(), p->int32Data);
        c[4]->properties.push_back(p);
    }

    //
    //  Smoothing properties
    //

    if (!m_nindices.empty())
    {
        p = new Property(GTO_PROPERTY_METHOD, Int, 1, 1, true);
        p->int32Data[0] = GTO_SMOOTHING_METHOD_PARTITIONED;
        c[5]->properties.push_back(p);
    }
}

//----------------------------------------------------------------------

void
multMat(const float* m, float& x, float& y, float& z)
{
    float i = m[0*4 + 0] * x + m[0*4 + 1] * y + m[0*4 + 2] * z + m[0*4 + 3];
    float j = m[1*4 + 0] * x + m[1*4 + 1] * y + m[1*4 + 2] * z + m[1*4 + 3];
    float k = m[2*4 + 0] * x + m[2*4 + 1] * y + m[2*4 + 2] * z + m[2*4 + 3];
    float w = m[3*4 + 0] * x + m[3*4 + 1] * y + m[3*4 + 2] * z + m[3*4 + 3];
    x = i / w;
    y = j / w;
    z = k / w;
}

void
writeObj(ostream& out, Object* o)
{
    Property* verts     = 0;
    Property* normals   = 0;
    Property* types     = 0;
    Property* sizes     = 0;
    Property* sts       = 0;
    Property* vIndices  = 0;
    Property* nIndices  = 0;
    Property* stIndices = 0;
    Property* globalMat = 0;

    for (int i = 0; i < o->components.size(); i++)
    {
        Component* c = o->components[i];

        for (int j = 0; j < c->properties.size(); j++)
        {
            Property* p = c->properties[j];

            if (c->name == GTO_COMPONENT_POINTS &&
                p->name == GTO_PROPERTY_POSITION)
            {
                verts = p;
            }
            else if (c->name == GTO_COMPONENT_OBJECT &&
                     p->name == GTO_PROPERTY_GLOBAL_MATRIX)
            {
                globalMat = p;
            }
            else if (c->name == GTO_COMPONENT_NORMALS &&
                     p->name == GTO_PROPERTY_NORMAL)
            {
                normals = p;
            }
            else if (c->name == GTO_COMPONENT_MAPPINGS &&
                     p->name == GTO_PROPERTY_ST)
            {
                sts = p;
            }
            else if (c->name == GTO_COMPONENT_ELEMENTS &&
                     p->name == GTO_PROPERTY_SIZE)
            {
                sizes = p;
            }
            else if (c->name == GTO_COMPONENT_ELEMENTS &&
                     p->name == GTO_PROPERTY_TYPE)
            {
                types = p;
            }
            else if (c->name == GTO_COMPONENT_INDICES &&
                     p->name == GTO_PROPERTY_VERTEX)
            {
                vIndices = p;
            }
            else if (c->name == GTO_COMPONENT_INDICES &&
                     p->name == GTO_PROPERTY_NORMAL)
            {
                nIndices = p;
            }
            else if (c->name == GTO_COMPONENT_INDICES &&
                     p->name == GTO_PROPERTY_ST)
            {
                stIndices = p;
            }
        }
    }

    if (!verts || !vIndices)
    {
        cerr << "Missing some necessary properties" << endl;
        exit(-1);
    }

    for (int i=0; i < verts->size * verts->width; i+=3)
    {
        float x = verts->floatData[i];
        float y = verts->floatData[i+1];
        float z = verts->floatData[i+2];

        if (globalMat)
        {
            multMat(globalMat->floatData, x, y, z);
        }

        out << "v " << x
            << " " << y
            << " " << z
            << endl;
    }

    if (normals)
    {
        for (int i=0; i < normals->size * normals->width; i+=3)
        {
            out << "vn " << normals->floatData[i]
                << " " << normals->floatData[i+1]
                << " " << normals->floatData[i+2]
                << endl;
        }
    }

    if (sts)
    {
        for (int i=0; i < sts->size * sts->width; i+=2)
        {
            out << "vt " << sts->floatData[i]
                << " " << sts->floatData[i+1]
                << endl;
        }
    }

    for (int i=0, base=0; i < sizes->size; i++)
    {
        //  should really check to make sure this data is not bogus
        uint16 s = sizes->uint16Data[i];
        uint8 t = types->uint8Data[i];

        if (t == GTO_POLYGON_POLYGON ||
            t == GTO_POLYGON_TRIANGLE ||
            t == GTO_POLYGON_QUAD)
        {
            out << "f";

            for (int j=0; j < s; j++, base++)
            {
                out << " " << (vIndices->int32Data[base] + 1);

                if (stIndices)
                {
                    out << "/" << (stIndices->int32Data[base] + 1);

                    if (normals)
                    {
                        out << "/" << (nIndices->int32Data[base] + 1);
                    }
                }
                else if (normals)
                {
                    out << "//" << (nIndices->int32Data[base] + 1);
                }
            }

            out << endl;
        }
        else
        {
            // should output expanded strips and fan here
        }
    }
}

bool
writeObjDB(const char* filename, const char* obj, RawDataBase* db)
{
    ofstream file(filename);
    if (!file) return false;

    Object* outObj = 0;

    for (int i=0; i < db->objects.size(); i++)
    {
        Object* o = db->objects[i];

        if (o->protocol == GTO_PROTOCOL_POLYGON ||
            o->protocol == GTO_PROTOCOL_CATMULL_CLARK ||
            o->protocol == GTO_PROTOCOL_LOOP)
        {
            if (obj && o->name == obj)
            {
                outObj = o;
            }
            else
            {
                outObj = o;
            }
        }
    }

    writeObj(file, outObj);

    return true;
}

//----------------------------------------------------------------------

string
basename(string path)
{
    size_t lastSlash = path.rfind( "/" );

    if (lastSlash == path.npos)
    {
        return path;
    }

    return path.substr( lastSlash + 1, path.size() );
}


string
extension(const std::string& path)
{
    string filename(basename(path));

    if (filename.find( "." ) == filename.npos)
    {
        return string("");
    }
    else
    {
        return filename.substr( filename.rfind(".") + 1, filename.size() );
    }
}

//----------------------------------------------------------------------

void
usage()
{
    cout << "gto2obj [OPTIONS] INFILE OUTFILE" << endl
         << endl
         << "INFILE     an obj or gto file" << endl
         << "OUTFILE    an obj or gto file" << endl
         << "-c         output as catmull-clark surface" << endl
         << "-l         output as loop surface" << endl
         << "-t         output as text GTO (when appropriate)" << endl
         << "-nc        force uncompressed GTO (when appropriate)" << endl
         << "-o NAME    select gto object NAME for output" << endl
         << endl;

    exit(-1);
}

//----------------------------------------------------------------------

int main(int argc, char *argv[])
{
    char* inFile     = 0;
    char* outFile    = 0;
    char* protocol   = GTO_PROTOCOL_POLYGON;
    char* outObj     = 0;
    int   replace    = 0;
    int   nocompress = 0;
    int   text       = 0;

    for (int i=1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-c"))
        {
            protocol = GTO_PROTOCOL_CATMULL_CLARK;
        }
        else if (!strcmp(argv[i], "-l"))
        {
            protocol = GTO_PROTOCOL_LOOP;
        }
        else if (!strcmp(argv[i], "-t"))
        {
            text = 1;
        }
        else if (!strcmp(argv[i], "-nc"))
        {
            nocompress = 1;
        }
        else if (!strcmp(argv[i], "-o"))
        {
            if (++i >= argc) usage();
            outObj = argv[i];
        }
        else if (*argv[i] == '-')
        {
            usage();
        }
        else
        {
            if (!inFile) inFile = argv[i];
            else if (!outFile) outFile = argv[i];
            else usage();
        }
    }

    //
    //  Check for bad options
    //

    if (!inFile || !outFile)
    {
        cerr << "ERROR: no infile or outfile specified.\n" << endl;
        usage();
    }

    string iext = extension(inFile);
    string oext = extension(outFile);

    if (iext == oext)
    {
        cerr << "ERROR: input and output extensions are the same" << endl;
        usage();
    }

    if ((iext != "obj" && iext != "gto") || (oext != "obj" && oext != "gto"))
    {
        cerr << "ERROR: ivalid file extension(s)" << endl;
        usage();
    }

    //
    //  In
    //

    RawDataBaseReader reader;
    RawDataBase* db;
    cout << "INFO: reading " << inFile << endl;

    if (iext == "obj")
    {
        db = new RawDataBase;
        ObjReader reader(db, protocol);

        if (!reader.parseFile(inFile))
        {
            cerr << "ERROR: reading file " << inFile << endl;
            exit(-1);
        }
    }
    else
    {
        if (!reader.open(inFile))
        {
            cerr << "ERROR: reading file " << inFile << endl;
            exit(-1);
        }

        db = reader.dataBase();
    }

    //
    //  Out
    //

    cout << "INFO: writing " << outFile << endl;

    if (oext == "gto")
    {
        RawDataBaseWriter writer;
        Writer::FileType type = Writer::CompressedGTO;
        if (nocompress) type = Writer::BinaryGTO;
        if (text) type = Writer::TextGTO;

        if (!writer.write(outFile, *db, type))
        {
            cerr << "ERRROR: writing file " << outFile << endl;
            exit(-1);
        }
    }
    else
    {
        if (!writeObjDB(outFile, outObj, db))
        {
            cerr << "ERRROR: writing file " << outFile << endl;
            exit(-1);
        }
    }

    return 0;
}
