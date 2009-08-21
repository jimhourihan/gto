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


