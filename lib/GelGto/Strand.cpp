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

#include <GelGto/Strand.h>
#include <GelGto/Exception.h>
// #include <GelGtoStub/Stubs.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include <algorithm>
#include <numeric>
#include <Gto/Protocols.h>
#include <GelGto/Reader.h>

#include <iostream>
#include <fstream>
#include <vector>

//-*****************************************************************************
// This stuff is a little suspect...
typedef float RtBasis[4][4];
extern RtBasis  RiBezierBasis, RiBSplineBasis, RiCatmullRomBasis,
                RiHermiteBasis, RiPowerBasis;

#define RI_BEZIERSTEP       (3)
#define RI_BSPLINESTEP      (1)
#define RI_CATMULLROMSTEP   (1)
#define RI_HERMITESTEP      (2)
#define RI_POWERSTEP        (4)

// using namespace GelGtoStub;

namespace GelGto {
using namespace Gto;
using namespace std;

//-*****************************************************************************
//-*****************************************************************************
// HELPFUL STUFF
//-*****************************************************************************
//-*****************************************************************************
namespace {

//-*****************************************************************************
enum CurveType
{
    Linear,
    Bezier,
    Bspline,
    CatmullRom,
    Hermite
};

//-*****************************************************************************
template <typename T>
static inline T smoothstep( const T &t )
{
    if ( t <= ( T )0 )
    {
        return ( T )0;
    }
    else if ( t >= ( T )1 )
    {
        return ( T )1;
    }
    else
    {
        return t * t * ( ( T )3 - ( t * ( 2 ) ) );
    }
}

//-*****************************************************************************
template <typename T>
static inline T smoothstep( const T &edge0, const T &edge1, const T &t )
{
    return smoothstep( ( t - edge0 ) / ( edge1 - edge0 ) );
}

//-*****************************************************************************
template <class T, typename T2>
static inline T lerp( const T &a, const T &b, const T2 &t )
{
    return (((T2)1) - t)*a + (t*b);
}

//-*****************************************************************************
struct V2
{
    float x;
    float y;
};

struct V3
{
    V3() {}
    V3(float a, float b, float c) { x = a; y = b; z = c; }
    float x;
    float y;
    float z;
};

V3 operator - (const V3& a, const V3& b)
{
    return V3(a.x - b.x, a.y - b.y, a.z - b.z);
}

ostream& operator << (ostream& o, const V3&  v)
{
    o << v.x << " " << v.y << " " << v.z;
    return o;
}

//----------------------------------------------------------------------
//  Super tiny vector algrebra implementation
//
//

//
//  Note: only works with affine matrices. And these are *transposed* from what
//  Ri will receive
//

V3 multiply(const V3& v, float* M)
{
    V3 o;

    o.x = M[0*4 + 0] * v.x + M[0*4 + 1] * v.y + M[0*4 + 2] * v.z + M[0*4 + 3];
    o.y = M[1*4 + 0] * v.x + M[1*4 + 1] * v.y + M[1*4 + 2] * v.z + M[1*4 + 3];
    o.z = M[2*4 + 0] * v.x + M[2*4 + 1] * v.y + M[2*4 + 2] * v.z + M[2*4 + 3];

    return o;
}


} // End anonymous namespace

//******************************************************************************
Strand::Strand( const std::string &name, 
                const unsigned int protocolVersion,
                Gto::Reader *reader )
  : Object( name, protocolVersion, reader ),
    m_typeStrId( 0 ),
    m_sizes( NULL ),
    m_sizesSize( 0 ),
    m_elementsWidth( NULL ),
    m_elementsWidthSize( 0 ),
    m_positionsRef( NULL ),
    m_positionsRefSize( 0 ),
    m_positionsOpen( NULL ),
    m_positionsOpenSize( 0 ),
    m_positionsClose( NULL ),
    m_positionsCloseSize( 0 ),
    m_elementsRootST( NULL ),
    m_elementsRootSTSize( 0 )
{
    // Set a default width.
    m_strandWidth[0] =
        m_strandWidth[1] = 0.001f;
    m_nomotion = false;
}

//******************************************************************************
Strand::~Strand()
{
    delete[] m_elementsWidth;
    delete[] m_positionsRef;
    delete[] m_positionsOpen;
    delete[] m_positionsClose;
    delete[] m_sizes;
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
                       ReaderPhase rp,
                       const PropertyInfo &pinfo ) const
{
    // During the reference phase, we read:
    // points.position
    // elements.size
    // elements.type
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
            else if ( name == "width" )
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
            else if ( name == "width" )
            {
                return ( void * )ELEMENTS_WIDTH_P;
            }
            else if ( name == "stRoot" )
            {
                return ( void * )ELEMENTS_STROOT_P;
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
void *Strand::data( void *componentData,
                    void *propertyData,
                    size_t numItems,
                    size_t itemWidth,
                    size_t numBytes,
                    ReaderPhase rp,
                    const PropertyInfo &pinfo )
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
        else if ( (( int )propertyData) == STRAND_TYPE_P )
        {
            if ( itemWidth != 1 ||
                 ( numItems * 1 * sizeof( int ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "strand.type" );
            }
            
            return typeData();
        }
        else if ( (( int )propertyData) == STRAND_WIDTH_P )
        {
            if ( itemWidth != 2 ||
                 ( numItems * 2 * sizeof( float ) ) != numBytes ||
                 numItems != 1 )
            {
                WEIRD_SIZE( m_name, "strand.width" );
            }

            return strandWidthData();
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

            return elementsWidthData( numItems * 2 );
        }
        else if ( (( int )propertyData) == ELEMENTS_STROOT_P )
        {
            if ( itemWidth != 2 ||
                 ( numItems * 2 * sizeof( float ) ) != numBytes )
            {
                WEIRD_SIZE( m_name, "elements.rootST" );
            }

            return elementsRootSTData( numItems * 2 );
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
                         numItems, itemWidth, numBytes, rp, pinfo );
}

//******************************************************************************
void Strand::dataRead( void *componentData,
                       void *propertyData,
                       ReaderPhase rp,
                       const PropertyInfo &pinfo )
{
    if ( rp == READER_REF )
    {
        if ( (( int )propertyData) == STRAND_TYPE_P )
        {
            m_type = m_reader->stringFromId( m_typeStrId );
        }
    }

    // Superclass
    Object::dataRead( componentData, propertyData, rp, pinfo );
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

//-*****************************************************************************
void *Strand::strandWidthData()
{
    return ( void * )&( m_strandWidth[0] );
}

//-*****************************************************************************
void *Strand::elementsWidthData( size_t numFloats )
{
    delete[] m_elementsWidth;
    m_elementsWidthSize = numFloats;
    m_elementsWidth = new float[m_elementsWidthSize];
    return ( void * )m_elementsWidth;
}

//-*****************************************************************************
void *Strand::elementsRootSTData( size_t numFloats )
{
    delete[] m_elementsRootST;
    m_elementsRootSTSize = numFloats;
    m_elementsRootST = new float[m_elementsRootSTSize];
    return ( void * )m_elementsRootST;
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

//-*****************************************************************************
static V3 *bsplinePile( const int *sizes,
                        int numSizes,
                        const V3 *cvs )
{
    int numCvsTotal = 0;
    for ( int i = 0; i < numSizes; ++i )
    {
        numCvsTotal += sizes[i];
    }

    // Each curve gets four new points.
    V3 *ret = new V3[numCvsTotal + ( numSizes * 4 )];

    int inCvsPtr = 0;
    int outCvsPtr = 0;
    for ( int i = 0; i < numSizes; ++i )
    {
        int numCvsThisCurve = sizes[i];
        if ( numCvsThisCurve <= 0 )
        {
            continue;
        }

        // Add two additional copies of the first CV.
        ret[outCvsPtr] = cvs[inCvsPtr];
        ++outCvsPtr;
        ret[outCvsPtr] = cvs[inCvsPtr];
        ++outCvsPtr;

        for ( int j = 0; j < numCvsThisCurve; ++j )
        {
            ret[outCvsPtr] = cvs[inCvsPtr];
            ++outCvsPtr;
            ++inCvsPtr;
        }

        // Add two additional copies of the last CV.
        ret[outCvsPtr] = cvs[inCvsPtr-1];
        ++outCvsPtr;
        ret[outCvsPtr] = cvs[inCvsPtr-1];
        ++outCvsPtr;
    }

    return ret;
}

//-*****************************************************************************
static int *pileSizes( const int *sizes, int numSizes )
{
    int *ret = new int[numSizes];
    for ( size_t i = 0; i < numSizes; ++i )
    {
        ret[i] = sizes[i] + 4;
    }
    return ret;
}


//-*****************************************************************************
static float *interpolateWidths( const int *sizes,
                                 int numSizes,
                                 CurveType curveType,
                                 float rootWidth,
                                 float tipWidth )
{
    int numCvsTotal = 0;
    for ( int i = 0; i < numSizes; ++i )
    {
        switch ( curveType )
        {
        case Linear:
            numCvsTotal += sizes[i];
            break;
        case Bezier:
            numCvsTotal += sizes[i] / 2;
            break;
        case Bspline:
            numCvsTotal += sizes[i] + 2;
            break;
        case CatmullRom:
            numCvsTotal += sizes[i];
            break;
        case Hermite:
            numCvsTotal += sizes[i] / 2;
            break;
        }
    }

    // Each curve gets two new points.
    float *ret = new float[numCvsTotal];

    int outCvsPtr = 0;
    for ( int i = 0; i < numSizes; ++i )
    {
        int numCvsThisCurve = 0;
        switch ( curveType )
        {
        case Linear:
            numCvsThisCurve = sizes[i];
            break;
        case Bezier:
            numCvsThisCurve = sizes[i] / 2;
            break;
        case Bspline:
            numCvsThisCurve = sizes[i] + 2;
            break;
        case CatmullRom:
            numCvsThisCurve = sizes[i];
            break;
        case Hermite:
            numCvsThisCurve = sizes[i] / 2;
            break;
        }

        if ( numCvsThisCurve <= 0 )
        {
            continue;
        }

        for ( int j = 0; j < numCvsThisCurve; ++j )
        {
            float interp = float( j ) /
                float( numCvsThisCurve - 1 );
            ret[outCvsPtr] = lerp( rootWidth, tipWidth, interp );
            ++outCvsPtr;
        }
    }

    return ret;
}

//-*****************************************************************************
static float *interpolateWidths( const int *sizes,
                                 int numSizes,
                                 CurveType curveType,
                                 const float *widths2 )
{
    int numCvsTotal = 0;
    for ( int i = 0; i < numSizes; ++i )
    {
        switch ( curveType )
        {
        case Linear:
            numCvsTotal += sizes[i];
            break;
        case Bezier:
            numCvsTotal += sizes[i] / 2;
            break;
        case Bspline:
            numCvsTotal += sizes[i] + 2;
            break;
        case CatmullRom:
            numCvsTotal += sizes[i];
            break;
        case Hermite:
            numCvsTotal += sizes[i] / 2;
            break;
        }
    }

    // Each curve gets two new points.
    float *ret = new float[numCvsTotal];

    int inCvsPtr = 0;
    int outCvsPtr = 0;
    for ( int i = 0; i < numSizes; ++i )
    {
        int numCvsThisCurve = 0;
        switch ( curveType )
        {
        case Linear:
            numCvsThisCurve = sizes[i];
            break;
        case Bezier:
            numCvsThisCurve = sizes[i] / 2;
            break;
        case Bspline:
            numCvsThisCurve = sizes[i] + 2;
            break;
        case CatmullRom:
            numCvsThisCurve = sizes[i];
            break;
        case Hermite:
            numCvsThisCurve = sizes[i] / 2;
            break;
        }

        if ( numCvsThisCurve <= 0 )
        {
            continue;
        }

        // Get two widths, interpolate across.
        float rootWidth = widths2[inCvsPtr];
        ++inCvsPtr;
        float tipWidth = widths2[inCvsPtr];
        ++inCvsPtr;

        for ( int j = 0; j < numCvsThisCurve; ++j )
        {
            float interp = float( j ) /
                float( numCvsThisCurve - 1 );
            ret[outCvsPtr] = lerp( rootWidth, tipWidth, interp );
            ++outCvsPtr;
        }
    }

    return ret;
}

//******************************************************************************
void Strand::internalDeclareRi(GelatoAPI *_rend) const
{
    std::string curveType = "cubic";
    CurveType ct = Linear;
    if ( m_type == "linear" )
    {
        curveType = "linear";
        ct = Linear;
    }
    else if ( m_type == "bspline" ||
              m_type == "interpolated bspline" )
    {
//        Stub_RiBasis( RiBSplineBasis, RI_BSPLINESTEP, 
//                      RiBSplineBasis, RI_BSPLINESTEP );
        curveType = "cubic";
        ct = Bspline;
    }
    else if ( m_type == "bezier" )
    {
//        Stub_RiBasis( RiBezierBasis, RI_BEZIERSTEP, 
//                      RiBezierBasis, RI_BEZIERSTEP );
        curveType = "cubic";
        ct = Bezier;
    }
    else if ( m_type == "catmull-rom" )
    {
//        Stub_RiBasis( RiCatmullRomBasis, RI_CATMULLROMSTEP, 
//                      RiCatmullRomBasis, RI_CATMULLROMSTEP );
        curveType = "cubic";
        ct = CatmullRom;
    }
    else if ( m_type == "hermite" )
    {
//        Stub_RiBasis( RiHermiteBasis, RI_HERMITESTEP, 
//                      RiHermiteBasis, RI_HERMITESTEP );
        curveType = "cubic";
        ct = Hermite;
    }
    else
    {
//        std::cerr << "Invalid curve type: " << m_type << std::endl;
        return;
    }
    if( m_positionsRef == NULL || m_positionsRefSize == 0 )
    {
        std::cerr << "ERROR: Did not receive ref positions for strands: "
                  << m_name << std::endl;
        return;
    }
    if( m_sizes == NULL || m_sizesSize == 0 )
    {
        std::cerr << "ERROR: Did not CV counts for strands: "
                  << m_name << std::endl;
        return;
    }
    
    float constStrandWidth[2];
    constStrandWidth[0] = m_strandWidth[0];
    constStrandWidth[1] = m_strandWidth[1];

    if( getenv( "RI_GTO_STRAND_WIDTHMULT" ) )
    {
        float widthMult = atof( getenv( "RI_GTO_STRAND_WIDTHMULT" ) );
        constStrandWidth[0]*= widthMult;
        constStrandWidth[1]*= widthMult;
        
        std::cout << "Using width multiplier: " << widthMult << std::endl;
    }

    float skipProb = 0.0f;
    const int *m_sizes2 = m_sizes;
    int m_sizesSize2 = m_sizesSize;

    const float *m_positionsRef2 = m_positionsRef;
    int m_positionsRefSize2 = m_positionsRefSize;

    const float *m_elementsRootST2 = m_elementsRootST;
    int m_elementsRootSTSize2 = m_elementsRootSTSize;
            
    std::vector<int> sizesSkip;
    std::vector<float> positionsRefSkip;
    std::vector<float> elementsRootSTSkip;

    if( getenv( "RI_GTO_STRAND_SKIP" ) != NULL )
    {
        skipProb = atof( getenv( "RI_GTO_STRAND_SKIP" ) );
        std::cout << "Using skip prob: " << skipProb << std::endl;
        if( skipProb > 0.0f )
        {
            int positionsRefPtr = 0;
            int rootSTPtr = 0;

            for( int sizesPtr = 0; sizesPtr < m_sizesSize; ++sizesPtr )
            {
                srand48(sizesPtr);
                int numCVs = m_sizes[sizesPtr];

                if( drand48() > skipProb )
                {
                    sizesSkip.push_back( numCVs );

                    if( m_elementsRootST != NULL )
                    {
                        for( int i = 0; i < 2; ++i )
                        {
                            elementsRootSTSkip.push_back( m_elementsRootST[rootSTPtr++] );
                        }
                    }

                    for( int n = 0; n < numCVs; ++n )
                    {
                        for( int i = 0; i < 3; ++i )
                        {
                            positionsRefSkip.push_back( m_positionsRef[positionsRefPtr++] );
                        }
                    }
                }
                else
                {
                    positionsRefPtr += 3 * numCVs;
                    rootSTPtr += 2;
                }
            }

            std::cout << "nCurves: " << sizesSkip.size() << std::endl;
//             std::cerr << "m_positionsRef2: " << positionsRefSkip.size() << std::endl;
//             std::cerr << "m_elementsRootST2: " << elementsRootSTSkip.size() << std::endl;


            m_sizes2 = &sizesSkip.front();
            m_sizesSize2 = sizesSkip.size();

            m_positionsRef2 = &positionsRefSkip.front();
            m_positionsRefSize2 = positionsRefSkip.size();
            
            if( m_elementsRootST )
            {
                m_elementsRootST2 = &elementsRootSTSkip.front();
                m_elementsRootSTSize2 = elementsRootSTSkip.size();
            }
        }
    }


    //-*************************************************************************
    // CREATE TEMP REFS TO CVS
    //-*************************************************************************
    int *sizesPile = NULL;
    V3 *posRefPile = NULL;
    V3 *posOpenPile = NULL;
    V3 *posClosePile = NULL;
    float *interpWidths = NULL;
    float *rootS = NULL;
    float *rootT = NULL;
    std::vector<float> rootSv;
    std::vector<float> rootTv;

    // Do widths first.
    if ( m_elementsWidth != NULL )
    {
        interpWidths = interpolateWidths( m_sizes2, m_sizesSize2,
                                          ct, m_elementsWidth );
    }
    else if ( constStrandWidth[0] != constStrandWidth[1] )
    {
        std::cout << "widths: " <<  constStrandWidth[0] << ", " << constStrandWidth[1] << std::endl;
        interpWidths = interpolateWidths( m_sizes2, m_sizesSize2,
                                          ct, constStrandWidth[0],
                                          constStrandWidth[1] );
        
        std::cout << "interpWidths: " << ((interpWidths==NULL) ? "false" : "true") << std::endl;
    }
    else
    {
        std::cout << "constantwidth: " << m_strandWidth[0] << std::endl;
    }

    if ( m_positionsRef != NULL )
    {
        if ( false && m_type == "interpolated bspline" )
        {
            posRefPile = bsplinePile( m_sizes2, m_sizesSize2,
                                      ( const V3 * )m_positionsRef2 );
        }
        else
        {
            posRefPile = ( V3 * )m_positionsRef2;
        }
    }

    
//     if ( m_positionsOpen != NULL )
//     {
//         if ( m_type == "interpolated bspline" )
//         {
//             posOpenPile = bsplinePile( m_sizes, m_sizesSize,
//                                        ( const V3 * )m_positionsOpen );
//         }
//         else
//         {
//             posOpenPile = ( V3 * )m_positionsOpen;
//         }
//     }

//     if ( m_positionsClose != NULL )
//     {
//         if ( m_type == "interpolated bspline" )
//         {
//             posClosePile = bsplinePile( m_sizes, m_sizesSize,
//                                         ( const V3 * )m_positionsClose );
//         }
//         else
//         {
//             posClosePile = ( V3 * )m_positionsClose;
//         }
//     }

    if ( false && m_type == "interpolated bspline" )
    {
        sizesPile = pileSizes( m_sizes2, m_sizesSize2 );
    }
    else
    {
        sizesPile = ( int * )m_sizes2;
    }

    if( m_elementsRootST2 != NULL )
    {
        for( int i = 0; i < m_sizesSize2; ++i )
        {
//             for( int j = 0; j < sizesPile[i]-2; ++j )
            {
                rootSv.push_back( m_elementsRootST2[i*2] );
                rootTv.push_back( m_elementsRootST2[i*2+1] );
            }
        }
        rootS = &rootSv.front();
        rootT = &rootTv.front();
    }

//     // Build curve ID values.  Entropy doesn't like uniform parameters for
//     // some reason, so we do it per-vertex (varying) 
    std::vector<float> ids;
//     for( int i = 0; i < m_sizesSize2; ++i )
//     {
//         for( int j = 0; j < sizesPile[i]-2; ++j )
//         {
//             ids.push_back( (float)i );
//         }
//     }


    if ( posOpenPile == NULL || m_nomotion)
    {
        //
        //  Build velocities if we're not doing regular motion blur
        //

        std::vector<V3> velocities;

        if (m_nomotion)
        {
            float* Mopen = m_transformOpen ? m_transformOpen : 0;
            float* Mclose = m_transformClose ? m_transformClose : Mopen;
            
            if (Mopen)
            {
                int sum = std::accumulate(sizesPile, sizesPile + m_sizesSize2, 0);
                velocities.resize(sum);

                if (posOpenPile == NULL)
                {
                    for (int i=0; i < velocities.size(); i++)
                    {
                        V3 a = multiply(posRefPile[i], Mclose);
                        V3 b = multiply(posRefPile[i], Mopen);
                        velocities[i] =  a - b;
                    }
                }
                else
                {
                    for (int i=0; i < velocities.size(); i++)
                    {
                        V3 a = multiply(posOpenPile[i], Mclose);
                        V3 b = multiply(posClosePile ? posClosePile[i] : posOpenPile[i], Mopen);

                        velocities[i] = a - b;
                    }
                }
            }
        }

/*
        // Only reference positions
        Stub_DeclareCurves( curveType.c_str(),
                            m_sizesSize2,
                            sizesPile,
                            constStrandWidth[0],
                            interpWidths,
                            ( const float * )posRefPile,
                            ids.empty() ? NULL : &ids.front(),
                            rootS,
                            rootT,
                            velocities.empty() ? NULL : (const float*)&velocities.front() );
*/                            
    }
    else
    {
        if ( posClosePile != NULL )
        {
/*            
            Stub_RiMotionBegin( 0.0f, 1.0f );

            Stub_DeclareCurves( curveType.c_str(),
                                m_sizesSize,
                                sizesPile,
                                m_strandWidth[0],
                                interpWidths,
                                ( const float * )posOpenPile,
                                &ids.front(),
                                rootS,
                                rootT );

            Stub_DeclareCurves( curveType.c_str(),
                                m_sizesSize,
                                sizesPile,
                                m_strandWidth[0],
                                interpWidths,
                                ( const float * )posClosePile,
                                &ids.front(),
                                rootS,
                                rootT );
            
            Stub_RiMotionEnd();
*/            
        }
        else
        {
/*
            Stub_DeclareCurves( curveType.c_str(),
                                m_sizesSize,
                                sizesPile,
                                m_strandWidth[0],
                                interpWidths,
                                ( const float * )posOpenPile,
                                &ids.front(),
                                rootS,
                                rootT );
*/
        }
    }

//     // Delete piles
//     delete[] interpWidths;
// 
//     if ( m_type == "interpolated bspline" )
//     {
// //         delete[] posRefPile;
// //         delete[] posOpenPile;
// //         delete[] posClosePile;
// //         delete[] sizesPile;
//     }
}

} // End namespace GelGto
