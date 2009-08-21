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
 
