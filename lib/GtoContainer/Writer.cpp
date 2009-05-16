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

#include <GtoContainer/Writer.h>
#include <GtoContainer/PropertyContainer.h>
#include <GtoContainer/StdProperties.h>
#include <GtoContainer/ObjectVector.h>
#include <iostream>

namespace GtoContainer {

//-*****************************************************************************
Writer::Writer( const char *stamp )
  : m_writer()
{
    if ( stamp ) 
    {
        m_stamp = stamp;
    }   
}

//-*****************************************************************************
Writer::~Writer()
{
    // Nothing
}

//-*****************************************************************************
void
Writer::writeProperty( bool header, const Property *property )
{
    if ( !property->isPersistent() )
    {
        return;
    }

    if ( property->layoutTrait() == CompoundLayout )
    {
        std::cerr << "WARNING: Writer does not understand "
                  << "type of property "
                  << property->name()
                  << std::endl;
        return;
    }

    Gto::DataType type = layoutToGtoType( property->layoutTrait() );
    std::string interp = property->interpretationTrait();
    if ( interp == GTO_INTERPRET_DEFAULT )
    {
        interp = "";
    }
    int width = property->widthTrait();

    //-*************************************************************************
    // Handle String properties differently, because Strings have to be
    // interred.
    if ( const StringProperty *sp =
         dynamic_cast<const StringProperty*>( property ) )
    {
	if ( header )
	{
	    for ( int i = 0; i < sp->size(); ++i )
	    {
		const std::string &s = (*sp)[i];
		m_writer.intern( s );
	    }

            m_writer.property( property->name().c_str(),
                               type,
                               property->size(),
                               width,
                               interp.c_str() );
	}
	else
	{
	    int *data = new int[ sp->size() ];

	    for ( int i = 0; i < sp->size(); ++i )
	    {
		const std::string &s = (*sp)[i];
		data[i] = m_writer.lookup( s );
	    }

	    m_writer.propertyData( data );
	}
    }
    else
    {
        if ( header )
        {
            m_writer.property( property->name().c_str(),
                               type,
                               property->size(),
                               width,
                               interp.c_str() );
        }
        else
        {
            if ( !property->empty() )
            {
                m_writer.propertyDataRaw( property->rawData() );
            }
            else
            {
                m_writer.propertyDataRaw( 0 );
            }
        }
    }
}

//-*****************************************************************************
void
Writer::writeComponent( bool header, const Component *component )
{
    if ( component->isPersistent() )
    {
        const Component::Container &props = component->properties();
    
        unsigned int flags = component->isTransposable() ? Gto::Matrix : 0;
        if ( header )
        {
            m_writer.beginComponent( component->name().c_str(), flags );
        }

        for ( int i = 0; i < props.size(); ++i )
        {
            const Property *p = props[i];
            writeProperty( header, p );
        }

        if ( header )
        {
            m_writer.endComponent();
        }
    }
}

//-*****************************************************************************
bool
Writer::write( const char *filename,
               const ObjectVector &objects,
               FileType type )
{
    if ( !m_writer.open( filename, type ) )
    {
        return false;
    }

    // Intern the stamp.
    if ( m_stamp != "" )
    {
        m_writer.intern( m_stamp );
    }

    // Intern useful build info.
    m_writer.intern( "(GtoContainer::Writer compiled " __DATE__
                     " " __TIME__ " )" );

    for ( int i = 0; i < objects.size(); ++i )
    {
	const PropertyContainer *container = objects[i];
        std::string name = container->name();
        Protocol protocol = container->protocol();

        if ( !container->isPersistent() ) { continue; }

        if ( name == "" ) { name = "NO_NAME"; }

        if ( protocol.protocol == "" )
        {
            protocol.protocol = "NO_PROTOCOL";
            protocol.version = 0;
        }

	m_writer.beginObject( name.c_str(), 
                              protocol.protocol.c_str(),
                              protocol.version );

	const PropertyContainer *g = container;
	const PropertyContainer::Components &comps = g->components();

	for ( int q = 0; q < comps.size(); ++q )
	{
	    writeComponent( true, comps[q] );
	}

	m_writer.endObject();
    }

#ifdef GTO_ORDERED_STRING_TABLE
    std::string orderedStrings = "";
    m_writer.beginData( &orderedStrings, 1 );
#else
    m_writer.beginData();
#endif
    
    for ( int i = 0; i < objects.size(); ++i )
    {
	const PropertyContainer *g = objects[i];
	const PropertyContainer::Components &comps = g->components();

	for ( int q = 0; q < comps.size(); ++q )
	{
	    writeComponent( false, comps[q] );
	}
    }

    m_writer.endData();
    m_writer.close();
    return true;
}

} // End namespace GtoContainer




