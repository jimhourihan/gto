//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#include <iostream>
#include <sstream>
#include <maya/MAnimControl.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MVectorArray.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MIntArray.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnDagNode.h>
#include <maya/MComputation.h>
#include <maya/MFileIO.h>

#include <TwkUtil/FrameUtils.h>

#include <Gto/Protocols.h>
#include <Gto/Writer.h>

#include "GtoParticleExport.h"

using namespace std;
using namespace TwkUtil;

// *****************************************************************************
#define CHECK_STATUS if( status != MS::kSuccess )               \
    {                                                           \
        std::cerr << __FILE__ << ", line " << __LINE__ << ": "   \
                 << status.errorString().asChar() << std::endl;  \
        return status;                                          \
    }                                                           \

// *****************************************************************************
#define CHECK_STATUS_MSG( msg ) if( status != MS::kSuccess )    \
    {                                                           \
        std::cerr << __FILE__ << ", line " << __LINE__ << ": "   \
                 << msg << std::endl;  \
        return status;                                          \
    }                                                           \

//******************************************************************************
#define GET_NON_DUP_VAR_STRS( FLAG, STRING_LIST )               \
while ( 1 )                                                     \
{                                                               \
    if ( i >= argLen - 1 ) { break; }                           \
    ++i;                                                        \
    arg = args.asString( i, &status );                          \
    if ( !status )                                              \
    {                                                           \
        MString err( "Invalid " ); err += FLAG; err += " flag"; \
        displayError( err );                                    \
        return MS::kFailure;                                    \
    }                                                           \
    if ( arg.index( '-' ) == 0 ) { --i; break; }                \
    int dupLen = STRING_LIST.length();                          \
    int j;                                                      \
    for ( j = 0; j < dupLen; ++j )                              \
    {                                                           \
        if ( arg == STRING_LIST[ j ] ) { break; }               \
    }                                                           \
    if ( j == dupLen ) { STRING_LIST.append( arg ); }           \
}


//******************************************************************************
#define GET_INT_FROM_ARG( INTO )                                        \
{                                                                       \
    ++i;                                                                \
    INTO = args.asInt( i, &status );                                    \
    if ( !status )                                                      \
    {                                                                   \
        const char *intStr = args.asString( i ).asChar();               \
        INTO = atoi( intStr );                                          \
        if ( !INTO && ( strcmp( intStr, "0" ) ) )                       \
        {                                                               \
            displayError( args.asString( i ) + " isn't a number" );     \
            return MS::kFailure;                                        \
        }                                                               \
    }                                                                   \
}
                                 

//******************************************************************************
void *GtoParticleExport::creator()
{
    return new GtoParticleExport;
}


//******************************************************************************
extern "C" MStatus initializePlugin( MObject obj )
{
    MStatus status = MS::kSuccess;
    MFnPlugin plugin( obj, "Tweak Films", "1.01", "Any" );
  
    status = plugin.registerCommand( "GtoParticleExport", 
                                     GtoParticleExport::creator );
    if ( !status )
    {
        status.perror( "registerCommand" );
        return status;
    }

    return status;
}


//******************************************************************************
extern "C" MStatus uninitializePlugin( MObject obj )
{
    MStatus status = MS::kSuccess;
    MFnPlugin plugin( obj );
  
    status = plugin.deregisterCommand( "GtoParticleExport" );
    if ( !status )
    {
        status.perror( "deregisterCommand" );
        return status;
    }
  
    return status;
}


// *****************************************************************************
GtoParticleExport::GtoParticleExport()
{
    int m_startFrame = (int)( MAnimControl::currentTime().value() );
    int m_endFrame = m_startFrame;

    // Always output id...
    m_attrs.append( "id" );
    m_outAttrs.append( "id" );
    m_defaultAttrs = true;

    m_idMult = 1;
    m_idOffset = 0;
    m_doFilter = false;
    
    m_verbose = false;
}


