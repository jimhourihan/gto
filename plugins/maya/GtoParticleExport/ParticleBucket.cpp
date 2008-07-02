//******************************************************************************
// Copyright (c) 2001-2004 Tweak Inc. All rights reserved.
//******************************************************************************

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
