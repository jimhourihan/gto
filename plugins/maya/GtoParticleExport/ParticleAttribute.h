//******************************************************************************
// Copyright (c) 2001-2004 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __PARTICLEATTRIBUTE_H__
#define __PARTICLEATTRIBUTE_H__

#include <string>
#include <vector>
#include <maya/MPlug.h>
#include <maya/MFnTypedAttribute.h>
#include <Gto/Writer.h>

//
// A ParticleAttribute represents a Maya attribute plug of type kDoubleArray, 
// kIntArray, or kVectorArray that knows how to write itself into a gto file.
//
class ParticleAttribute
{
public:
    ParticleAttribute( const std::string &attrName, 
                       const std::string &objectName );
    ~ParticleAttribute();

    MPlug attrPlug() const;

    const std::string &name() const;
    MFnData::Type type() const;
    size_t numElements() const;
    size_t partsPerElement() const;
    
    void writeGtoHeader( Gto::Writer *writer );
    void writeGtoData( Gto::Writer *writer );

private:
    const std::string m_attrName;
    const std::string m_objectName;

};

typedef std::vector<ParticleAttribute *> ParticleAttributes;

#endif    // End #ifdef __PARTICLEATTRIBUTE_H__