// *****************************************************************************
MStatus GtoParticleExport::parseArgs( const MArgList &args )
{
    MStatus status = MS::kSuccess;

    MString arg;
    const MString vFlag( "-v" );
    bool verbose = false;

    if( args.length() == 1 )
    {
        // GtoParticleExport -v  is a valid use...
        arg = args.asString( 0, &status );
        if ( status && ( arg == vFlag ) )
        {
            verbose = true;
        }
        else
        {
            displayError(
                "Usage: GtoParticleExport\n"
                "// [-f <filenamePattern> (i.e. \"foo.#.gto\") \n"
                "// [-atr <attrName> ... "
                "(default exports id, position, and velocity, "
                "id is always exported)]\n"
                "// [-oatr <ouputAttrName> ... (use to rename attributes)]\n"
                "// [-node <particleName> ... (default selected)]\n"
                "// [-mnf <startFrame> (default current frame)]\n"
                "// [-mxf <endFrame> (default startFrame)]\n"
//                 "// [-idMult <multiplier> (id multiplier, default 1)]\n"
//                 "// [-idOffset <offset> (id offset value, default 0 )]\n"
//                 "// [-fatr <attrName> (filter attribute, particle will not be "
//                 "exported if value is nonzero)]\n"
                "// [-depNode <particleName> ... (force evaluation on these"
                "particles before exporting)]\n"
                "// [-v (verbose)]" );
            return status;
        }
    }
    
    int argLen = args.length();

    const MString atrFlag( "-atr" );
    const MString oaFlag( "-oatr" );
    const MString nFlag( "-node" );
    const MString filenameFlag( "-f" );
    const MString mnfFlag( "-mnf" );
    const MString mxfFlag( "-mxf" );
    const MString idmFlag( "-idMult" );
    const MString idoFlag( "-idOffset" );
    const MString faFlag( "-fatr" );
    const MString dpFlag( "-depNode" );

    int i;

    //**************************************************************************
    //**************************************************************************
    // PARSE ARGUMENTS
    //**************************************************************************
    //**************************************************************************
    for( i = 0; ( i + 1 ) < argLen; ++i )
    {
        arg = args.asString( i, &status );

        if( ! status )
        {
            continue;
        }

        //**********************************************************************
        // ATTRIBUTE FLAG
        if ( arg == atrFlag )
        {
            m_defaultAttrs = false;
            GET_NON_DUP_VAR_STRS( atrFlag, m_attrs );
        }
        //**********************************************************************
        // OUTPUT ATTRIBUTE FLAG
        else if ( arg == oaFlag )
        {
            GET_NON_DUP_VAR_STRS( oaFlag, m_outAttrs );
        }
        //**********************************************************************
        // PARTICLE NODE FLAG
        else if ( arg == nFlag )
        {
            GET_NON_DUP_VAR_STRS( nFlag, m_particles );
        }
        //**********************************************************************
        // FILENAME FLAG
        else if ( arg == filenameFlag )
        {
            ++i;
            status = args.get( i, m_filename );
            CHECK_STATUS;
        }
        //**********************************************************************
        // MIN FRAME FLAG
        else if ( arg == mnfFlag )
        {
            GET_INT_FROM_ARG( m_startFrame );
        }
        //**********************************************************************
        // END FRAME FLAG
        else if ( arg == mxfFlag )
        {
            GET_INT_FROM_ARG( m_endFrame );
        }
        //**********************************************************************
        // ID MULTIPLY FLAG
        else if ( arg == idmFlag )
        {
            GET_INT_FROM_ARG( m_idMult );
        }
        //**********************************************************************
        // ID OFFSET FLAG
        else if ( arg == idoFlag )
        {
            GET_INT_FROM_ARG( m_idOffset );
        }
        //**********************************************************************
        // FILTER ATTRIBUTE FLAG
        else if ( arg == faFlag )
        {
            ++i;
            status = args.get( i, m_filterAttr );
            CHECK_STATUS;
            if ( m_filterAttr.length() )
            {
                m_doFilter = true;
            }
        }
        //**********************************************************************
        // DEPENDENT PARTICLE FLAG
        else if ( arg == dpFlag )
        {
            GET_NON_DUP_VAR_STRS( dpFlag, m_depParticles );
        }
        //**********************************************************************
        // VERBOSITY FLAG
        else if ( arg == vFlag )
        {
            m_verbose = true;
        }
        //**********************************************************************
        // INVALID!!!
        else if ( arg.index( '-' ) == 0 )
        {
            displayError( "Invalid flag: " + arg );
            return MS::kFailure;
        }
        else
        {
            displayWarning( "Extra argument: " + arg );
        }
    }

    // Fix frame range
    if( m_endFrame < m_startFrame )
    {
        m_endFrame = m_startFrame;
    }

    // Check for lone verbose flag
    if( i < argLen )
    {
        // Check for lone verbose flag
        arg = args.asString( i, &status );
        if( status )
        {
            if ( arg == vFlag )
            {
                m_verbose = true;
            }
            else if ( arg.index( '-' ) == 0 )
            {
                displayError( "Invalid flag: " + arg );
                return MS::kFailure;
            }
            else
            {
                displayWarning( "Extra argument: " + arg );
            }
        }
    }

    // Check for default attributes
    if( m_defaultAttrs )
    {
        m_attrs.append( "position" );
        m_attrs.append( "velocity" );
    }

    // Verify that outAttributes matches in attributes
    int attrsLen = m_attrs.length();
    int outAttrsLen = m_outAttrs.length();
    if( outAttrsLen < attrsLen )
    {
        for( i = outAttrsLen; i < attrsLen; ++i )
        {
            m_outAttrs.append( m_attrs[i] );
        }
    }

    return status;
}


