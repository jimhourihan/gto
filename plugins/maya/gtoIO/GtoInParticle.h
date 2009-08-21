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

#ifndef __GTOINPARTICLE_H__
#define __GTOINPARTICLE_H__

#include "GtoInObject.h"
#include <maya/MObject.h>
#include <maya/MString.h>
#include <vector>
#include <Gto/Reader.h>

namespace GtoIOPlugin {

typedef Gto::Reader::Request Request;
typedef Gto::Reader::StringTable StringTable;

class Particle : public Object
{
public:

    struct Attribute
    {
        Attribute(const std::string &n)
            : name(n), width(0), size(0), floatData(0) {}
        ~Attribute() { delete [] floatData; }

        std::string name;
        size_t width;
        size_t size;
        float*  floatData;
    };

    Particle( const std::string &name, 
              const std::string &protocol, 
              const unsigned int protocolVersion );

    virtual ~Particle();

    enum
    {
        POINTS_C = Object::NEXT_C,
    };

    virtual Request component( const std::string &name ) const;

    virtual Request property( const std::string &name,
                              void *componentData ) const;

    virtual void *data( const PropertyInfo &pinfo, 
                        size_t bytes,
                        void *componentData,
                        void *propertyData );

    virtual void dataRead( const PropertyInfo &pinfo,
                           void *componentData,
                           void *propertyData,
                           const StringTable &strings );

    virtual void declareMaya();

private:
    std::vector<Attribute*> m_attributes;
    Attribute *m_position;
    size_t m_numParticles;
};

} // End namespace GtoIOPlugin


#endif    // End #ifdef __GTOINPARTICLE_H__
