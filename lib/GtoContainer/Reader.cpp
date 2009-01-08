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

#include <GtoContainer/Reader.h>
#include <GtoContainer/Exception.h>
#include <GtoContainer/StdProperties.h>
#include <iostream>
#include <algorithm>

namespace GtoContainer {

//-*****************************************************************************
Reader::Reader() 
  : Gto::Reader( 0 ),
    m_useExisting( false ),
    m_objects( NULL )
{
    // Add the standard meta propreties.
    AppendStdMetaProperties( m_metaProperties );
}

//-*****************************************************************************
Reader::~Reader()
{
    for ( MetaProperties::iterator iter = m_metaProperties.begin();
          iter != m_metaProperties.end(); ++iter )
    {
        delete (*iter);
    }
}

//-*****************************************************************************
void
Reader::read( const std::string &filename,
              ObjectVector &objects,
              bool readIntoExisting )
{
    m_useExisting = readIntoExisting;
    m_objects = &objects;
    
    if ( !open( filename.c_str() ) )
    {
        std::string msg = ", opening .gto file \"";
        msg += filename;
        msg += "\", " + why();
        throw ReadFailedExc( msg.c_str() );
    }

    // Reset.
    m_useExisting = false;
    m_objects = NULL;
}

//-*****************************************************************************
PropertyContainer *
Reader::newContainer( const Protocol &protocol )
{
    return new PropertyContainer;
}

//-*****************************************************************************
Property * 
Reader::newProperty( const std::string &name, const PropertyInfo & )
{
    return NULL;
}

//-*****************************************************************************
Reader::Request
Reader::object( const std::string &name, 
                const std::string &prot, 
                unsigned int protVersion,
                const ObjectInfo &info ) 
{
    if ( m_objects == NULL )
    {
        GTC_THROW( "Reader reading without objects" );
    }
    
    PropertyContainer *g = NULL;
    Protocol protocol( prot, protVersion );
    bool found = false;

    if ( m_useExisting )
    {
        for ( int i = 0; i < m_objects->size(); ++i )
        {
            PropertyContainer *pc = (*m_objects)[i];

            if ( pc->name() == name &&
                 pc->protocol() == protocol )
            {
                found = true;
                g = pc;
            }
        }
    }
    
    if ( !g )
    {
        g = newContainer( protocol );
        g->setName( name );
        g->setProtocol( protocol );
    }

    if ( !found ) { m_objects->push_back( g ); }

    return Request( true, g );
}

//-*****************************************************************************
Reader::Request
Reader::component( const std::string &name, 
                      const std::string &ininterp,
                      const ComponentInfo &info ) 
{
    std::string interp = ininterp;
    if ( fileHeader().version < 3 ) { interp = GTO_INTERPRET_DEFAULT; }
    
    if ( m_objects == NULL )
    {
        GTC_THROW( "Reader reading without objects" );
    }
    
    PropertyContainer *g  = 
        reinterpret_cast<PropertyContainer*>( info.object->objectData );
    Component *c = g->component( name );
    
    if ( !c )
    {
        // adds it too.
        c = g->createComponent( name, info.flags & Gto::Matrix );
    }

    return Request( true, c );
}

//-*****************************************************************************
Reader::Request
Reader::property( const std::string &name, 
                  const std::string &pre_ininterp,
                  const PropertyInfo &info ) 
{
    // Fix interp for versions prior to 3.
    std::string ininterp = pre_ininterp;
    if ( fileHeader().version < 3 )
    {
        ininterp = GTO_INTERPRET_DEFAULT;
    }
    
    if ( m_objects == NULL )
    {
        GTC_THROW( "Reader reading without objects" );
    }
    
    std::string interp;

    // In case you need the property container, here it is.
    // PropertyContainer *pc  = 
    //    reinterpret_cast<PropertyContainer*>(
    //         info.component->object->objectData );
    
    Component *c =
        reinterpret_cast<Component*>( info.component->componentData );
    Property *p  = c->find( name );
    Property *np = NULL;

    // Only use the part of the interpretation up to the first semicolon.
    size_t sq = ininterp.find( ';' );

    if ( sq != std::string::npos )
    {
        interp = ininterp.substr( 0, sq );
    }
    else
    {
        interp = ininterp;
    }

    // If there's no interpretation, change the interpretation to
    // GTO_INTERPRET_DEFAULT
    if ( interp == "" )
    {
        interp = GTO_INTERPRET_DEFAULT;
    }   

    // If the property doesn't exist, try the virtual 'newProperty' function
    // which returns NULL by default. If it makes a property, awesome! Use
    // that. If not, continue below.
    if ( p == NULL )
    {
        if ( np = newProperty( name, info ) )
        {
            c->add( np );
            np->resize( info.size );
            return Request( true, np );
        }
    }
    
    // From the layout, width & interpretation, build us a new
    // property, OR, verify that the old property is the right upcastable
    // type.
    Layout layout = gtoTypeToLayout( ( Gto::DataType )( info.type ) );
    const MetaProperty *metaProp = findMetaProperty( layout,
                                                     info.width,
                                                     interp );
    if ( metaProp == NULL )
    {
        std::cerr << "GtoContainer::Reader WARNING: "
                  << "Ignoring property \"" << name << "\""
                  << std::endl;
        return Request( false );
    }

    Property *newProp = NULL;
    if ( p != NULL )
    {
        if ( !metaProp->validUpcast( p ) )
        {
            throw TypeMismatchExc();
        }

        newProp = p;
    }
    else
    {
        newProp = metaProp->create( name );
        c->add( newProp );
    }

    // If we get here, we've got a new property, ready to receive data.
    // Resize it and send out the Request.
    assert( newProp != NULL );
    newProp->resize( info.size );
    return Request( true, newProp );
}

//-*****************************************************************************
void *
Reader::data( const PropertyInfo &info, size_t bytes )
{
    Property *p = reinterpret_cast<Property*>( info.propertyData );
    p->resize( info.size );

    // no dereferencing empty array's
    if ( p->size() == 0 )
    {
        return NULL;
    }

    if ( StringProperty *sp = dynamic_cast<StringProperty*>( p ) )
    {
        m_tempstrings.resize( sp->size() );
        return &m_tempstrings.front();
    }

    return p->rawData();
}

//-*****************************************************************************
void
Reader::dataRead( const PropertyInfo &info )
{
    Property *p  = reinterpret_cast<Property*>( info.propertyData );

    if ( StringProperty *sp = dynamic_cast<StringProperty*>( p ) )
    {
        for ( int i = 0; i < m_tempstrings.size(); ++i )
        {
            (*sp)[i] = stringFromId( m_tempstrings[i] );
        }

        m_tempstrings.clear();
    }
}

//-*****************************************************************************
const MetaProperty *
Reader::findMetaProperty( Layout lyt, size_t wdth,
                          const std::string &interp ) const
{
    for ( MetaProperties::const_reverse_iterator iter =
              m_metaProperties.rbegin();
          iter != m_metaProperties.rend(); ++iter )
    {
        if ( (*iter)->canHandle( lyt, wdth, interp ) > 0 )
        {
            return (*iter);
        }
    }

    return NULL;
}

} // End namespace GtoContainer
