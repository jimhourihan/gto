//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************
#include "version.h"

#include "GtoInParticle.h"
#include <maya/MGlobal.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <float.h>
#include <algorithm>

namespace GtoIOPlugin {
using namespace std;

//******************************************************************************
Particle::Particle( const std::string &name, 
                    const std::string &protocol, 
                    const unsigned int protocolVersion )
    : Object( name, protocol, protocolVersion ),
      m_position(0),
      m_numParticles(0)
{
    // Nothing
}

Particle::~Particle()
{
    for (size_t i=0; i < m_attributes.size(); i++)
    {
        delete m_attributes[i];
    }
}

void *
Particle::component( const std::string &name ) const
{
    if ( name == "points" )
    {
        return (void*)POINTS_C;
    }

    return Object::component( name );
}

void *
Particle::property( const std::string &name,
                    void *componentData ) const
{
    if ( (( int )componentData) == POINTS_C )
    {
        Attribute *a = new Attribute(name);
        return a;
    }

    return Object::property( name, componentData );
}

void
Particle::data( void *componentData,
                void *propertyData,
                const double *items,
                size_t numItems,
                size_t width)
{
    if (int(componentData) != POINTS_C) return;
    if (m_numParticles == 0) m_numParticles = numItems;
    Attribute *a = reinterpret_cast<Attribute*>(propertyData);

    if (m_numParticles != numItems) 
    {
        delete a;
        return;
    }

    a->width = width;
    a->size  = numItems;
    a->floatData = new float[numItems * width];

    for (int i=0, s=numItems*width; i < s; i++)
    {
        a->floatData[i] = float(items[i]);
    }

    if (a->name == GTO_PROPERTY_POSITION)
    {
        m_position = a;
    }

    m_attributes.push_back(a);
    Object::data( componentData, propertyData, items, numItems, width );
}
 

void 
Particle::data( void *componentData,
                void *propertyData,
                const float *items,
                size_t numItems,
                size_t width)
{
    Attribute *a = reinterpret_cast<Attribute*>(propertyData);
    if (int(componentData) != POINTS_C) return;

    if (m_numParticles == 0) m_numParticles = numItems;
    if (m_numParticles != numItems) 
    {
        delete a;
        return;
    }

    a->width = width;
    a->size  = numItems;
    a->floatData = new float[numItems * width];
    memcpy(a->floatData, items, sizeof(float)* numItems * width);

    m_attributes.push_back(a);

    if (a->name == GTO_PROPERTY_POSITION)
    {
        m_position = a;
    }

    Object::data( componentData, propertyData, items, numItems, width );
}

void 
Particle::data( void *componentData,
                void *propertyData,
                const int *items,
                size_t numItems,
                size_t width)
{
    if (int(componentData) != POINTS_C) return;
    if (m_numParticles == 0) m_numParticles = numItems;

    Attribute *a = reinterpret_cast<Attribute*>(propertyData);

    if (m_numParticles != numItems) 
    {
        delete a;
        return;
    }

    a->width = width;
    a->size  = numItems;
    a->floatData = new float[numItems * width];

    for (int i=0, s=numItems*width; i < s; i++)
    {
        a->floatData[i] = float(items[i]);
    }

    m_attributes.push_back(a);
    Object::data( componentData, propertyData, items, numItems, width );
}


void
Particle::data( void *componentData,
                void *propertyData,
                const unsigned short *items,
                size_t numItems,
                size_t width)
{
    if (int(componentData) != POINTS_C) return;
    if (m_numParticles == 0) m_numParticles = numItems;

    Attribute *a = reinterpret_cast<Attribute*>(propertyData);

    if (m_numParticles != numItems) 
    {
        delete a;
        return;
    }

    a->width = width;
    a->size  = numItems;
    a->floatData = new float[numItems * width];

    for (int i=0, s=numItems*width; i < s; i++)
    {
        a->floatData[i] = float(items[i]);
    }

    m_attributes.push_back(a);
    Object::data( componentData, propertyData, items, numItems, width );
}

void
Particle::data( void *componentData,
                void *propertyData,
                const unsigned char *items,
                size_t numItems,
                size_t width)
{
    if (int(componentData) != POINTS_C) return;
    if (m_numParticles == 0) m_numParticles = numItems;

    Attribute *a = reinterpret_cast<Attribute*>(propertyData);

    if (m_numParticles != numItems) 
    {
        delete a;
        return;
    }

    a->width = width;
    a->size  = numItems;
    a->floatData = new float[numItems * width];

    for (int i=0, s=numItems*width; i < s; i++)
    {
        a->floatData[i] = float(items[i]);
    }

    m_attributes.push_back(a);
    Object::data( componentData, propertyData, items, numItems, width );
}

void
Particle::declareMaya()
{
    MFnDagNode particle;
    MStatus status;

    //
    //        If you use the create functions that takes a typeid it will
    //        fail for some reason.
    //

    m_mayaParentObject = particle.create( "particle",
                                          MObject::kNullObj, 
                                          &status );
    if ( status != MS::kSuccess )
    {
        status.perror( "Particle object creation failed" );
        MGlobal::displayError(status.errorString());
        m_mayaObject = MObject::kNullObj;
        m_mayaParentObject = MObject::kNullObj;
        return;
    }

    //
    // Now use that transform node we got earlier to store a handle to
    // the real geometry node.
    //

    MFnDagNode parentDN( m_mayaParentObject );
    m_mayaObject = parentDN.child( 0 );
    MFnDependencyNode shape(m_mayaObject);
    setName();
    addToDefaultSG();

    //
    //        Jesus H. Christ, why can there not be an MFnParticle class?
    //
    // addAttr -ln foobar0 -dt vectorArray  |particle1|particleShape1;
    // addAttr -ln foobar -dt vectorArray  |particle1|particleShape1;

    for (size_t i=0; i < m_attributes.size(); i++)
    {
        Attribute *a = m_attributes[i];

        //
        //  Add the attributes
        //

        for (int q=0; q < 2; q++)
        {
            MString command = "addAttr -ln ";
            command += a->name.c_str();
            if (!q) command += "0";
            command += " -dt ";

            if (a->width == 3) command += "vectorArray";
            else if (a->width == 1) command += "doubleArray";
            else continue;

            command += " ";
            command += parentDN.fullPathName();
            command += "|";
            command += shape.name();
            command += ";";

            if (q)
            {
                command += "setAttr -e -keyable true ";
                command += parentDN.fullPathName();
                command += "|";
                command += shape.name();
                command += ".";
                command += a->name.c_str();
                command += ";";
            }

            MGlobal::executeCommand(command, false, false);
        }
    }

    MString command;

    for (size_t i=0; i < m_numParticles; i++)
    {
        if ( !(i % 512) )
        {
            if (i)
            {
                command += ";";
                MGlobal::executeCommand(command, false, false);
            }

            command = "emit -o ";
            command += parentDN.fullPathName();
            command += "|";
            command += shape.name();
        }

        command += " -pos ";
        command += double(m_position->floatData[i*3 + 0]); 
        command += " ";
        command += double(m_position->floatData[i*3 + 1]);
        command += " ";
        command += double(m_position->floatData[i*3 + 2]);
        command += " ";

        for (size_t q=0; q < m_attributes.size(); q++)
        {
            Attribute *a = m_attributes[q];

            if (a->width == 1)
            {
                command += " -at ";
                command += a->name.c_str();
                command += " -fv ";
                command += double(a->floatData[i]);
            }
            else if (a->width == 3)
            {
                command += " -at ";
                command += a->name.c_str();
                command += " -vv ";
                command += double(a->floatData[i*3 + 0]);
                command += " ";
                command += double(a->floatData[i*3 + 1]);
                command += " ";
                command += double(a->floatData[i*3 + 2]);
                command += " ";
            }
        }
    }

    command += ";";
    MGlobal::executeCommand(command, false, false);

    command = "saveInitialState ";
    command += parentDN.fullPathName();
    command += "|";
    command += shape.name();
    command += ";";
    command += "connectAttr time1.outTime ";
    command += parentDN.fullPathName();
    command += "|";
    command += shape.name();
    command += ".currentTime;";
    MGlobal::executeCommand(command, false, false);
}

} // End namespace GtoIOPlugin
