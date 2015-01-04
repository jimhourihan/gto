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

#ifndef __Gto__Utilities__h__
#define __Gto__Utilities__h__
#include <sys/types.h>
#include <Gto/Header.h>
#include <vector>
#include <string>

namespace Gto {

struct DimStruct
{
    uint32 x;
    uint32 y;
    uint32 z;
    uint32 w;
};

inline Dimensions makeDimensions(const DimStruct& ds)
{
    return Dimensions(ds.x, ds.y, ds.y, ds.w);
}

inline DimStruct makeDimStruct(const Dimensions& d)
{
    DimStruct ds;
    ds.x = d.x;
    ds.y = d.y;
    ds.z = d.z;
    ds.w = d.w;
    return ds;
}

struct TypeSpec
{
    DataType  type;
    uint32    size;
    DimStruct dims;
};

struct Number
{
    union
    {
        int    _int;
        double _double;
    };

    DataType  type;
};

//
//  NOTE: for bufferSize() if xsize, ysize, or zsize == 0 the
//  dimension is not considered (i.e. think if it as being 1)
//

size_t dataSizeInBytes(Gto::uint32);
size_t elementSize(const TypeSpec&);
size_t elementSize(const Dimensions&);
size_t bufferSizeInBytes(Gto::uint32, const Dimensions&);
const char* typeName(Gto::DataType);
bool isNumber(Gto::DataType);
Number asNumber(void*, Gto::DataType);
bool isGTOFile(const char*);
void splitComponentName(const char* name, std::vector<std::string>& buffer);


} // Gto

#endif // __Gto__Utilities__h__
