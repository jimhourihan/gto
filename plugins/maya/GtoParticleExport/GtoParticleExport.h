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
