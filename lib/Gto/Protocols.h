//******************************************************************************
// Copyright (c) 2003 Tweak Films. 
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
#ifndef __Gto__Protocols__h__
#define __Gto__Protocols__h__

#define GTO_PROTOCOL_OBJECT              "object"
#define GTO_PROTOCOL_POLYGON             "polygon"
#define GTO_PROTOCOL_NURBS               "NURBS"
#define GTO_PROTOCOL_CATMULL_CLARK       "catmull-clark"     // sub-d
#define GTO_PROTOCOL_LOOP                "loop"              // sub-d
#define GTO_PROTOCOL_TRANSFORM           "transform"
#define GTO_PROTOCOL_TEXCHANNEL          "texChannel"        // deprecated
#define GTO_PROTOCOL_PARTICLE            "particle"
#define GTO_PROTOCOL_DIFFERENCE          "difference"
#define GTO_PROTOCOL_SORTED              "sorted"
#define GTO_PROTOCOL_IMAGE               "image"

// PROPOSED
#define GTO_PROTOCOL_MATERIAL            "material"
#define GTO_PROTOCOL_GROUP               "group"

#define GTO_COMPONENT_POINTS             "points"
#define GTO_COMPONENT_SURFACE            "surface"
#define GTO_COMPONENT_INDICES            "indices"
#define GTO_COMPONENT_ELEMENTS           "elements"
#define GTO_COMPONENT_OBJECT             "object"
#define GTO_COMPONENT_MAPPINGS           "mappings"
#define GTO_COMPONENT_SMOOTHING          "smoothing"
#define GTO_COMPONENT_NORMALS            "normals"
#define GTO_COMPONENT_SHELLS             "shells"
#define GTO_COMPONENT_IMAGE              "image"

#define GTO_PROPERTY_POSITION            "position"          // float[3][]
#define GTO_PROPERTY_VELOCITY            "velocity"          // float[3][]
#define GTO_PROPERTY_MASS                "mass"              // float[1][]
#define GTO_PROPERTY_WEIGHT              "weight"            // float[1][]
#define GTO_PROPERTY_DEGREE              "degree"            // float[1][]
#define GTO_PROPERTY_UKNOTS              "uKnots"            // float[1][]
#define GTO_PROPERTY_VKNOTS              "vKnots"            // float[1][]
#define GTO_PROPERTY_URANGE              "uRange"            // float[1][]
#define GTO_PROPERTY_VRANGE              "vRange"            // float[1][]
#define GTO_PROPERTY_UFORM               "uForm"             // float[1][]
#define GTO_PROPERTY_VFORM               "vForm"             // float[1][]
#define GTO_PROPERTY_NAME                "name"              // string[1][1]
#define GTO_PROPERTY_PROTOCOL            "protocol"          // string[1][]
#define GTO_PROPERTY_PROTOCOL_VERSION    "protocolVersion"   // string[1][]
#define GTO_PROPERTY_GLOBAL_MATRIX       "globalMatrix"      // float[16][1]
#define GTO_PROPERTY_BOUNDINGBOX         "boundingBox"       // float[6][1]
#define GTO_PROPERTY_PARENT              "parent"            // string[1][1]
#define GTO_PROPERTY_TYPE                "type"              // byte[1][]
#define GTO_PROPERTY_SIZE                "size"              // multiple
#define GTO_PROPERTY_VERTEX              "vertex"            // int[1][]
#define GTO_PROPERTY_NORMAL              "normal"            // multiple
#define GTO_PROPERTY_ST                  "st"                // multiple
#define GTO_PROPERTY_METHOD              "method"            // int[1][1]
#define GTO_PROPERTY_VERTICES            "vertices"          // int[1][]
#define GTO_PROPERTY_ELEMENTS            "elements"          // int[1][]
#define GTO_PROPERTY_PIXELS              "pixels"

#define GTO_POLYGON_POLYGON              0
#define GTO_POLYGON_TRIANGLE             1
#define GTO_POLYGON_QUAD                 2
#define GTO_POLYGON_TSTRIP               3
#define GTO_POLYGON_QSTRIP               4
#define GTO_POLYGON_FAN                  5

#define GTO_SMOOTHING_METHOD_NONE           0
#define GTO_SMOOTHING_METHOD_SMOOTH         1
#define GTO_SMOOTHING_METHOD_FACETED        2
#define GTO_SMOOTHING_METHOD_PARTITIONED    3
#define GTO_SMOOTHING_METHOD_DISCONTINUOUS  4

#endif // __Gto__Protocols__h__
