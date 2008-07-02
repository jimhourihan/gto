//******************************************************************************
// Copyright (c) 2001-2004 Tweak Inc. All rights reserved.
//******************************************************************************

#include <TwkIos/TwkIosIostream.h>
#include "GtoParticleDisplay.h"

#include <maya/MFnPlugin.h>

//*****************************************************************************
MStatus initializePlugin( MObject obj )
{
    MStatus status;
    MFnPlugin plugin( obj, "Tweak Films", "1.21", "Any" );

    status = plugin.registerNode( "GtoParticleDisplay",
                                  GtoParticleDisplay::id, 
                                  &GtoParticleDisplay::creator, 
                                  &GtoParticleDisplay::initialize,
                                  MPxNode::kLocatorNode );
    if ( !status )
    {
        status.perror( "registerNode" );
    }

    return status;
}

//*****************************************************************************
MStatus uninitializePlugin( MObject obj )
{
    MStatus status;
    MFnPlugin plugin( obj );

    status = plugin.deregisterNode( GtoParticleDisplay::id );
    if ( !status )
    {
        status.perror( "deregisterNode" );
    }

    return status;
}
