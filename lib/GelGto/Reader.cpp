//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#include <GelGto/Reader.h>
#include <GelGto/Object.h>
#include <GelGto/Subd.h>
#include <GelGto/NURBS.h>
#include <GelGto/Strand.h>
#include <GelGto/Poly.h>
#include <GelGto/Exception.h>
#include <Gto/Protocols.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

namespace GelGto {

//******************************************************************************
Reader::Reader( Set &set, ReaderPhase rp )
  : Gto::Reader(),
    m_set( set ),
    m_readerPhase( rp )
{
    // Nothing
}

//******************************************************************************
Reader::~Reader()
{
    // Nothing
}

//******************************************************************************
Reader::Request Reader::object( const std::string &name,
                                const std::string &protocol,
                                const unsigned int protocolVersion,
                                const ObjectInfo &header )
{
    bool noSubds = false;
    bool noSubdsWarn = true;
    const char *noSubdsEnv = getenv( "TWK_RI_GTO_NO_SUBDS" );
    if( getenv( "TWK_RI_GTO_NO_SUBDS" ) )
    {
        noSubds = true;
        if( std::string( getenv( "TWK_RI_GTO_NO_SUBDS" ) ) == "quiet" )
        {
            noSubdsWarn = false;
        }
    }
    
    if ( m_readerPhase == READER_REF )
    {
        if ( m_set.object( name.c_str() ) != NULL )
        {
            std::cerr << "Duplicate object name in GTO ref file: " 
                      << name << std::endl;
            return Request( false );
        }
    }
    
    std::string mungedName = stripNamePrefix( name );
    
//     if ( protocol == GTO_PROTOCOL_TRANSFORM )
//     {
//         if ( m_readerPhase == READER_REF )
//         {
//             Object *newObj = new Object( mungedName, protocolVersion, this );
//             m_set.addObject( newObj );
//             return Request( true, ( void * )newObj );
//         }
//         else
//         {
//             void *obj = ( void * )( m_set.object( mungedName ) );
//             if ( obj != NULL )
//             {
//                 return Request( true, obj );
//             }
//             else
//             {
//                 return Request( false );
//             }
//         }
//     }
//     else 
    if ( protocol == GTO_PROTOCOL_CATMULL_CLARK )
    {
        if ( m_readerPhase == READER_REF )
        {
            if ( noSubds )
            {
                if( noSubdsWarn )
                {
                    std::cout << "WARNING: Treating subd surface: "
                              << mungedName << " as a polymesh."
                              << std::endl;
                }
                
                Poly *newObj = new Poly( mungedName, protocolVersion, this );
                m_set.addObject( newObj );
                return Request( true, ( void * )newObj );
            }
            else
            {
                Subd *newObj = new Subd( mungedName, protocolVersion, this );
                m_set.addObject( newObj );
                return Request( true, ( void * )newObj );
            }
        }
        else
        {
            void *obj = ( void * )( m_set.object( mungedName ) );
            if ( obj != NULL )
            {
                return Request( true, obj );
            }
            else
            {
                return Request( false );
            }
        }
    }
    else if ( protocol == GTO_PROTOCOL_NURBS )
    {
#warning TODO: Implement NURBS
        std::cerr << "WARNING: NURBS not yet supported: " << name << std::endl;
//         if( m_readerPhase == READER_REF )
//         {
//             NURBS *newObj = new NURBS( mungedName, protocolVersion, this );
//             m_set.addObject( newObj );
//             return Request( true, ( void * )newObj );
//         }
//         else
//         {
//             void *obj = ( void * )( m_set.object( mungedName ) );
//             if ( obj != NULL )
//             {
//                 return Request( true, obj );
//             }
//             else
//             {
//                 return Request( false );
//             }
//         }
    }
    else if ( protocol == GTO_PROTOCOL_STRAND )
    {
#warning TODO: Implement Strand?
        std::cerr << "WARNING: Strand not yet supported: " << name << std::endl;
//         if ( m_readerPhase == READER_REF )
//         {
//             Strand *newObj = new Strand( mungedName, protocolVersion, this );
//             m_set.addObject( newObj );
//             return Request( true, ( void * )newObj );
//         }
//         else
//         {
//             void *obj = ( void * )( m_set.object( mungedName ) );
//             if ( obj != NULL )
//             {
//                 return Request( true, obj );
//             }
//             else
//             {
//                 return Request( false );
//             }
//         }
    }
    else if ( protocol == GTO_PROTOCOL_POLYGON )
    {
        if ( m_readerPhase == READER_REF )
        {
            Poly *newObj = new Poly( mungedName, protocolVersion, this );
            m_set.addObject( newObj );
            return Request( true, ( void * )newObj );
        }
        else
        {
            void *obj = ( void * )( m_set.object( mungedName ) );
            if ( obj != NULL )
            {
                return Request( true, obj );
            }
            else
            {
                return Request( false );
            }
        }
    }
    else
    {
        return Request( false );
    }
}

//******************************************************************************
Reader::Request Reader::component( const std::string &name,
                                   const ComponentInfo &header )
{
    const Object *object = ( const Object * )( header.object->objectData );
    void *ret = object->component( name, m_readerPhase );
    if ( ret == NULL )
    {
        return Request( false );
    }
    else
    {
        return Request( true, ret );
    }
}

//******************************************************************************
Reader::Request Reader::property( const std::string &name,
                                  const PropertyInfo &header )
{
    const Object *object =
        ( const Object * )( header.component->object->objectData );
    void *componentData = ( void * )( header.component->componentData );
    void *ret = object->property( name, componentData, m_readerPhase, header );
    if ( ret == NULL )
    {
        return Request( false );
    }
    else
    {
        return Request( true, ret );
    }
}

//******************************************************************************
void *Reader::data( const PropertyInfo &pinfo,
                    size_t numBytes )
{
    Object *object =
        ( Object * )( pinfo.component->object->objectData );
    void *componentData = ( void * )( pinfo.component->componentData );
    void *propertyData = ( void * )( pinfo.propertyData );
    return object->data( componentData,
                         propertyData,
                         pinfo.size,
                         pinfo.width,
                         numBytes,
                         m_readerPhase,
                         pinfo );
}

//******************************************************************************
void Reader::dataRead( const PropertyInfo &pinfo )
{
    Object *object =
        ( Object * )( pinfo.component->object->objectData );
    void *componentData = ( void * )( pinfo.component->componentData );
    void *propertyData = ( void * )( pinfo.propertyData );
    object->dataRead( componentData, propertyData, m_readerPhase, pinfo );
}

// *****************************************************************************
const char *Reader::stripNamePrefix( const std::string &name )
{
    const char *prefix = m_set.stripPrefix();
    if( ! prefix )
    {
        return name.c_str();
    }
    if( name.substr( 0, strlen( prefix ) ) == prefix )
    {
        return &(name[strlen(prefix)]);
    }
    return name.c_str();
}

} // End namespace GelGto
