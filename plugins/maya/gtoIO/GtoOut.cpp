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

#include <maya/MGlobal.h>
#include <maya/MMatrix.h>
#include <maya/MAnimUtil.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MPlug.h>
#include <maya/MBoundingBox.h>
#include <maya/MFnDagNode.h>
#include <maya/MDataHandle.h>
#include <maya/MItDag.h>
#include <maya/MSelectionList.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnMesh.h>
#include <maya/MFnCamera.h>
#include <maya/MFnLight.h>
#include <maya/MFnAmbientLight.h>
#include <maya/MFnAreaLight.h>
#include <maya/MFnDirectionalLight.h>
#include <maya/MFnPointLight.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MColor.h>
#include <maya/MFloatPointArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItSurfaceCV.h>
#include <maya/MItCurveCV.h>
#include <maya/MFileIO.h>
#include <maya/MComputation.h>
#include <maya/MFnAttribute.h>
#include <maya/M3dView.h>
#include <maya/MObjectHandle.h>
#include <maya/MItDependencyNodes.h>

#include <Gto/Protocols.h>

#include "TwkMaya.h"

#ifndef M_PI
#define M_PI 3.1415926535897931
#endif

#include <string.h>
#include <assert.h>
#include <algorithm>
#include "GtoOut.h"

