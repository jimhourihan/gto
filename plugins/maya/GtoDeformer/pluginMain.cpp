//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#include "GtoDeformer.h"
#include <maya/MFnPlugin.h>


MStatus initializePlugin( MObject obj )
{
    MStatus status;
    const char *version = "0.0.1";
    MFnPlugin plugin( obj, "Tweak Films", version, "Any" );

    status = plugin.registerNode( "GtoDeformer", 
                                  GtoDeformer::id, 
                                  GtoDeformer::creator,
                                  GtoDeformer::initialize,
                                  MPxNode::kDeformerNode );
    if ( !status ) 
    {
        status.perror( "registerNode" );
        return status;
    }

    std::cout << "Tweak Films GtoDeformer v" << version
              << std::endl;

    return status;
}


MStatus uninitializePlugin( MObject obj )
{
    MStatus status;
    MFnPlugin plugin( obj );

    status = plugin.deregisterNode( GtoDeformer::id );
    if ( !status ) 
    {
        status.perror( "deregisterNode" );
        return status;
    }

    return status;
}
