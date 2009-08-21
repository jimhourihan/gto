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
#ifndef __WFObj__Reader__h__
#define __WFObj__Reader__h__
#include <vector>
#include <string>
#include <iostream>

namespace WFObj {

//
//  The Reader class will call its own public virtual functions when
//  its state changes during parsing. The default actions are to
//  ignore everything.
//

class Reader
{
  public:
    Reader();
    virtual ~Reader();

    //
    //	Parse it. parseFile() returns false if it fails to open the file.
    //

    virtual void parse(std::istream&i);
    virtual bool parseFile(const char*);

    virtual void finished();

    //
    //	types
    //

    typedef std::vector<int>		VertexIndices;
    typedef std::vector<int>		NormalIndices;
    typedef std::vector<int>		TextureIndices;
    typedef std::vector<std::string>	Strings;
    typedef int				Id;
    
    //
    //	v? data
    //

    virtual void v(float,float,float);
    virtual void v(float,float,float,float);
    virtual void vt(float);
    virtual void vt(float,float);
    virtual void vt(float,float,float);
    virtual void vn(float,float,float);
    virtual void vp(float,float);
    virtual void vp(float);

    //
    //	element data: faces / lines / points
    //

    virtual void f(const VertexIndices&,
		   const NormalIndices&,
		   const TextureIndices&);

    virtual void l(const VertexIndices&,
		   const NormalIndices&,
		   const TextureIndices&);

    virtual void p(const VertexIndices&,
		   const NormalIndices&,
		   const TextureIndices&);

    //
    //	Called when a group name is used that has not yet been seen
    //

    virtual void newGroup(const std::string&);

    //
    //	Called when active groups change
    //

    virtual void activeGroups(const Strings&);

    //
    //	Called when an "o" line is encountered.
    //

    virtual void activeObject(const std::string&);

    //
    //	Called when smoothing group changes -- note that Id == 0 means
    //	no smoothing -- this is equivalent to the "s off" statement.
    //

    virtual void smoothingGroup(Id);

    //
    //	Bevel/cinterp/dinterp
    //

    virtual void bevel(bool);
    virtual void cinterp(bool);
    virtual void dinterp(bool);

    //
    //	Material and map
    //

    virtual void mtllib(const std::string&);
    virtual void maplib(const std::string&);
    virtual void usemap(const std::string&);
    virtual void usemtl(const std::string&);

    //
    //	Trace/Shadow objects

    virtual void trace_obj(const std::string&);
    virtual void shadow_obj(const std::string&);

    //
    //	Level of detail number
    //

    virtual void lod(int);
};


} // WFObj

#endif // __WFObj__Reader__h__