namespace GtoIOPlugin {
using namespace std;

extern MString replaceFrameCookies( const MString &in, int frame );


// *****************************************************************************
GtoExporter::GtoExporter( MTime fs, 
                          MTime fe,
                          bool quiet,
                          double shutter,
                          bool subd,
                          bool normals,
                          bool exportST,
                          MString filename,
                          int maxRecurse,
                          bool normalize,
                          bool hidden,
                          bool verify,
                          bool isDifferenceFile,
                          bool diffPoints,
                          bool diffMatrix,
                          bool diffNormals,
                          bool allUserAttributes,
                          bool allMayaAttributes,
                          bool faceMaterials,
                          bool ascii,
                          bool exportTransformAttrs )
{
    m_fs = fs;
    m_fe = fe;
    m_quiet = quiet;
    m_shutterAngle = shutter;
    m_maxRecursion = maxRecurse;
    m_fileName = filename;
    m_asSubD = subd;
    m_normals = normals;
    m_exportST = exportST;
    m_normalize = normalize;
    m_hidden = hidden;
    m_verify = verify;
    m_isDifferenceFile = isDifferenceFile;
    m_diffPoints = diffPoints;
    m_diffMatrix = diffMatrix;
    m_diffNormals = diffNormals;
    m_allUserAttributes = allUserAttributes;
    m_allMayaAttributes = allMayaAttributes;
    
    m_faceMaterials = faceMaterials;
    m_ascii = ascii;
    m_exportTransformAttrs = exportTransformAttrs;

    // All of the transform attributes are handled as a special case
    m_noExportAttributes.insert( "translateX" );
    m_noExportAttributes.insert( "translateY" );
    m_noExportAttributes.insert( "translateZ" );
    
    m_noExportAttributes.insert( "rotateX" );
    m_noExportAttributes.insert( "rotateY" );
    m_noExportAttributes.insert( "rotateZ" );
    m_noExportAttributes.insert( "rotateOrder" );
    m_noExportAttributes.insert( "rotationInterpolation" );
    
    m_noExportAttributes.insert( "scaleX" );
    m_noExportAttributes.insert( "scaleY" );
    m_noExportAttributes.insert( "scaleZ" );
    
    m_noExportAttributes.insert( "shearXY" );
    m_noExportAttributes.insert( "shearXZ" );
    m_noExportAttributes.insert( "shearYZ" );

    m_noExportAttributes.insert( "rotatePivotX" );
    m_noExportAttributes.insert( "rotatePivotY" );
    m_noExportAttributes.insert( "rotatePivotZ" );

    m_noExportAttributes.insert( "rotatePivotTranslateX" );
    m_noExportAttributes.insert( "rotatePivotTranslateY" );
    m_noExportAttributes.insert( "rotatePivotTranslateZ" );

    m_noExportAttributes.insert( "scalePivotX" );
    m_noExportAttributes.insert( "scalePivotY" );
    m_noExportAttributes.insert( "scalePivotZ" );

    m_noExportAttributes.insert( "scalePivotTranslateX" );
    m_noExportAttributes.insert( "scalePivotTranslateY" );
    m_noExportAttributes.insert( "scalePivotTranslateZ" );

    m_noExportAttributes.insert( "rotateAxisX" );
    m_noExportAttributes.insert( "rotateAxisY" );
    m_noExportAttributes.insert( "rotateAxisZ" );

    m_noExportAttributes.insert( "transMinusRotatePivotX" );
    m_noExportAttributes.insert( "transMinusRotatePivotY" );
    m_noExportAttributes.insert( "transMinusRotatePivotZ" );
    //  -- End transform attributes
}


// *****************************************************************************
GtoExporter::~GtoExporter()
{
}


// *****************************************************************************
static float radToDeg( float rad )
{
    return rad * 57.2957795130823230; // == 180.0 / PI
}


// *****************************************************************************
static bool isVisible( MDagPath dp )
{
    // Check if given node OR any of its parents are hidden...
    do
    {
        MFnDependencyNode dn( dp.node() );
        MPlug iPlug = dn.findPlug( "visibility" );
        bool visible;
        iPlug.getValue( visible );
        if( ! visible )
        {
            return false;
        }
    } while( dp.pop() == MS::kSuccess );

    return true;
}


// *****************************************************************************
MStatus GtoExporter::doIt()
{
    MSelectionList list;

    MGlobal::getActiveSelectionList( list );
    if( list.length() == 0 )
    {
        MGlobal::displayError( "Nothing selected." );
        return MS::kFailure;
    }

    if( m_verify && ! verify() )
    {
        MGlobal::displayError( "gtoOut: Verify FAILED" );
        return MS::kFailure;
    }

    //
    // Loop over time
    //
    MComputation computation;
    computation.beginComputation();
    for( MTime t( m_fs ); t <= m_fe; t += 1.0 )
    {
        doFrame( t );

        if( m_shutterAngle != 0.0 )
        {
            MString fname = m_fileName;
            m_fileName += ".closed";
            doFrame( t + m_shutterAngle );
            m_fileName = fname;
        }
        if( computation.isInterruptRequested() )
        {
            break;
        }
    }
    computation.endComputation();

    return MS::kSuccess;
}

//******************************************************************************
string acceptableName( MDagPath &dp )
{
    MStatus status;
    MString name = dp.partialPathName( &status );

    if( status == MS::kFailure )
    {
        MGlobal::displayError( "FAILED to get partialPathName()" );
        return string( "***GTO: BAD NAME***" );
    }

    string n( name.asChar() );

    size_t p = n.rfind( '|' );
    if( p != string::npos )
    {
        n = n.substr( p + 1 );
    }

    return n;
}

//******************************************************************************
void GtoExporter::getNames( MDagPath &dp, int rlevel )
{
    if( rlevel != 0 )
    {
        std::string name = acceptableName( dp );
        
        m_allNames.push_back( name );
        MObject object = dp.node();

        if( object.hasFn( MFn::kTransform ) )
        {
            for( size_t i=0; i < dp.childCount(); i++ )
            {
                MObject child = dp.child( i );
                if( child.hasFn( MFn::kDagNode ) )
                {
                    MFnDagNode dn( child );
                    MDagPath childDp;
                    dn.getPath( childDp );
                    getNames( childDp, rlevel - 1 );
                }
            }
        }
    }
}


//******************************************************************************
bool GtoExporter::verify()
{
    MSelectionList list;
    MGlobal::getActiveSelectionList( list );
    MSelectionList sortedList;

    for( MItDag it( MItDag::kBreadthFirst ); !it.isDone(); it.next() )
    {
        MDagPath dp;
        it.getPath( dp );

        if( list.hasItem( dp ) )
        {
            MObject object = dp.node();
            if( object.hasFn( MFn::kDagNode ) )
            {
                MFnDagNode dn( object );
                MPlug iPlug = dn.findPlug( "intermediateObject" );
                bool intermediate;
                iPlug.getValue( intermediate );

                if( ! intermediate ) sortedList.add( dp );
            }
        }
    }

    if( sortedList.length() == 0 )
    {
        return false;
    }

    m_allNames.clear();

    for( size_t i=0; i < sortedList.length(); i++ )
    {
        MDagPath dp;
        sortedList.getDagPath( i, dp );
        getNames( dp, m_maxRecursion );
    }

    std::sort( m_allNames.begin(), m_allNames.end() );

    bool ok = true;

    for( size_t i=1; i < m_allNames.size(); i++ )
    {
        if( m_allNames[i] == m_allNames[i-1] )
        {
            MString err = "gtoOut: Bad/Duplicate name for \"";
            err += m_allNames[i].c_str();
            err += "\"\n";
            ok = false;
            if( ! m_quiet )
            {
                MGlobal::displayError( err );
            }
        }
    }

    m_allNames.clear();
    
    return ok;
}

//******************************************************************************
MStatus GtoExporter::doFrame( MTime t )
{
    MSelectionList list;
    MGlobal::getActiveSelectionList( list );
    MSelectionList sortedList;

    MAnimControl::setCurrentTime( t );

// Is this necessary to force an eval?  Sometimes?
//    MGlobal::viewFrame( t );   

//     M3dView currentView = M3dView::active3dView();
//     currentView.refresh( true, true, true );

    m_writer = new Gto::Writer;
    if( m_writer == NULL )
    {
        MGlobal::displayError( "Couldn't make writer instance." );
        return MStatus::kFailure;
    }

    if( m_fileName != "" )
    {
        MString fname = replaceFrameCookies( m_fileName, int( t.value() ) );

        m_writer->open( fname.asChar(), 
                    m_ascii ? Gto::Writer::TextGTO : Gto::Writer::CompressedGTO );
    }
    else
    {
        MGlobal::displayError( "No filename specified." );
        return MStatus::kFailure;
    }
    
    // Intern an orphan string to embed the gtoIO version 
    m_writer->intern( versionString() );

    // Intern an orphan string to embed the Maya scene filename
    MString exportedFromStr = "(Exported from " + MFileIO::currentFile() + ")";
    m_writer->intern( exportedFromStr.asChar() );

    for( MItDag it( MItDag::kBreadthFirst ); !it.isDone(); it.next() )
    {
        MDagPath dp;
        it.getPath( dp );

        if( list.hasItem( dp ) )
        {
            sortedList.add( dp );
        }
    }
    
    MStringArray tmp;
    list.getSelectionStrings( tmp );

    if( sortedList.length() == 0 )
    {
        return MS::kFailure;
    }

    //
    // Write object headers
    //    
    for( size_t i=0; i < sortedList.length(); i++ )
    {
        MDagPath dp;
        sortedList.getDagPath( i, dp );
        
        //
        //  If any of the parents are selected skip them.
        //
        output( dp, false, m_maxRecursion );
    }
    m_objects.clear();

    //
    // Write object data
    // 
    m_writer->beginData();
    for( size_t i=0; i < sortedList.length(); i++ )
    {
        MDagPath dp;
        sortedList.getDagPath( i, dp );
        //
        //  If any of the parents are selected skip them.
        //
        output( dp, true, m_maxRecursion );
    }
    m_objects.clear();
    m_writer->endData();

    // Close up shop...
    m_writer->close();
    delete m_writer;
    m_writer = 0;

    return MS::kSuccess;
}

//******************************************************************************
void GtoExporter::output( MDagPath &dp, bool data, int recursionLevel )
{
    if( recursionLevel == 0 )
    {
        return;
    }

    // Export hidden objects?
    if( ! ( isVisible( dp ) || m_hidden ) )
    {
        return;
    }

    MObject object = dp.node();

    if( find( m_objects.begin(), m_objects.end(), object ) != m_objects.end() )
    {
        //  Already wrote this one out
        return;
    }

    if( object.hasFn( MFn::kDependencyNode ) )
    {
        MFnDependencyNode dn( object );
        m_objectName = acceptableName( dp ).c_str();

        if( object.hasFn( MFn::kDagNode ) )
        {
            MFnDagNode dn( object );
            MPlug iPlug = dn.findPlug( "intermediateObject" );
            bool intermediate;
            iPlug.getValue( intermediate );
            if( intermediate )
            {
                return;
            }
        }
    }

    m_objects.push_back( object );

    if( ! data )
    {
        GtoMayaAttributes *attrs = new GtoMayaAttributes;
        m_attributes[dp.partialPathName().asChar()] = attrs;

        if( m_allUserAttributes || m_allMayaAttributes ) 
        {
            findAttributes( dp, attrs );
        }
        if( m_exportTransformAttrs && (object.apiType() == MFn::kTransform) )
        {
            findTransformAttributes( dp, attrs );
        }
        
        std::sort( attrs->begin(), attrs->end(), 
                   GtoMayaAttribute::sortByComponent );
    }
    

    if( object.hasFn( MFn::kTransform ) )
    {
        if( ! ( m_isDifferenceFile && ! m_diffMatrix ) )
        {
            if( data ) 
            {
                CSData( dp );
            }
            else
            {
                CSHeader( dp );
            }
        }

        for( size_t i=0; i < dp.childCount(); i++ )
        {
            MObject child = dp.child( i );
            if( child.hasFn( MFn::kDagNode ) )
            {
                MFnDagNode dn( child );
                MDagPath childDp;
                dn.getPath( childDp );

                output( childDp, data, recursionLevel - 1 );
            }
        }
        
    }
    else if( object.hasFn( MFn::kNurbsSurface ) )
    {
        if( data )
        {
            NURBSData( dp );
        }
        else
        {
            NURBSHeader( dp );
        }
    }
    else if( object.hasFn( MFn::kNurbsCurve ) )
    {
        if( data )
        {
            CurveData( dp );
        }
        else
        {
            CurveHeader( dp );
        }
    }
    else if( object.hasFn( MFn::kMesh ) )
    {
        if( m_asSubD || isSubd( dp ) )
        {
            if( data )
            {
                PolygonData( dp, GTO_PROTOCOL_CATMULL_CLARK );
            }
            else
            {
                PolygonHeader( dp, GTO_PROTOCOL_CATMULL_CLARK, 
                                   GTO_CATMULL_CLARK_VERSION );
            }
        }
        else
        {
            if( data )
            {
                PolygonData( dp, GTO_PROTOCOL_POLYGON );
            }
            else
            {
                PolygonHeader( dp, GTO_PROTOCOL_POLYGON, 
                                   GTO_POLYGON_VERSION );
            }
        }
    }
    else if( object.hasFn( MFn::kCamera ) )
    {
        if( data )
        {
            CameraData( dp );
        }
        else
        {
            CameraHeader( dp, GTO_CAMERA_VERSION );
        }
    }
    else if( object.hasFn( MFn::kLight ) )
    {
        if( data )
        {
            LightData( dp );
        }
        else
        {
            LightHeader( dp, GTO_LIGHT_VERSION );
        }
    }
    else if (!m_quiet)
    {
        MString er = "gtoOut: the object \"";
        er += m_objectName;
        er += "\" is not exportable to gto\n";
        MGlobal::displayWarning( er );
        return;
    }
}

//******************************************************************************
void GtoExporter::CSHeader( MDagPath &dp )
{
    string protocol = GTO_PROTOCOL_TRANSFORM;
    int protocolVersion = GTO_TRANSFORM_VERSION;
    if( hasUserProtocol( dp ) )
    {
        protocol = userProtocol( dp );
    }
    if( hasUserProtocolVersion( dp ) )
    {
        protocolVersion = userProtocolVersion( dp );
    }
    m_writer->beginObject( acceptableName( dp ).c_str(), 
                           protocol.c_str(), protocolVersion );
    TransformHeader( dp );
    m_writer->endObject();
}

//******************************************************************************
void GtoExporter::CSData( MDagPath &dp )
{
    TransformData( dp );
}

//******************************************************************************
void GtoExporter::TransformHeader( MDagPath &dp )
{
    MDagPathArray allPaths;
    MDagPath::getAllPathsTo( dp.node(), allPaths );
    
    int numInstances = allPaths.length();

    for( int i = 0; i < numInstances; ++i )
    {    
        MDagPath parent = allPaths[i];
        parent.pop();
        m_writer->intern( acceptableName( parent ).c_str() );
    }
    
    m_writer->beginComponent( GTO_COMPONENT_OBJECT );
    m_writer->property( GTO_PROPERTY_GLOBAL_MATRIX, Gto::Float, 
                        numInstances, 16 );
    
    if( m_hidden )
    {
        m_writer->property( "visibility", Gto::Byte, numInstances, 1 );
    }

    if( dp.apiType() != MFn::kTransform 
        && ! dp.hasFn( MFn::kCamera )
        && ! dp.hasFn( MFn::kLight ) )
    {
        m_writer->property( GTO_PROPERTY_BOUNDINGBOX, Gto::Float, 
                            numInstances, 6 );
    }

    if( ! m_isDifferenceFile )
    {
        m_writer->property( GTO_PROPERTY_PARENT, Gto::String, 
                            numInstances, 1 );
    }
    else
    {
        m_writer->intern( GTO_PROTOCOL_DIFFERENCE );
        m_writer->property( GTO_PROPERTY_PROTOCOL, Gto::String, 
                            numInstances, 1 );
    }

    m_writer->endComponent();

    attributesHeader( dp );

    if( ! m_isDifferenceFile )
    {
        animatedAttributesHeader( dp );
        TexChannelsHeader( dp );
    }
}

//******************************************************************************
void GtoExporter::TransformData( MDagPath &dp )
{
    MDagPathArray allPaths;
    MDagPath::getAllPathsTo( dp.node(), allPaths );
    int numInstances = allPaths.length();

    // Write globalMatrix for each instance, ref or difference
    float *outG = new float[16 * numInstances];
    int outGptr = 0;
    for( int i = 0; i < numInstances; ++i )
    {    
        MMatrix gm = allPaths[i].inclusiveMatrix();

        for( int row = 0; row < 4; row++ )
        {
            for( int col = 0; col < 4; col++ )
            {
                outG[outGptr++] = gm( col, row );
            }
        }

    }
    m_writer->propertyData( outG );
    delete [] outG;

    if( m_hidden )
    {
        char v = (char)isVisible( dp );
        m_writer->propertyData( &v );
    }
    
    if( dp.apiType() != MFn::kTransform 
        && ! dp.hasFn( MFn::kCamera )
        && ! dp.hasFn( MFn::kLight ) )
    {
        // Write boundingBox for each instance, ref or difference
        float *bboxes = new float[6 * numInstances];
        int outBBptr = 0;
        for( int i = 0; i < numInstances; ++i )
        {    
            MBoundingBox bbox = MFnDagNode( allPaths[i] ).boundingBox();
            MPoint min = bbox.min() * allPaths[i].inclusiveMatrix();
            MPoint max = bbox.max() * allPaths[i].inclusiveMatrix();

            bboxes[outBBptr++] = min[0];
            bboxes[outBBptr++] = min[1];
            bboxes[outBBptr++] = min[2];
            bboxes[outBBptr++] = max[0];
            bboxes[outBBptr++] = max[1];
            bboxes[outBBptr++] = max[2];
        }
        m_writer->propertyData( bboxes );
        delete [] bboxes;
    }


    int *parents = new int[numInstances];
    if( ! m_isDifferenceFile )
    {
        // Write parent for each instance, ref only
        for( int i = 0; i < numInstances; ++i )
        {    
            MDagPath parent = allPaths[i];
            parent.pop();
            parents[i] = m_writer->lookup( acceptableName( parent ).c_str() );
        }
    }
    else
    {
        // Write difference protocol property, difference only
        int n = m_writer->lookup( GTO_PROTOCOL_DIFFERENCE );
        for( int i = 0; i < numInstances; ++i )
        {
            parents[i] = n;
        }
    }
    m_writer->propertyData( parents );
    delete [] parents;

    attributesData( dp );

    if( ! m_isDifferenceFile )
    {
        animatedAttributesData( dp );
        TexChannelsData( dp );
    }
}

//******************************************************************************
void GtoExporter::NURBSHeader( MDagPath &dp )
{
    MFnNurbsSurface nurbs( dp.node() );

    int nu = nurbs.numCVsInU();
    int nv = nurbs.numCVsInV();
    
    int npoints = nu * nv;

    string protocol = GTO_PROTOCOL_NURBS;
    int protocolVersion = GTO_NURBS_VERSION;
    if( hasUserProtocol( dp ) )
    {
        protocol = userProtocol( dp );
    }
    if( hasUserProtocolVersion( dp ) )
    {
        protocolVersion = userProtocolVersion( dp );
    }

    m_writer->beginObject( m_objectName.asChar(), 
                           protocol.c_str(), protocolVersion );

    // Only bother with all this if we're not writing a difference file
    // that should ONLY have matrices in it...
    if( ! ( m_isDifferenceFile 
            && m_diffMatrix 
            && ! m_diffPoints ) )
    {
        MDoubleArray rawUknots, rawVknots;
        nurbs.getKnotsInU( rawUknots );
        nurbs.getKnotsInV( rawVknots );
        int uKnots = rawUknots.length() + 2; //spansU + 2 * degreeU - 1;
        int vKnots = rawVknots.length() + 2; //spansV + 2 * degreeV - 1;

        m_writer->beginComponent( GTO_COMPONENT_POINTS );
        m_writer->property( GTO_PROPERTY_POSITION, Gto::Float, npoints, 3 );
        
        // Only output points.weights property if this is NOT a difference file
        if( ! m_isDifferenceFile )
        {
            m_writer->property( GTO_PROPERTY_WEIGHT,  Gto::Float, npoints, 1 );
        }
        
        m_writer->endComponent();

        // Only output surface component if this is NOT a difference file
        if( ! m_isDifferenceFile )
        {
            m_writer->beginComponent( GTO_COMPONENT_SURFACE );
            m_writer->property( GTO_PROPERTY_DEGREE, Gto::Int, 2, 1 );
            m_writer->property( GTO_PROPERTY_UKNOTS, Gto::Float, uKnots, 1 );
            m_writer->property( GTO_PROPERTY_VKNOTS, Gto::Float, vKnots, 1 );
            m_writer->property( GTO_PROPERTY_URANGE, Gto::Float, 2, 1 );
            m_writer->property( GTO_PROPERTY_VRANGE, Gto::Float, 2, 1 );
            m_writer->property( GTO_PROPERTY_UFORM, Gto::Int, 1, 1 );
            m_writer->property( GTO_PROPERTY_VFORM, Gto::Int, 1, 1 );
            m_writer->endComponent();
        }
    }

    // Output the transform header, unless this is a difference file and
    // we didn't ask for difference matrices
    if( ! ( m_isDifferenceFile && ! m_diffMatrix ) )
    {
        TransformHeader( dp );
    }

    m_writer->endObject();
}

//******************************************************************************
void GtoExporter::NURBSData( MDagPath &dp )
{
    // Only bother with all this if we're not writing a difference file
    // that should ONLY have matrices in it...
    if( ! ( m_isDifferenceFile 
            && m_diffMatrix 
            && ! m_diffPoints ) )
    {
        MObject object = dp.node();
        MFnNurbsSurface nurbs( object );
        nurbs.updateSurface();

        int nu = nurbs.numCVsInU();
        int nv = nurbs.numCVsInV();

        if( nu < 2 || nv < 2 ) 
        {
            if( ! m_quiet )
            {
                MString error( "Ill-defined nurbs object: " );
                error += nurbs.name();
                MGlobal::displayError( error );
            }
            return;
        }

        vector<float> positions;
        vector<float> weights;
        MItSurfaceCV rawCVs(object, true); // v-major order for gto

        for (;!rawCVs.isDone(); rawCVs.nextRow()) 
        {
            for(;!rawCVs.isRowDone();rawCVs.next()) 
            {
                MPoint pt = rawCVs.position(MSpace::kObject);
                positions.push_back(float(pt.x));
                positions.push_back(float(pt.y));
                positions.push_back(float(pt.z));
                weights.push_back(float(pt.w));
            }
        }

        // output points.positions here
        m_writer->propertyDataInContainer( positions );

        // Only output points.weights property data and
        // surface component data if this is NOT a difference file
        if( ! m_isDifferenceFile )
        {
            // output points.weights here
            m_writer->propertyDataInContainer(weights);

            //----------------------------------------------------------------------

            int uorder = nurbs.degreeU();
            int vorder = nurbs.degreeV();
            int degrees[2] = { uorder, vorder };

            m_writer->propertyData(degrees);

            MDoubleArray rawUknots, rawVknots;
            nurbs.getKnotsInU(rawUknots); 
            nurbs.getKnotsInV(rawVknots); 

            vector<float> uknots(rawUknots.length()+2); // TODO: Don't hard-code degree 3
            vector<float> vknots(rawVknots.length()+2);

            //
            // Double up end knots.
            //

            for (size_t i=0; i < rawUknots.length(); i++)
            {
                uknots[i+1] = float(rawUknots[i]);
            }

            uknots[0] = uknots[1];
            uknots.back() = uknots[uknots.size()-2];

            // same for v

            for (size_t i=0; i < rawVknots.length(); i++)
            {
                vknots[i+1] = float(rawVknots[i]);
            }

            vknots[0] = vknots[1];
            vknots.back() = vknots[vknots.size()-2];

            //

            float uRange[2];
            float vRange[2];

            double uMin_d, uMax_d, vMin_d, vMax_d;
            nurbs.getKnotDomain(uMin_d, uMax_d, vMin_d, vMax_d);
            float umin = float(uMin_d);
            float umax = float(uMax_d);
            float vmin = float(vMin_d);
            float vmax = float(vMax_d);

            uRange[0] = umin;
            uRange[1] = umax;
            vRange[0] = vmin;
            vRange[1] = vmax;

            // Normalize knots, if requested...
            if( m_normalize )
            {
                if ( umax != umin )
                {
                    for ( std::vector<float>::iterator iter = uknots.begin();
                          iter != uknots.end(); ++iter )
                    {
                        (*iter) = ( (*iter) - umin ) / ( umax - umin );
                    }

                    uRange[0] = 0.0f;
                    uRange[1] = 1.0f;
                }

                if ( vmax != vmin )
                {
                    for ( std::vector<float>::iterator iter = vknots.begin();
                          iter != vknots.end(); ++iter )
                    {
                        (*iter) = ( (*iter) - vmin ) / ( vmax - vmin );
                    }

                    vRange[0] = 0.0f;
                    vRange[1] = 1.0f;
                }
            }

            // Write knots
            m_writer->propertyDataInContainer(uknots);
            m_writer->propertyDataInContainer(vknots);

            // Write range
            m_writer->propertyData(uRange);
            m_writer->propertyData(vRange);
            
            // Write form
            MFnNurbsSurface::Form formU = nurbs.formInU();
            MFnNurbsSurface::Form formV = nurbs.formInV();
            m_writer->propertyData( &formU );
            m_writer->propertyData( &formV );
            
        }  //  End if( ! m_isDifference...
    }

    // Output the transform header, unless this is a difference file and
    // we didn't ask for difference matrices
    if( ! ( m_isDifferenceFile && ! m_diffMatrix ) )
    {
        TransformData( dp );
    }
}

//******************************************************************************
void GtoExporter::PolygonHeader( MDagPath &dp, 
                                 const char *protocol, 
                                 unsigned int ver )
{
    MFnMesh mesh( dp.node() );
    int npoints   = mesh.numVertices();
    int nelements = mesh.numPolygons();
    int nnormals = mesh.numNormals();
    int nindices  = 0;
    bool interpBoundary = subdInterpBoundary( dp );

    for( MItMeshPolygon p( dp.node() ); ! p.isDone(); p.next() ) 
    {
        nindices += p.polygonVertexCount();
    }

    string prot = protocol;
    int protocolVersion = ver;
    if( hasUserProtocol( dp ) )
    {
        prot = userProtocol( dp );
    }
    if( hasUserProtocolVersion( dp ) )
    {
        protocolVersion = userProtocolVersion( dp );
    }
    m_writer->beginObject( m_objectName.asChar(), 
                           prot.c_str(), protocolVersion );

    // Only bother with all this if we're not writing a difference file
    // that should ONLY have matrices in it...
    if( ! ( m_isDifferenceFile 
            && m_diffMatrix 
            && ! m_diffPoints 
            && ! m_diffNormals ) )
    {
        int nuv = mesh.numUVs();
        
        // Write vertex positions unless this is a difference file
        // and we weren't asked to save them
        if( ! ( m_isDifferenceFile && ! m_diffPoints ) )
        {
            m_writer->beginComponent( GTO_COMPONENT_POINTS );
            m_writer->property( GTO_PROPERTY_POSITION, Gto::Float, npoints, 3 );
            m_writer->endComponent();
        }

        // Only write elements & indices components 
        // if this is NOT a difference file
        if( ! m_isDifferenceFile )
        {
            m_writer->beginComponent( GTO_COMPONENT_ELEMENTS);
            m_writer->property( GTO_PROPERTY_TYPE, Gto::Byte, nelements, 1 );
            m_writer->property( GTO_PROPERTY_SIZE, Gto::Short, nelements, 1 );
            if( m_faceMaterials )
            {
                m_writer->property( "materialID", Gto::Int, nelements, 1 );
            }
            m_writer->endComponent();

            //--

            m_writer->beginComponent( GTO_COMPONENT_INDICES );
            m_writer->property( GTO_PROPERTY_VERTEX, Gto::Int, nindices, 1 );

            if( nuv && m_exportST ) 
            {
                m_writer->property( GTO_PROPERTY_ST, Gto::Int, nindices, 1 );
            }

            if( nnormals && m_normals )
            {
                m_writer->property( GTO_PROPERTY_NORMAL, Gto::Int, 
                                    nindices, 1 );
            }

            // End component indices
            m_writer->endComponent(); 
        }  //  End if( ! m_isDifference...

        // Write normals if we're supposed to
        if( nnormals
            && ( ( ! m_isDifferenceFile && m_normals )
                 || ( m_isDifferenceFile && m_diffNormals ) ) )
        {
            m_writer->beginComponent( GTO_COMPONENT_NORMALS );
            m_writer->property( GTO_PROPERTY_NORMAL, Gto::Float, 
                                nnormals, 3 );
            m_writer->endComponent();
        }

        // Only write mappings and smoothing components 
        // if this is NOT a difference file
        if( ! m_isDifferenceFile )
        {
            if( nuv && m_exportST )
            {
                m_writer->beginComponent( GTO_COMPONENT_MAPPINGS );
                m_writer->property( GTO_PROPERTY_ST, Gto::Float, nuv, 2 );
                m_writer->endComponent();
            }

            //--

            m_writer->beginComponent( GTO_COMPONENT_SMOOTHING );
            m_writer->property( GTO_PROPERTY_METHOD, Gto::Int, 1, 1 );
            m_writer->endComponent();
            
            //--
            
            if( ( protocol == GTO_PROTOCOL_CATMULL_CLARK )
                  && ( ! interpBoundary ) )
            {
                m_writer->beginComponent( GTO_COMPONENT_SURFACE );
                m_writer->property( GTO_PROPERTY_INTERP_BOUNDARY, 
                                    Gto::Byte, 1, 1 );
                m_writer->endComponent();
            }
        }  //  End if( ! m_isDifference...
    }

    // Output the transform header, unless this is a difference file and
    // we didn't ask for difference matrices
    if( ! ( m_isDifferenceFile && ! m_diffMatrix ) )
    {
        TransformHeader( dp );
    }

    m_writer->endObject();
}


//******************************************************************************
void GtoExporter::PolygonData( MDagPath &dp, const char *protocol  )
{
    // Only bother with all this if we're not writing a difference file
    // that should ONLY have matrices in it...
    if( ! ( m_isDifferenceFile 
            && m_diffMatrix 
            && ! m_diffPoints 
            && ! m_diffNormals ) )
    {
        MFnMesh mesh( dp );
        
        mesh.updateSurface();
        mesh.syncObject();
        
        MFloatPointArray rawPoints;
        mesh.getPoints( rawPoints );
        bool interpBoundary = subdInterpBoundary( dp );

        int nuv = mesh.numUVs();
        int nnormals = mesh.numNormals();

        // Write vertex positions unless this is a difference file
        // and we weren't asked to save them
        if( ! ( m_isDifferenceFile && ! m_diffPoints ) )
        {
            float *vertices = new float[rawPoints.length() * 3];

            for( size_t i=0; i < rawPoints.length(); i++ )
            {
                vertices[i*3 + 0] = rawPoints[i].x;
                vertices[i*3 + 1] = rawPoints[i].y;
                vertices[i*3 + 2] = rawPoints[i].z;
            }

            // output points.position here
            m_writer->propertyData( vertices );
            delete [] vertices;
        }
        
        // Only write elements & indices components 
        // if this is NOT a difference file
        if( ! m_isDifferenceFile )
        {
            int npolys = mesh.numPolygons();

            unsigned short* polySizes = new unsigned short[npolys];
            unsigned char* polyTypes  = new unsigned char[npolys];

            int pindex = 0;
            int total = 0;

            for( MItMeshPolygon p( dp ); !p.isDone(); p.next() ) 
            {
                int nv = p.polygonVertexCount();
                total += nv;
                polySizes[pindex] = nv;
                polyTypes[pindex] = 0;
                pindex++;
            }

            // output elements.type here
            m_writer->propertyData( polyTypes );
            
            // output elements.size here
            m_writer->propertyData( polySizes );
            delete [] polySizes;
            delete [] polyTypes;

            int *indices = new int[total];
            int *stindices = nuv ? (new int[total]) : 0;
            int *normalindices = nnormals ? (new int[total]) : 0;

            pindex = 0;
            for( MItMeshPolygon p( dp ); ! p.isDone(); p.next() ) 
            {
                for( int q = 0, nv = p.polygonVertexCount(); q < nv; q++ )
                {
                    indices[pindex] = p.vertexIndex( q );

                    if( nuv && m_exportST )
                    {
                        int uvi;
                        p.getUVIndex( q, uvi );
                        stindices[pindex] = uvi;
                    }

                    if( nnormals && m_normals )
                    {
                        normalindices[pindex] = p.normalIndex( q );
                    }

                    pindex++;
                }
            }

            // output elements.materialID here
            if( m_faceMaterials )
            {
                MObjectArray shaders;
                MIntArray indices;

                MStatus status = mesh.getConnectedShaders( 0, shaders, indices );  STATUS;

                m_writer->propertyData( &(indices[0]) );
            }

            // output indices.vertex here
            m_writer->propertyData( indices );
            delete [] indices;

            // output indices.st here
            if( nuv && m_exportST )
            {
                m_writer->propertyData( stindices );
                delete [] stindices;
            }
            
            // output indices.normal here
            if( nnormals && m_normals )
            {
                m_writer->propertyData( normalindices );
                delete [] normalindices;
            }
        }  //  End if( ! m_isDifference...

        // Write normals if we're supposed to
        if( nnormals
            && ( ( ! m_isDifferenceFile && m_normals )
                 || ( m_isDifferenceFile && m_diffNormals ) ) )
        {
            MFloatVectorArray normArray;
            mesh.getNormals( normArray, MSpace::kObject );
            float *normals = new float[nnormals * 3];
            for( int i = 0, ni = 0; i < nnormals; ++i )
            {
                MFloatVector n = normArray[i];
                normals[ni] = n[0];
                ++ni;
                normals[ni] = n[1];
                ++ni;
                normals[ni] = n[2];
                ++ni;
            }

            // output normals.normal here
            m_writer->propertyData( normals );
            delete[] normals;
        }

        // Only write mappings and smoothing components 
        // if this is NOT a difference file
        if( ! m_isDifferenceFile )
        {
            // output mappings.st here
            if( nuv && m_exportST )
            {
                MFloatArray u,v;
                mesh.getUVs( u, v );
                float *sts = new float[u.length() * 2];

                for (size_t i=0; i < u.length(); i++)
                {
                    sts[i*2]   = u[i];
                    sts[i*2+1] = v[i];
                }

                m_writer->propertyData( sts );
                delete sts;
            }
            
            // output smoothing.smoothing
            int method = m_normals ? GTO_SMOOTHING_METHOD_PARTITIONED 
                                   : GTO_SMOOTHING_METHOD_NONE;
            m_writer->propertyData( &method );

            // output surface.interpolateBoundary
            if( ( protocol == GTO_PROTOCOL_CATMULL_CLARK )
                  && ( ! interpBoundary ) )
            {
                char interpBoundaryData = (char)interpBoundary;
                m_writer->propertyData( &interpBoundaryData );
            }
        }
    }

    // Output the transform, unless this is a difference file and
    // we didn't ask for difference matrices
    if( ! ( m_isDifferenceFile && ! m_diffMatrix ) )
    {
        TransformData( dp );
    }
}

//******************************************************************************
void GtoExporter::CameraHeader( MDagPath &dp, int protocolVersion )
{
    m_writer->intern( m_objectName.asChar() );

    m_writer->beginObject( m_objectName.asChar(), "camera", 1 );

    m_writer->beginComponent( "camera" );
    m_writer->property( "shutter", Gto::Float, 1, 1 );
    m_writer->property( "background", Gto::Float, 1, 3, "color" );
    m_writer->endComponent();

    m_writer->beginComponent( "frustum" );
    m_writer->property( "left", Gto::Float, 1, 1 );
    m_writer->property( "right", Gto::Float, 1, 1 );
    m_writer->property( "bottom", Gto::Float, 1, 1 );
    m_writer->property( "top", Gto::Float, 1, 1 );
    m_writer->property( "near", Gto::Float, 1, 1 );
    m_writer->property( "far", Gto::Float, 1, 1 );
    m_writer->property( "ortho", Gto::Int, 1, 1 );
    m_writer->endComponent();

    // Output the transform header, unless this is a difference file and
    // we didn't ask for difference matrices
    if( ! ( m_isDifferenceFile && ! m_diffMatrix ) )
    {
        TransformHeader( dp );
    }

    m_writer->endObject();
}

//******************************************************************************
void GtoExporter::CameraData( MDagPath &dp )
{
    MFnCamera camera( dp.node() );

    // Component "camera"
    float shutter = camera.shutterAngle() * 180.0 / M_PI;
    
    MFnDependencyNode cameraDn = dp.node();
    MPlug bgColorRPlug = cameraDn.findPlug ( "backgroundColorR" );
    MPlug bgColorGPlug = cameraDn.findPlug ( "backgroundColorG" );
    MPlug bgColorBPlug = cameraDn.findPlug ( "backgroundColorB" );
    float background[3];
    bgColorRPlug.getValue( background[0] );
    bgColorGPlug.getValue( background[1] );
    bgColorBPlug.getValue( background[2] );
    
    m_writer->propertyData( &shutter );
    m_writer->propertyData( background );

    // Component "frustum"
    float aspect = camera.aspectRatio();
    float near = camera.nearClippingPlane();
    float far = camera.farClippingPlane();

    int ortho = camera.isOrtho();

    CameraFrustum frustum( RIBCameraFOV( camera ), aspect, near, far );

    float left = frustum.left();
    float right = frustum.right();
    float bottom = frustum.bottom();
    float top = frustum.top();

    m_writer->propertyData( &left );
    m_writer->propertyData( &right );
    m_writer->propertyData( &bottom );
    m_writer->propertyData( &top );
    m_writer->propertyData( &near );
    m_writer->propertyData( &far );
    m_writer->propertyData( &ortho );

    // Output the transform, unless this is a difference file and
    // we didn't ask for difference matrices
    if( ! ( m_isDifferenceFile && ! m_diffMatrix ) )
    {
        TransformData( dp );
    }
}

//******************************************************************************
void GtoExporter::LightHeader( MDagPath &dp, int protocolVersion )
{
    MDagPath parent = dp;
    parent.pop();
    m_writer->intern( m_objectName.asChar() );

    m_writer->beginObject( m_objectName.asChar(), "light", protocolVersion );

    // Output the transform header, unless this is a difference file and
    // we didn't ask for difference matrices
    if( ! ( m_isDifferenceFile && ! m_diffMatrix ) )
    {
        TransformHeader( dp );
    }

    m_writer->endObject();
}

//******************************************************************************
void GtoExporter::LightData( MDagPath &dp )
{
    MFnLight light( dp.node() );
    MStatus status;

//     const char *shader = NULL;
//     if( dp.hasFn( MFn::kAmbientLight ) ) shader = "ambient_lgt";
//     else if( dp.hasFn( MFn::kAreaLight ) ) shader = "area_lgt";
//     else if( dp.hasFn( MFn::kDirectionalLight ) ) shader = "distant_lgt";
//     else if( dp.hasFn( MFn::kPointLight ) ) shader = "point_lgt";
//     else if( dp.hasFn( MFn::kSpotLight ) ) shader = "mayaspot_lgt";
// 
//     assert( shader );
//  
//     // Properties common to all light types:
//     int shaderId = m_writer->lookup( shader );
//     m_writer->propertyData( &shaderId );
// 
//     float intensity = light.intensity();
//     m_writer->propertyData( &intensity );
// 
//     float color[3];
//     light.color().get( color );
//     m_writer->propertyData( color );
// 
//     float Ldiff = 1.0;
//     if ( !light.lightDiffuse( ) ){
//         //this doesn't work for some reason    Ldiff = 0.0;
//     }
//     m_writer->propertyData( &Ldiff );
// 
//     float Lspec = 1.0;
//     if ( !light.lightSpecular( ) ){
//         //this doesn't work for some reason    Lspec = 0.0; 
//     }
//     m_writer->propertyData( &Lspec );
// 
//     // Type-specific properties:
//     if( dp.hasFn( MFn::kAmbientLight ) )
//     {
//         // None
//     }
//     else if( dp.hasFn( MFn::kAreaLight ) )
//     {
//         MFnAreaLight area( dp.node() );
// 
//         float falloff = (float)area.decayRate();
//         m_writer->propertyData( &falloff );
//     }
//     else if( dp.hasFn( MFn::kDirectionalLight ) )
//     {
//         // None
//     }
//     else if( dp.hasFn( MFn::kPointLight ) )
//     {
//         MFnPointLight point( dp.node() );
// 
//         float falloff = (float)point.decayRate();
//         m_writer->propertyData( &falloff );
//     }
//     else if( dp.hasFn( MFn::kSpotLight ) )
//     {
//         MFnSpotLight spot( dp.node() );
//         MFnSpotLight::MDecayRegion decayRegion = MFnSpotLight::kFirst;
//         MFnSpotLight::MBarnDoor    barnDoor    = MFnSpotLight::kRight;
//  
//         float falloff = (float)spot.decayRate();
//         m_writer->propertyData( &falloff );
// 
//         float coneangle = spot.coneAngle() * 180.0f / M_PI;
//         m_writer->propertyData( &coneangle );
// 
//         float conedeltaangle = spot.penumbraAngle() * 180.0f / M_PI;
//         m_writer->propertyData( &conedeltaangle );
// 
//         float beamdistribution = (float)spot.dropOff();
//         m_writer->propertyData( &beamdistribution );
// 
//         float usedecayregions = (float)spot.useDecayRegions( );
//         m_writer->propertyData( &usedecayregions );
//         
//         float decayreg1start = (float)spot.startDistance( decayRegion );
//         m_writer->propertyData( &decayreg1start );
// 
//         float decayreg1end = (float)spot.endDistance( decayRegion );
//         m_writer->propertyData( &decayreg1end );
//         
//         decayRegion = MFnSpotLight::kSecond;
//         float decayreg2start = (float)spot.startDistance( decayRegion );
//         m_writer->propertyData( &decayreg2start );
// 
//         float decayreg2end = (float)spot.endDistance( decayRegion );
//         m_writer->propertyData( &decayreg2end );
//         
//         decayRegion = MFnSpotLight::kThird;
//         float decayreg3start = (float)spot.startDistance( decayRegion );
//         m_writer->propertyData( &decayreg3start );
// 
//         float decayreg3end = (float)spot.endDistance( decayRegion );
//         m_writer->propertyData( &decayreg3end );
// 
//         float usebarndoors = (float)spot.barnDoors( );
//         m_writer->propertyData( &usebarndoors );
//        
//         barnDoor = MFnSpotLight::kRight; 
//         float barnanglepx = (float)spot.barnDoorAngle( barnDoor );
//         m_writer->propertyData( &barnanglepx );
//  
//         barnDoor = MFnSpotLight::kLeft; 
//         float barnanglenx = (float)spot.barnDoorAngle( barnDoor );
//         m_writer->propertyData( &barnanglenx );
//  
//         barnDoor = MFnSpotLight::kTop; 
//         float barnanglepy = (float)spot.barnDoorAngle( barnDoor );
//         m_writer->propertyData( &barnanglepy );
// 
//         barnDoor = MFnSpotLight::kBottom; 
//         float barnangleny = (float)spot.barnDoorAngle( barnDoor );
//         m_writer->propertyData( &barnangleny );
//  
//         float shadowcolor[3];
//         spot.shadowColor().get( shadowcolor );
//         m_writer->propertyData( shadowcolor );
// 
//         float usedepthmap = (float)spot.MFnNonExtendedLight::useDepthMapShadows( );
//         m_writer->propertyData( &usedepthmap );
// 
//         float shadowres = (float)spot.MFnNonExtendedLight::depthMapResolution( &status );
//         m_writer->propertyData( &shadowres );
// 
//         float shadowblur = (float)spot.MFnNonExtendedLight::depthMapFilterSize( &status );
//         m_writer->propertyData( &shadowblur );
//         
//         float shadowbias = (float)spot.MFnNonExtendedLight::depthMapBias( &status );
//         m_writer->propertyData( &shadowbias );
// 
//         float useRayTrace = (float)spot.useRayTraceShadows( );
//         m_writer->propertyData( &useRayTrace );
// 
//         float shadowradius = (float)spot.MFnNonExtendedLight::shadowRadius( );
//         m_writer->propertyData( &shadowradius );
// 
//         float shadowsamples = (float)spot.numShadowSamples( );
//         m_writer->propertyData( &shadowsamples );
//     }


    // Output the transform, unless this is a difference file and
    // we didn't ask for difference matrices
    if( ! ( m_isDifferenceFile && ! m_diffMatrix ) )
    {
        TransformData( dp );
    }
}

// *****************************************************************************
bool GtoExporter::hasUserProtocol( MDagPath &dp )
{
    MFnDependencyNode dn( dp.node() );
    MStatus status;
    
    dn.attribute( MString( GTO_MAYA_USER_PROTOCOL_ATTRIBUTE ), &status );

    return status == MS::kSuccess;
}

// *****************************************************************************
string GtoExporter::userProtocol( MDagPath &dp )
{
    MFnDependencyNode dn( dp.node() );

    MObject attrObj;
    attrObj = dn.attribute( MString( GTO_MAYA_USER_PROTOCOL_ATTRIBUTE ) );

    MPlug attrPlug( dp.node(), attrObj );
    
    MString attrValue;
    attrPlug.getValue( attrValue );

    return string( attrValue.asChar() );
}

// *****************************************************************************
bool GtoExporter::hasUserProtocolVersion( MDagPath &dp )
{
    MFnDependencyNode dn( dp.node() );
    MStatus status;
    
    dn.attribute( MString( GTO_MAYA_USER_PROTOCOL_VERSION_ATTRIBUTE ), &status );

    return status == MS::kSuccess;
}

// *****************************************************************************
int GtoExporter::userProtocolVersion( MDagPath &dp )
{
    MFnDependencyNode dn( dp.node() );

    MObject attrObj;
    attrObj = dn.attribute( 
                    MString( GTO_MAYA_USER_PROTOCOL_VERSION_ATTRIBUTE ) );

    MPlug attrPlug( dp.node(), attrObj );
    
    int attrValue;
    attrPlug.getValue( attrValue );

    return attrValue;
}


// *****************************************************************************
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
void GtoExporter::CurveHeader( MDagPath &dp )
{
    MFnNurbsCurve curve( dp.node() );

    int npoints = curve.numCVs();

    string protocol = "NURBSCurve";
    int protocolVersion = 1;
    if( hasUserProtocol( dp ) )
    {
        protocol = userProtocol( dp );
    }
    if( hasUserProtocolVersion( dp ) )
    {
        protocolVersion = userProtocolVersion( dp );
    }

    m_writer->beginObject( m_objectName.asChar(), 
                           protocol.c_str(), protocolVersion );

    // Only bother with all this if we're not writing a difference file
    // that should ONLY have matrices in it...
    if( ! ( m_isDifferenceFile 
            && m_diffMatrix 
            && ! m_diffPoints ) )
    {
        MDoubleArray rawVknots;
        curve.getKnots( rawVknots );

        int vKnots = rawVknots.length() + 2; //spansU + 2 * degreeU - 1;

        m_writer->beginComponent( GTO_COMPONENT_POINTS );
        m_writer->property( GTO_PROPERTY_POSITION, Gto::Float, npoints, 3 );
        
        // Only output points.weights property if this is NOT a difference file
        if( ! m_isDifferenceFile )
        {
            m_writer->property( GTO_PROPERTY_WEIGHT,  Gto::Float, npoints, 1 );
        }
        
        m_writer->endComponent();

        // Only output surface component if this is NOT a difference file
        if( ! m_isDifferenceFile )
        {
            m_writer->beginComponent( GTO_COMPONENT_SURFACE );
            m_writer->property( GTO_PROPERTY_DEGREE, Gto::Int, 1, 1 );
            m_writer->property( GTO_PROPERTY_VKNOTS, Gto::Float, vKnots, 1 );
            m_writer->property( GTO_PROPERTY_VRANGE, Gto::Float, 2, 1 );
            m_writer->property( GTO_PROPERTY_VFORM, Gto::Int, 1, 1 );
            m_writer->endComponent();
        }
    }

    // Output the transform header, unless this is a difference file and
    // we didn't ask for difference matrices
    if( ! ( m_isDifferenceFile && ! m_diffMatrix ) )
    {
        TransformHeader( dp );
    }

    m_writer->endObject();
}


// *****************************************************************************
void GtoExporter::CurveData( MDagPath &dp )
{
    // Only bother with all this if we're not writing a difference file
    // that should ONLY have matrices in it...
    if( ! ( m_isDifferenceFile 
            && m_diffMatrix 
            && ! m_diffPoints ) )
    {
        MObject object = dp.node();
        MFnNurbsCurve curve( dp.node() );

        int npoints = curve.numCVs();

        if( npoints < 2 ) 
        {
            if( ! m_quiet )
            {
                MString error( "Ill-defined curve object: " );
                error += curve  .name();
                MGlobal::displayError( error );
            }
            return;
        }

        vector<float> positions;
        vector<float> weights;
        MItCurveCV rawCVs( object );

        for (;!rawCVs.isDone(); rawCVs.next()) 
        {
            MPoint pt = rawCVs.position(MSpace::kObject);
            positions.push_back(float(pt.x));
            positions.push_back(float(pt.y));
            positions.push_back(float(pt.z));
            weights.push_back(float(pt.w));
        }

        // output points.positions here
        m_writer->propertyDataInContainer(positions);

        // Only output points.weights property data and
        // surface component data if this is NOT a difference file
        if( ! m_isDifferenceFile )
        {
            // output points.weights here
            m_writer->propertyDataInContainer(weights);

            //----------------------------------------------------------------------

            int vDegree = curve.degree();
            m_writer->propertyData(&vDegree);

            MDoubleArray rawVknots;
            curve.getKnots(rawVknots); 

            // TODO: Don't hard-code degree 3
            vector<float> vknots(rawVknots.length()+2);

            //
            // Double up end knots.
            //
            for (size_t i=0; i < rawVknots.length(); i++)
            {
                vknots[i+1] = float(rawVknots[i]);
            }
            vknots[0] = vknots[1];
            vknots.back() = vknots[vknots.size()-2];

            //

            float vRange[2];

            double vMin_d, vMax_d;
            curve.getKnotDomain(vMin_d, vMax_d);
            float vmin = float(vMin_d);
            float vmax = float(vMax_d);
            vRange[0] = vmin;
            vRange[1] = vmax;

            // Normalize knots, if requested...
            if( m_normalize )
            {
                if ( vmax != vmin )
                {
                    for ( std::vector<float>::iterator iter = vknots.begin();
                          iter != vknots.end(); ++iter )
                    {
                        (*iter) = ( (*iter) - vmin ) / ( vmax - vmin );
                    }

                    vRange[0] = 0.0f;
                    vRange[1] = 1.0f;
                }
            }

            // Write knots
            m_writer->propertyDataInContainer(vknots);

            // Write range
            m_writer->propertyData(vRange);
            
            // Write form
            MFnNurbsCurve::Form formV = curve.form();
            m_writer->propertyData( &formV );
            
        }  //  End if( ! m_isDifference...
    }

    // Output the transform header, unless this is a difference file and
    // we didn't ask for difference matrices
    if( ! ( m_isDifferenceFile && ! m_diffMatrix ) )
    {
        TransformData( dp );
    }
}

// *****************************************************************************
void GtoExporter::TexChannelsHeader( MDagPath &dp )
{
    MStatus status;
    MFnDependencyNode dn( dp.node() );

    MString prefix( "texChan_" );
    int prefixLen = prefix.length();
    
    std::vector<std::string> texChannels;

    for( int i = 0; i < dn.attributeCount(); ++i )
    {
        MObject attrObj = dn.attribute( i );
        MFnAttribute attr( attrObj );
        MString longName = attr.name();
        if( longName.substring( 0, prefixLen-1 ) == prefix )
        {
            MPlug channelPlug = dn.findPlug( attrObj, &status );
            CHECK_MSTATUS( status );

            MPlug mappingTypeElement = channelPlug.elementByPhysicalIndex( 0, &status  );
            CHECK_MSTATUS( status );
            MPlug filenameElement = channelPlug.elementByPhysicalIndex( 1, &status  );
            CHECK_MSTATUS( status );

            MString mappingType;
            mappingTypeElement.getValue( mappingType );

            MString filename;
            filenameElement.getValue( filename );

            m_writer->intern( mappingType.asChar() );
            m_writer->intern( filename.asChar() );

            texChannels.push_back( attr.shortName().asChar() );
        }
    }

    if( texChannels.size() > 0 )
    {
        m_writer->beginComponent( GTO_COMPONENT_CHANNELS );
        for( int i = 0; i < texChannels.size(); ++i )
        {
            m_writer->property( texChannels[i].c_str(), Gto::String, 2, 1 );
        }
        m_writer->endComponent();
    }
}


// *****************************************************************************
void GtoExporter::TexChannelsData( MDagPath &dp )
{
    MStatus status;
    MFnDependencyNode dn( dp.node() );

    MString prefix( "texChan_" );
    int prefixLen = prefix.length();

    for( int i = 0; i < dn.attributeCount(); ++i )
    {
        MObject attrObj = dn.attribute( i );
        MFnAttribute attr( attrObj );
        MString longName = attr.name();
        if( longName.substring( 0, prefixLen-1 ) == prefix )
        {
            MPlug channelPlug = dn.findPlug( attrObj, &status );
            CHECK_MSTATUS( status );

            MPlug mappingTypeElement = channelPlug.elementByPhysicalIndex( 0, &status  );
            CHECK_MSTATUS( status );
            MPlug filenameElement = channelPlug.elementByPhysicalIndex( 1, &status  );
            CHECK_MSTATUS( status );

            MString mappingType;
            mappingTypeElement.getValue( mappingType );
            
            MString filename;
            filenameElement.getValue( filename );

            int assignment[2];
            assignment[0] = m_writer->lookup( mappingType.asChar() );
            assignment[1] = m_writer->lookup( filename.asChar() );
            m_writer->propertyData( assignment );
        }
    }
}

// *****************************************************************************
bool GtoExporter::subdInterpBoundary( MDagPath &dp )
{
    MFnDependencyNode dn( dp.node() );

    MPlug attrPlug = dn.findPlug( GTO_MAYA_INTERP_BOUNDARY_ATTRIBUTE );
    if( attrPlug.isNull() )
    {
        return true; // (default is to interpolate boundaries)
    }

    bool attrValue;
    attrPlug.getValue( attrValue );

    return attrValue;
}

// *****************************************************************************
bool GtoExporter::isSubd( MDagPath &dp )
{
    MFnDependencyNode dn( dp.node() );

    MPlug attrPlug = dn.findPlug( GTO_MAYA_SUBD_ATTRIBUTE );
    if( attrPlug.isNull() )
    {
        return false; // (default is to regular polys)
    }

    bool attrValue;
    attrPlug.getValue( attrValue );

    return attrValue;
}


// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

void GtoExporter::findAttributes( MDagPath &dp, GtoMayaAttributes *attrs )
{
// #define CONT(x) { std::cerr << plug.info() << ": " << x << std::endl; continue; }
#define CONT(x) { continue; }

    MStatus status;

    MFnDependencyNode dn( dp.node(), &status );    STATUS;

    for( int i = 0; i < dn.attributeCount(); ++i )
    {
        MObject attrObj = dn.attribute( i );
        MFnAttribute attr( attrObj ); 

        MPlug plug( dp.node(), attrObj );
        
        if( ! GtoMayaAttribute::canExport( plug ) ) CONT( "! canExport()" );
        if( m_noExportAttributes.count( attr.name().asChar() ) > 0 ) CONT( "noExportAttributes" );

        if( ! attr.isDynamic() && ! m_allMayaAttributes )
        {
            continue;
        }

        MObjectHandle objHandle( dp.node() );
        MObjectHandle attrHandle( attrObj );
        
        GtoMayaAttribute* gma = new GtoMayaAttribute( objHandle, attrHandle );
        attrs->push_back( gma );
    }
}


// *****************************************************************************
void GtoExporter::getAttribute( MDagPath& dp, const char* attrName, 
                                GtoMayaAttributes* attrs,
                                const char *gtoComponentName )
{
    MStatus status;
    MFnDependencyNode dn( dp.node(), &status );   STATUS;
    if( ! status ) return;

    for( int i = 0; i < dn.attributeCount(); ++i )
    {
        MObject attrObj = dn.attribute( i );
        MFnAttribute attr( attrObj ); 
        if( attr.name() == attrName )
        {
            MObjectHandle objHandle( dp.node() );
            MObjectHandle attrHandle( attrObj );

            GtoMayaAttribute* gma = new GtoMayaAttribute( objHandle, attrHandle, 
                                                          gtoComponentName );
            attrs->push_back( gma );
            break;
        }
    }
}


// *****************************************************************************
void GtoExporter::findTransformAttributes( MDagPath &dp, 
                                           GtoMayaAttributes *attrs )
{
    if( GetVectorAttr( dp.node(), "translate" ) != MVector( 0, 0, 0 ) )
    {
        getAttribute( dp, "translateX", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "translateY", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "translateZ", attrs, GTO_TRANSFORM_COMPONENT_NAME );
    }
    
    if( GetVectorAttr( dp.node(), "rotate" ) != MVector( 0, 0, 0 ) )
    {
        getAttribute( dp, "rotateX", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "rotateY", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "rotateZ", attrs, GTO_TRANSFORM_COMPONENT_NAME );
    }
    if( GetEnumAttr( dp.node(), "rotateOrder" ) != 0 )
    {
        getAttribute( dp, "rotateOrder", attrs, GTO_TRANSFORM_COMPONENT_NAME );
    }
    if( GetEnumAttr( dp.node(), "rotationInterpolation" ) != 1 )
    {
        getAttribute( dp, "rotationInterpolation", attrs, GTO_TRANSFORM_COMPONENT_NAME );
    }
    
    if( GetVectorAttr( dp.node(), "scale" ) != MVector( 1, 1, 1 ) )
    {
        getAttribute( dp, "scaleX", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "scaleY", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "scaleZ", attrs, GTO_TRANSFORM_COMPONENT_NAME );
    }
    
    if( GetVectorAttr( dp.node(), "shear" ) != MVector( 0, 0, 0 ) )
    {
        getAttribute( dp, "shearXY", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "shearXZ", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "shearYZ", attrs, GTO_TRANSFORM_COMPONENT_NAME );
    }

    if( GetVectorAttr( dp.node(), "rotatePivot" ) != MVector( 0, 0, 0 ) )
    {
        getAttribute( dp, "rotatePivotX", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "rotatePivotY", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "rotatePivotZ", attrs, GTO_TRANSFORM_COMPONENT_NAME );
    }

    if( GetVectorAttr( dp.node(), "rotatePivotTranslate" ) != MVector( 0, 0, 0 ) )
    {
        getAttribute( dp, "rotatePivotTranslateX", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "rotatePivotTranslateY", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "rotatePivotTranslateZ", attrs, GTO_TRANSFORM_COMPONENT_NAME );
    }

    if( GetVectorAttr( dp.node(), "scalePivot" ) != MVector( 0, 0, 0 ) )
    {
        getAttribute( dp, "scalePivotX", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "scalePivotY", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "scalePivotZ", attrs, GTO_TRANSFORM_COMPONENT_NAME );
    }

    if( GetVectorAttr( dp.node(), "scalePivotTranslate" ) != MVector( 0, 0, 0 ) )
    {
        getAttribute( dp, "scalePivotTranslateX", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "scalePivotTranslateY", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "scalePivotTranslateZ", attrs, GTO_TRANSFORM_COMPONENT_NAME );
    }

    if( GetVectorAttr( dp.node(), "rotateAxis" ) != MVector( 0, 0, 0 ) )
    {
        getAttribute( dp, "rotateAxisX", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "rotateAxisY", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "rotateAxisZ", attrs, GTO_TRANSFORM_COMPONENT_NAME );
    }
    if( GetVectorAttr( dp.node(), "transMinusRotatePivot" ) != MVector( 0, 0, 0 ) )
    {
        getAttribute( dp, "transMinusRotatePivotX", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "transMinusRotatePivotX", attrs, GTO_TRANSFORM_COMPONENT_NAME );
        getAttribute( dp, "transMinusRotatePivotX", attrs, GTO_TRANSFORM_COMPONENT_NAME );
    }
}


// *****************************************************************************
void GtoExporter::attributesHeader( MDagPath &dp ) const
{
    MStatus status;

    MFnDependencyNode dn( dp.node(), &status );    STATUS;
    GtoMayaAttributes* myAttrs = m_attributes.find( dp.partialPathName().asChar() )->second;

    if( (! myAttrs) || (myAttrs->size() == 0) ) return;

    std::string component = "";
    for( int i = 0; i < myAttrs->size(); ++i )
    {
        const GtoMayaAttribute *attr = (*myAttrs)[i];
        if( component != attr->componentName() )
        {
            if( component != "" ) m_writer->endComponent();
            component = attr->componentName();
            m_writer->beginComponent( component.c_str() );
        }
        attr->writeGtoHeader( m_writer );
    }
    if( component != "" ) m_writer->endComponent();
}


// *****************************************************************************
void GtoExporter::attributesData( MDagPath &dp ) const
{
    MStatus status;

    MFnDependencyNode dn( dp.node(), &status );    STATUS;
    GtoMayaAttributes* myAttrs = m_attributes.find( dp.partialPathName().asChar() )->second;

    if( (! myAttrs) || (myAttrs->size() == 0) ) return;

    for( int i = 0; i < myAttrs->size(); ++i )
    {
        const GtoMayaAttribute *attr = (*myAttrs)[i];
        attr->writeGtoData( m_writer );
    }
}


// *****************************************************************************
void GtoExporter::animatedAttributesHeader( MDagPath &dp ) const
{
    MStatus status;

    MFnDependencyNode dn( dp.node(), &status );    STATUS;
    GtoMayaAttributes* myAttrs = m_attributes.find( dp.partialPathName().asChar() )->second;

    if( (! myAttrs) || (myAttrs->size() == 0) ) return;

    GtoMayaAttributes animatedAttrs;

    for( int i = 0; i < myAttrs->size(); ++i )
    {
        GtoMayaAttribute *attr = (*myAttrs)[i];
        if( attr->isAnimated() ) animatedAttrs.push_back( attr );
    }
    
    if( ! animatedAttrs.empty() )
    {
        m_writer->beginComponent( ":curves" );
        for( int i = 0; i < animatedAttrs.size(); ++i )
        {
            animatedAttrs[i]->animCurve()->writeGtoHeader( (*m_writer) );
        }
        m_writer->endComponent();
    }
}


// *****************************************************************************
void GtoExporter::animatedAttributesData( MDagPath &dp ) const
{
    MStatus status;

    MFnDependencyNode dn( dp.node(), &status );    STATUS;
    GtoMayaAttributes* myAttrs = m_attributes.find( dp.partialPathName().asChar() )->second;

    if( (! myAttrs) || (myAttrs->size() == 0) ) return;

    GtoMayaAttributes animatedAttrs;

    for( int i = 0; i < myAttrs->size(); ++i )
    {
        GtoMayaAttribute *attr = (*myAttrs)[i];
        if( attr->isAnimated() ) attr->animCurve()->writeGtoData( (*m_writer) );
    }
}


} // End namespace GtoIOPlugin

