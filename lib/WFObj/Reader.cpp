//******************************************************************************
// Copyright (c) 2002 Tweak Films
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
//******************************************************************************

#include "Reader.h"
#include <fstream>

extern void WFObjParse(std::istream&,WFObj::Reader*);

namespace WFObj {
using namespace std;

Reader::Reader() {}
Reader::~Reader() {}

void Reader::finished() {}

void Reader::v(float x,float y,float z) {}
void Reader::v(float x,float y,float z,float w) { v(x,y,z); }
void Reader::vt(float) {}
void Reader::vt(float,float) {}
void Reader::vt(float,float,float) {}
void Reader::vn(float,float,float) {}
void Reader::vp(float,float) {}
void Reader::vp(float) {}
void Reader::f(const Reader::VertexIndices&, 
	       const Reader::NormalIndices&,
	       const Reader::TextureIndices&) {}
void Reader::l(const Reader::VertexIndices&,
	       const Reader::NormalIndices&,
	       const Reader::TextureIndices&) {}
void Reader::p(const Reader::VertexIndices&,
	       const Reader::NormalIndices&,
	       const Reader::TextureIndices&) {}

void Reader::newGroup(const std::string&) {}
void Reader::activeGroups(const Reader::Strings&) {}
void Reader::smoothingGroup(Reader::Id) {}
void Reader::activeObject(const std::string&) {}
void Reader::usemtl(const std::string&) {}
void Reader::usemap(const std::string&) {}
void Reader::mtllib(const std::string&) {}
void Reader::maplib(const std::string&) {}
void Reader::shadow_obj(const std::string&) {}
void Reader::trace_obj(const std::string&) {}
void Reader::lod(int) {}

void Reader::bevel(bool) {}
void Reader::cinterp(bool) {}
void Reader::dinterp(bool) {}

void
Reader::parse(istream &i)
{
    WFObjParse(i,this);
    finished();
}

bool
Reader::parseFile(const char *filename)
{
    ifstream file(filename);

    if (file) 
    {
	parse(file);
	return true;
    }
    else
    {
	cerr << "Couldn't open the file " << filename << endl << flush;
	return false;
    }
}


} // WFObj
 
