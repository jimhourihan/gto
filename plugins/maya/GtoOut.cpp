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
#include <maya/MFloatPointArray.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItSurfaceCV.h>
#include <Gto/Protocols.h>

#include <string.h>
#include <algorithm>
#include "GtoOut.h"

namespace GtoIOPlugin {
using namespace std;

// *****************************************************************************
GtoExporter::GtoExporter( MTime fs, 
                          MTime fe,
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
                          bool diffNormals )
{
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
    
    m_fs = fs;
    m_fe = fe;
    m_shutterAngle = shutter;
}

// *****************************************************************************
GtoExporter::~GtoExporter()
{
}

// *****************************************************************************
static bool isVisible( MDagPath &dp )
{
    MFnDependencyNode dn( dp.node() );
    MPlug iPlug = dn.findPlug( "visibility" );
    bool visibility;
    iPlug.getValue( visibility );
    return visibility;
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
    }

    return MS::kSuccess;
}

//******************************************************************************
MString replaceFrameCookies( const MString &in, int frame )
{
    char fn[40];
    sprintf( fn, "%04d", frame );
    MStringArray array;
    in.split( '#', array );

    MString out = array[0];

    for( size_t i=1; i < array.length(); i++ )
    {
        out += fn;
        out += array[i];
    }

    return out;
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
            sortedList.add( dp );
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

            MGlobal::displayError( err );
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
    MGlobal::viewFrame( t );

    m_writer = new Gto::Writer;
    if( m_writer == NULL )
    {
        MGlobal::displayError( "Couldn't make writer instance." );
        return MStatus::kFailure;
    }

    if( m_fileName != "" )
    {
        MString fname = replaceFrameCookies( m_fileName, int( t.value() ) );
        m_writer->open( fname.asChar() );
    }
    else
    {
        MGlobal::displayError( "No filename specified." );
        return MStatus::kFailure;
    }
    
    // Intern an orphan string to embed the gtoIO version 
    m_writer->intern( versionString() );

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
    else if( object.hasFn( MFn::kMesh ) )
    {
        if( m_asSubD )
        {
            if( data )
            {
                PolygonData( dp );
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
                PolygonData( dp );
            }
            else
            {
                PolygonHeader( dp, GTO_PROTOCOL_POLYGON, 
                                   GTO_POLYGON_VERSION );
            }
        }
    }
    else
    {
        MString er = "gtoOut: the object \"";
        er += m_objectName;
        er += "\" is not exportable to gto\n";
        MGlobal::displayWarning( er );
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

    if( dp.apiType() != MFn::kTransform )
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
    
    if( dp.apiType() != MFn::kTransform )
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
}

//******************************************************************************
void GtoExporter::NURBSHeader( MDagPath &dp )
{
    MFnNurbsSurface nurbs( dp.node() );

    int nu = nurbs.numCVsInU();
    int nv = nurbs.numCVsInV();
    
    int npoints = nu * nv;

//     int degreeU = nurbs.degreeU();
//     int degreeV = nurbs.degreeV();
    
//     int spansU = nu - degreeU;
//     int spansV = nv - degreeV;

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

        int nu = nurbs.numCVsInU();
        int nv = nurbs.numCVsInV();

        if( nu < 2 || nv < 2 ) 
        {
            MString error( "Ill-defined nurbs object: " );
            error += nurbs.name();
            MGlobal::displayError( error );
            return;
        }

        vector<float> positions;
        vector<float> weights;
        MItSurfaceCV rawcvs(object, true); // v-major order for gto

        for (;!rawcvs.isDone(); rawcvs.nextRow()) 
        {
            for(;!rawcvs.isRowDone();rawcvs.next()) 
            {
                MPoint pt = rawcvs.position(MSpace::kObject);
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
            m_writer->property( "method", Gto::Int, 1, 1 );
            m_writer->endComponent();
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
void GtoExporter::PolygonData( MDagPath &dp )
{
    // Only bother with all this if we're not writing a difference file
    // that should ONLY have matrices in it...
    if( ! ( m_isDifferenceFile 
            && m_diffMatrix 
            && ! m_diffPoints 
            && ! m_diffNormals ) )
    {
        MObject object = dp.node();
        MFnMesh mesh( object );
        MFloatPointArray rawPoints;
        mesh.getPoints( rawPoints );

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

            for( MItMeshPolygon p( object ); !p.isDone(); p.next() ) 
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
            for( MItMeshPolygon p( object ); ! p.isDone(); p.next() ) 
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
            }

            // output smoothing.smoothing
            int method = m_normals ? GTO_SMOOTHING_METHOD_PARTITIONED 
                                   : GTO_SMOOTHING_METHOD_NONE;
            m_writer->propertyData( &method );
        }
    }

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

} // End namespace GtoIOPlugin

