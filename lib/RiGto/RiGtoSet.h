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

#ifndef _RiGtoSet_h_
#define _RiGtoSet_h_

#include <RiGto/RiGtoObject.h>
#include <RiGto/RiGtoOnOffList.h>
#include <string>
#include <vector>

namespace RiGto {

// Just a list of objects that understands regular expressions.
class Set
{
protected:
    friend class DataBase;
    Set( const std::string &ref,
         const std::string &open,
         const std::string &close );
    
    ~Set();
    
    bool isMe( const std::string &ref,
               const std::string &open,
               const std::string &close ) const
    {
        return ( m_ref == ref &&
                 m_open == open &&
                 m_close == close );
    }

    void doneReading( ReaderPhase rp );

public:
    void addObject( Object *obj ) { m_objects.push_back( obj ); }

    Object *object( const std::string &name );

    void declareRi( const OnOffList &onList,
                    const OnOffList &offList,
                    const OnOffList &onList2,
                    const OnOffList &offList2 ) const;

    const std::string &ref() const { return m_ref; }
    const std::string &open() const { return m_open; }
    const std::string &close() const { return m_close; }

private:
    std::string m_ref;
    std::string m_open;
    std::string m_close;
    
    std::vector<Object *> m_objects;
};

} // End namespace RiGto

#endif
