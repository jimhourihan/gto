//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __GTOINPARTICLE_H__
#define __GTOINPARTICLE_H__

#include "GtoInObject.h"
#include <maya/MObject.h>
#include <maya/MString.h>
#include <vector>

namespace GtoIOPlugin {

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

    virtual void *component( const std::string &name ) const;

    virtual void *property( const std::string &name,
                            void *componentData ) const;

    virtual void data( void *componentData,
                       void *propertyData,
                       const double *items,
                       size_t numItems,
                       size_t width);

    virtual void data( void *componentData,
                       void *propertyData,
                       const float *items,
                       size_t numItems,
                       size_t width);
    
    virtual void data( void *componentData,
                       void *propertyData,
                       const int *items,
                       size_t numItems,
                       size_t width);

    virtual void data( void *componentData,
                       void *propertyData,
                       const unsigned short *items,
                       size_t numItems,
                       size_t width);

    virtual void data( void *componentData,
                       void *propertyData,
                       const unsigned char *items,
                       size_t numItems,
                       size_t width);

    virtual void declareMaya();

private:
    std::vector<Attribute*> m_attributes;
    Attribute *m_position;
    size_t m_numParticles;
};

} // End namespace GtoIOPlugin


#endif    // End #ifdef __GTOINPARTICLE_H__
