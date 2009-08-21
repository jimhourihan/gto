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
#include <assert.h>
#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MMessage.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MDGContext.h>
#include <maya/MDagPath.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MDataHandle.h>
#include <maya/MPoint.h>
#include "GtoDeformer.h"
#include "GtoPointReader.h"

#define CHECK_ERR( STAT, MSG )                  \
if ( STAT != MS::kSuccess )                     \
{                                               \
    STAT.perror( MSG );                         \
    return STAT;                                \
}

#define CHECK( STAT, MSG )                      \
if ( STAT != MS::kSuccess )                     \
{                                               \
    STAT.perror( MSG );                         \
    return;                                     \
}

MTypeId GtoDeformer::id( 0x555003 );

MObject GtoDeformer::m_gtoBaseAttr;
MObject GtoDeformer::m_gtoStartFrameAttr;
MObject GtoDeformer::m_gtoEndFrameAttr;
MObject GtoDeformer::m_timeAttr;
MObject GtoDeformer::m_accessoryMatrixAttr;
MObject GtoDeformer::m_useCacheAttr;
MObject GtoDeformer::m_silentAttr;



static void PrintConnections(const MPlug& plug)
{
    MPlugArray plugs;

    cout << "plug " << plug.name().asChar() << endl;

    plug.connectedTo(plugs,true,false);

    cout << "\tinputs " << plugs.length() << endl;
    for(int i=0;i<plugs.length();++i)
    {
        cout<<"\t\t"<< plugs[i].name().asChar() <<endl;
    }

    plug.connectedTo(plugs,false,true);

    cout << "\toutputs " << plugs.length() << endl;
    for(int i=0;i<plugs.length();++i)
    {
        cout<<"\t\t"<< plugs[i].name().asChar() <<endl;
    }
}

static void PrintNodeConnections(const MObject& node)
{
    cout << "----------- " << endl;

    MPlugArray nodeplugs;

    MFnDependencyNode fn(node);

    cout << "node " << fn.name().asChar() << endl;

    fn.getConnections(nodeplugs);

    cout << "numplugs " << nodeplugs.length() << endl;
    for(int i=0;i<nodeplugs.length();++i) 
    {
        PrintConnections( nodeplugs[i] );

    }
    cout << "----------- " << endl;
}




GtoDeformer::GtoDeformer() :
    m_errorFlag( false ),
    m_gtoCacheValid( false ),
    m_useCache( false ),
    m_silent( false )
{
}

GtoDeformer::~GtoDeformer()
{
    MMessage::removeCallback( m_callbackId );
}

void *GtoDeformer::creator()
{
    return new GtoDeformer;
}

MStatus GtoDeformer::initialize()
{
    MFnNumericAttribute numAttr;
    MFnTypedAttribute typedAttr;
    MFnMatrixAttribute matrixAttr;
    MStatus status;

    m_gtoBaseAttr = typedAttr.create( "gtoBase", "mbas",
                                      MFnData::kString, &status );

    CHECK_ERR( status, "ERROR creating gtoBase attribute" );

    m_gtoStartFrameAttr = numAttr.create( "gtoStartFrame", "msf",
                                          MFnNumericData::kShort, 1, &status );
    CHECK_ERR( status, "ERROR creating gtoStartFrame attribute" );

    m_gtoEndFrameAttr = numAttr.create( "gtoEndFrame", "mef",
                                        MFnNumericData::kShort, 1, &status );
    CHECK_ERR( status, "ERROR creating gtoEndFrame attribute" );

    m_timeAttr = numAttr.create( "time", "tme",
                                 MFnNumericData::kDouble, 0.0, &status );
    CHECK_ERR( status, "ERROR creating time attribute" );
    numAttr.setStorable( true );
    numAttr.setKeyable( true );

    m_accessoryMatrixAttr = matrixAttr.create( "accessoryMatrix", "am",
                                               MFnMatrixAttribute::kDouble,
                                               &status );
    CHECK_ERR( status, "ERROR creating accessoryMatrix attribute" );
    matrixAttr.setHidden( true );

    m_useCacheAttr = numAttr.create( "useCache", "uc", 
                                     MFnNumericData::kBoolean, false,
                                     &status );
    CHECK_ERR( status, "ERROR creating useCache attribute" );
    numAttr.setStorable( true );
    numAttr.setKeyable( false );
    
    m_silentAttr = numAttr.create( "silent", "bq", 
                                     MFnNumericData::kBoolean, false,
                                     &status );
    CHECK_ERR( status, "ERROR creating silent attribute" );
    numAttr.setStorable( true );
    numAttr.setKeyable( false );
    
    addAttribute( m_gtoBaseAttr );
    addAttribute( m_gtoStartFrameAttr );
    addAttribute( m_gtoEndFrameAttr );
    addAttribute( m_timeAttr );
    addAttribute( m_accessoryMatrixAttr );
    addAttribute( m_useCacheAttr );
    addAttribute( m_silentAttr );
 
    attributeAffects( m_gtoBaseAttr, outputGeom );
    attributeAffects( m_gtoStartFrameAttr, outputGeom );
    attributeAffects( m_gtoEndFrameAttr, outputGeom );
    attributeAffects( m_timeAttr, outputGeom );

    return MS::kSuccess;
}


void GtoDeformer::postConstructor()
{
    MStatus status;
    
    m_callbackId = MDGMessage::addTimeChangeCallback( timeChangeCallback, this, &status );

    if( status != MS::kSuccess )
    {
        MGlobal::displayWarning( "Failed to add timeChangeCallback" );
    }
}


