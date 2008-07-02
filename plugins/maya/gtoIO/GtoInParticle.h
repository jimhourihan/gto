//
// Copyright (C) 2003 Tweak Films
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
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
