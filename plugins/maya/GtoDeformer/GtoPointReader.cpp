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

#include "GtoPointReader.h"
#include <Gto/Protocols.h>

using namespace std;


Gto::Reader::Request
GtoPointReader::object(const string& name,
                       const string& protocol,
                       unsigned int protocolVersion,
                       const ObjectInfo& info)
{
    //
    //  We're only handling the basic geometric types here
    //

    if (protocol == GTO_PROTOCOL_POLYGON ||
        protocol == GTO_PROTOCOL_NURBS ||
        protocol == GTO_PROTOCOL_CATMULL_CLARK ||
        protocol == GTO_PROTOCOL_LOOP)
    {
        //
        //  Create it if its not there.
        //

        ObjectData& obj = m_objectMap[name];
        return Request(true);
    }
    else
    {
        return Request(false);
    }
}

Gto::Reader::Request
GtoPointReader::component(const string& name,
                          const string& interp,
                          const ComponentInfo& info)
{
    return Request(name == "object" || name == "points");
}

Gto::Reader::Request
GtoPointReader::property(const string& name, 
                         const string& interp,
                         const PropertyInfo& info)
{
    const ComponentInfo& comp = *info.component;
    const string& compname = stringFromId(comp.name);

    return Request((name == "position"     && compname == "points") ||
                   (name == "globalMatrix" && compname == "object"));
}

void*
GtoPointReader::data(const PropertyInfo& info, size_t bytes)
{
    const ComponentInfo& comp     = *info.component;
    const ObjectInfo&    obj      = *comp.object;
    ObjectData&          data     = m_objectMap[stringFromId(obj.name)];
    string               propname = stringFromId(info.name);

    if (propname == "globalMatrix")
    {
        assert(bytes == sizeof(float) * 16);
        return data.globalMatrix.elements;
    }

    if (propname == "position")
    {
        assert(bytes == sizeof(float) * info.size * info.width);
        data.dimensions = info.width;
        data.numPoints  = info.size;
        data.points.resize(data.dimensions * data.numPoints);
        return &data.points.front();
    }

    abort();
    return 0;
}
