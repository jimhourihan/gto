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

#include "Poly.h"
#include <UT/UT_Vector4.h>
#include <Gto/Protocols.h>
#include <GU/GU_PrimPoly.h>
#include <GU/GU_Types.h>
#include <iostream>
#include <map>

namespace HGto {

// *****************************************************************************
Poly::Poly( std::string name ) : 
    m_smoothingMethod( 0 ),
    Object( name, GTO_PROTOCOL_POLYGON, 2 )
{
}

// *****************************************************************************
Poly::~Poly()
{
}


// *****************************************************************************
void Poly::writeHeader( Gto::Writer &writer )
{
    if( m_indicesVertex.size() == 0 ) return; // Nothing to write...

    writer.beginObject( m_name.c_str(), m_protocol.c_str(), m_protoVersion );

    Object::writeHeader( writer );

    writer.beginComponent( GTO_COMPONENT_POINTS );
    writer.property( GTO_PROPERTY_POSITION, Gto::Float, m_pointsPositions.size(), 3 );
    writer.endComponent();

    writer.beginComponent( GTO_COMPONENT_ELEMENTS);
    writer.property( GTO_PROPERTY_TYPE, Gto::Byte, m_elementsType.size(), 1 );
    writer.property( GTO_PROPERTY_SIZE, Gto::Short, m_elementsSize.size(), 1 );
    writer.endComponent();

    writer.beginComponent( GTO_COMPONENT_INDICES );
    writer.property( GTO_PROPERTY_VERTEX, Gto::Int, m_indicesVertex.size(), 1 );

    writer.beginComponent( GTO_COMPONENT_SMOOTHING );
    writer.property( GTO_PROPERTY_METHOD, Gto::Int, 1, 1 );

    writer.endObject();
}


// *****************************************************************************
void Poly::writeData( Gto::Writer &writer )
{
    if( m_indicesVertex.size() == 0 ) return; // Nothing to write...

    Object::writeData( writer );
    
    writer.propertyData( (void *)&m_pointsPositions.front() );
    writer.propertyData( (void *)&m_elementsType.front() );
    writer.propertyData( (void *)&m_elementsSize.front() );
    writer.propertyData( (void *)&m_indicesVertex.front() );
    writer.propertyData( (void *)&m_smoothingMethod );
}


// *****************************************************************************
void Poly::addFace( const GEO_PrimPoly *prim )
{
    m_elementsType.push_back( 0 ); //  Type 0 == General N-sided polygon

    m_elementsSize.push_back( prim->getVertexCount() );
    
    for( int i = 0; i < prim->getVertexCount(); ++i )
    {
        m_indicesVertex.push_back( prim->getVertex(i).getPt()->getNum() );
    }
}


// *****************************************************************************
void Poly::getVertices( const GEO_PointList &points )
{
    std::map<int,int> indexMap;  // oldIndex -> newIndex

    std::vector<int> sortedIndices( m_indicesVertex );
    sort( sortedIndices.begin(), sortedIndices.end() );

    int prevIndex = -1;
    for( int i = 0; i < sortedIndices.size(); ++i )
    {
        const int index = sortedIndices[i];
        if( prevIndex == index ) continue;

        const UT_Vector4 &p = points[index]->getPos();
        m_pointsPositions.push_back( UT_Vector3( p.x(), p.y(), p.z() ) );

        indexMap[index] = m_pointsPositions.size()-1;
        prevIndex = index;
    }

    for( int i = 0; i < m_indicesVertex.size(); ++i )
    {
        const int oldIndex = m_indicesVertex[i];
        m_indicesVertex[i] = indexMap[oldIndex];
    }
}


// *****************************************************************************
void Poly::declareHoudini( GU_Detail &gdp )
{
    const size_t pointsOffset = gdp.points().entries();

    // Houdini's convention is for vectors to be rows, not columns. (drw900)
    UT_Matrix4 gm(m_globalMatrix);
    gm.transpose();

    for( size_t i = 0; i < m_pointsPositions.size(); ++i )
    {
        const UT_Vector3 &v = m_pointsPositions[i] * gm;

        GEO_Point *p = gdp.appendPoint();
        p->getPos().assign((const float*)&v, 3);
    }

    std::vector<int>::iterator viter = m_indicesVertex.begin();
    
    for( size_t i = 0; i < m_elementsSize.size(); ++i )
    {
        const int nVerts = m_elementsSize[i];
        
        GU_PrimPoly *face = GU_PrimPoly::build( &gdp, 0 );
        for( size_t p = 0; p < nVerts; ++p )
        {
            const int vIndex = (*viter++);
            face->appendVertex( gdp.points()[vIndex+pointsOffset] );
        }
    }
}


}  //  End namespace HGto
