//******************************************************************************
// Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
//******************************************************************************

#include "NURBS.h"
#include <UT/UT_Vector4.h>
#include <Gto/Protocols.h>
#include <iostream>
#include <map>

namespace HGto {

int NURBS::m_nurbsObjNumber = 0;

// *****************************************************************************
std::string NURBS::newName()
{
    char str[16];
    snprintf( str, 16, "hNURBSShape%d", m_nurbsObjNumber++ );
    return std::string( str );
}


// *****************************************************************************
NURBS::NURBS( const GEO_PrimNURBSurf *nurbsSrf ) : 
    m_hNurbsSurf( nurbsSrf ),
    Object( newName(), GTO_PROTOCOL_NURBS, 1 )
{
    m_degree[0] = nurbsSrf->getUOrder() - 1;
    m_degree[1] = nurbsSrf->getVOrder() - 1;
}

// *****************************************************************************
NURBS::~NURBS()
{
}


// *****************************************************************************
void NURBS::writeHeader( Gto::Writer &writer )
{
    writer.beginObject( m_name.c_str(), m_protocol.c_str(), m_protoVersion );

    Object::writeHeader( writer );

    writer.beginComponent( GTO_COMPONENT_POINTS );
    writer.property( GTO_PROPERTY_POSITION, Gto::Float, m_pointsPositions.size(), 4 );
    writer.endComponent();

    writer.beginComponent( GTO_COMPONENT_SURFACE );
    writer.property( GTO_PROPERTY_DEGREE, Gto::Int, 2, 1 );
    writer.property( GTO_PROPERTY_UKNOTS, Gto::Float, m_uKnots.size(), 1 );
    writer.property( GTO_PROPERTY_VKNOTS, Gto::Float, m_vKnots.size(), 1 );
    writer.property( GTO_PROPERTY_URANGE, Gto::Float, 2, 1 );
    writer.property( GTO_PROPERTY_VRANGE, Gto::Float, 2, 1 );
    writer.property( GTO_PROPERTY_UFORM, Gto::Int, 1, 1 );
    writer.property( GTO_PROPERTY_VFORM, Gto::Int, 1, 1 );
    writer.endComponent();

    writer.endObject();
}


// *****************************************************************************
void NURBS::writeData( Gto::Writer &writer )
{
    Object::writeData( writer );
//     
//     writer.propertyData( (void *)&m_pointsPositions.front() );
//     writer.propertyData( (void *)&m_elementsType.front() );
//     writer.propertyData( (void *)&m_elementsSize.front() );
//     writer.propertyData( (void *)&m_indicesVertex.front() );
//     writer.propertyData( (void *)&m_smoothingMethod );
}


// *****************************************************************************
void NURBS::getVertices( const GEO_PointList &points )
{
//     std::vector<int> sortedIndices( m_indicesVertex );
//     std::map<int,int> indexMap;
//     sort( sortedIndices.begin(), sortedIndices.end() );
// 
//     int prevIndex = -1;
//     for( int i = 0; i < sortedIndices.size(); ++i )
//     {
//         const UT_Vector4 &p = points[sortedIndices[i]]->getPos();
//         m_pointsPositions.push_back( TwkMath::Vec3f( p.x(), p.y(), p.z() ) );
// 
//         indexMap[sortedIndices[i]] = m_pointsPositions.size()-1;
//         prevIndex = sortedIndices[i];
//         while( prevIndex == sortedIndices[i] ) ++i;
//     }
// 
//     for( int i = 0; i < m_indicesVertex.size(); ++i )
//     {
//         int oldIndex = m_indicesVertex[i];
//         m_indicesVertex[i] = indexMap[oldIndex];
//     }
}

}  //  End namespace HGto
