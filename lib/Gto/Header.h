//******************************************************************************
// Copyright (c) 2002 Tweak Films. 
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
#ifndef __Gto__Header__h__
#define __Gto__Header__h__

#ifdef __APPLE__
#ifndef PLATFORM_DARWIN
#define PLATFORM_DARWIN
#endif
#endif

namespace Gto {

//
//  Types and MACROS
//

#define GTO_MAGIC   671
#define GTO_VERSION 2

typedef unsigned int        uint32;
typedef int                 int32;
typedef unsigned short      uint16;
typedef unsigned char       uint8;
typedef float               float32;
typedef double              float64;

//
//  File Header
//

struct Header
{
    static const unsigned int Magic = 0x0000029f;
    static const unsigned int Cigam = 0x9f020000;

    uint32        magic;
    uint32        numStrings;
    uint32        numObjects;
    uint32        version;
    uint32        flags;                    // undetermined;
};

//
//  Object Header
//

struct ObjectHeader
{
    uint32        name;
    uint32        protocolName;
    uint32        protocolVersion;
    uint32        numComponents;
};

//
//  Componenent Header
//

enum ComponentFlags
{
    Transposed  = 1 << 0,
    Matrix      = 1 << 1,
};

struct ComponentHeader
{
    uint32        name;
    uint32        numProperties;
    uint32        flags;
};

//
//  Property Header
//

enum DataType
{
    Int,                // int32
    Float,              // float32
    Double,             // float64
    Half,               // float16
    String,             // string table indices
    Boolean,            // bit
    Short,              // uint16
    Byte,               // uint8

    NumberOfDataTypes
};


struct PropertyHeader
{
    uint32        name;
    uint32        size;
    uint32        type;
    uint32        width;
};

} // Gto

#endif // __Gto__Header__h__
