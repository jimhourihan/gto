//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

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
