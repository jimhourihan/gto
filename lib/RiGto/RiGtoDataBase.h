//
// Copyright (C) 2004 Tweak Films
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

#ifndef _RiGtoDataBase_h_
#define _RiGtoDataBase_h_

#include <RiGto/RiGtoSet.h>
#include <string>
#include <vector>

namespace RiGto {

class DataBase
{
public:
    DataBase();
    ~DataBase();
    
    const Set *set( const std::string &rest,
                    const std::string &open,
                    const std::string &close );

    void destroyAll();
    
    void destroySet( const std::string &rest,
                     const std::string &open,
                     const std::string &close );

protected:
    std::vector<Set *> m_sets;
};

} // End namespace RiGto

#endif
