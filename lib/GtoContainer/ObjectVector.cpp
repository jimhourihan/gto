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

#include "ObjectVector.h"
#include <algorithm>

namespace GtoContainer {

//-*****************************************************************************
void ObjectVector::deleteContents()
{
    for ( iterator iter = begin(); iter != end(); ++iter )
    {
        delete (*iter);
    }
    
    erase( begin(), end() );
}

//-*****************************************************************************
void ObjectVector::removeWithoutDeleting( const PropertyContainer *pc )
{
    iterator startOfFind = begin();
    while( 1 )
    {
        iterator found = std::find( startOfFind, end(), pc );
        if ( found == end() )
        {
            break;
        }

        startOfFind = found;
        ++startOfFind;

        erase( found );
    }
}

//-*****************************************************************************
//-*****************************************************************************
// PROTOCOL-BASED SEARCH FUNCTIONS
//-*****************************************************************************
//-*****************************************************************************
PropertyContainer *ObjectVector::findFirstOfProtocol( const Protocol &p )
{
    for ( iterator iter = begin(); iter != end(); ++iter )
    {
        if ( (*iter)->protocol() == p )
        {
            return (*iter);
        }
    }

    return NULL;
}

//-*****************************************************************************
const PropertyContainer *
ObjectVector::findFirstOfProtocol( const Protocol &p ) const
{
    for ( const_iterator iter = begin(); iter != end(); ++iter )
    {
        if ( (*iter)->protocol() == p )
        {
            return (*iter);
        }
    }

    return NULL;
}

//-*****************************************************************************
void ObjectVector::findAllOfProtocol( const Protocol &p,
                                      std::vector<PropertyContainer *> &into )
{
    for ( iterator iter = begin(); iter != end(); ++iter )
    {
        if ( (*iter)->protocol() == p )
        {
            into.push_back( (*iter) );
        }
    }
}


//-*****************************************************************************
void
ObjectVector::findAllOfProtocol(
    const Protocol &p,
    std::vector<const PropertyContainer *> &into ) const
{
    for ( const_iterator iter = begin(); iter != end(); ++iter )
    {
        if ( (*iter)->protocol() == p )
        {
            into.push_back( (*iter) );
        }
    }
}


//-*****************************************************************************
//-*****************************************************************************
// NAME-BASED SEARCH FUNCTIONS
//-*****************************************************************************
//-*****************************************************************************
PropertyContainer *ObjectVector::findFirstOfName( const std::string &nme )
{
    for ( iterator iter = begin(); iter != end(); ++iter )
    {
        if ( (*iter)->name() == nme )
        {
            return (*iter);
        }
    }

    return NULL;
}

//-*****************************************************************************
const PropertyContainer *
ObjectVector::findFirstOfName( const std::string &nme ) const
{
    for ( const_iterator iter = begin(); iter != end(); ++iter )
    {
        if ( (*iter)->name() == nme )
        {
            return (*iter);
        }
    }

    return NULL;
}

//-*****************************************************************************
void ObjectVector::findAllOfName( const std::string &nme,
                                  std::vector<PropertyContainer *> &into )
{
    for ( iterator iter = begin(); iter != end(); ++iter )
    {
        if ( (*iter)->name() == nme )
        {
            into.push_back( (*iter) );
        }
    }
}


//-*****************************************************************************
void
ObjectVector::findAllOfName(
    const std::string &nme,
    std::vector<const PropertyContainer *> &into ) const
{
    for ( const_iterator iter = begin(); iter != end(); ++iter )
    {
        if ( (*iter)->name() == nme )
        {
            into.push_back( (*iter) );
        }
    }
}

} // End namespace GtoContainer
