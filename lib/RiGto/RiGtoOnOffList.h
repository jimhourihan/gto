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

#ifndef __RIGTOONOFFLIST_H__
#define __RIGTOONOFFLIST_H__

#include <vector>
#include <string>
#include <regex.h>

// Implements a very efficient regular expression based on/off list

namespace RiGto {

class OnOffList
{
public:
    OnOffList( const std::string &lst )
    {
        init( lst.c_str() );
    }
    OnOffList( const char *lst )
    {
        init( lst );
    }
    ~OnOffList();

    bool has( const std::string &str ) const;
    bool has( const char *str ) const { return has( std::string( str ) ); }

protected:
    void init( const char *str );

private:
    std::vector<regex_t> m_regexList;
};




}  //  End namespace RiGto

#endif    // End #ifdef __RIGTOONOFFLIST_H__

