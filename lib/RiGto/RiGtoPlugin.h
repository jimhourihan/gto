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

#ifndef _RiGtoPlugin_h_
#define _RiGtoPlugin_h_

#include <RiGto/RiGtoDataBase.h>
#include <RiGto/RiGtoSet.h>
#include <RiGto/RiGtoOnOffList.h>

namespace RiGto {

class Plugin
{
public:
    Plugin( DataBase &dataBase,
            const char *configStr,
            bool verbose = true );
    ~Plugin();

    void declareRi() const;
        
protected:
    DataBase &m_dataBase;
    const Set *m_set;
    OnOffList m_onList;
    OnOffList m_offList;
    OnOffList m_onList2;
    OnOffList m_offList2;
};

} // End namespace RiGto

#endif
