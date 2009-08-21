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
