//******************************************************************************
// Copyright (c) 2004 Tweak Inc. 
// All rights reserved.
//******************************************************************************
#ifndef __GtoParticleCache__GtoCacheEmitter__h__
#define __GtoParticleCache__GtoCacheEmitter__h__
#include <TwkMaya/EmitterNode.h>
#include <Gto/RawData.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MIntArray.h>
#include <maya/MFnArrayAttrsData.h>

//----------------------------------------------------------------------

class GtoCacheEmitter : public EmitterNode
{
public:
    typedef EmitterNode  Parent;
    typedef MVectorArray VArray;
    typedef MDoubleArray DArray;
    typedef MIntArray    IArray;

    TWK_MAYA_PLUGIN_CLASS(GtoCacheEmitter);

    GtoCacheEmitter();
    virtual ~GtoCacheEmitter();
    virtual void postConstructor();
    static MStatus initialize();

    virtual MStatus compute(const MPlug&, MDataBlock&);
    
    void dataRead(Gto::Property*);
    void floatData(Gto::Property*);
    void intData(Gto::Property*);

private:
    static MObject m_gtoAttr;
    static MObject m_keepAttr;
    MObject m_output;
    size_t m_count;
};

//----------------------------------------------------------------------

class GtoCacheReader : public Gto::RawDataBaseReader
{
public:
    typedef Gto::RawDataBaseReader Parent;

    GtoCacheReader(GtoCacheEmitter* e) : m_emitter(e), Gto::RawDataBaseReader() {}
    virtual ~GtoCacheReader();

protected:
    virtual Request object(const std::string& name,
                           const std::string& protocol,
                           unsigned int protocolVersion,
                           const ObjectInfo &header);

    virtual Request component(const std::string& name,
                              const std::string& interp,
                              const ComponentInfo &header);

    virtual void dataRead(const PropertyInfo&);

    GtoCacheEmitter* m_emitter;
};


#endif // __GtoParticleCache__GtoCacheEmitter__h__
