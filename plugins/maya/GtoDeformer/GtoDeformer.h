//******************************************************************************
// Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
//******************************************************************************
#include <maya/MPxDeformerNode.h>
#include <maya/MDataBlock.h>
#include <maya/MItGeometry.h>
#include <maya/MStatus.h>
#include <maya/MTypeId.h> 
#include <maya/MDGMessage.h>
#include <maya/MTime.h>
#include <maya/MString.h>
#include <maya/MDagModifier.h>
#include "GtoPointReader.h"

class GtoDeformer : public MPxDeformerNode
{
public:
    GtoDeformer();
    virtual ~GtoDeformer();

    static void *creator();
    static MStatus initialize();

    virtual MStatus accessoryNodeSetup( MDagModifier &cmd );
    virtual MObject &accessoryAttribute() const;

    virtual MStatus compute( const MPlug &plug, 
                             MDataBlock &dataBlock );

    virtual void postConstructor();
          
    // Unique id
    static MTypeId id;

    // Attributes
    static MObject m_gtoBaseAttr;
    static MObject m_gtoStartFrameAttr;
    static MObject m_gtoEndFrameAttr;
    static MObject m_timeAttr;
    static MObject m_useCacheAttr;
    static MObject m_silentAttr;

    static MObject m_accessoryMatrixAttr;

private:
    
    static void timeChangeCallback( MTime &time, void *clientData );

private:
    MCallbackId m_callbackId;

    MString m_gtoBase;
    int m_gtoStartFrame;
    int m_gtoEndFrame;
    double m_time;    

    bool m_errorFlag;
    bool m_gtoCacheValid;
    bool m_useCache;
    bool m_silent;

    GtoPointReader m_reader;
};

