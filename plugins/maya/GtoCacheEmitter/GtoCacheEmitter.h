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
