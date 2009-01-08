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

#include <GtoContainer/Component.h>
#include <GtoContainer/Property.h>
#include <GtoContainer/Exception.h>
#include <GtoContainer/Utilities.h>

#include <algorithm>
#include <functional>
#include <iostream>

namespace GtoContainer {

//-*****************************************************************************
Component::Component( const std::string& name, bool transposable )
  : m_name( name ),
    m_transposable( transposable )
{
    // nothing
}

//-*****************************************************************************
// We've removed the ability for properties to be shared between property
// containers, moving to an aggregation model for interpretations. Therefore,
// we just nuke the properties.
Component::~Component()
{
    for ( Properties::iterator iter = m_properties.begin();
          iter != m_properties.end(); ++iter )
    {
        delete (*iter);
    }
    
    m_properties.clear();
}

//-*****************************************************************************
void Component::remove( Property *p )
{
    // This removes the property from the list. The property is NOT deleted.
    GtoContainer::remove( m_properties, p );
}

//-*****************************************************************************
void Component::remove( const std::string &name )
{
    if ( Property *p = find( name ) ) { remove( p ); }
}

//-*****************************************************************************
void Component::add( Property *p )
{
    for ( int i = 0; i < m_properties.size(); ++i )
    {
	if ( m_properties[i]->name() == p->name() )
	{
	    throw UnexpectedExc(
                ", property with same name already exists" );
	}
    }
    
    m_properties.push_back( p );
}

//-*****************************************************************************
const Property *Component::find( const std::string &name ) const
{
    for ( int i = 0; i < m_properties.size(); ++i )
    {
	if ( m_properties[i]->name() == name ) { return m_properties[i]; }
    }
    
    return NULL;
}

//-*****************************************************************************
Property *Component::find( const std::string &name )
{
    for ( int i = 0; i < m_properties.size(); ++i )
    {
	if ( m_properties[i]->name() == name ) { return m_properties[i]; }
    }

    return NULL;
}

//-*****************************************************************************
void Component::resize( size_t s )
{
    for ( int i = 0, n = m_properties.size(); i < n; ++i )
    {
        if ( m_properties[i]->size() != s ) { m_properties[i]->resize(s); }
    }
}

//-*****************************************************************************
void Component::resizeNonZero()
{
    for ( int i = 0, s = m_properties.size(); i < s; ++i )
    {
        if ( m_properties[i]->size() != 0 )
        {
            // This->resize
            resize( m_properties[i]->size() );
            return;
        }
    }
}

//-*****************************************************************************
Component *Component::copy() const
{
    Component *c = new Component( name(), isTransposable() );
    
    for ( int i = 0; i < m_properties.size(); ++i )
    {
	c->add( m_properties[i]->copy() );
    }

    return c;
}

//-*****************************************************************************
void Component::copy( const Component *other )
{
    const Properties &props = other->properties();

    for ( int i = 0; i < props.size(); ++i )
    {
        const Property *p0 = props[i];

        if ( Property *p1 = find( p0->name() ) )
        {
            p1->copy( p0 );
        }
        else
        {
            add( p0->copy() );
        }
    }
}

//-*****************************************************************************
void Component::concatenate( const Component *c )
{
    for ( int i = 0; i < m_properties.size(); ++i )
    {
        Property *p0 = m_properties[i];

        if ( const Property *p1 = c->find( p0->name() ) )
        {
            p0->concatenate( p1 );
        }
    }
}

//-*****************************************************************************
void Component::erase( size_t start, size_t num )
{
    for ( int i = 0; i < m_properties.size(); ++i )
    {
        Property *p = m_properties[i];
        p->erase( start, num );
    }
}

//-*****************************************************************************
void Component::eraseUnsorted( size_t start, size_t num )
{
    for ( int i = 0; i < m_properties.size(); ++i )
    {
        Property *p = m_properties[i];
        p->eraseUnsorted( start, num );
    }
}

//-*****************************************************************************
bool Component::isPersistent() const
{
    for ( int i = 0; i < m_properties.size(); ++i )
    {
        Property *p = m_properties[i];
        if ( !p->isPersistent() )
        {
            return true;
        }
    }
    
    return false;
}

} // End namespace GtoContainer


