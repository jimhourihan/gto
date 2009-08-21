//
//  Copyright (c) 2009, Tweak Software
//  All rights reserved.
// 
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//     * Redistributions of source code must retain the above
//       copyright notice, this list of conditions and the following
//       disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials
//       provided with the distribution.
//
//     * Neither the name of the Tweak Software nor the names of its
//       contributors may be used to endorse or promote products
//       derived from this software without specific prior written
//       permission.
// 
//  THIS SOFTWARE IS PROVIDED BY Tweak Software ''AS IS'' AND ANY EXPRESS
//  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL Tweak Software BE LIABLE FOR
//  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
//  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
//  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
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
#include "GtoInCamera.h"

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
//     else if( protocol == GTO_PROTOCOL_CAMERA )
//     {
//         Object *newObj = new Camera( name, protocol, protocolVersion );
//         m_set.addObject( newObj );
//         return Request( true, (void *)newObj );
//     }
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