//******************************************************************************
std::string acceptableName( MDagPath &dp )
{
    MStatus status;
    MString name = dp.partialPathName( &status );

    if (status == MS::kFailure)
    {
        MGlobal::displayError("FAILED to get partialPathName()");
        return std::string("***GTO: BAD NAME***");
    }

    std::string n(name.asChar());

    size_t p = n.rfind('|');
    if (p != std::string::npos)
    {
        n = n.substr(p+1);
    }

    return n;
}

//******************************************************************************
void GtoParticleExport::getNames( MDagPath &dp, int rlevel )
{
    if( rlevel != 0 )
    {
        std::string name = acceptableName( dp );

        MObject object = dp.node();
        if (object.hasFn(MFn::kTransform))
        {
            for (int i=0; i < dp.childCount(); i++)
            {
                MObject child = dp.child(i);
                if (child.hasFn(MFn::kDagNode))
                {
                    MFnDagNode dn(child);
                    MDagPath childDp;
                    dn.getPath(childDp);
                    getNames(childDp, rlevel - 1);
                }
            }
        }
        else if( object.hasFn( MFn::kParticle ) )
        {
            for( int i = 0; i < m_particles.length(); ++i )
            {
                if( m_particles[i] == name.c_str() )
                {
                    // Already have this particle bucket...
                    return;
                }
            }
            m_particles.append( name.c_str() );
        }
    }
}


// *****************************************************************************
MStatus GtoParticleExport::getParticleNodes()
{
    MStatus status = MS::kSuccess;

    // Get particle node
    MSelectionList slist;

    if( m_particles.length() > 0 )
    {
        for( int i = 0; i < m_particles.length(); ++i )
        {
            status = slist.add( m_particles[i] );
            if( ! status )
            {
                displayError( "Invalid particle node name: " + m_particles[i] );
                return status;
            }
        }
    }
    else
    {
        if( m_verbose )
        {
            std::cerr << "Getting active selection" << std::endl;
        }
        MGlobal::getActiveSelectionList( slist );
        if( slist.length() == 0 )
        {
            displayError( "No active particle node selection!" );
            return MS::kFailure;
        }
    }

    for( int i = 0; i < slist.length(); i++ )
    {
        MDagPath dp;
        slist.getDagPath(i,dp);
        getNames( dp, 10000 ); // 10000 == maxRecursion
    }

    // Get the particle nodes
    for( int i = 0; i < m_particles.length(); ++i )
    {
        m_particleBuckets.push_back( 
                new ParticleBucket( m_particles[i].asChar() ) );
    }

    return MS::kSuccess;
}


