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
#include "version.h"

#include <iostream>
#include <maya/MGlobal.h>
#include <Gto/Protocols.h>
#include "GtoInReader.h"
#include "GtoInObject.h"
#include "GtoInTransform.h"
#include "GtoInNURBS.h"
#include "GtoInPoly.h"
#include "GtoInParticle.h"

namespace GtoIOPlugin {
using namespace std;

//******************************************************************************
Reader::Reader( Set &set )
  : Gto::Reader(),
    m_set( set )
{
    // Nothing
}

//******************************************************************************
Reader::~Reader()
{
    // Nothing
}

//******************************************************************************
Request Reader::object( const std::string &name,
                        const std::string &protocol,
                        unsigned int protocolVersion,
                        const ObjectInfo &header )
{
    if( m_set.object( name ) != NULL )
    {
        string str = "Skipping duplicate object name in file: "
                     + name;
        MGlobal::displayWarning( str.c_str() );
        return Request( false );
    }
    
    if( protocol == GTO_PROTOCOL_TRANSFORM )
    {
        Object *newObj = new Transform( name, protocol, protocolVersion );
        m_set.addObject( newObj );
        return Request( true, (void *)newObj );
    }
    else if( protocol == GTO_PROTOCOL_NURBS )
    {
        NURBS *newObj = new NURBS( name, protocol, protocolVersion );
        m_set.addObject( newObj );
        return Request( true, (void *)newObj );
    }
    else if( ( protocol == GTO_PROTOCOL_POLYGON ) 
              || ( protocol == GTO_PROTOCOL_CATMULL_CLARK ) )
    {
        Poly *newObj = new Poly( name, protocol, protocolVersion );
        m_set.addObject( newObj );
        return Request( true, (void *)newObj );
    }
    else if( protocol == GTO_PROTOCOL_PARTICLE )
    {
        Particle *newObj = new Particle( name, protocol, protocolVersion );
        m_set.addObject( newObj );
        return Request( true, (void *)newObj );
    }
    else
    {
        string str = "Skipping " + name + ", unknown protocol: " + protocol;
        MGlobal::displayWarning( str.c_str() );
        return Request( false );
    }
}

//******************************************************************************
Request Reader::component( const std::string &name,
                           const ComponentInfo &header )
{
    const Object *object = ( const Object * )( header.object->objectData );
    return object->component( name );
}

//******************************************************************************
Request Reader::property( const std::string &name,
                          const PropertyInfo &header )
{
    const Object *object =
        ( const Object * )( header.component->object->objectData );
    void *componentData = ( void * )( header.component->componentData );
    return object->property( name, componentData );
}

// *****************************************************************************
void *Reader::data( const PropertyInfo &pinfo, size_t bytes )
{
    Object *object = ( Object * )( pinfo.component->object->objectData );
    void *componentData = (void *)( pinfo.component->componentData );
    void *propertyData = (void *)( pinfo.propertyData );
    return object->data( pinfo, bytes, componentData, propertyData );
}

// *****************************************************************************
void Reader::dataRead( const PropertyInfo &pinfo )
{
    Object *object = ( Object * )( pinfo.component->object->objectData );
    void *componentData = (void *)( pinfo.component->componentData );
    void *propertyData = (void *)( pinfo.propertyData );
    object->dataRead( pinfo, componentData, propertyData, stringTable() );
}

} // End namespace GtoIOPlugin
