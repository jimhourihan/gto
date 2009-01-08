//-*****************************************************************************
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
//-*****************************************************************************

#ifndef _GtoContainer_StdProperties_h_
#define _GtoContainer_StdProperties_h_

#include <GtoContainer/Component.h>
#include <GtoContainer/Exception.h>
#include <GtoContainer/Property.h>
#include <GtoContainer/TypedProperty.h>
#include <GtoContainer/PropertyContainer.h>
#include <GtoContainer/Foundation.h>
#include <string>
#include <vector>

namespace GtoContainer {

//-*****************************************************************************
#define PROPERTY_DECLARE( PTYPENAME, VALUE_TYPE, LAYOUT, WIDTH, INTERP, DFLT ) \
struct PTYPENAME ## _Traits                                             \
{                                                                       \
    typedef std::vector<VALUE_TYPE> container_type;                     \
    typedef VALUE_TYPE value_type;                                      \
    static inline Layout layout() { return LAYOUT; }                    \
    static inline size_t width() { return WIDTH; }                      \
    static inline std::string interpretation()                          \
    { return std::string( INTERP ); }                                   \
    static inline VALUE_TYPE defaultValue() { return DFLT ; }           \
};                                                                      \
                                                                        \
typedef TypedProperty< PTYPENAME ## _Traits > PTYPENAME;                \
typedef TypedMetaProperty< PTYPENAME > Meta ## PTYPENAME ;

//-*****************************************************************************
//-*****************************************************************************

//-*****************************************************************************
// The regulars
PROPERTY_DECLARE( DoubleProperty, double, DoubleLayout, 1,
                  GTO_INTERPRET_DEFAULT, 0.0 );
PROPERTY_DECLARE( FloatProperty, float, FloatLayout, 1,
                  GTO_INTERPRET_DEFAULT, 0.0f );
#ifdef GTO_SUPPORT_HALF
PROPERTY_DECLARE( HalfProperty, half, HalfLayout, 1,
                  GTO_INTERPRET_DEFAULT, ( half )0.0f );
#endif
PROPERTY_DECLARE( IntProperty, int32, IntLayout, 1,
                  GTO_INTERPRET_DEFAULT, 0 );
PROPERTY_DECLARE( UshortProperty, uint16, ShortLayout, 1,
                  GTO_INTERPRET_DEFAULT, 0 );
PROPERTY_DECLARE( ByteProperty, byte, ByteLayout, 1,
                  GTO_INTERPRET_DEFAULT, 0 );
PROPERTY_DECLARE( StringProperty, std::string, StringLayout, 1,
                  GTO_INTERPRET_DEFAULT, std::string( "" ) );

//-*****************************************************************************
// The doubles
PROPERTY_DECLARE( Double2Property, double2, DoubleLayout, 2,
                  GTO_INTERPRET_DEFAULT, double2( 0.0 ) );
PROPERTY_DECLARE( Double3Property, double3, DoubleLayout, 3,
                  GTO_INTERPRET_DEFAULT, double3( 0.0 ) );
PROPERTY_DECLARE( Double4Property, double4, DoubleLayout, 4,
                  GTO_INTERPRET_DEFAULT, double4( 0.0 ) );
PROPERTY_DECLARE( Double6Property, double6, DoubleLayout, 6,
                  GTO_INTERPRET_DEFAULT, double6( 0.0 ) );
PROPERTY_DECLARE( Double9Property, double9, DoubleLayout, 9,
                  GTO_INTERPRET_DEFAULT, double9( 0.0 ) );
PROPERTY_DECLARE( Double16Property, double16, DoubleLayout, 16,
                  GTO_INTERPRET_DEFAULT, double16( 0.0 ) );

//-*****************************************************************************
// The floats
PROPERTY_DECLARE( Float2Property, float2, FloatLayout, 2,
                  GTO_INTERPRET_DEFAULT, float2( 0.0 ) );
PROPERTY_DECLARE( Float3Property, float3, FloatLayout, 3,
                  GTO_INTERPRET_DEFAULT, float3( 0.0 ) );
PROPERTY_DECLARE( Float4Property, float4, FloatLayout, 4,
                  GTO_INTERPRET_DEFAULT, float4( 0.0 ) );
PROPERTY_DECLARE( Float6Property, float6, FloatLayout, 6,
                  GTO_INTERPRET_DEFAULT, float6( 0.0 ) );
PROPERTY_DECLARE( Float9Property, float9, FloatLayout, 9,
                  GTO_INTERPRET_DEFAULT, float9( 0.0 ) );
PROPERTY_DECLARE( Float16Property, float16, FloatLayout, 16,
                  GTO_INTERPRET_DEFAULT, float16( 0.0 ) );

//-*****************************************************************************
// The halfs
#ifdef GTO_SUPPORT_HALF
PROPERTY_DECLARE( Half2Property, half2, HalfLayout, 2,
                  GTO_INTERPRET_DEFAULT, half2( 0.0 ) );
PROPERTY_DECLARE( Half3Property, half3, HalfLayout, 3,
                  GTO_INTERPRET_DEFAULT, half3( 0.0 ) );
PROPERTY_DECLARE( Half4Property, half4, HalfLayout, 4,
                  GTO_INTERPRET_DEFAULT, half4( 0.0 ) );
PROPERTY_DECLARE( Half6Property, half6, HalfLayout, 6,
                  GTO_INTERPRET_DEFAULT, half6( 0.0 ) );
PROPERTY_DECLARE( Half9Property, half9, HalfLayout, 9,
                  GTO_INTERPRET_DEFAULT, half9( 0.0 ) );
PROPERTY_DECLARE( Half16Property, half16, HalfLayout, 16,
                  GTO_INTERPRET_DEFAULT, half16( 0.0 ) );
#endif

//-*****************************************************************************
// The ints
PROPERTY_DECLARE( Int2Property, int2, IntLayout, 2,
                  GTO_INTERPRET_DEFAULT, int2( 0 ) );
PROPERTY_DECLARE( Int3Property, int3, IntLayout, 3,
                  GTO_INTERPRET_DEFAULT, int3( 0 ) );
PROPERTY_DECLARE( Int4Property, int4, IntLayout, 4,
                  GTO_INTERPRET_DEFAULT, int4( 0 ) );
PROPERTY_DECLARE( Int6Property, int6, IntLayout, 6,
                  GTO_INTERPRET_DEFAULT, int6( 0 ) );

//-*****************************************************************************
// The ushorts
PROPERTY_DECLARE( Ushort2Property, ushort2, ShortLayout, 2,
                  GTO_INTERPRET_DEFAULT, ushort2( 0 ) );
PROPERTY_DECLARE( Ushort3Property, ushort3, ShortLayout, 3,
                  GTO_INTERPRET_DEFAULT, ushort3( 0 ) );
PROPERTY_DECLARE( Ushort4Property, ushort4, ShortLayout, 4,
                  GTO_INTERPRET_DEFAULT, ushort4( 0 ) );
PROPERTY_DECLARE( Ushort6Property, ushort6, ShortLayout, 6,
                  GTO_INTERPRET_DEFAULT, ushort6( 0 ) );

//-*****************************************************************************
// The bytes
PROPERTY_DECLARE( Byte2Property, byte2, ByteLayout, 2,
                  GTO_INTERPRET_DEFAULT, byte2( 0 ) );
PROPERTY_DECLARE( Byte3Property, byte3, ByteLayout, 3,
                  GTO_INTERPRET_DEFAULT, byte3( 0 ) );
PROPERTY_DECLARE( Byte4Property, byte4, ByteLayout, 4,
                  GTO_INTERPRET_DEFAULT, byte4( 0 ) );
PROPERTY_DECLARE( Byte6Property, byte6, ByteLayout, 6,
                  GTO_INTERPRET_DEFAULT, byte6( 0 ) );

#undef PROPERTY_DECLARE

//-*****************************************************************************
//-*****************************************************************************
// THIS FUNCTION APPENDS METAPROPERTIES, CORRELATING TO THE STANDARD PROPERTIES
// TO A LIST OF METAPROPERTIES.
void AppendStdMetaProperties( MetaProperties &mp );

} // End namespace GtoContainer

#endif 
