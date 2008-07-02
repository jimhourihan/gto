//******************************************************************************
// Copyright (c) 2004 Tweak Inc. 
// All rights reserved.
//******************************************************************************
#include <Gto/Reader.h>
#include "GtoCacheEmitter.h"
#include <TwkMaya/Liberation.h>
#include <math.h>
#include <maya/MFnArrayAttrsData.h>
#include <maya/MFnPlugin.h>
#include <stdlib.h>

using namespace TwkMath;
using namespace std;

TWK_MAYA_PLUGIN_IMPLEMENTATION(GtoCacheEmitter, 0x485);
MObject GtoCacheEmitter::m_gtoAttr;
MObject GtoCacheEmitter::m_keepAttr;

//----------------------------------------------------------------------

GtoCacheReader::~GtoCacheReader() {}

Gto::Reader::Request
GtoCacheReader::object(const string& name, 
                       const string& protocol, 
                       unsigned int protocolVersion,
                       const Gto::Reader::ObjectInfo& info) 
{
    if (protocol == "particle")
    {
        return Parent::object(name, protocol, protocolVersion, info);
    }
    else
    {
        return Request(false);
    }
}


Gto::Reader::Request
GtoCacheReader::component(const string& name,
                          const string& interp,
                          const Gto::Reader::ComponentInfo &header)
{
    if (name == "points")
    {
        return Parent::component(name, interp, header);
    }
    else
    {
        return Request(false);
    }
}

void
GtoCacheReader::dataRead(const Gto::Reader::PropertyInfo& info)
{
    Parent::dataRead(info);

    Gto::Object *o    = 
        reinterpret_cast<Gto::Object*>(info.component->object->objectData);

    Gto::Component *c = 
        reinterpret_cast<Gto::Component*>(info.component->componentData);

    Gto::Property *p  = 
        reinterpret_cast<Gto::Property*>(info.propertyData);

    //
    //  Tell the emitter about it
    //

    m_emitter->dataRead(p);

    //
    //  Nuke the data since we don't need it anymore
    //

    if (p->type == Gto::String)
    {
        delete [] p->stringData;
    }
    else
    {
        delete [] (char*)(p->voidData);
    }

    p->voidData = 0;
}

//----------------------------------------------------------------------

GtoCacheEmitter::GtoCacheEmitter() : EmitterNode()
{
    m_requiresOwnerMsgNode = true;
}
GtoCacheEmitter::~GtoCacheEmitter() {}

void 
GtoCacheEmitter::postConstructor()
{
    Parent::postConstructor();
}

MStatus
GtoCacheEmitter::initialize()
{
    MS::MStatus status = EmitterNode::initialize();

    if (status == MS::kSuccess)
    {
        m_gtoAttr = DefineStringAttribute("GtoBasePath", "gtobp", "");
        attributeAffects(m_gtoAttr, mOutput);

        m_keepAttr = DefineBoolAttribute("Keep", "k",  
                                         false,     // defaultValue
                                         false);    // keyable
        attributeAffects(m_keepAttr, mOutput);
    }

    return status;
}

