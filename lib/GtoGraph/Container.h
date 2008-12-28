#ifndef __GtoGraph__Container__h__
#define __GtoGraph__Container__h__
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
#include <string>
#include <GtoGraph/AttributeGroup.h>

class Container 
{
  public:
    //
    //  Associated Types
    //

    typedef std::vector<AttributeGroup*> AttributeGroupVector;

    Container();
    virtual ~Container();

    void setName(const std::string&);
    const std::string& name() const { return m_name; }

    void createGroup(const std::string&);
    AttributeGroup* group(const std::string&);
    const AttributeGroup* group(const std::string&) const;

    template <class T, class Base = Attribute>
    Attribute* createAttribute(const std::string& group,
                               const std::string& name);

  private:
    std::string             m_name;
    AttributeGroupVector    m_groups;
};

template <class AttrType, class AttrBase>
Attribute* 
Container::createAttribute<AttrType,AttrBase>(const std::string& group,
                                              const std::string& name)
{
    AttributeGroup* group = createGroup(group);
    TypedAttribute<AttrType, AttrBase>* attr = 
        new TypedAttribute<AttrType, AttrBase>(name);

    group->add(attr);
    return attr;
}


#endif // __GtoGraph__Container__h__
