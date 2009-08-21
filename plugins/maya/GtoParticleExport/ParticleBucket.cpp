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

#include <maya/MPlug.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnDagNode.h>
#include <maya/MSelectionList.h>

#include <Gto/Protocols.h>
#include <TwkMaya/Liberation.h>
#include "ParticleBucket.h"

// *****************************************************************************
ParticleBucket::ParticleBucket( std::string name )
    : m_name( name )
{
} 


// *****************************************************************************
ParticleBucket::~ParticleBucket()
{
    for( int i = 0; i < m_exportAttrs.size(); ++i )
    {
        delete m_exportAttrs[i];
    }
}


// *****************************************************************************
MObject ParticleBucket::mayaObject() const
{
    MSelectionList slist;
    slist.add( m_name.c_str() );
    
    MObject obj;
    slist.getDependNode( 0, obj );
    return obj;
}

// *****************************************************************************
int ParticleBucket::startFrame() const
{
    MStatus status;
    MFnDependencyNode dn( mayaObject() );

    MPlug startFramePlug = dn.findPlug( "startFrame", &status );
    if( status != MS::kSuccess )
    {
        std::cerr << "Error getting " << m_name << ".startFrame" << std::endl;
        return 0;
    }

    double startFrameVal = 0.0;
    status = startFramePlug.getValue( startFrameVal );
    if( status != MS::kSuccess )
    {
        std::cerr << "Error getting " << m_name << ".startFrame" << std::endl;
        return 0;
    }
    return (int)startFrameVal;
}


// *****************************************************************************
MStatus ParticleBucket::exportAttr( std::string attrName )
{
    MStatus status = MS::kSuccess;
    MFnDependencyNode dn( mayaObject() );

    MPlug attrPlug = dn.findPlug( attrName.c_str(), &status );
    if( status != MS::kSuccess )
    {
        return status;
    }

    MObject attrObj = attrPlug.attribute( &status );
    CHECK( status );

    MFnTypedAttribute attrTA( attrObj, &status );
    CHECK( status );

    MFnData::Type attrType = attrTA.attrType( &status );
    CHECK( status );
    if( ( attrType != MFnData::kDoubleArray )
        && ( attrType != MFnData::kIntArray )
        && ( attrType != MFnData::kVectorArray ) )
    {
        std::string err( "Unsupported attribute type: " );
        err += m_name + "." + attrName;
        status.perror( err.c_str() );
        return MS::kInvalidParameter;
    }

    m_exportAttrs.push_back( new ParticleAttribute( attrName, m_name ) );

    return MS::kSuccess;
}


// *****************************************************************************
void ParticleBucket::writeGtoHeader( Gto::Writer *writer )
{
    writer->beginObject( m_name.c_str(), GTO_PROTOCOL_PARTICLE, 1 );
    writer->beginComponent( GTO_COMPONENT_POINTS );

    for( int i = 0; i < m_exportAttrs.size(); ++i )
    {
        m_exportAttrs[i]->writeGtoHeader( writer );
    }

    writer->endComponent();
    writer->endObject();
}


// *****************************************************************************
void ParticleBucket::writeGtoData( Gto::Writer *writer )
{
    for( int i = 0; i < m_exportAttrs.size(); ++i )
    {
        m_exportAttrs[i]->writeGtoData( writer );
    }
}