// *****************************************************************************
MStatus GtoParticleExport::getDependentNodes()
{
    MStatus status = MS::kSuccess;
    
    for( int i = 0; i < m_depParticles.length(); ++i )
    {
        MSelectionList slist;
        status = slist.add( m_depParticles[i] );
        CHECK_STATUS;
        
        MDagPath dp;
        slist.getDagPath(i,dp);
        
        MFnDependencyNode dn( dp.node(), &status );
        CHECK_STATUS;

        MPlug attrPlug = dn.findPlug( "forceDynamics", &status );
        CHECK_STATUS;

        m_depPlugs.append( attrPlug );
    }

    return MS::kSuccess;
}


// *****************************************************************************
void GtoParticleExport::evalDependents()
{
    bool forceHack;
    MStatus status;
    for( int i = 0; i < m_depPlugs.length(); ++i )
    {
        status = m_depPlugs[i].getValue( forceHack );
        if( ! status )
        {
            std::cerr << "Failed to evaluate dependency" << std::endl;
        }
    }
}


// *****************************************************************************
MStatus GtoParticleExport::getAttributes()
{
    MStatus status;
    int numValidAttrs = 0;

    for( int p = 0; p < m_particleBuckets.size(); ++p )
    {
        for( int a = 0; a < m_outAttrs.length(); ++a )
        {
            status = m_particleBuckets[p]->exportAttr( m_outAttrs[a].asChar() );
            if( ! status )
            {
                std::string err = "Can't export ";
                err += m_particleBuckets[p]->name();
                err += std::string(".") + m_outAttrs[a].asChar();
                MGlobal::displayWarning( err.c_str() );
            }
            else
            {
                ++numValidAttrs;
            }   
        }
    }

    return (numValidAttrs > 0) ? MS::kSuccess : MS::kFailure;
}


// *****************************************************************************
MStatus GtoParticleExport::writeGtos()
{
    MStatus status;

    MComputation computation;
    computation.beginComputation();
    for( int frame = m_startFrame; frame <= m_endFrame; ++frame )
    {
        std::string filename = replaceFrameSymbols( m_filename.asChar(), 
                                                    frame );
        if( m_verbose )
        {
            MGlobal::displayInfo(std::string( "Writing " + filename ).c_str());
        }

        status = MGlobal::viewFrame( frame );
        CHECK_STATUS;
        
        evalDependents();

        Gto::Writer *writer = new Gto::Writer;
        writer->open( filename.c_str() );

        MString exportedFromStr = "(Exported from " + MFileIO::currentFile()
                                  + ")";
        writer->intern( exportedFromStr.asChar() );

        for( int p = 0; p < m_particleBuckets.size(); ++p )
        {
            m_particleBuckets[p]->writeGtoHeader( writer );
        }

        writer->beginData();
        for( int p = 0; p < m_particleBuckets.size(); ++p )
        {
            m_particleBuckets[p]->writeGtoData( writer );
        }
        writer->endData();

        writer->close();
        delete writer;

        if( computation.isInterruptRequested() )
        {
            break;
        }
    }
    computation.endComputation();

    return MS::kSuccess;
}

//******************************************************************************
MStatus GtoParticleExport::doIt( const MArgList &args )
{
    MStatus status = MS::kSuccess;

    status = parseArgs( args );
    CHECK_STATUS;

    status = getParticleNodes();
    if( m_particles.length() <= 0 )
    {
        MGlobal::displayError( "No particle nodes selected for export" );
        return MS::kFailure;
    }
    std::ostringstream info;
    info << "Exporting: " << m_particles;
    MGlobal::displayInfo( info.str().c_str() );

    status = getDependentNodes();
    CHECK_STATUS;

    status = getAttributes();
    CHECK_STATUS;

    status = writeGtos();    
    CHECK_STATUS;

    return MS::kSuccess;
}
