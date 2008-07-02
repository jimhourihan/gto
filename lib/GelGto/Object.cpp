//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#include <GelGto/Object.h>
#include <GelGto/Exception.h>
#include <Gto/Protocols.h>
#include <GelGto/Poly.h>
#include <GelGto/Subd.h>
#include <stdio.h>
#include <assert.h>
#include <iostream>

#include "gelatoapi.h"

namespace GelGto {
using namespace Gto;

//******************************************************************************
Object::Object( const std::string &name, 
                const unsigned int protocolVersion,
                Reader *reader )
  : m_name( name ), 
    m_protocolVersion( protocolVersion ),
    m_reader( reader ),
    m_transformRef( NULL ),
    m_transformOpen( NULL ),
    m_transformClose( NULL ),
    m_numInstances( 1 ),
    m_visibility( NULL ),
    m_textureCoordinates( NULL ),
    m_nomotion(false)
{
    // Nothing
}

//******************************************************************************
Object::~Object()
{
    delete[] m_transformRef;
    delete[] m_transformOpen;
    delete[] m_transformClose;
    delete[] m_visibility;
    
    for( int i = 0; i < m_additionalTokens.size(); ++i )
    {
        delete[] m_additionalTokens[i];
    }
    for( int i = 0; i < m_additionalData.size(); ++i )
    {
        delete[] m_additionalData[i];
    }
    
    delete[] m_textureCoordinates;
}

//******************************************************************************
void *Object::component( const std::string &name,
                         ReaderPhase rp ) const
{
    // During all phases we read the "object" component
    if( name == GTO_COMPONENT_OBJECT )
    {
        return ( void * )OBJECT_C;
    }

    // During reference phase only, we read "mappings" and "channels" components
    if ( rp == READER_REF )
    {
        if( name == GTO_COMPONENT_MAPPINGS )
        {
            return ( void * )MAPPINGS_C;
        }
        else if( name == GTO_COMPONENT_CHANNELS )
        {
            return ( void * )CHANNELS_C;
        }
    }

    return ( void * )false;
}

//******************************************************************************
void *Object::property( const std::string &name,
                        void *componentData,
                        ReaderPhase rp,
                        const PropertyInfo &pinfo ) const
{
    // During all three phases we read the "globalMatrix" and "visibility"
    // properties of the "object" component
    if ( (( int )componentData) == OBJECT_C )
    {
        if ( name == GTO_PROPERTY_GLOBAL_MATRIX )
        {
            return ( void * )OBJECT_GLOBALMATRIX_P;
        }
        else if( name == "visibility" && getenv( "TWK_RI_GTO_USE_VISIBILITY" ) )
        {
            return ( void * )OBJECT_VISIBILITY_P;
        }
    } 
    else if ( (( int )componentData) == MAPPINGS_C
            || (( int )componentData) == Poly::MAPPINGS_C
            || (( int )componentData) == Subd::MAPPINGS_C )
    {
        if ( name != GTO_PROPERTY_ST )
        {
            // STs are dealt with in derived classes, here we 
            // ask for any other float or string properties
            if( pinfo.type == Gto::Float || pinfo.type == Gto::String )
            {
                return ( void * )MAPPINGS_P;
            }
        }
    }
    else if ( (( int )componentData) == CHANNELS_C )
    {
        // We want any float and string properties in the channels component
        if( pinfo.type == Gto::Float || pinfo.type == Gto::String )
        {
            return ( void * )CHANNELS_P;
        }
    }

    return ( void * )false;
}

//******************************************************************************
void *Object::data( void *componentData,
                    void *propertyData,
                    size_t numItems,
                    size_t itemWidth,
                    size_t numBytes,
                    ReaderPhase rp,
                    const PropertyInfo &pinfo )
{
    // During the "reference" phase, we stuff the
    // globalMatrix float property into transformRef.
    // 
    // During the "open" phase, we stuff the
    // globalMatrix float property into transformOpen.
    //
    // During the "close" phase, we stuff the
    // globalMatrix float property into transformClose.
    if ( (( int )componentData) == OBJECT_C &&
         (( int )propertyData) == OBJECT_GLOBALMATRIX_P )
    {
        if ( itemWidth != 16 ||
             numBytes != ( 16 * numItems * sizeof( float ) ) )
        {
            std::cerr << "ERROR: "
                      << "Wrong number of floats for transform in object:"
                      << m_name << std::endl;
            return NULL;
        }

        if ( rp == READER_REF )
        {
            m_numInstances = numItems;
        }

        if ( m_numInstances == 0 ||
             m_numInstances != numItems )
        {
            std::cerr << "ERROR: "
                      << "Rest/Open/Close number of instances incompatible "
                      << "in object: " << m_name << std::endl;
            return NULL;
        }

        if ( rp == READER_REF )
        {
            return transformRefData();
        }
        else if ( rp == READER_OPEN )
        {
            return transformOpenData();
        }
        else if ( rp == READER_CLOSE )
        {
            return transformCloseData();
        }
    }
    else if ( (( int )componentData) == OBJECT_C &&
         (( int )propertyData) == OBJECT_VISIBILITY_P )
    {
        // In ref & open phases, we want the visibility attribute
        if( rp == READER_REF || rp == READER_OPEN )
        {
            return visibilityData();
        }
    }
    else if ( rp == READER_REF )
    {
        // Store any properties in the mappings or channels component, to
        // be set as RiAttributes on the object later (except "*.mappings.st",
        // which is already filtered out above)
        if ( (( int )propertyData) == MAPPINGS_P
             || (( int )propertyData) == CHANNELS_P  )
        {
            if( ( m_reader->stringFromId( pinfo.interpretation ) 
                 == "TextureCoordinates" )
                && pinfo.size == 8 )
            {
                m_textureCoordinates = new float[8];
                return m_textureCoordinates;
            }
            else
            {
                return additionalTokenData( pinfo, numBytes );
            }
        }
    }

    return NULL;
}

//******************************************************************************
void Object::dataRead( void *componentData,
                       void *propertyData,
                       ReaderPhase rp,
                       const PropertyInfo &pinfo )
{
    if ( rp == READER_REF )
    {
        if ( (( int )propertyData) == MAPPINGS_P
             || (( int )propertyData) == CHANNELS_P  )
        {
            if( ( m_reader->stringFromId( pinfo.interpretation ) 
                 == "TextureCoordinates" )
                && pinfo.size == 8 )
            {
                // Already dealt with.
                return;
            }

            // If we're storing additional mappings/channels data, we
            // need to convert stringIds to strings here.
            if( pinfo.type == Gto::String )
            {
                size_t nStrings = pinfo.size * pinfo.width;
                const int *stringIds = (const int *)( m_currentAddtionalData );
                char **stringPointers = new char *[nStrings];
                for( int i = 0; i < nStrings; ++i )
                {
                    char *str = strdup( 
                            m_reader->stringFromId( stringIds[i] ).c_str() );
                    m_additionalData.push_back( str );
                    stringPointers[i] = strdup( str );
                }
                m_additionalData.push_back( (char *)stringPointers );
                m_additionalValuePointers.push_back( (char *)stringPointers );
            }
            else
            {
                m_additionalData.push_back( m_currentAddtionalData );
                m_additionalValuePointers.push_back( m_currentAddtionalData );
            }
        }
    }
}

//******************************************************************************
void Object::doneReading( ReaderPhase rp )
{
    // Nothing
}

//******************************************************************************
void Object::declareRi(GelatoAPI *g) const
{
    if( m_numInstances == 0 )
    {
        std::cerr << "WARNING: " << m_name << " has 0 instances." << std::endl;
    }

    // Make an alias.
    const float *tOpen = m_transformOpen == NULL ?
        m_transformRef : m_transformOpen;
    
    for( int inst = 0; inst < m_numInstances; ++inst )
    {
        if( m_visibility != NULL )
        {
            if( m_visibility[inst] == 0 ) continue;
        }

        int instGM = 16 * inst;

        // Begin the attribute block
        g->PushAttributes();

        const char *nameCstr = m_name.c_str();
        g->Attribute("string name", nameCstr);


        // Add any mappings / channels attributes to the Attribute Block
        assert( m_additionalTokens.size() == m_additionalValuePointers.size() );
#warning TODO: Implement arbitrary user attributes
//        Stub_RiAttributes( m_additionalTokens.size(),
//                           (char **)&(m_additionalTokens.front()),
//                           (char **)&(m_additionalValuePointers.front()) );


#warning TODO: Implement refToWorld attribute
/*
        // Put the global matrix on the stack.
        Stub_RtMatrix matrix;
        int row, col, i;
        for ( col = 0, i = 0; col < 4; ++col )
        {
            for ( row = 0; row < 4; ++row, ++i )
            {
                if( m_transformRef != NULL )
                {
                    matrix[row][col] = m_transformRef[instGM + i];
                }
                else
                {
                    matrix[row][col] = (row == col ? 1 : 0);
                }
            }
        }
        Stub_DeclareObjectRefToWorld( matrix );
*/

        g->PushTransform();

        // Declare the global transform
        if ( tOpen != NULL )
        {
            if ( m_transformClose != NULL && !m_nomotion )
            {
                // Motion blur
                float tOpenX[16];
                float tCloseX[16];
                copyTransposed( tOpen, tOpenX );
                copyTransposed( m_transformClose, tCloseX );
                
                g->Motion( 2, 0.0f, 1.0f );
                g->SetTransform( tOpenX );
                g->SetTransform( tCloseX );
            }
            else
            {
                std::cerr << "no moblur" << std::endl;
                // No motion blur
                float tOpenX[16];
                copyTransposed( tOpen, tOpenX );

                g->SetTransform( tOpen );
            }
        }

#warning TODO: Implement texture coordinates
/*        
        if( m_textureCoordinates )
        {
            Stub_RiTextureCoordinates( m_textureCoordinates[0],
                                       m_textureCoordinates[1],
                                       m_textureCoordinates[2],
                                       m_textureCoordinates[3],
                                       m_textureCoordinates[4],
                                       m_textureCoordinates[5],
                                       m_textureCoordinates[6],
                                       m_textureCoordinates[7] );
        }

*/
        // Call derived class Ri calls
        internalDeclareRi( g );

        g->PopTransform();

        // End the attribute block
        g->PopAttributes();
    }
}

//******************************************************************************
void *Object::transformRefData()
{
    if ( m_transformRef != NULL )
    {
        std::cerr << "ERROR: ref-phase object.globalTransform property data "
                  << "received twice for object: " << m_name << std::endl;
        return NULL;
    }
    if( getenv( "TWK_RI_GTO_DEBUG_ANIM" ) )
    {
        if( m_name.find( getenv( "TWK_RI_GTO_DEBUG_ANIM" ) ) != m_name.npos )
        {
            std::cerr << m_name << ": got ref transform" << std::endl;
        }
    }

    m_transformRef = new float[16 * m_numInstances];

    return ( void * )m_transformRef;
}

//******************************************************************************
void *Object::transformOpenData()
{
    if ( m_transformOpen != NULL )
    {
        std::cerr << "ERROR: open-phase object.globalTransform property data "
                  << "received twice for object: " << m_name << std::endl;
        return NULL;
    }
    if( getenv( "TWK_RI_GTO_DEBUG_ANIM" ) )
    {
        if( m_name.find( getenv( "TWK_RI_GTO_DEBUG_ANIM" ) ) != m_name.npos )
        {
            std::cerr << m_name << ": got open transform" << std::endl;
        }
    }

    m_transformOpen = new float[16 * m_numInstances];
    return ( void * )m_transformOpen;
}

//******************************************************************************
void *Object::transformCloseData()
{
    if ( m_transformClose != NULL )
    {
        std::cerr << "ERROR: close-phase object.globalTransform property data "
                  << "received twice for object: " << m_name << std::endl;
        return NULL;
    }
    if( getenv( "TWK_RI_GTO_DEBUG_ANIM" ) )
    {
        if( m_name.find( getenv( "TWK_RI_GTO_DEBUG_ANIM" ) ) != m_name.npos )
        {
            std::cerr << m_name << ": got close transform" << std::endl;
        }
    }
    
    m_transformClose = new float[16 * m_numInstances];
    return ( void * )m_transformClose;
}

//******************************************************************************
void *Object::visibilityData()
{
    delete m_visibility;
    m_visibility = new char[m_numInstances];
    return ( void * )m_visibility;
}

//******************************************************************************
void *Object::additionalTokenData( const PropertyInfo &pinfo, size_t numBytes )
{
    const char *propName = m_reader->stringFromId( pinfo.name ).c_str();
    const char *interp = m_reader->stringFromId( pinfo.interpretation ).c_str();
    std::string ribType = "";

    if( pinfo.type == Gto::Float )
    {
        if( strlen( interp ) > 1 ) 
        {
            ribType = interp;
        }
        else
        {
            if( pinfo.width == 16 ) ribType = "matrix";
            else if( pinfo.width == 3 ) ribType = "point";
            else ribType = "float";
        }
    }
    else if( pinfo.type == Gto::String )
    {
        ribType = "string";
    }
    else
    {
        abort();
    }

    if( pinfo.size > 1 )
    {
        char array[16];
        snprintf( array, 16, "[%d]", pinfo.size );
        ribType += std::string( array );
    }

    std::string token = ribType + " " + std::string( propName );
    m_additionalTokens.push_back( strdup( token.c_str() ) );

    m_currentAddtionalData = new char[numBytes];
    return (void *)m_currentAddtionalData;
}

} // End namespace GelGto
