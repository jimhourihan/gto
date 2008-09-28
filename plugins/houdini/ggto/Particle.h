//******************************************************************************
// Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __Particle_H__
#define __Particle_H__

#include <vector>
#include <GEO/GEO_PrimPart.h>
#include <Gto/Writer.h>
#include <TwkMath/TwkMathVec3.h>
#include "Object.h"
#include "GtoAttribute.h"

namespace HGto {

class Particle : public Object {
public:
    Particle( const GEO_PrimParticle *particle );
    Particle( std::string name ) : 
            Object( Object( name, GTO_PROTOCOL_PARTICLE, 1 ) ) {}
    virtual ~Particle();

    virtual void writeHeader( Gto::Writer &writer );
    virtual void writeData( Gto::Writer &writer );

    static std::string newName();

    void declareHoudini( GU_Detail &gdp );
    
    std::vector<TwkMath::Vec3f> &positions() { return m_pointsPositions; }
    GtoAttributes& attributes() { return m_attributes; }

protected:
    std::vector<TwkMath::Vec3f> m_pointsPositions;
    GtoAttributes m_attributes;

private:
    static int m_particleObjNumber;

    std::vector<TwkMath::Vec2f> *combineAgeAndLifespan(void);
};


}  //  End namespace HGto

#endif    // End #ifdef __Particle_H__


