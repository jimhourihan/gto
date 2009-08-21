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

#include <RiGto/RiGtoStrand.h>
#include <RiGto/RiGtoException.h>
#include <RiGtoStub/Stubs.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include <algorithm>
#include <Gto/Protocols.h>
#include <RiGto/RiGtoReader.h>

#include <iostream>
#include <fstream>

using namespace RiGtoStub;

namespace RiGto {

//******************************************************************************
Strand::Strand( const std::string &name, 
                const unsigned int protocolVersion,
                Reader *reader )
  : Object( name, protocolVersion, reader ),
    m_typeStrId( 0 ),
    m_sizes( NULL ),
    m_sizesSize( 0 ),
    m_constantWidth( 1.0f ),
    m_widths( NULL ),
    m_widthsSize( 0 ),
    m_positionsRef( NULL ),
    m_positionsRefSize( 0 ),
    m_positionsOpen( NULL ),
    m_positionsOpenSize( 0 ),
    m_positionsClose( NULL ),
    m_positionsCloseSize( 0 )
{
    // Nothing
}

//******************************************************************************
Strand::~Strand()
{
    delete[] m_positionsRef;
    delete[] m_positionsOpen;
    delete[] m_positionsClose;
    delete[] m_sizes;
    delete[] m_widths;
}

//******************************************************************************
//******************************************************************************
// READER STUFF
//******************************************************************************
//******************************************************************************

//******************************************************************************
void *Strand::component( const std::string &name,
                        ReaderPhase rp ) const
{
    // During the reference phase, we read:
    // points, elements
    if ( rp == READER_REF )
    {
        if ( name == GTO_COMPONENT_POINTS )
        {
            return ( void * )POINTS_C;
        }
        else if ( name == GTO_COMPONENT_STRAND )
        {
            return ( void * )STRAND_C;
        }
        else if ( name == GTO_COMPONENT_ELEMENTS )
        {
            return ( void * )ELEMENTS_C;
        }
    }
    // During the open & close phases, we read:
    // points
    else if ( rp == READER_OPEN ||
              rp == READER_CLOSE )
    {
        if ( name == GTO_COMPONENT_POINTS )
        {
            return ( void * )POINTS_C;
        }
    }

    // Superclass
    return Object::component( name, rp );
}

//******************************************************************************
void *Strand::property( const std::string &name,
                       void *componentData,
                       ReaderPhase rp ) const
{
    // During the reference phase, we read:
    // points.position
    // elements.size
    // elements.type
    // elements.width
    if ( rp == READER_REF )
    {
        if ( (( int )componentData) == POINTS_C )
        {
            if ( name == GTO_PROPERTY_POSITION )
            {
                return ( void * )POINTS_POSITION_P;
            }
        }
        else if ( (( int )componentData ) == STRAND_C )
        {
            if ( name == "type" )
            {
                return ( void * )STRAND_TYPE_P;
            }
            if ( name == "width" )
            {
                return ( void * )STRAND_WIDTH_P;
            }
        }
        else if ( (( int )componentData ) == ELEMENTS_C )
        {
            if ( name == "size" )
            {
                return ( void * )ELEMENTS_SIZE_P;
            }
            if ( name == "width" )
            {
                return ( void * )ELEMENTS_WIDTH_P;
            }
        }
    }
    else if ( rp == READER_OPEN ||
              rp == READER_CLOSE )
    {
        if ( (( int )componentData) == POINTS_C )
        {
            if ( name == GTO_PROPERTY_POSITION )
            {
                return ( void * )POINTS_POSITION_P;
            }
        }
    }

    // Superclass
    return Object::property( name, componentData, rp );
}


//******************************************************************************
#define WEIRD_SIZE( NAME, PROP )                                \
{                                                               \
    std::cerr << "ERROR: Bad size loading property: " << (PROP) \
              << " from surface: " << (NAME) << std::endl;      \
    return NULL;                                                \
}

//******************************************************************************
void *Strand::data( void *componentData,
                    void *propertyData,
                    size_t numItems,
                    size_t itemWidth,
                    size_t numBytes,
                    ReaderPhase rp )
{
    if ( rp == READER_REF )
    {
        if ( (( int )propertyData) == POINTS_POSITION_P )
        {
            if ( itemWidth != 3 ||
                 ( numItems * 3 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "points.position" );
            }
            
            return positionsRefData( numItems * 3 );
        }
        else if ( (( int )propertyData) == STRAND_WIDTH_P )
        {
            if ( itemWidth != 1 ||
                 ( numItems * 1 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "strand.width" );
            }
            return constantWidthData();
        }
        else if ( (( int )propertyData) == STRAND_TYPE_P )
        {
            if ( itemWidth != 1 ||
                 ( numItems * 1 * sizeof( int ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "strand.type" );
            }
            
            return typeData();
        }
        else if ( (( int )propertyData) == ELEMENTS_SIZE_P )
        {
            if ( itemWidth != 1 ||
                 ( numItems * 1 * sizeof( int ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "elements.size" );
            }
            
            return sizeData( numItems * 1 );
        }
        else if ( (( int )propertyData) == ELEMENTS_WIDTH_P )
        {
            if ( itemWidth != 2 ||
                 ( numItems * 2 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "elements.width" );
            }
            return widthData( numItems * 2 );
        }
    }
    else if ( rp == READER_OPEN )
    {
        if ( (( int )propertyData) == POINTS_POSITION_P )
        {
            if ( itemWidth != 3 ||
                 ( numItems * 3 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "points.position" );
            }

            return positionsOpenData( numItems * 3 );
        }
    }
    else if ( rp == READER_CLOSE )
    {
        if ( (( int )propertyData) == POINTS_POSITION_P )
        {
            if ( itemWidth != 3 ||
                 ( numItems * 3 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "points.position" );
            }
            
            return positionsCloseData( numItems * 3 );
        }
    }

    // Superclass
    return Object::data( componentData, propertyData,
                         numItems, itemWidth, numBytes, rp );
}

void Strand::dataRead( void *componentData,
                       void *propertyData,
                       ReaderPhase rp )
{
    m_type = m_reader->stringFromId( m_typeStrId );
}

//******************************************************************************
//******************************************************************************
// INTERNAL SETTINGS
//******************************************************************************
//******************************************************************************
void *Strand::typeData()
{
    return ( void * )&m_typeStrId;
}

//******************************************************************************
void *Strand::sizeData( size_t sizesSize )
{
    delete[] m_sizes;
    m_sizesSize = sizesSize;
    m_sizes = new int[m_sizesSize];
    return ( void * )m_sizes;
}

//******************************************************************************
void *Strand::widthData( size_t widthsSize )
{
    delete[] m_widths;
    m_widthsSize = widthsSize;
    m_widths = new float[m_widthsSize];
    return ( void * )m_widths;
}

//******************************************************************************
void *Strand::constantWidthData()
{
    return ( void * )&m_constantWidth;
}


//******************************************************************************
void *Strand::positionsRefData( size_t positionsRefSize )
{
    delete[] m_positionsRef;
    delete[] m_positionsOpen;
    m_positionsOpen = NULL;
    delete[] m_positionsClose;
    m_positionsClose = NULL;
    m_positionsRefSize = positionsRefSize;
    m_positionsRef = new float[m_positionsRefSize];
    return ( void * )m_positionsRef;
}

//******************************************************************************
void *Strand::positionsOpenData( size_t positionsOpenSize )
{
    delete[] m_positionsOpen;
    delete[] m_positionsClose;
    m_positionsClose = NULL;
    m_positionsOpenSize = positionsOpenSize;
    m_positionsOpen = new float[m_positionsOpenSize];
    return ( void * )m_positionsOpen;
}

//******************************************************************************
void *Strand::positionsCloseData( size_t positionsCloseSize )
{
    delete[] m_positionsClose;
    m_positionsCloseSize = positionsCloseSize;
    m_positionsClose = new float[m_positionsCloseSize];
    return ( void * )m_positionsClose;
}

//******************************************************************************
void Strand::internalDeclareRi() const
{
    if( m_type != "linear" && m_type != "cubic" )
    {
        std::cerr << "Invalid curve type: " << m_type << std::endl;
        return;
    }

    if( m_widths && m_widthsSize != m_sizesSize * 2 )
    {
        std::cerr << "Invalid number of widths.  Must be either one value or "
                  << "2*nCurves values." 
                  << std::endl;
        return;
    }

    if( m_positionsOpen == NULL )
    {
        // Only reference positions
        Stub_DeclareCurves( m_type.c_str(),
                            m_sizesSize,
                            m_sizes,
                            m_constantWidth,
                            m_widths,
                            m_positionsRef );
    }
    else
    {
        if ( m_positionsClose != NULL )
        {
            Stub_RiMotionBegin( 0.0f, 1.0f );

            Stub_DeclareCurves( m_type.c_str(),
                                m_sizesSize,
                                m_sizes,
                                m_constantWidth,
                                m_widths,
                                m_positionsOpen );

            Stub_DeclareCurves( m_type.c_str(),
                                m_sizesSize,
                                m_sizes,
                                m_constantWidth,
                                m_widths,
                                m_positionsClose );
            
            Stub_RiMotionEnd();
        }
        else
        {
            Stub_DeclareCurves( m_type.c_str(),
                                m_sizesSize,
                                m_sizes,
                                m_constantWidth,
                                m_widths,
                                m_positionsOpen );
        }
    }
}

} // End namespace RiGto