MStatus GtoDeformer::accessoryNodeSetup( MDagModifier &cmd )
{
    MStatus result;

    MObject objLoc = cmd.createNode( MString("locator"),
                                     MObject::kNullObj,
                                     &result );

    cmd.renameNode( objLoc, MString( "GtoDeformerHandle" ) );

    if( MS::kSuccess == result )
    {
        MFnDependencyNode fnLoc( objLoc );
        MString attrName;
        attrName.set( "matrix" );
        MObject attrMat = fnLoc.attribute( attrName );
        result = cmd.connect( objLoc, attrMat, this->thisMObject(),
                              m_accessoryMatrixAttr );
    }
    return result;
}


MObject &GtoDeformer::accessoryAttribute() const
{
    return m_accessoryMatrixAttr;
}


MStatus GtoDeformer::compute( const MPlug &plug, MDataBlock &dataBlock )
{
    MDataHandle handle;
    MStatus status;

    if( m_errorFlag )
    {
    }

    if( plug.attribute() == outputGeom )
    {
        handle = dataBlock.inputValue( m_gtoBaseAttr, &status );
        CHECK_ERR( status, "ERROR getting m_gtoBaseAttr data handle\n" );
        m_gtoBase = handle.asString();

        handle = dataBlock.inputValue( m_gtoStartFrameAttr, &status );
        CHECK_ERR( status, "ERROR getting m_gtoStartFrameAttr data handle\n" );
        m_gtoStartFrame = handle.asInt();

        handle = dataBlock.inputValue( m_gtoEndFrameAttr, &status );
        CHECK_ERR( status, "ERROR getting m_gtoEndFrameAttr data handle\n" );
        m_gtoEndFrame = handle.asInt();

        handle = dataBlock.inputValue( m_timeAttr, &status );
        CHECK_ERR( status, "ERROR getting m_timeAttr data handle\n" );
        m_time = handle.asDouble();

        handle = dataBlock.inputValue( m_useCacheAttr, &status );
        CHECK_ERR( status, "ERROR getting m_useCacheAttr data handle\n" );
        m_useCache = handle.asBool();

        handle = dataBlock.inputValue( m_silentAttr, &status );
        CHECK_ERR( status, "ERROR getting m_silentAttr data handle\n" );
        m_silent = handle.asBool();

        if( ! m_gtoCacheValid )
        {
            int itime = int(m_time);
            char fname[512];
            snprintf( fname, 512, "%s.%04d.gto", m_gtoBase.asChar(), itime );

            m_reader.close();   // won't hurt to close it if not opened yet

            if (!m_reader.open(fname))
            {
                m_errorFlag = true;
                m_gtoCacheValid = false;
                if( ! m_silent )
                {
                    MGlobal::displayError( MString("Error reading cache GTO: ") + fname );
                }
                return MS::kFailure;
            }
            m_gtoCacheValid = true;
        }

        unsigned index = plug.logicalIndex();
        MObject thisNode = this->thisMObject();
        MPlug inPlug( thisNode, input );
        inPlug.selectAncestorLogicalIndex( index, input );
        MDataHandle hInput = dataBlock.inputValue( inPlug );

        MDataHandle hGeom = hInput.child( inputGeom );
        MDataHandle hGroup = hInput.child( groupId );
        unsigned int groupId = hGroup.asLong();
        MDataHandle hOutput = dataBlock.outputValue( plug );
        hOutput.copy( hGeom );

        MPlugArray plugs;
        plug.connectedTo( plugs, false, true );
        if( plugs.length() != 1 )
        {
            std::cerr << "Error, I am confused." << std::endl;
            return MS::kFailure;
        }
        MFnDependencyNode dn( plugs[0].node() );
        const GtoPointReader::ObjectMap& omap = m_reader.objectMap();
        GtoPointReader::ObjectMap::const_iterator i = omap.find(dn.name().asChar());

        if (i != omap.end())
        {
            const GtoPointReader::ObjectData& data = i->second;
            size_t width = data.dimensions;
            const float* M = data.globalMatrix.elements;

            //
            //  Only handle 3 or greater dimensional points
            //

            if (width > 2)
            {
                for (MItGeometry iter(hOutput, groupId, false);
                     !iter.isDone() && (iter.index() < data.numPoints);
                     iter.next())
                {
                    size_t pindex = iter.index() * width;
                    
                    const float px = data.points[pindex+0];
                    const float py = data.points[pindex+1];
                    const float pz = data.points[pindex+2];

#if 0
                    MPoint outPoint(px * M[0] + py * M[1] + pz * M[2] + M[3],
                                    px * M[4] + py * M[5] + pz * M[6] + M[7],
                                    pz * M[8] + py * M[9] + pz * M[10] + M[11]);
#endif

                    MPoint outPoint(px, py, pz);
                    iter.setPosition(outPoint);
                }
            }
        }
        else
        {
            if( ! m_silent )
            {
                MGlobal::displayWarning( "Nothing in gto cache for " + dn.name() );
            }
            return MS::kSuccess;
        }

        return MS::kSuccess;
    }

    return MPxDeformerNode::compute( plug, dataBlock );
}


void GtoDeformer::timeChangeCallback( MTime &time, void *clientData )
{
    GtoDeformer *thisNode = (GtoDeformer *)clientData;

    thisNode->m_errorFlag = false;
    thisNode->m_gtoCacheValid = false;
}


