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
#include "version.h"

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>

#include <maya/MTime.h>
#include <maya/MString.h>
#include <maya/MStringArray.h>
#include <maya/MAnimControl.h>
#include <Gto/Header.h>

#include "GtoIO.h"
#include "GtoInDataBase.h"
#include "GtoInSet.h"
#include "GtoOut.h"
#include "version.h"

#define DEFAULT_OPTIONS "recurse=1;"                \
                        "recurseLimit=0;"           \
                        "subd=1;"                   \
                        "st=1;"                     \
                        "faceMat=0;"                \
                        "normals=1;"                \
                        "diffpositions=0;"          \
                        "diffmatrices=0;"           \
                        "diffnormals=0;"            \
                        "isdifference=0;"           \
                        "normalize=1;"              \
                        "hidden=0;"                 \
                        "verify=1;"                 \
                        "userAttr=1;"               \
                        "mayaAttr=1;"               \
                        "xformAttr=1;"              \
                        "quiet=0;"                  \
                        "ascii=0;"                  \
                        "anim=0;"                   \
                        "fs=1;"                     \
                        "fe=240;"                   \
                        "shutter=0"


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
    MString filename = file.fullName();
    bool readAsDifference = false;
    int fs = 0;
    int fe = 0;

    MStringArray args;
    optionsString.split( ';', args );
    for( size_t i = 0; i < args.length(); ++i )
    {
        MStringArray thisArg;
        args[i].split( '=', thisArg );
        MString argName( thisArg[0] );
        MString argValue( thisArg[1] );

        if( argName == "readDiff" && argValue == "1" )
        {
            readAsDifference = true;
        }
        else if( argName == "fs" )
        {
            fs = argValue.asInt();
        }
        else if( argName == "fe" )
        {
            fe = argValue.asInt();
        }
    }

    if( readAsDifference )
    {
        MGlobal::displayInfo( "PreMunge name: " + filename );

        if( filename.index( '#' ) < 0 )
        {
            // By this point, Maya will have already appended a 
            // ".gto" to the filename if the user didn't include it,
            // so we're guaranteed to find a '.' in the filename
            filename = filename.substring( 0, filename.index( '.' ) )
                       + "#.gto";
        }

        for( int f = fs; f <= fe; ++f )
        {
            MGlobal::viewFrame( MTime( double(f) ) );

            MString fname = replaceFrameCookies( filename, f );

            MGlobal::displayInfo( "Reading " + fname );

            DataBase dataBase;
            Set *set = dataBase.set( fname.asChar() );
            if( set == NULL )
            {
                MGlobal::displayError( "Unable to open file for some "
                                       "reason.  Permissions?" );
                return MS::kFailure;
            }

            set->computeLocalTransforms();

            set->declareMayaDiff();

            dataBase.destroyAll();
        }
    }
    else
    {
        DataBase dataBase;
        Set *set = dataBase.set( filename.asChar() );
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
    }

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
    bool quiet = false;
    bool allUserAttributes = false;
    bool allMayaAttributes = false;
    bool allXformAttributes = false;
    bool faceMaterials = false;
    bool ascii = false;

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
        else if( argName == "quiet" && argValue == "1" )
        {
            quiet = true;
        }
        else if( argName == "ascii" && argValue == "1" )
        {
            ascii = true;
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
        else if( argName == "faceMat" && argValue == "1" )
        {
            faceMaterials = true;
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
        else if( argName == "userAttr" && argValue == "1" )
        {
            allUserAttributes = true;
        }
        else if( argName == "mayaAttr" && argValue == "1" )
        {
            allMayaAttributes = true;
        }
        else if( argName == "xformAttr" && argValue == "1" )
        {
            allXformAttributes = true;
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

    GtoExporter exporter( fs, fe, quiet, shutter, subd, normals, exportST, filename, 
                          maxRecurse, normalize, hidden, verify, 
                          isDifferenceFile, diffPoints, diffMatrix, 
                          diffNormals, allUserAttributes, allMayaAttributes, 
                          faceMaterials, ascii, allXformAttributes );

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
        return MPxFileTranslator::kNotMyFileType;
    }

    const unsigned int magicInt = *((unsigned int *)magic );
    if( magicInt == Gto::Header::Magic
        || magicInt == Gto::Header::Cigam )
    {
        return MPxFileTranslator::kIsMyFileType;
    }

    const char gz_magic[2] = {0x1f, 0x8b}; /* gzip magic header */
    if( magic[0] == gz_magic[0] && magic[1] == gz_magic[1] )
    {
        if( strstr( file.name().asChar(), ".gto" ) )
        {
            return MPxFileTranslator::kIsMyFileType;
        }
    }

    return MPxFileTranslator::kNotMyFileType;
}


// *****************************************************************************
void *GtoIO::creator()
{
    return new GtoIO;
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
                                          "gtoIOOptions", "",
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

