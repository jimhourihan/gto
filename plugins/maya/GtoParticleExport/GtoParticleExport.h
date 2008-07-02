//******************************************************************************
// Copyright (c) 2001-2004 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __GTOPARTICLEEXPORT_H__
#define __GTOPARTICLEEXPORT_H__

#include <string>
#include <vector>
#include <maya/MPxCommand.h>
#include <maya/MGlobal.h>
#include <maya/MArgList.h>
#include <maya/MObjectArray.h>
#include <maya/MPlugArray.h>
#include <maya/MFnData.h>
#include <maya/MDagPath.h>
#include "ParticleBucket.h"


class GtoParticleExport : public MPxCommand
{
public:
    GtoParticleExport();
    virtual ~GtoParticleExport() {}

    static void *creator();
    virtual MStatus doIt( const MArgList &args );

private:
    MStatus parseArgs( const MArgList &args );
    MStatus getParticleNodes();
    MStatus getDependentNodes();
    void evalDependents();
    MStatus getAttributes();
    MStatus writeGtos();
    void getNames( MDagPath &dp, int rlevel );
        
private:        
    int m_startFrame;
    int m_endFrame;
    bool m_defaultAttrs;

    MString m_filename;

    bool m_verbose;
    bool m_doFilter;
    MStringArray m_attrs;
    MStringArray m_outAttrs;
    MStringArray m_particles;
    MStringArray m_particleOut;
    int m_idMult;
    int m_idOffset;
    MString m_filterAttr;
    MStringArray m_depParticles;
    MPlugArray m_depPlugs;
    
    ParticleBuckets m_particleBuckets;
};



#endif    // End #ifdef __GTOPARTICLEEXPORT_H__
