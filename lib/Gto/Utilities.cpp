//**********************************************************************
// Copyright (c) 2003 Tweak Films
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
#include <Gto/Utilities.h>

namespace Gto {

static unsigned int Csizes[] = 
{
    Int,        sizeof(int32),
    Float,      sizeof(float32),
    Double,     sizeof(float64),
    Half,       sizeof(float32) / 2,
    String,     sizeof(uint32),
    Boolean,    sizeof(uint8), 
    Short,      sizeof(uint16),
    Byte,       sizeof(uint8)
};

size_t
dataSize(Gto::uint32 type)
{
    return Csizes[type * 2 + 1];
}


} // Gto
