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

#include "GtoInPoly.h"
#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>
#include <maya/MIntArray.h>
#include <maya/MFloatArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MItMeshPolygon.h>
#include <assert.h>
#include <string.h>
#include <float.h>
#include <algorithm>
#include <iostream>

namespace GtoIOPlugin {
using namespace std;

//******************************************************************************
Poly::Poly( const std::string &name, 
            const std::string &protocol, 
            const unsigned int protocolVersion )
  : Object( name, protocol, protocolVersion ),
        m_numVerts( NULL ),
        m_numVertsSize( 0 ),
        m_indices( NULL ),
        m_indicesSize( 0 ),
        m_positionsRef( NULL ),
        m_positionsSize( 0 ),
        m_stValues( NULL ),
        m_stValuesSize( 0 ),
        m_stIndices( NULL ),
        m_stIndicesSize( 0 ),
        m_normalIndices( NULL ),
        m_normalIndicesSize( 0 ),
        m_normalValues( NULL ),
        m_normalValuesSize( 0 )
{
        // Nothing
}

//******************************************************************************
Poly::~Poly()
{
    delete[] m_numVerts;
    delete[] m_indices;
    delete[] m_positionsRef;
    delete[] m_stValues;
    delete[] m_stIndices;
    delete[] m_normalValues;
    delete[] m_normalIndices;
}

//******************************************************************************
//******************************************************************************
// READER STUFF
//******************************************************************************
//******************************************************************************

//******************************************************************************
Request Poly::component( const std::string &name ) const
{
    if ( name == GTO_COMPONENT_POINTS )
    {
        return Request( true, ( void * )POINTS_C );
    }
    else if ( name == GTO_COMPONENT_ELEMENTS )
    {
        return Request( true, ( void * )ELEMENTS_C );
    }
    else if ( name == GTO_COMPONENT_INDICES )
    {
        return Request( true, ( void * )INDICES_C );
    }
    else if ( name == GTO_COMPONENT_MAPPINGS )
    {
        return Request( true, ( void * )MAPPINGS_C );
    }
    else if ( name == GTO_COMPONENT_NORMALS )
    {
        return Request( true, ( void * )NORMALS_C );
    }

    // Superclass
    return Object::component( name );
}

//******************************************************************************
Request Poly::property( const std::string &name,
                        void *componentData ) const
{
    // We read:
    // points.positions
    // elements.primitives
    // indices.vertices
    //

    if ( (( int )componentData) == POINTS_C )
    {
        if ( name == GTO_PROPERTY_POSITION )
        {
            return Request( true, ( void * )POINTS_POSITION_P );
        }
    }
    else if ( (( int )componentData ) == ELEMENTS_C )
    {
        if ( name == GTO_PROPERTY_SIZE )
        {
            return Request( true, ( void * )ELEMENTS_SIZE_P );
        }
    }
    else if ( (( int )componentData ) == INDICES_C )
    {
        if ( name == GTO_PROPERTY_VERTEX )
        {
            return Request( true, ( void * )INDICES_VERTEX_P );
        }
        else if ( name == GTO_PROPERTY_ST )
        {
            return Request( true, ( void * )INDICES_ST_P );
        }
        else if ( name == GTO_PROPERTY_NORMAL )
        {
            return Request( true, ( void * )INDICES_NORMAL_P );
        }
    }
    else if ( (( int )componentData ) == MAPPINGS_C )
    {
        if ( name == GTO_PROPERTY_ST )
        {
            return Request( true, ( void * )MAPPINGS_ST_P );
        }
    }
    else if ( (( int )componentData ) == NORMALS_C )
    {
        if ( name == GTO_PROPERTY_NORMAL )
        {
            return Request( true, ( void * )NORMALS_NORMAL_P );
        }
    }

    // Superclass
    return Object::property( name, componentData );
}

// *****************************************************************************
void *Poly::data( const PropertyInfo &pinfo, 
                  size_t bytes,
                  void *componentData,
                  void *propertyData )
{
    if( (int)propertyData == POINTS_POSITION_P )
    {
        delete[] m_positionsRef;
        m_positionsSize = pinfo.size * pinfo.width;
        m_positionsRef = new float[m_positionsSize];
        return (void *)m_positionsRef;
    }
    else if ( ((int)propertyData) == MAPPINGS_ST_P )
    {
        delete[] m_stValues;
        m_stValuesSize = pinfo.size * pinfo.width;
        m_stValues = new float[m_stValuesSize];
        return (void *)m_stValues;
    }
    else if ( ((int)propertyData) == NORMALS_NORMAL_P )
    {
        delete[] m_stIndices;
        m_stIndicesSize = pinfo.size * pinfo.width;
        m_stIndices = new int[m_stIndicesSize];
        return (void *)m_stIndices;
    }
    else if ( ((int)propertyData) == INDICES_VERTEX_P )
    {
        delete[] m_indices;
        m_indicesSize = pinfo.size * pinfo.width;
        m_indices = new int[m_indicesSize];
        return (void *)m_indices;
    }
    else if ( ((int)propertyData) == INDICES_ST_P )
    {
        delete[] m_stIndices;
        m_stIndicesSize = pinfo.size * pinfo.width;
        m_stIndices = new int[m_stIndicesSize];
        return (void *)m_stIndices;
    }
    else if ( ((int)propertyData) == INDICES_NORMAL_P )
    {
        delete[] m_normalIndices;
        m_normalIndicesSize = pinfo.size * pinfo.width;;
        m_normalIndices = new int[m_normalIndicesSize];
        return (void *)m_normalIndices;
    }
    if ( ((int)propertyData) == ELEMENTS_SIZE_P )
    {
        m_tmpShortData.resize( pinfo.size * pinfo.width );
        return (void *)&m_tmpShortData.front();
    }

    // Superclass
    return Object::data( pinfo, bytes, componentData, propertyData );
}

// *****************************************************************************
void Poly::dataRead( const PropertyInfo &pinfo,
                     void *componentData,
                     void *propertyData,
                     const StringTable &strings )
{
    if ( ((int)propertyData) == ELEMENTS_SIZE_P )
    {
        setNumVerts( &m_tmpShortData.front(), pinfo.size * pinfo.width );
        m_tmpShortData.clear();
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
void Poly::setNumVerts( const unsigned short *numVerts, size_t numVertsSize )
{
    delete[] m_numVerts;
    m_numVertsSize = numVertsSize;
    m_numVerts = new int[m_numVertsSize];

    for ( size_t i = 0; i < numVertsSize; ++i )
    {
        m_numVerts[i] = numVerts[i];
    }
}

//******************************************************************************
void Poly::setIndices( const int *indices, size_t indicesSize )
{
    delete[] m_indices;
    m_indicesSize = indicesSize;
    m_indices = new int[m_indicesSize];
    memcpy( ( void * )m_indices,
            ( const void * )indices,
            m_indicesSize * sizeof( int ) );
}

//******************************************************************************
void Poly::setPositionsRef( const float *positionsRef, size_t positionsRefSize )
{
    delete[] m_positionsRef;

    m_positionsSize = positionsRefSize;
    m_positionsRef = new float[m_positionsSize];
    memcpy( ( void * )m_positionsRef,
            ( const void * )positionsRef,
            m_positionsSize * sizeof( float ) );
}

//******************************************************************************
void Poly::setStValues( const float *stValues, size_t stValuesSize )
{
    delete[] m_stValues;

    m_stValuesSize = stValuesSize;
    m_stValues = new float[m_stValuesSize];
    memcpy( ( void * )m_stValues,
            ( const void * )stValues,
            m_stValuesSize * sizeof( float ) );
}

//******************************************************************************
void Poly::setStIndices( const int *stIndices, size_t stIndicesSize )
{
    delete[] m_stIndices;

    m_stIndicesSize = stIndicesSize;
    m_stIndices = new int[m_stIndicesSize];
    memcpy( ( void * )m_stIndices,
            ( const void * )stIndices,
            m_stIndicesSize * sizeof( int ) );
}

// *****************************************************************************
void Poly::setNormalValues( const float *normValues, size_t normValuesSize )
{
    delete[] m_normalValues;

    m_normalValuesSize = normValuesSize;
    m_normalValues = new float[m_normalValuesSize];
    memcpy( ( void * )m_normalValues,
            ( const void * )normValues,
            m_normalValuesSize * sizeof( float ) );
}

// *****************************************************************************
void Poly::setNormalIndices( const int *normIndices, size_t normIndicesSize )
{
    delete[] m_normalIndices;

    m_normalIndicesSize = normIndicesSize;
    m_normalIndices = new int[m_normalIndicesSize];
    memcpy( ( void * )m_normalIndices,
            ( const void * )normIndices,
            m_normalIndicesSize * sizeof( int ) );
}

// *****************************************************************************
//
// These are a data structure and function object used below 
// for sorting normals by vertex index

struct NormalEntry
{
    int faceIndex;
    int vertIndex;
    float normal[3];
};

struct Comp
{
    bool operator() ( const NormalEntry &a, const NormalEntry &b )
    {
        return a.vertIndex < b.vertIndex;
    }
};

//******************************************************************************
void Poly::declareMaya()
{
    // Check for an incomplete surface definition.  This can happen if
    // the user tries to load an animation pose .gto file
    if( m_numVerts == NULL || m_indices == NULL || m_positionsRef == NULL )
    {
        string str = "Incomplete surface definition for object: " 
                     + m_name;
        MGlobal::displayError( str.c_str() );
        return;
    }

    // Massage the data...
    int numFaces = m_numVertsSize;
    MIntArray faceCounts( m_numVerts, m_numVertsSize );
    MIntArray faceConnects( m_indices, m_indicesSize );

    int numPoints = m_positionsSize / 3;
    MPointArray points( numPoints );
    for( int i = 0, j = 0; i < numPoints; ++i, j += 3 )
    {
        MPoint fp( m_positionsRef[j], m_positionsRef[j+1],
                           m_positionsRef[j+2] );
        points.set( fp, i );
    }

    // Create the poly mesh
    MFnMesh newMesh;
    MStatus status;

    // newMesh.create() will create an empty transform node
    // and return it to us
    m_mayaParentObject = newMesh.create( numPoints,
                                         numFaces,
                                         points, 
                                         faceCounts, 
                                         faceConnects,
                                         MObject::kNullObj, 
                                         &status );

    if( status != MS::kSuccess )
    {
        status.perror( "newMesh.create() failed" );
        m_mayaObject = MObject::kNullObj;
        m_mayaParentObject = MObject::kNullObj;
        return;
    }
    
    // Set normals if included in file
    if( m_normalValues != NULL && m_normalIndices != NULL )
    {
        // Due to a bizarre internal Maya issue, we have to sort the
        // normals based on vertex index to avoid O(N^2) time for
        // setting the normals on meshes with many shells

        // First, we need to build a list of all the normals, along
        // with the faceID and vertexID to which they belong
        vector<NormalEntry> normalsSort( m_indicesSize );
        int vindex = 0;
        for( MItMeshPolygon p( m_mayaParentObject ); ! p.isDone(); p.next() ) 
        {
            for( int q = 0, nv = p.polygonVertexCount(); q < nv; q++ )
            {
                normalsSort[vindex].faceIndex = p.index();
                normalsSort[vindex].vertIndex = p.vertexIndex( q );
                int nIndex = m_normalIndices[vindex] * 3;
                normalsSort[vindex].normal[0] = m_normalValues[nIndex + 0];
                normalsSort[vindex].normal[1] = m_normalValues[nIndex + 1];
                normalsSort[vindex].normal[2] = m_normalValues[nIndex + 2];
                ++vindex;
            }
        }
        
        sort( normalsSort.begin(), normalsSort.end(), Comp() );

        // Now that the normals are sorted by vertexIndex, put them into
        // maya data structures.
        MIntArray vertVertList( m_indicesSize );
        MIntArray vertFaceList( m_indicesSize );
        MVectorArray normalList( m_indicesSize );
        for( size_t i = 0; i < normalsSort.size(); ++i )
        {
            vertFaceList[i] = normalsSort[i].faceIndex;
            vertVertList[i] = normalsSort[i].vertIndex;
            normalList[i] = MVector( normalsSort[i].normal[0], 
                                     normalsSort[i].normal[1], 
                                     normalsSort[i].normal[2] );
        }

        status = newMesh.setFaceVertexNormals( normalList,
                                               vertFaceList, 
                                               vertVertList,
                                               MSpace::kObject );
        
        if( status != MS::kSuccess )
        {
            status.perror( "newMesh.setVertexNormals() failed" );
        }
    }

    // Set ST coords if included in file
    if( m_stValues != NULL && m_stIndices != NULL )
    {
        MIntArray stIndices( m_stIndicesSize );
        for( size_t i = 0; i < m_stIndicesSize; ++i )
        {
            stIndices.set( m_stIndices[i], i );
        }

        MIntArray stFaceCounts( m_numVertsSize );
        for( size_t i =  0; i < m_numVertsSize; ++i )
        {
            stFaceCounts.set( m_numVerts[i], i );
        }


        int numSTs = m_stValuesSize / 2;
        MFloatArray sArray( numSTs );
        MFloatArray tArray( numSTs );

        for( int i = 0, p = 0; i < numSTs; ++i, p += 2 )
        {
            sArray[i] = m_stValues[p];
            tArray[i] = m_stValues[p+1];
        }

        // Set the ST coordinates (which Maya erroneously calls UV)
        newMesh.setUVs( sArray, tArray );
        newMesh.assignUVs( stFaceCounts, stIndices );
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
