//******************************************************************************
// Copyright (c) 2007 Tweak Inc. 
// All rights reserved.
//******************************************************************************

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
