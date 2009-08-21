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

