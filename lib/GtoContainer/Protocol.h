//-*****************************************************************************
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
//-*****************************************************************************

#ifndef _GtoContainer_Protocol_h_
#define _GtoContainer_Protocol_h_

#include <Gto/Header.h>
#include <iostream>
#include <string>

namespace GtoContainer {

//-*****************************************************************************
struct Protocol
{
    Protocol() : protocol( "" ), version( 0 ) {}
    Protocol( const std::string &p, Gto::uint32 v )
      : protocol( p ), version( v ) {}
    Protocol( const Protocol &copy )
      : protocol( copy.protocol ),
        version( copy.version ) {}

    Protocol &operator=( const Protocol &copy )
    {
        protocol = copy.protocol;
        version = copy.version;
        return *this;
    }
    
    std::string protocol;
    Gto::uint32 version;
};

} // End namespace GtoContainer

//-*****************************************************************************
inline bool operator==( const GtoContainer::Protocol &a,
                        const GtoContainer::Protocol &b )
{
    return ( a.protocol == b.protocol ) && ( a.version == b.version );
}


//-*****************************************************************************
inline bool operator!=( const GtoContainer::Protocol &a,
                        const GtoContainer::Protocol &b )
{
    return ( a.protocol != b.protocol ) || ( a.version != b.version );
}

//-*****************************************************************************
inline bool operator<( const GtoContainer::Protocol &a,
                       const GtoContainer::Protocol &b )
{
    if ( a.protocol == b.protocol )
    {
        return a.version < b.version;
    }
    else
    {
        return a.protocol < b.protocol;
    }
}

//-*****************************************************************************
inline bool operator>( const GtoContainer::Protocol &a,
                       const GtoContainer::Protocol &b )
{
    if ( a.protocol == b.protocol )
    {
        return a.version > b.version;
    }
    else
    {
        return a.protocol > b.protocol;
    }
}

//-*****************************************************************************
inline bool operator<=( const GtoContainer::Protocol &a,
                        const GtoContainer::Protocol &b )
{
    if ( a.protocol == b.protocol )
    {
        return a.version <= b.version;
    }
    else
    {
        return a.protocol < b.protocol;
    }
}

//-*****************************************************************************
inline bool operator>=( const GtoContainer::Protocol &a,
                        const GtoContainer::Protocol &b )
{
    if ( a.protocol == b.protocol )
    {
        return a.version >= b.version;
    }
    else
    {
        return a.protocol > b.protocol;
    }
}

//-*****************************************************************************
inline std::ostream &operator<<( std::ostream &str,
                                 const GtoContainer::Protocol &a )
{
    str << a.protocol << "(" << a.version << ")";
    return str;
}

#endif







