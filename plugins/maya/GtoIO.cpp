//******************************************************************************
// Copyright (c) 2002 Tweak Inc.
// All rights reserved.
//******************************************************************************
#include "version.h"

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>

#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MAnimControl.h>
#include <Gto/Header.h>

#include "GtoIO.h"
#include "GtoInDataBase.h"
#include "GtoInSet.h"
#include "GtoOut.h"
#include "version.h"

namespace GtoIOPlugin {
using namespace std;

// *****************************************************************************
GtoIO::GtoIO()
{
}

// *****************************************************************************
MStatus GtoIO::reader( const MFileObject &file, 
                       const MString &optionsString,
                       MPxFileTranslator::FileAccessMode mode )
{
    DataBase dataBase;
    Set *set = dataBase.set( file.fullName().asChar() );
    if( set == NULL )
    {
        MGlobal::displayError( "Unable to open file for some "
                               "reason.  Permissions?" );
        return MS::kFailure;
    }

    set->computeLocalTransforms();

    set->declareMaya();
    
    set->reparentAll();
    
    dataBase.destroyAll();

    return MS::kSuccess;
}

// *****************************************************************************
MStatus GtoIO::writer( const MFileObject &file, 
                       const MString &optionsString,
                       MPxFileTranslator::FileAccessMode mode )
{
    MTime fs = MAnimControl::currentTime(); 
    MTime fe = fs;
    double shutter = 0.0;
    MString filename = file.fullName();
    bool subd = false;
    bool normals = false;
    bool exportST = false;
    int maxRecurse = 1;
    bool normalize = false;
    bool hidden = true;
    bool verify = true;
    bool doAnim = false;
    bool diffPoints = false;
    bool diffMatrix = false;
    bool diffNormals = false;
    bool isDifferenceFile = false;
    
    if( ( mode == MPxFileTranslator::kExportAccessMode ) ||
        ( mode == MPxFileTranslator::kSaveAccessMode ) )
    {
        MGlobal::displayError( "The GTO plugin can only be used for Export "
                               "Selection...");
        return MS::kFailure;
    }

    MStringArray args;
    optionsString.split( ';', args );
    for( size_t i = 0; i < args.length(); ++i )
    {
        MStringArray thisArg;
        args[i].split( '=', thisArg );
        MString argName( thisArg[0] );
        MString argValue( thisArg[1] );

        if( argName == "recurse" && argValue == "1" )
        {
            maxRecurse = 100000;
        }
        else if( argName == "subd" && argValue == "1" )
        {
            subd = true;
        }
        else if( argName == "normals" && argValue == "1" )
        {
            normals = true;
        }
        else if( argName == "st" && argValue == "1" )
        {
            exportST = true;
        }
        else if( argName == "diffpositions" && argValue == "1" )
        {
            diffPoints = true;
        }
        else if( argName == "diffmatrices" && argValue == "1" )
        {
            diffMatrix = true;
        }
        else if( argName == "diffnormals" && argValue == "1" )
        {
            diffNormals = true;
        }
        else if( argName == "isdifference" && argValue == "1" )
        {
            isDifferenceFile = true;
        }
        else if( argName == "normalize" && argValue == "1" )
        {
            normalize = true;
        }
        else if( argName == "hidden" && argValue == "0" )
        {
            hidden = false;
        }
        else if( argName == "verify" && argValue == "0" )
        {
            verify = false;
        }
        else if( argName == "anim" && argValue == "1" )
        {
            doAnim = true;
            // If user didn't include a # in the filename, but
            // is exporting multiple frames, do it automatically
            if( filename.index( '#' ) < 0 )
            {
                // By this point, Maya will have already appended a 
                // ".gto" to the filename if the user didn't include it,
                // so we're guaranteed to find a '.' in the filename
                filename = filename.substring( 0, filename.rindex( '.' ) )
                           + "#.gto";
            }
        }
        else if( argName == "fs" && doAnim )
        {
            fs = MTime( argValue.asDouble(), MTime::uiUnit() );
        }
        else if( argName == "fe" && doAnim  )
        {
            fe = MTime( argValue.asDouble(), MTime::uiUnit() );
        }
        else if( argName == "shutter" && doAnim )
        {
            shutter = argValue.asDouble() / 360.0;
        }
        else if( argName == "recurseLimit" )
        {
            if( argValue.asInt() > 0 )
            {
                maxRecurse = argValue.asInt();
            }
        }
    }
    
    if( ! isDifferenceFile )
    {
        diffPoints = false;
        diffMatrix = false;
        diffNormals = false;
    }
    
    // TODO: Find a more graceful way to get options to GtoExporter

    GtoExporter exporter( fs, fe, shutter, subd, normals, exportST, filename, 
                          maxRecurse, normalize, hidden, verify, 
                          isDifferenceFile, diffPoints, diffMatrix, 
                          diffNormals );

    MStatus result = exporter.doIt();
    return result;
}

// *****************************************************************************
MPxFileTranslator::MFileKind GtoIO::identifyFile( const MFileObject &file,
                                                  const char *magic,
                                                  short magicSize ) const
{
    if( magicSize < 4 )
    {
        return MPxFileTranslator::kCouldBeMyFileType;
    }

    const unsigned int magicInt = *((unsigned int *)magic );
    if( magicInt == Gto::Header::Magic
        || magicInt == Gto::Header::Cigam )
    {
        return MPxFileTranslator::kIsMyFileType;
    }

    const int gz_magic[2] = {0x1f, 0x8b}; /* gzip magic header */
    if( magic[0] == gz_magic[0] && magic[1] == gz_magic[1] )
    {
        return MPxFileTranslator::kCouldBeMyFileType;
    }

    return MPxFileTranslator::kNotMyFileType;
}


// *****************************************************************************
void *GtoIO::creator()
{
    return new GtoIO;
}

} // End namespace GtoIOPlugin

// *****************************************************************************
//                            PLUGIN REGISTRATION
// *****************************************************************************

// *****************************************************************************
MStatus initializePlugin( MObject _obj )
{
    MFnPlugin   plugin( _obj, "Tweak Films", GtoIOPlugin::versionString() );
    MStatus     stat;
    stat = plugin.registerFileTranslator( "GTO",
                                          "none",
                                          GtoIOPlugin::GtoIO::creator,
                                          "gtoExportOptions", "",
                                          false );
    if( ! stat )
    {
        stat.perror( "initializePlugin" );
    }
    return stat;
}

// *****************************************************************************
MStatus uninitializePlugin( MObject _obj )                     
{
    MFnPlugin   plugin( _obj );
    MStatus     stat;
    stat = plugin.deregisterFileTranslator( "GTO" );
    if ( !stat )
    {
        stat.perror( "uninitializePlugin" );
    }
    return stat;
}

