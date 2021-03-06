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

#include <GelGto/NURBS.h>
#include <GelGto/Exception.h>
// #include <GelGtoStub/Stubs.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include <algorithm>
#include <Gto/Protocols.h>
#include <paramtype.h>

// using namespace GelGtoStub;

#define AUTO_NORMALIZE_NURBS 0

namespace GelGto {
using namespace Gto;

//******************************************************************************
NURBS::NURBS( const std::string &name, 
              const unsigned int protocolVersion,
              Reader *reader )
  : Object( name, protocolVersion, reader ),
    m_degreeU( 3 ),
    m_degreeV( 3 ),
    m_knotsU( NULL ),
    m_knotsUSize( 0 ),
    m_knotsV( NULL ),
    m_knotsVSize( 0 ),
    m_minU( 0.0f ),
    m_maxU( 1.0f ),
    m_minV( 0.0f ),
    m_maxV( 1.0f ),
    m_positionsRef( NULL ),
    m_positionsRefSize( 0 ),
    m_positionsOpen( NULL ),
    m_positionsOpenSize( 0 ),
    m_positionsClose( NULL ),
    m_positionsCloseSize( 0 ),
    m_weights( NULL ),
    m_homogPosRef( NULL ),
    m_homogPosOpen( NULL ),
    m_homogPosClose( NULL )
{
    // Nothing
}

//******************************************************************************
NURBS::~NURBS()
{
    delete[] m_knotsU;
    delete[] m_knotsV;
    delete[] m_positionsRef;
    delete[] m_positionsOpen;
    delete[] m_positionsClose;
    delete[] m_weights;
    delete[] m_homogPosRef;
    delete[] m_homogPosOpen;
    delete[] m_homogPosClose;
}

//******************************************************************************
//******************************************************************************
// READER STUFF
//******************************************************************************
//******************************************************************************

//******************************************************************************
void *NURBS::component( const std::string &name,
                        ReaderPhase rp ) const
{
    // During the reference phase, we read:
    // points, surface
    if ( rp == READER_REF )
    {
        if ( name == GTO_COMPONENT_POINTS )
        {
            return ( void * )POINTS_C;
        }
        else if ( name == GTO_COMPONENT_SURFACE )
        {
            return ( void * )SURFACE_C;
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
void *NURBS::property( const std::string &name,
                       void *componentData,
                       ReaderPhase rp,
                       const PropertyInfo &pinfo ) const
{
    // During the reference phase, we read:
    // points.position
    // points.weight
    // surface.degree
    // surface.uKnots
    // surface.vKnots
    // surface.uRange
    // surface.vRange
    if ( rp == READER_REF )
    {
        if ( (( int )componentData) == POINTS_C )
        {
            if ( name == GTO_PROPERTY_POSITION )
            {
                return ( void * )POINTS_POSITION_P;
            }
            else if ( name == GTO_PROPERTY_WEIGHT )
            {
                return ( void * )POINTS_WEIGHT_P;
            }
        }
        else if ( (( int )componentData ) == SURFACE_C )
        {
            if ( name == GTO_PROPERTY_DEGREE )
            {
                return ( void * )SURFACE_DEGREE_P;
            }
            else if ( name == GTO_PROPERTY_UKNOTS )
            {
                return ( void * )SURFACE_UKNOTS_P;
            }
            else if ( name == GTO_PROPERTY_VKNOTS )
            {
                return ( void * )SURFACE_VKNOTS_P;
            }
            else if ( name == GTO_PROPERTY_URANGE )
            {
                return ( void * )SURFACE_URANGE_P;
            }
            else if ( name == GTO_PROPERTY_VRANGE )
            {
                return ( void * )SURFACE_VRANGE_P;
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
    return Object::property( name, componentData, rp, pinfo );
}


//******************************************************************************
#define WEIRD_SIZE( NAME, PROP )                                \
{                                                               \
    std::cerr << "ERROR: Bad size loading property: " << (PROP) \
              << " from surface: " << (NAME) << std::endl;      \
    return NULL;                                                \
}

//******************************************************************************
void *NURBS::data( void *componentData,
                   void *propertyData,
                   size_t numItems,
                   size_t itemWidth,
                   size_t numBytes,
                   ReaderPhase rp,
                   const PropertyInfo &pinfo )
{
    // FLOAT
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
        else if ( (( int )propertyData) == POINTS_WEIGHT_P )
        {
            if ( itemWidth != 1 ||
                 ( numItems * 1 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "points.weight" );
            }
            
            return weightsData( numItems * 1 );
        }
        else if ( (( int )propertyData) == SURFACE_UKNOTS_P )
        {
            if ( itemWidth != 1 ||
                 ( numItems * 1 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "surface.uKnots" );
            }
            
            return knotsUData( numItems * 1 );
        }
        else if ( (( int )propertyData) == SURFACE_VKNOTS_P )
        {
            if ( itemWidth != 1 ||
                 ( numItems * 1 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "surface.vKnots" );
            }
            
            return knotsVData( numItems * 1 );
        }
        else if ( (( int )propertyData) == SURFACE_URANGE_P )
        {
            if ( itemWidth != 1 ||
                 numItems != 2 ||
                 ( numBytes *= 2 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "surface.uRange" );
            }
            
            return rangeUData();
        }
        else if ( (( int )propertyData) == SURFACE_VRANGE_P )
        {
            if ( itemWidth != 1 ||
                 numItems != 2 ||
                 ( numBytes *= 2 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "surface.vRange" );
            }

            return rangeVData();
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

    // INT
    if ( rp == READER_REF )
    {
        if ( (( int )propertyData) == SURFACE_DEGREE_P )
        {
            if ( itemWidth != 1 ||
                 numItems != 2 ||
                 numBytes != 2 * sizeof( int ) )
            {
                WEIRD_SIZE( m_name, "surface.degree" );
            }

            return degreeData();
        }
    }

    // Superclass
    return Object::data( componentData, propertyData,
                         numItems, itemWidth, numBytes, rp, pinfo );
}

//******************************************************************************
void NURBS::doneReading( ReaderPhase rp )
{
    if ( rp != READER_CLOSE )
    {
        return;
    }

    // Need to transfer positions/weights into homogenous coordinates.
    if ( m_homogPosRef == NULL )
    {
        assert( m_homogPosOpen == NULL );
        assert( m_homogPosClose == NULL );

        if ( m_positionsRef == NULL ||
             m_positionsRefSize == 0 )
        {
            std::cerr << "ERROR: Did not receive ref positions for surface: "
                      << m_name << std::endl;
            return;
        }
        if( m_knotsU == NULL || m_knotsUSize == 0 
            || m_knotsV == NULL || m_knotsVSize == 0 )
        {
            std::cerr << "ERROR: Did not receive knots for surface: "
                      << m_name << std::endl;
            return;
        }
        if( m_weights == NULL || m_weightsSize == 0 )
        {
            std::cerr << "ERROR: Did not receive wieghts for surface: "
                      << m_name << std::endl;
            return;
        }

        // Now we know that positionsRef is good.
        // Check positionsOpen
        if ( m_positionsOpen != NULL &&
             m_positionsOpenSize != m_positionsRefSize )
        {
            std::cerr << "ERROR: Open positions are different size than "
                      << "ref positions in surface: " << m_name
                      << std::endl;
            return;
        }

        // Check positions close is good.
        if ( m_positionsClose != NULL &&
             m_positionsCloseSize != m_positionsRefSize )
        {
            std::cerr << "ERROR: Close positions are different size than "
                      << "ref positions in surface: " << m_name
                      << std::endl;
            return;
        }

        // Check weights
        if ( m_weights != NULL &&
             ( m_positionsRefSize/3 ) != m_weightsSize )
        {
            std::cerr << "ERROR: Weights size different than positions size "
                      << "in surface: " << m_name << std::endl;
            return;
        }

        // Okay, all is well.
        if ( m_weights == NULL )
        {
            size_t numPos = m_positionsRefSize / 3;
            m_homogPosRef = new float[numPos * 4];
            for ( size_t i = 0, j = 0, k = 0; i < numPos;
                  ++i, j += 3, k += 4 )
            {
                m_homogPosRef[k] = m_positionsRef[j];
                m_homogPosRef[k+1] = m_positionsRef[j+1];
                m_homogPosRef[k+2] = m_positionsRef[j+2];
                m_homogPosRef[k+3] = 1.0f;
            }

            if ( m_positionsOpen != NULL )
            {
                m_homogPosOpen = new float[numPos * 4];
                for ( size_t i = 0, j = 0, k = 0; i < numPos;
                      ++i, j += 3, k += 4 )
                {
                    m_homogPosOpen[k] = m_positionsOpen[j];
                    m_homogPosOpen[k+1] = m_positionsOpen[j+1];
                    m_homogPosOpen[k+2] = m_positionsOpen[j+2];
                    m_homogPosOpen[k+3] = 1.0f;
                }
            }

            if ( m_positionsClose != NULL )
            {
                m_homogPosClose = new float[numPos * 4];
                for ( size_t i = 0, j = 0, k = 0; i < numPos;
                      ++i, j += 3, k += 4 )
                {
                    m_homogPosClose[k] = m_positionsClose[j];
                    m_homogPosClose[k+1] = m_positionsClose[j+1];
                    m_homogPosClose[k+2] = m_positionsClose[j+2];
                    m_homogPosClose[k+3] = 1.0f;
                }
            }
        }
        else
        {
            size_t numPos = m_positionsRefSize / 3;
            m_homogPosRef = new float[numPos * 4];
            for ( size_t i = 0, j = 0, k = 0; i < numPos;
                  ++i, j += 3, k += 4 )
            {
                m_homogPosRef[k] = m_positionsRef[j];
                m_homogPosRef[k+1] = m_positionsRef[j+1];
                m_homogPosRef[k+2] = m_positionsRef[j+2];
                m_homogPosRef[k+3] = m_weights[i];
            }

            if ( m_positionsOpen != NULL )
            {
                m_homogPosOpen = new float[numPos * 4];
                for ( size_t i = 0, j = 0, k = 0; i < numPos;
                      ++i, j += 3, k += 4 )
                {
                    m_homogPosOpen[k] = m_positionsOpen[j];
                    m_homogPosOpen[k+1] = m_positionsOpen[j+1];
                    m_homogPosOpen[k+2] = m_positionsOpen[j+2];
                    m_homogPosOpen[k+3] = m_weights[i];
                }
            }

            if ( m_positionsClose != NULL )
            {
                m_homogPosClose = new float[numPos * 4];
                for ( size_t i = 0, j = 0, k = 0; i < numPos;
                      ++i, j += 3, k += 4 )
                {
                    m_homogPosClose[k] = m_positionsClose[j];
                    m_homogPosClose[k+1] = m_positionsClose[j+1];
                    m_homogPosClose[k+2] = m_positionsClose[j+2];
                    m_homogPosClose[k+3] = m_weights[i];
                }
            }
        }
    }
    
}

//******************************************************************************
//******************************************************************************
// INTERAL SETTINGS
//******************************************************************************
//******************************************************************************
void *NURBS::degreeData()
{
    return ( void * )&m_degreeU;
}

//******************************************************************************
void *NURBS::knotsUData( size_t knotsUSize )
{
    delete[] m_knotsU;
    m_knotsUSize = knotsUSize;
    m_knotsU = new float[m_knotsUSize];
    return ( void * )m_knotsU;
}

//******************************************************************************
void *NURBS::knotsVData( size_t knotsVSize )
{
    delete[] m_knotsV;
    m_knotsVSize = knotsVSize;
    m_knotsV = new float[m_knotsVSize];
    return ( void * )m_knotsV;
}

//******************************************************************************
void *NURBS::rangeUData()
{
    return ( void * )&m_minU;
}

//******************************************************************************
void *NURBS::rangeVData()
{
    return ( void * )&m_minV;
}

//******************************************************************************
void *NURBS::weightsData( size_t weightsSize )
{
    delete[] m_weights;
    m_weightsSize = weightsSize;
    m_weights = new float[m_weightsSize];
    return ( void * )m_weights;
}

//******************************************************************************
void *NURBS::positionsRefData( size_t positionsRefSize )
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
void *NURBS::positionsOpenData( size_t positionsOpenSize )
{
    delete[] m_positionsOpen;
    delete[] m_positionsClose;
    m_positionsClose = NULL;
    m_positionsOpenSize = positionsOpenSize;
    m_positionsOpen = new float[m_positionsOpenSize];
    return ( void * )m_positionsOpen;
}

//******************************************************************************
void *NURBS::positionsCloseData( size_t positionsCloseSize )
{
    delete[] m_positionsClose;
    m_positionsCloseSize = positionsCloseSize;
    m_positionsClose = new float[m_positionsCloseSize];
    return ( void * )m_positionsClose;
}

//******************************************************************************
void NURBS::internalDeclareRi(GelatoAPI *g) const
{
    if ( m_homogPosOpen == NULL )
    {
        // No reference positions
//         g->PushAttributes();
//         g->Parameter("vertex hpoint Pw", m_homogPosRef );        
//         g->Patch( m_knotsUSize, m_degreeU, m_knotsU, m_minU, m_maxU, 
//                   m_knotsVSize, m_degreeV, m_knotsV, m_minV, m_maxV );
//         g->PopAttributes();
//         
//         for( int i = 0; i < m_positionsRefSize / 3; ++i )
//         {
//             fprintf( stderr, "Pw[%d]: %f\t%f\t%f\t%f\n", i, m_homogPosRef[i*4+0],
//                                                             m_homogPosRef[i*4+1], 
//                                                             m_homogPosRef[i*4+2], 
//                                                             m_homogPosRef[i*4+3] );
//         }
//         
//         fprintf( stderr, "range U: %f, %f\n", m_minU, m_maxU );
//         fprintf( stderr, "%d U knots:\n", m_knotsUSize );
//         for( int u = 0; u < m_knotsUSize; u++ )
//         {
//             fprintf( stderr, "\tuKnot[%d]: %f\n", u, m_knotsU[u] );
//         }
//         
//         fprintf( stderr, "range V: %f, %f\n", m_minV, m_maxV );
//         fprintf( stderr, "%d V knots:\n", m_knotsVSize );
//         for( int v = 0; v < m_knotsVSize; v++ )
//         {
//             fprintf( stderr, "\tvKnot[%d]: %f\n", v, m_knotsV[v] );
//         }
        
/*
        Stub_DeclareNURBS( m_knotsU, m_knotsUSize,
                           m_degreeU, m_minU, m_maxU,
                           
                           m_knotsV, m_knotsVSize,
                           m_degreeV, m_minV, m_maxV,
                           
                           m_homogPosRef, m_homogPosRef );
*/
    }
    else
    {
        if ( m_homogPosClose != NULL )
        {
/*            
            Stub_RiMotionBegin( 0.0f, 1.0f );

            Stub_DeclareNURBS( m_knotsU, m_knotsUSize,
                               m_degreeU, m_minU, m_maxU,
                               
                               m_knotsV, m_knotsVSize,
                               m_degreeV, m_minV, m_maxV,
                               
                               m_homogPosOpen, m_homogPosRef );

            Stub_DeclareNURBS( m_knotsU, m_knotsUSize,
                               m_degreeU, m_minU, m_maxU,
                               
                               m_knotsV, m_knotsVSize,
                               m_degreeV, m_minV, m_maxV,
                               
                               m_homogPosClose, m_homogPosRef );
            
            Stub_RiMotionEnd();
*/            
        }
        else
        {
/*
            Stub_DeclareNURBS( m_knotsU, m_knotsUSize,
                               m_degreeU, m_minU, m_maxU,
                               
                               m_knotsV, m_knotsVSize,
                               m_degreeV, m_minV, m_maxV,
                               
                               m_homogPosOpen, m_homogPosRef );
*/
        }
    }
}

} // End namespace GelGto
