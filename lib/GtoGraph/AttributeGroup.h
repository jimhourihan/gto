#ifndef __GtoGraph__AttributeGroup__h__
#define __GtoGraph__AttributeGroup__h__
//
// Copyright (C) 2008 Tweak Software
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
#include <GtoGraph/Attribute.h>

class AttributeGroup
{
  public:
    //
    //  Associated Types
    //

    typedef std::vector<Attribute*> Attributes;

    explicit AttributeGroup(const std::string& name, bool transposable=false);
    ~AttributeGroup();

    bool isSynchronized() const { return m_transposable; }
    bool isTransposable() const { return m_transposable; }
    const std::string& name() const { return m_name; }

    void add(Attribute* a) { m_attributes.push_back(a); }

  private:
    Attributes  m_attributes;
}


#endif // __GtoGraph__AttributeGroup__h__
