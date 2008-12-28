#ifndef __GtoGraph__Context__h__
#define __GtoGraph__Context__h__
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
#include <map>
#include <GtoGraph/Attribute.h>
#include <Gto/Header.h>

/// 

class Context
{
  public:
    //
    //  Associated Types
    //

    typedef Attribute* (*AttributeConstructor)(const std::string&, Gto::DataType, size_t);
    typedef std::map<std::string, AttributeConstructor> ConstructorMap;

    

  private:
    ConstructorMap m_constructorMap;
};


#endif // __GtoGraph__Context__h__
