#ifndef __GtoGraph__Connection__h__
#define __GtoGraph__Connection__h__
//
// Copyright (C) 2008  Jim Hourihan
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

class ConnectionType
{
  public:
    //
    //  Associated Types
    //

    enum Attributes
    {
        Many,
        One,
        Zero,
        Ground
    };

    typedef std::vector<ConnectionType*> Types;

    //
    //  Constructor
    //

    ConnectionType(const std::string &name,
                   Attributes LHS = Many,
                   Attributes RHS = Many);

    virtual ~ConnectionType();

    //
    //  Info
    //

    const std::string& name() const { return m_name; }
    Attributes lhsAttributes() const { return m_lhs; }
    Attributes rhsAttributes() const { return m_rhs; }
    Dependancy dependancy() const { return m_dependancy; }

    static const Types& types() { return m_types; }
    static const ConnectionType* find(const std::string& type);

    //
    //  Information about the connection type
    //

    virtual bool compatible(const Container* lhs, const Container* rhs) const;

    //
    //  Create/Destroy actual connection machinations. Establish
    //  creates the literal underlying structures and abolish removes
    //  them.
    //

    virtual Connection* establish(Container* lhs, Container* rhs) const;
    virtual void abolish(Connection*) const;

    //
    //  File I/O
    //

    virtual void serialize(const std::vector<Connection*>&,
                           AttributeGroup*) const;

    virtual void unserialize(Scene*,
                             AttributeGroup*,
                             const std::string& prefix) const;

private:
    static Types    m_types;
    std::string     m_name;
    Attributes      m_lhs;
    Attributes      m_rhs;
};
}

class Connection
{
  public:
    Connection();
};


#endif // __GtoGraph__Connection__h__
