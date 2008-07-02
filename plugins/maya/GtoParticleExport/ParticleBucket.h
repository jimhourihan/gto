//******************************************************************************
// Copyright (c) 2001-2004 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __PARTICLEBUCKET_H__
#define __PARTICLEBUCKET_H__

#include <string>

#include <maya/MObject.h>
#include <maya/MPlugArray.h>

#include <Gto/Writer.h>

#include "ParticleAttribute.h"

//
// A ParticleBucket represents a Maya particle dependency node that 
// knows how to write itself into a gto file.
//
class ParticleBucket
{
public:
    ParticleBucket( std::string name );
    ~ParticleBucket();

    MObject mayaObject() const;
    const std::string &name() const { return m_name; }

    int startFrame() const;
    
    // Mark the given attribute for export.  Fails if the
    // attribute can't be exported for some reason (not 
    // there, bad type, etc.)
    MStatus exportAttr( std::string attrName );

    void writeGtoHeader( Gto::Writer *writer );
    void writeGtoData( Gto::Writer *writer );
        
private:
    bool m_doRunup;

    std::string m_name;
    ParticleAttributes m_exportAttrs;
};

typedef std::vector<ParticleBucket *> ParticleBuckets;


#endif    // End #ifdef __PARTICLEBUCKET_H__
