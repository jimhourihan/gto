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

#ifndef __GTOOUT_H__
#define __GTOOUT_H__

#include <string>
#include <vector>
#include <map>
#include <set>

#include <maya/MString.h>
#include <maya/MStatus.h>
#include <maya/MAnimControl.h>

#include <Gto/Writer.h>
#include "GtoMayaAttribute.h"

namespace GtoIOPlugin {

#define GTO_TRANSFORM_VERSION 2
#define GTO_POLYGON_VERSION 2
#define GTO_CATMULL_CLARK_VERSION 2
#define GTO_NURBS_VERSION 1
#define GTO_CAMERA_VERSION 1
#define GTO_LIGHT_VERSION 1

#define GTO_MAYA_USER_PROTOCOL_ATTRIBUTE "GTO_protocol"
#define GTO_MAYA_USER_PROTOCOL_VERSION_ATTRIBUTE "GTO_protocolVersion"
#define GTO_MAYA_INTERP_BOUNDARY_ATTRIBUTE "GTO_interpBoundary"
#define GTO_MAYA_SUBD_ATTRIBUTE "GTO_subd"

#define GTO_TRANSFORM_COMPONENT_NAME "transform"

#ifdef verify
// STUPID name collision with AssertMacros.h on OSX
#undef verify
#endif


class GtoExporter
{
public:

    // object name -> attribute vector 
    typedef std::map<std::string, GtoMayaAttributes*> NodeUserAttributesMap;

    GtoExporter( MTime fs, 
                 MTime fe,
                 bool quiet,
                 double shutter,
                 bool subd,
                 bool normals,
                 bool exportST,
                 MString filename,
                 int maxRecurse,
                 bool normalize,
                 bool hidden,
                 bool verify,
                 bool isDifferenceFile,
                 bool diffPoints,
                 bool diffMatrix,
                 bool diffNormals,
                 bool allUserAttributes,
                 bool allMayaAttributes,
                 bool faceMaterials,
                 bool ascii,
                 bool exportTransformAttrs );
    ~GtoExporter();
    
    MStatus doIt();

    struct vec3 { float x,y,z; vec3(int) : x(0), y(0), z(0) {} vec3() {} };
    struct vec2 { float u,v;   vec2(int) : u(0), v(0) {}       vec2() {} };

    void output(MDagPath&, bool, int recursionLevel=-1);

    void NURBSHeader(MDagPath&);
    void NURBSData(MDagPath&);

    void CurveHeader( MDagPath & );
    void CurveData( MDagPath & );

    void CSHeader(MDagPath&);
    void CSData(MDagPath&);

    void TransformHeader(MDagPath&);
    void TransformData(MDagPath&);

    void DagHeader(MDagPath&);
    void DagData(MDagPath&);

    void PolygonHeader( MDagPath &, const char *protocol, unsigned int );
    void PolygonData( MDagPath &, const char *protocol );

    void CameraHeader( MDagPath &, int protocolVersion );
    void CameraData( MDagPath & );

    void LightHeader( MDagPath &, int protocolVersion );
    void LightData( MDagPath & );

    void TexChannelsHeader( MDagPath & );
    void TexChannelsData( MDagPath & );

    MStatus doFrame( MTime );

    bool verify();
    void getNames( MDagPath &, int );

    bool hasUserProtocol( MDagPath &dp );
    std::string userProtocol( MDagPath &dp );

    bool hasUserProtocolVersion( MDagPath &dp );
    int userProtocolVersion( MDagPath &dp );

    bool subdInterpBoundary( MDagPath &dp );
    bool isSubd( MDagPath &dp );

//--
    void findAttributes( MDagPath &, GtoMayaAttributes * );
    void findTransformAttributes( MDagPath &, GtoMayaAttributes * );
    void getAttribute( MDagPath &, const char* attrName, GtoMayaAttributes *,
                       const char* gtoComponentName=NULL );

    void attributesHeader( MDagPath & ) const;
    void attributesData( MDagPath & ) const;

    void animatedAttributesHeader( MDagPath & ) const;
    void animatedAttributesData( MDagPath & ) const;


private:
    struct TextureAssignment
    {
        std::string mappingType;
        std::string textureFile;
    };

    int m_maxRecursion;
    MString m_fileName;
    MString m_outputString;
    MString m_objectName;
    bool m_asSubD;
    bool m_normals;
    bool m_exportST;
    bool m_diffPoints;
    bool m_diffMatrix;
    bool m_diffNormals;
    bool m_isDifferenceFile;
    bool m_hasUV;
    bool m_normalize;
    bool m_verify;
    bool m_hidden;
    bool m_quiet;
    bool m_allUserAttributes;
    bool m_allMayaAttributes;
    bool m_faceMaterials;
    bool m_ascii;
    bool m_exportTransformAttrs;

    MTime m_fs;
    MTime m_fe;
    double m_shutterAngle;

    std::vector<std::string> m_allNames;

    Gto::Writer *m_writer;

    std::vector<MObject> m_objects;
    
    std::set<std::string> m_noExportAttributes;

    NodeUserAttributesMap m_attributes;
};

} // End namespace GtoIOPlugin

#endif    // End #ifdef __GTOOUT_H__