MStatus 
GtoCacheEmitter::compute(const MPlug& plug, MDataBlock& block)
{
    MStatus status;
    if (!okToEmit(plug, block, status)) return status;

    //
    //  Get input data
    //

    m_count = 0;

    MString gtoPath = GetStringAttr(m_gtoAttr, block);
    bool keep       = GetBoolAttr(m_keepAttr, block);
    MTime t         = timeAttr(block);
    int frame       = int( t.as(MTime::uiUnit()) );
    int plugIndex   = plug.logicalIndex(&status);

    char gtofile[256];
    sprintf(gtofile, "%s.%04d.gto", gtoPath.asChar(), frame);

    //
    // Get outputs
    //

    MFnArrayAttrsData fnOut;
    m_output = fnOut.create(&status);                               STATUS;

    //
    //  Read the GTO
    //
    
    GtoCacheReader reader(this);
    if (!reader.open(gtofile))
    {
        fprintf(stderr, "GtoCacheEmitter:: no %s\n", gtofile);
    }

    //
    //  Set outgoing lifespan
    //

    DArray lifespan = fnOut.doubleArray("lifespanPP", &status);     STATUS;

    for (int i=0; i < m_count; i++)
    {
        lifespan.append(1000000.0);
    }

    //
    //  Cull existing lifespans
    //

    if( ! keep )
    {
        MDoubleArray lp = ownerDoubleArray("lifespanPP", status);
        for (int i=0; i < lp.length(); i++)
        {
            lp[i] = -1000000.0;
        }
    }

    //
    // Update the data block with new dOutput and set plug clean.
    //

    MArrayDataHandle  hOutArray = block.outputArrayValue(mOutput, &status);
    STATUS;

    MArrayDataBuilder bOutArray = hOutArray.builder(&status);
    STATUS;

    MDataHandle       hOut      = bOutArray.addElement(plugIndex, &status);
    STATUS;

    hOut.set(m_output);
    block.setClean( plug );

    return MS::kSuccess;
}

void
GtoCacheEmitter::dataRead(Gto::Property* p)
{
    MStatus status;

    if (m_count < p->size) m_count = p->size;

    switch (p->type)
    {
      case Gto::Float:
          floatData(p);
          break;
      case Gto::Int:
          intData(p);
          break;
      default:
          break;
    };
}


void
GtoCacheEmitter::floatData(Gto::Property* p)
{
    MStatus status;
    MFnArrayAttrsData fn(m_output);

    if (p->width == 3 || p->width == 2)
    {
        VArray array = fn.vectorArray(p->name.c_str(), &status);

        if (status == MS::kSuccess)
        {
            size_t len = array.length();
            size_t s   = p->size * p->width;
            
            for (size_t i=0; i < s; i += p->width)
            {
                MVector v;

                if (p->width == 2)
                {
                    v = MVector(p->floatData[i], p->floatData[i+1]);
                }
                else
                {
                    v = MVector(p->floatData[i], 
                                p->floatData[i+1], 
                                p->floatData[i+2]);
                }

                if (i < len)
                {
                    array[i] = v;
                }
                else
                {
                    array.append(v);
                }
            }
        }
    }
    else if (p->width == 1)
    {
        DArray array = fn.doubleArray(p->name.c_str(), &status);

        if (status == MS::kSuccess)
        {
            size_t len = array.length();
            size_t s   = p->size;
            
            for (size_t i=0; i < s; i++)
            {
                double d = p->floatData[i];

                if (i < len)
                {
                    array[i] = d;
                }
                else
                {
                    array.append(d);
                }
            }
        }
    }
}

void
GtoCacheEmitter::intData(Gto::Property* p)
{
    MStatus status;
    MFnArrayAttrsData fn(m_output);

    if (p->width == 1)
    {
        IArray array = fn.intArray(p->name.c_str(), &status);

        if (status == MS::kSuccess)
        {
            size_t len = array.length();
            size_t s   = p->size;
            
            for (size_t i=0; i < s; i++)
            {
                int d = p->int32Data[i];

                if (i < len)
                {
                    array[i] = d;
                }
                else
                {
                    array.append(d);
                }
            }
        }
    }
}

//----------------------------------------------------------------------

MStatus 
initializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj, "Tweak Films - GtoCacheEmitter", "1.11", "Any");

    status = plugin.registerNode( "GtoCacheEmitter", GtoCacheEmitter::id,
                                  &GtoCacheEmitter::creator, 
                                  &GtoCacheEmitter::initialize,
                                  MPxNode::kEmitterNode );
    if (!status) 
    {
        status.perror("registerNode");
        return status;
    }

    return status;
}

MStatus 
uninitializePlugin(MObject obj)
{
    MStatus status;
    MFnPlugin plugin(obj);

    status = plugin.deregisterNode(GtoCacheEmitter::id);

    if (!status) 
    {
        status.perror("deregisterNode");
        return status;
    }

    return status;
}

