//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************
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
void *Reader::object( const std::string &name,
                      const std::string &protocol,
                      unsigned int protocolVersion,
                      const ObjectInfo &header )
{
    if( m_set.object( name ) != NULL )
    {
        string str = "Skipping duplicate object name in file: "
                     + name;
        MGlobal::displayWarning( str.c_str() );
        return GTO_REJECT;
    }
    
    if( protocol == GTO_PROTOCOL_TRANSFORM )
    {
        Object *newObj = new Transform( name, protocol, protocolVersion );
        m_set.addObject( newObj );
        return (void *)newObj;
    }
    else if( protocol == GTO_PROTOCOL_NURBS )
    {
        NURBS *newObj = new NURBS( name, protocol, protocolVersion );
        m_set.addObject( newObj );
        return (void *)newObj;
    }
    else if( ( protocol == GTO_PROTOCOL_POLYGON ) 
              || ( protocol == GTO_PROTOCOL_CATMULL_CLARK ) )
    {
        Poly *newObj = new Poly( name, protocol, protocolVersion );
        m_set.addObject( newObj );
        return (void *)newObj;
    }
    else if( protocol == GTO_PROTOCOL_PARTICLE )
    {
        Particle *newObj = new Particle( name, protocol, protocolVersion );
        m_set.addObject( newObj );
        return (void *)newObj;
    }
    else
    {
        string str = "Skipping " + name + ", unknown protocol: " + protocol;
        MGlobal::displayWarning( str.c_str() );
        return GTO_REJECT;
    }
}

//******************************************************************************
void *Reader::component( const std::string &name,
                         const ComponentInfo &header )
{
    const Object *object = ( const Object * )( header.object->objectData );
    return object->component( name );
}

//******************************************************************************
void *Reader::property( const std::string &name,
                        const PropertyInfo &header )
{
    const Object *object =
        ( const Object * )( header.component->object->objectData );
    void *componentData = ( void * )( header.component->componentData );
    void *ret = object->property( name, componentData );
    return ret;
}

//******************************************************************************
void Reader::data( const PropertyInfo &pinfo,
                   const float *items,
                   size_t numItems )
{
    Object *object =
        ( Object * )( pinfo.component->object->objectData );
    void *componentData = ( void * )( pinfo.component->componentData );
    void *propertyData = ( void * )( pinfo.propertyData );
    object->data( componentData,
                  propertyData,
                  items,
                  pinfo.size,
                  pinfo.width );
}

//******************************************************************************
void Reader::data( const PropertyInfo &pinfo,
                   const double *items,
                   size_t numItems )
{
    Object *object =
        ( Object * )( pinfo.component->object->objectData );
    void *componentData = ( void * )( pinfo.component->componentData );
    void *propertyData = ( void * )( pinfo.propertyData );
    object->data( componentData,
                  propertyData,
                  items,
                  pinfo.size,
                  pinfo.width );
}

//******************************************************************************
void Reader::data( const PropertyInfo &pinfo,
                   const int *items,
                   size_t numItems )
{
    Object *object =
        ( Object * )( pinfo.component->object->objectData );
    void *componentData = ( void * )( pinfo.component->componentData );
    void *propertyData = ( void * )( pinfo.propertyData );
    
    if( pinfo.type == Gto::String )
    {
        vector<string> strings;
        for( size_t s = 0; s < pinfo.size; ++s )
        {
            strings.push_back( stringFromId( items[s] ) );
        }
        object->data( componentData,
                      propertyData,
                      strings,
                      pinfo.size,
                      pinfo.width );
        return;
    }
    object->data( componentData,
                  propertyData,
                  items,
                  pinfo.size,
                  pinfo.width );
}

//******************************************************************************
void Reader::data( const PropertyInfo &pinfo,
                   const unsigned short *items,
                   size_t numItems )
{
    Object *object =
        ( Object * )( pinfo.component->object->objectData );
    void *componentData = ( void * )( pinfo.component->componentData );
    void *propertyData = ( void * )( pinfo.propertyData );
    object->data( componentData,
                  propertyData,
                  items,
                  pinfo.size,
                  pinfo.width );
}

//******************************************************************************
void Reader::data( const PropertyInfo &pinfo,
                   const unsigned char *items,
                   size_t numItems )
{
    Object *object =
        ( Object * )( pinfo.component->object->objectData );
    void *componentData = ( void * )( pinfo.component->componentData );
    void *propertyData = ( void * )( pinfo.propertyData );
    object->data( componentData,
                  propertyData,
                  items,
                  pinfo.size,
                  pinfo.width );
}

} // End namespace GtoIOPlugin
