//
// Copyright (C) 2003 Tweak Films
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

#ifndef _GtoInSet_h_
#define _GtoInSet_h_

#include <string>
#include <map>
#include <vector>

#include "GtoInObject.h"

namespace GtoIOPlugin {

// Just a list of objects
class Set
{
protected:
    friend class DataBase;
    Set( const std::string &fileName );
    
    ~Set();

public:
    void addObject( Object *obj );

    Object *object( const std::string &name );

    void computeLocalTransforms();

    void declareMaya();

    void declareMayaDiff();
    
    void reparentAll();

private:
    std::string m_fileName;
    std::vector<Object *> m_objects;

    // Map of old->new name changes
    std::map<std::string, std::string> m_changedNames;
};

} // End namespace GtoIOPlugin

#endif
