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

#ifndef __GTOOUT_H__
#define __GTOOUT_H__

#include <string>
#include <vector>

#include <maya/MString.h>
#include <maya/MStatus.h>
#include <maya/MAnimControl.h>

#include <Gto/Writer.h>

namespace GtoIOPlugin {

#define GTO_TRANSFORM_VERSION 2
#define GTO_POLYGON_VERSION 2
#define GTO_CATMULL_CLARK_VERSION 2
#define GTO_NURBS_VERSION 1
#define GTO_CAMERA_VERSION 1
#define GTO_LIGHT_VERSION 1

#define GTO_MAYA_USER_PROTOCOL_ATTRIBUTE "GTO_protocol"
#define GTO_MAYA_USER_PROTOCOL_VERSION_ATTRIBUTE "GTO_protocolVersion"

class GtoExporter
{
public:
    GtoExporter( MTime fs, 
                 MTime fe,
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
                 bool diffNormals );
    ~GtoExporter();
    
    MStatus doIt();

    struct vec3 { float x,y,z; vec3(int) : x(0), y(0), z(0) {} vec3() {} };
    struct vec2 { float u,v;   vec2(int) : u(0), v(0) {}       vec2() {} };

    void output(MDagPath&, bool, int recursionLevel=-1);

    void NURBSHeader(MDagPath&);
    void NURBSData(MDagPath&);

    void CSHeader(MDagPath&);
    void CSData(MDagPath&);

    void TransformHeader(MDagPath&);
    void TransformData(MDagPath&);

    void DagHeader(MDagPath&);
    void DagData(MDagPath&);

    void PolygonHeader( MDagPath &, const char *protocol, unsigned int );
    void PolygonData( MDagPath & );

    void CameraHeader( MDagPath &, int protocolVersion );
    void CameraData( MDagPath & );

    void LightHeader( MDagPath &, int protocolVersion );
    void LightData( MDagPath & );

    MStatus doFrame( MTime );

    bool verify();
    void getNames( MDagPath &, int );

    bool hasUserProtocol( MDagPath &dp );
    std::string userProtocol( MDagPath &dp );

    bool hasUserProtocolVersion( MDagPath &dp );
    int userProtocolVersion( MDagPath &dp );

private:
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
    
    MTime m_fs;
    MTime m_fe;
    double m_shutterAngle;

    std::vector<std::string> m_allNames;

    Gto::Writer *m_writer;

    std::vector<MObject> m_objects;
};

} // End namespace GtoIOPlugin

#endif    // End #ifdef __GTOOUT_H__
