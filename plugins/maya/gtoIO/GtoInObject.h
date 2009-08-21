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

#ifndef _GtoInObject_h_
#define _GtoInObject_h_
#include <maya/MObject.h>
#include <maya/MMatrix.h>
#include <string>
#include <vector>
#include <map>
#include <sys/types.h>
#include <Gto/Protocols.h>
#include <Gto/Reader.h>

namespace GtoIOPlugin {

typedef Gto::Reader::Request Request;
typedef Gto::Reader::PropertyInfo PropertyInfo;
typedef Gto::Reader::StringTable StringTable;

class Object
{
public:
    Object( const std::string &name, 
            const std::string &protocol,
            const unsigned int protocolVersion );
    virtual ~Object();

    const std::string &name() const { return m_name; }
    const std::string &parent() const { return m_parent; }
    const std::string &protocol() const { return m_protocol; }
    const MObject &mayaObject() const { return m_mayaObject; }
    const MObject &mayaParentObject() const { return m_mayaParentObject; }
    bool wasRenamed() const { return m_wasRenamed; }

    void mayaObject( MObject &o )  { m_mayaObject = o; }
    void mayaParentObject( MObject &o ) { m_mayaParentObject = o; }

    //**************************************************************************
    // READER STUFF
    enum
    {
        OBJECT_C = 1,
        CHANNELS_C,
        NEXT_C
    };

    enum
    {
        OBJECT_GLOBALMATRIX_P = 1,
        OBJECT_PARENT_P,
        NEXT_P
    };
    
    struct TextureChannel
    {
        std::string mappingType;
        std::string texFilename;
    };
    typedef std::map<std::string, TextureChannel> TexChannelMap;
        
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

    // Override to declare this as a new object to Maya
    virtual void declareMaya() {};

    // Override to modify an existing Maya object with keyframe data
    virtual void declareMayaDiff() {};
    
    void computeLocalTransform( const Object *parent );
    const MMatrix &globalTransform() const { return m_globalTransform; }

    static void filePrefix( std::string filePrefix )
                                     { m_filePrefix = filePrefix; }
    
protected:
    void setTransform( const float *transform );
    void setName();
    void addToDefaultSG();
    void addTextureChannelAttributes();
    
protected:
    std::string m_name;
    std::string m_protocol;
    unsigned int m_protocolVersion;
    std::string m_parent;

    MMatrix m_globalTransform;
    MMatrix m_localTransform;

    MObject m_mayaObject;
    MObject m_mayaParentObject;
    bool m_wasRenamed;

    static std::string m_filePrefix;
    
    std::vector<float> m_tmpFloatData;
    int m_tmpIntData;
    
    int m_textureAssignmentIds[2];
    TexChannelMap m_textureMappings;
};

} // End namespace GtoIOPlugin

#endif

