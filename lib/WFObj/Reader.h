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

