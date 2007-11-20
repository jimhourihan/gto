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
#include <assert.h>
#include <fstream>
#ifdef GTO_SUPPORT_ZIP
#include <zlib.h>
#endif
/* AJG - this should already be here? */
#ifdef HAVE_HALF
#include <half.h>
#endif

namespace Gto {
using namespace std;

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

const char* 
typeName(Gto::DataType t)
{
    switch (t)
    {
      case Float:   return "float";
      case Boolean:   return "bool";
      case Double:   return "double";
      case Int:   return "int";
      case Short:   return "short";
      case Byte:   return "byte";
      case String:   return "string";
      case Half:   return "half";
      default:
			assert(false);
			/* AJG - needed a return value */
			return "";
	}
}

bool 
isNumber(Gto::DataType t)
{
    return t != String && t != ErrorType;
}

Number 
asNumber(void* data, Gto::DataType t)
{
    Number n;
    n.type = t;

    switch (t)
    {
      case Float: 
          n._double = *reinterpret_cast<float*>(data); 
          n.type = Float;
          break;
      case Double: 
          n._double = *reinterpret_cast<double*>(data);
          n.type = Float;
          break;
#ifdef HAVE_HALF
      case Half: 
          n._double = double(*reinterpret_cast<half*>(data));
          n.type = Float;
          break;
#endif
      case Int: 
          n._int = int(*reinterpret_cast<int*>(data));
          n.type = Int;
          break;
      case Short: 
          n._int = int(*reinterpret_cast<short*>(data));
          n.type = Int;
          break;
      case Byte: 
          n._int = int(*reinterpret_cast<unsigned char*>(data));
          n.type = Int;
          break;
      default:
          n.type = ErrorType;
    }

    return n;
}

bool isGTOFile(const char* infile)
{
    Header header;

#ifdef GTO_SUPPORT_ZIP
    if (gzFile file = gzopen(infile, "rb"))
    {
        if (gzread(file, &header, sizeof(header)) != sizeof(Header))
        {
            gzclose(file);
            return false;
        }
    }
#else
    ifstream file(infile);
    if (!file) return false;
    if (file.readsome((char*)&header, sizeof(Header)) != sizeof(Header)) return false;
    if (file.fail()) return false;
#endif

    return  header.magic == GTO_MAGIC ||
            header.magic == GTO_MAGICl ||
            header.magic == GTO_MAGIC_TEXT ||
            header.magic == GTO_MAGIC_TEXTl;
}

} // Gto
