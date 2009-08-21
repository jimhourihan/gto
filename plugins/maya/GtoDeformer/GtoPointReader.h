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
#ifndef __GtoDeformer__GtoPointReader__h__
#define __GtoDeformer__GtoPointReader__h__
#include <string>
#include <Gto/Reader.h>

//
//  This will read the and cache the object.globalMatrix and
//  points.position properties for every polygon, sub-d, or NURBS in
//  the Gto file. The points have to be float in the file, but the
//  width can be anything greater than 2 (3D 4D, etc). You can read
//  multiple files with this reader, but the cache data will be
//  replaced each time.
//

class GtoPointReader : public Gto::Reader
{
public:
    typedef std::vector<float> PointData;

    //
    //  A matrix struct so we get default constructor to make identity
    //

    struct Matrix
    {
        Matrix()
        {
            memset(elements, 0, sizeof(float) * 16);
            elements[0]  = 1.0;
            elements[5]  = 1.0;
            elements[10] = 1.0;
            elements[15] = 1.0;
        }

        float elements[16];
    };

    struct ObjectData
    {
        Matrix      globalMatrix;
        PointData   points;
        size_t      numPoints;
        size_t      dimensions;
    };

    typedef std::map<std::string,ObjectData> ObjectMap;

    GtoPointReader() : Reader() {}

    const ObjectMap& objectMap() const { return m_objectMap; }

protected:
    virtual Request object(const std::string& name,
                           const std::string& protocol,
                           unsigned int protocolVersion,
                           const ObjectInfo &header);

    virtual Request component(const std::string& name,
                              const std::string& interp,
                              const ComponentInfo &header);
    
    virtual Request property(const std::string& name,
                             const std::string& interp,
                             const PropertyInfo &header);

    void* data(const PropertyInfo& info, size_t bytes);

private:
    ObjectMap m_objectMap;
};

#endif // __GtoDeformer__GtoPointReader__h__
