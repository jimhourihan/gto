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
    m_knotsU( NULL ),
    m_knotsUSize( 0 ),
    m_knotsV( NULL ),
    m_knotsVSize( 0 ),
    m_positionsRef( NULL ),
    m_positionsSize( 0 )
{
    m_degree[0] = 3;       // U
    m_degree[1] = 3;       // V

    m_form[0] = MFnNurbsSurface::kOpen; // U
    m_form[1] = MFnNurbsSurface::kOpen; // V

    m_Urange[0] = 0.0f;    // U
    m_Urange[1] = 1.0f;    // V

    m_Vrange[0] = 0.0f;    // U
    m_Vrange[1] = 1.0f;    // V
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
Request NURBS::component( const std::string &name ) const
{
    if ( name == GTO_COMPONENT_POINTS )
    {
      return Request( true, (void *)POINTS_C );
    }
    else if ( name == GTO_COMPONENT_SURFACE )
    {
      return Request( true, (void *)SURFACE_C );
    }

    // Superclass
    return Object::component( name );
}

//******************************************************************************
Request NURBS::property( const std::string &name,
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
            return Request( true, ( void * )POINTS_POSITION_P );
        }
        else if ( name == GTO_PROPERTY_WEIGHT )
        {
            return Request( true, ( void * )POINTS_WEIGHT_P );
        }
    }
    else if ( (( int )componentData ) == SURFACE_C )
    {
        if ( name == GTO_PROPERTY_DEGREE )
        {
            return Request( true, ( void * )SURFACE_DEGREE_P );
        }
        else if ( name == GTO_PROPERTY_UKNOTS )
        {
            return Request( true, ( void * )SURFACE_UKNOTS_P );
        }
        else if ( name == GTO_PROPERTY_VKNOTS )
        {
            return Request( true, ( void * )SURFACE_VKNOTS_P );
        }
        else if ( name == GTO_PROPERTY_URANGE )
        {
            return Request( true, ( void * )SURFACE_URANGE_P );
        }
        else if ( name == GTO_PROPERTY_VRANGE )
        {
            return Request( true, ( void * )SURFACE_VRANGE_P );
        }
    }

    // Superclass
    return Object::property( name, componentData );
}

// *****************************************************************************
void *NURBS::data( const PropertyInfo &pinfo, 
                   size_t bytes,
                   void *componentData,
                   void *propertyData )
{
    if( ( (int)propertyData ) == POINTS_POSITION_P )
    {
        m_tmpFloatData.resize( pinfo.size * pinfo.width );
        return (void *)&m_tmpFloatData.front();
    }
    else if( (int)propertyData == POINTS_WEIGHT_P )
    {
        m_tmpFloatData.resize( pinfo.size * pinfo.width );
        return (void *)&m_tmpFloatData.front();
    }
    else if( (int)propertyData == SURFACE_UKNOTS_P )
    {
        delete[] m_knotsU;
        m_knotsUSize = pinfo.size * pinfo.width;
        m_knotsU = new float[m_knotsUSize];
        return (void *)m_knotsU;
    }
    else if( (int)propertyData == SURFACE_VKNOTS_P )
    {
        delete[] m_knotsV;
        m_knotsVSize = pinfo.size * pinfo.width;
        m_knotsV = new float[m_knotsVSize];
        return (void *)m_knotsV;
    }
    else if( (int)propertyData == SURFACE_URANGE_P )
    {
        if ( pinfo.size * pinfo.width != 2 )
        {
            string str = "Invalid number of u range items in object: "
                         + m_name;
            MGlobal::displayWarning( str.c_str() );
            return NULL;
        }
        return (void *)&m_Urange[0];
    }
    else if( (int)propertyData == SURFACE_VRANGE_P )
    {
        if ( pinfo.size * pinfo.width != 2 )
        {
            string str = "Invalid number of v range items in object: "
                         + m_name;
            MGlobal::displayWarning( str.c_str() );
            return NULL;
        }
        return (void *)&m_Vrange[0];
    }
    else if( (int)propertyData == SURFACE_DEGREE_P )
    {
        if( pinfo.size != 2 )
        {
            string str = "Invalid number of degree items in object: "
                         + m_name;
            MGlobal::displayWarning( str.c_str() );
            return NULL;
        }
        return (void *)&m_degree[0];
    }
    else if( (int)propertyData == SURFACE_UFORM_P )
    {
        if( pinfo.size != 1 )
        {
            string str = "Invalid number of U form items in object: "
                         + m_name;
            MGlobal::displayWarning( str.c_str() );
            return NULL;
        }
        return (void *)&m_form[0];
    }
    else if( (int)propertyData == SURFACE_VFORM_P )
    {
        if( pinfo.size != 1 )
        {
            string str = "Invalid number of V form items in object: "
                         + m_name;
            MGlobal::displayWarning( str.c_str() );
            return NULL;
        }
        return (void *)&m_form[1];
    }

    // Superclass
    return Object::data( pinfo, bytes, componentData, propertyData );    
}

// *****************************************************************************
void NURBS::dataRead( const PropertyInfo &pinfo,
                      void *componentData,
                      void *propertyData,
                      const StringTable &strings )
{
    if( (int)propertyData == POINTS_POSITION_P )
    {
        setPositionsRef( &m_tmpFloatData.front(), pinfo.size * pinfo.width );
        m_tmpFloatData.clear();
    }
    else if( (int)propertyData == POINTS_WEIGHT_P )
    {
        setWeights( &m_tmpFloatData.front(), pinfo.size * pinfo.width );
        m_tmpFloatData.clear();
    }
    else
    {
        // Superclass
        Object::dataRead( pinfo, componentData, propertyData, strings );
    }
}

//******************************************************************************
//******************************************************************************
// INTERAL SETTINGS
//******************************************************************************
//******************************************************************************

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
    MFnNurbsSurface::Form formU = (MFnNurbsSurface::Form)m_form[0];
    MFnNurbsSurface::Form formV = (MFnNurbsSurface::Form)m_form[1];

    // Tell maya to use the weights (?)
    bool createRational = true;

    // Calculate some other handy values:
    unsigned int degreeU = m_degree[0];
    unsigned int degreeV = m_degree[1];
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
    addTextureChannelAttributes();
}

} // End namespace GtoIOPlugin
