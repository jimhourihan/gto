//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************
#include "version.h"

#include "GtoInNURBS.h"
#include <maya/MGlobal.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include <algorithm>

namespace GtoIOPlugin {
using namespace std;

//******************************************************************************
NURBS::NURBS( const std::string &name, 
              const std::string &protocol, 
              const unsigned int protocolVersion )
  : Object( name, protocol, protocolVersion ),
    m_degreeU( 3 ),
    m_degreeV( 3 ),
    m_formU( MFnNurbsSurface::kOpen ),
    m_formV( MFnNurbsSurface::kOpen ),
    m_knotsU( NULL ),
    m_knotsUSize( 0 ),
    m_knotsV( NULL ),
    m_knotsVSize( 0 ),
    m_minU( 0.0f ),
    m_maxU( 1.0f ),
    m_minV( 0.0f ),
    m_maxV( 1.0f ),
    m_positionsRef( NULL ),
    m_positionsSize( 0 )
{
    // Nothing
}

//******************************************************************************
NURBS::~NURBS()
{
    delete[] m_knotsU;
    delete[] m_knotsV;
    delete[] m_positionsRef;
}

//******************************************************************************
//******************************************************************************
// READER STUFF
//******************************************************************************
//******************************************************************************

//******************************************************************************
void *NURBS::component( const std::string &name ) const
{
    if ( name == GTO_COMPONENT_POINTS )
    {
      return ( void * )POINTS_C;
    }
    else if ( name == GTO_COMPONENT_SURFACE )
    {
      return ( void * )SURFACE_C;
    }

    // Superclass
    return Object::component( name );
}

//******************************************************************************
void *NURBS::property( const std::string &name,
                       void *componentData ) const
{
    // We read:
    // points.position
    // points.weight
    // surface.degree
    // surface.uKnots
    // surface.vKnots
    // surface.uRange
    // surface.vRange
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

    // Superclass
    return Object::property( name, componentData );
}

//******************************************************************************
void NURBS::data( void *componentData,
                  void *propertyData,
                  const float *items,
                  size_t numItems,
                  size_t width)
{
    if ( (( int )propertyData) == POINTS_POSITION_P )
    {
        setPositionsRef( items, numItems * width );
        return;
    }
    else if ( (( int )propertyData) == POINTS_WEIGHT_P )
    {
        setWeights( items, numItems * width );
        return;
    }
    else if ( (( int )propertyData) == SURFACE_UKNOTS_P )
    {
        setKnotsU( items, numItems * width );
        return;
    }
    else if ( (( int )propertyData) == SURFACE_VKNOTS_P )
    {
        setKnotsV( items, numItems * width );
        return;
    }
    else if ( (( int )propertyData) == SURFACE_URANGE_P )
    {
        if ( numItems * width != 2 )
        {
            string str = "Invalid number of u range items in object: "
                         + m_name;
            MGlobal::displayWarning( str.c_str() );
            return;
        }
        setRangeU( items[0], items[1] );
        return;
    }
    else if ( (( int )propertyData) == SURFACE_VRANGE_P )
    {
        if ( numItems * width != 2 )
        {
            string str = "Invalid number of v range items in object: "
                         + m_name;
            MGlobal::displayWarning( str.c_str() );
            return;
        }
        setRangeV( items[0], items[1] );
        return;
    }

    // Superclass
    Object::data( componentData, propertyData, items, numItems, width );
}

//******************************************************************************
void NURBS::data( void *componentData,
                  void *propertyData,
                  const int *items,
                  size_t numItems,
                  size_t width)
{
    if ( (( int )propertyData) == SURFACE_DEGREE_P )
    {
        if ( numItems != 2 )
        {
            string str = "Invalid number of degree items in object: "
                         + m_name;
            MGlobal::displayWarning( str.c_str() );
            return;
        }
        setDegree( items[0], items[1] );
        return;
    }
    else if ( (( int )propertyData) == SURFACE_UFORM_P )
    {
        setFormU( items[0] );
        return;
    }
    else if ( (( int )propertyData) == SURFACE_VFORM_P )
    {
        setFormV( items[0] );
        return;
    }

    // Superclass
    Object::data( componentData, propertyData, items, numItems, width );
}

//******************************************************************************
//******************************************************************************
// INTERAL SETTINGS
//******************************************************************************
//******************************************************************************
void NURBS::setDegree( int degreeU, int degreeV )
{
    m_degreeU = degreeU;
    m_degreeV = degreeV;
}

// *****************************************************************************
void NURBS::setFormU( int formU )
{
    m_formU = formU;
}

// *****************************************************************************
void NURBS::setFormV( int formV )
{
    m_formV = formV;
}

//******************************************************************************
void NURBS::setKnotsU( const float *knotsU, size_t knotsUSize )
{
    delete[] m_knotsU;
    m_knotsUSize = knotsUSize;
    m_knotsU = new float[m_knotsUSize];
    memcpy( ( void * )m_knotsU,
            ( const void * )knotsU,
            m_knotsUSize * sizeof( float ) );
}

//******************************************************************************
void NURBS::setKnotsV( const float *knotsV, size_t knotsVSize )
{
    delete[] m_knotsV;
    m_knotsVSize = knotsVSize;
    m_knotsV = new float[m_knotsVSize];
    memcpy( ( void * )m_knotsV,
            ( const void * )knotsV,
            m_knotsVSize * sizeof( float ) );
}

//******************************************************************************
void NURBS::setRangeU( float minU, float maxU )
{
    m_minU = minU;
    m_maxU = maxU;
}

//******************************************************************************
void NURBS::setRangeV( float minV, float maxV )
{
    m_minV = minV;
    m_maxV = maxV;
}

//******************************************************************************
void NURBS::setWeights( const float *weights, size_t weightsSize )
{
    if ( m_positionsSize != weightsSize * 4 )
    {
        delete[] m_positionsRef;
        m_positionsSize = weightsSize * 4;
        m_positionsRef = new float[m_positionsSize];
        memset( ( void * )m_positionsRef, 0,
                m_positionsSize * sizeof( float ) );
    }

    size_t  i, j;
    for ( i = 0, j = 3; i < weightsSize; ++i, j += 4 )
    {
        m_positionsRef[j] = weights[i];
    }
}

//******************************************************************************
void NURBS::setPositionsRef( const float *positionsRef,
                             size_t positionsRefSize )
{
    if ( ( m_positionsSize / 4 ) != ( positionsRefSize / 3 ) )
    {
        delete[] m_positionsRef;
        m_positionsSize = ( positionsRefSize / 3 ) * 4;
        m_positionsRef = new float[m_positionsSize];
        memset( ( void * )m_positionsRef, 0,
                m_positionsSize * sizeof( float ) );

        // Default the weights to 1
        for ( size_t  i = 3; i < m_positionsSize; i += 4 )
        {
            m_positionsRef[i] = 1.0f;
        }
    }
    
    size_t i, j;
    for ( i = 0, j = 0; i < m_positionsSize; i += 4, j += 3 )
    {
        m_positionsRef[i] = positionsRef[j];
        m_positionsRef[i+1] = positionsRef[j+1];
        m_positionsRef[i+2] = positionsRef[j+2];
    }
}

//******************************************************************************
void NURBS::declareMaya()
{
    // Check for an incomplete surface definition.  This can happen if
    // the user tries to load an animation pose .gto file
    if( m_knotsU == NULL || m_knotsV == NULL || m_positionsRef == NULL )
    {
        string str = "Incomplete surface definition for object: " + m_name;
        MGlobal::displayError( str.c_str() );
        return;
    }

    // The gto NURBS protocol adds two knots to force knot[0]==knot[1]
    // and knot[last]==knot[last-1].  Maya doesn't need or want these
    // extras, so we throw them away
    int knotsUSize = m_knotsUSize - 2;
    int knotsVSize = m_knotsVSize - 2;
    float *knotsU = &m_knotsU[1];
    float *knotsV = &m_knotsV[1];
    
    // For now, we only handle open surfaces
    MFnNurbsSurface::Form formU = (MFnNurbsSurface::Form)m_formU;
    MFnNurbsSurface::Form formV = (MFnNurbsSurface::Form)m_formV;

    // Tell maya to use the weights (?)
    bool createRational = true;

    // Calculate some other handy values:
    unsigned int degreeU = m_degreeU;
    unsigned int degreeV = m_degreeV;
    int uSpans = knotsUSize - ( 2 * degreeU ) + 1;
    int vSpans = knotsVSize - ( 2 * degreeV ) + 1;
    unsigned int expectedCVs = ( uSpans + degreeU ) * ( vSpans + degreeV );

    unsigned int numpoints = (int)(m_positionsSize / 4);
    if( expectedCVs != numpoints )
    {
        string str = "Invalid number of CVs in NURBS shape: " + m_name;
        MGlobal::displayError( str.c_str() );
        return;
    }
    
    // Build the controlVertices.  Note that the gto NURBS protocol
    // specifies that CVs are V-major.  Maya expects U-major, so we
    // transpose as we copy data into the MPointArray
    MPointArray controlVertices( numpoints, MPoint( 0.0f, 0.0f, 0.0f, 0.0f ) );

    int sizeV = vSpans + degreeV;
    int sizeU = uSpans + degreeU;

    for( int v = 0; v < sizeV; ++v )
    {
        for( int u = 0; u < sizeU; ++u )
        {
            int srcPos = ( v * sizeU ) + u;
            int dstPos = ( u * sizeV ) + v;
            const MPoint p( m_positionsRef[(srcPos*4)],
                            m_positionsRef[(srcPos*4)+1],
                            m_positionsRef[(srcPos*4)+2],
                            m_positionsRef[(srcPos*4)+3] );

            controlVertices.set( p, dstPos );
        }
    }

    // Repackage the U knots
    const MDoubleArray uKnotSequences( knotsU, knotsUSize );

    // Repackage the V knots
    const MDoubleArray vKnotSequences( knotsV, knotsVSize );

    // Ask Maya to create the surface
    MFnNurbsSurface nurbsSurf;
    MStatus status;
    
    // nurbsSurf.create() will create an empty transform node
    // and return it to us
    m_mayaParentObject = nurbsSurf.create( controlVertices,
                                           uKnotSequences,
                                           vKnotSequences,
                                           degreeU,
                                           degreeV,
                                           formU,
                                           formV,
                                           createRational,
                                           MObject::kNullObj,
                                           &status );
    if( status != MS::kSuccess )
    {
        MString err( "nurbsSurf.create('" );
        err += MString( m_name.c_str() );
        err += "'):";
        status.perror( err );
        m_mayaObject = MObject::kNullObj;
        m_mayaParentObject = MObject::kNullObj;
        return;
    }

    // Now use that transform node we got earlier to store a handle to
    // the real geometry node.
    MFnDagNode parentDN( m_mayaParentObject );
    m_mayaObject = parentDN.child( 0 );

    // Set misc stuff...
    setName();
    addToDefaultSG();
}

} // End namespace GtoIOPlugin
