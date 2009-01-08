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

#include <GtoContainer/StdProperties.h>

namespace GtoContainer {

//-*****************************************************************************
#define PROPERTY_INSTANCE( PTYPENAME, VALUE_TYPE, LYT, WDTH, NTRP, DFLT ) \
do                                                                        \
{                                                                         \
    Meta ## PTYPENAME * meta = new Meta ## PTYPENAME ;                    \
    metas.push_back( meta );                                              \
}                                                                         \
while( 0 )

//-*****************************************************************************
void AppendStdMetaProperties( MetaProperties &metas )
{
    //-*************************************************************************
    // The regulars
    PROPERTY_INSTANCE( DoubleProperty, double, DoubleLayout, 1,
                       GTO_INTERPRET_DEFAULT, 0.0 );
    PROPERTY_INSTANCE( FloatProperty, float, FloatLayout, 1,
                       GTO_INTERPRET_DEFAULT, 0.0f );
#ifdef GTO_SUPPORT_HALF
    PROPERTY_INSTANCE( HalfProperty, half, HalfLayout, 1,
                       GTO_INTERPRET_DEFAULT, ( half )0.0f );
#endif
    PROPERTY_INSTANCE( IntProperty, int32, IntLayout, 1,
                       GTO_INTERPRET_DEFAULT, 0 );
    PROPERTY_INSTANCE( UshortProperty, uint16, ShortLayout, 1,
                       GTO_INTERPRET_DEFAULT, 0 );
    PROPERTY_INSTANCE( ByteProperty, byte, ByteLayout, 1,
                       GTO_INTERPRET_DEFAULT, 0 );
    PROPERTY_INSTANCE( StringProperty, std::string, StringLayout, 1,
                       GTO_INTERPRET_DEFAULT, std::string( "" ) );

    //-*************************************************************************
    // The doubles
    PROPERTY_INSTANCE( Double2Property, double2, DoubleLayout, 2,
                       GTO_INTERPRET_DEFAULT, double2( 0.0 ) );
    PROPERTY_INSTANCE( Double3Property, double3, DoubleLayout, 3,
                       GTO_INTERPRET_DEFAULT, double3( 0.0 ) );
    PROPERTY_INSTANCE( Double4Property, double4, DoubleLayout, 4,
                       GTO_INTERPRET_DEFAULT, double4( 0.0 ) );
    PROPERTY_INSTANCE( Double6Property, double6, DoubleLayout, 6,
                       GTO_INTERPRET_DEFAULT, double6( 0.0 ) );
    PROPERTY_INSTANCE( Double9Property, double9, DoubleLayout, 9,
                       GTO_INTERPRET_DEFAULT, double9( 0.0 ) );
    PROPERTY_INSTANCE( Double16Property, double16, DoubleLayout, 16,
                       GTO_INTERPRET_DEFAULT, double16( 0.0 ) );

    //-*************************************************************************
    // The floats
    PROPERTY_INSTANCE( Float2Property, float2, FloatLayout, 2,
                       GTO_INTERPRET_DEFAULT, float2( 0.0 ) );
    PROPERTY_INSTANCE( Float3Property, float3, FloatLayout, 3,
                       GTO_INTERPRET_DEFAULT, float3( 0.0 ) );
    PROPERTY_INSTANCE( Float4Property, float4, FloatLayout, 4,
                       GTO_INTERPRET_DEFAULT, float4( 0.0 ) );
    PROPERTY_INSTANCE( Float6Property, float6, FloatLayout, 6,
                       GTO_INTERPRET_DEFAULT, float6( 0.0 ) );
    PROPERTY_INSTANCE( Float9Property, float9, FloatLayout, 9,
                       GTO_INTERPRET_DEFAULT, float9( 0.0 ) );
    PROPERTY_INSTANCE( Float16Property, float16, FloatLayout, 16,
                       GTO_INTERPRET_DEFAULT, float16( 0.0 ) );

    //-*************************************************************************
    // The halfs
#ifdef GTO_SUPPORT_HALF
    PROPERTY_INSTANCE( Half2Property, half2, HalfLayout, 2,
                       GTO_INTERPRET_DEFAULT, half2( 0.0 ) );
    PROPERTY_INSTANCE( Half3Property, half3, HalfLayout, 3,
                       GTO_INTERPRET_DEFAULT, half3( 0.0 ) );
    PROPERTY_INSTANCE( Half4Property, half4, HalfLayout, 4,
                       GTO_INTERPRET_DEFAULT, half4( 0.0 ) );
    PROPERTY_INSTANCE( Half6Property, half6, HalfLayout, 6,
                       GTO_INTERPRET_DEFAULT, half6( 0.0 ) );
    PROPERTY_INSTANCE( Half9Property, half9, HalfLayout, 9,
                       GTO_INTERPRET_DEFAULT, half9( 0.0 ) );
    PROPERTY_INSTANCE( Half16Property, half16, HalfLayout, 16,
                       GTO_INTERPRET_DEFAULT, half16( 0.0 ) );
#endif

    //-*************************************************************************
    // The ints
    PROPERTY_INSTANCE( Int2Property, int2, IntLayout, 2,
                       GTO_INTERPRET_DEFAULT, int2( 0 ) );
    PROPERTY_INSTANCE( Int3Property, int3, IntLayout, 3,
                       GTO_INTERPRET_DEFAULT, int3( 0 ) );
    PROPERTY_INSTANCE( Int4Property, int4, IntLayout, 4,
                       GTO_INTERPRET_DEFAULT, int4( 0 ) );
    PROPERTY_INSTANCE( Int6Property, int6, IntLayout, 6,
                       GTO_INTERPRET_DEFAULT, int6( 0 ) );

    //-*************************************************************************
    // The ushorts
    PROPERTY_INSTANCE( Ushort2Property, ushort2, ShortLayout, 2,
                       GTO_INTERPRET_DEFAULT, ushort2( 0 ) );
    PROPERTY_INSTANCE( Ushort3Property, ushort3, ShortLayout, 3,
                       GTO_INTERPRET_DEFAULT, ushort3( 0 ) );
    PROPERTY_INSTANCE( Ushort4Property, ushort4, ShortLayout, 4,
                       GTO_INTERPRET_DEFAULT, ushort4( 0 ) );
    PROPERTY_INSTANCE( Ushort6Property, ushort6, ShortLayout, 6,
                       GTO_INTERPRET_DEFAULT, ushort6( 0 ) );

    //-*************************************************************************
    // The bytes
    PROPERTY_INSTANCE( Byte2Property, byte2, ByteLayout, 2,
                       GTO_INTERPRET_DEFAULT, byte2( 0 ) );
    PROPERTY_INSTANCE( Byte3Property, byte3, ByteLayout, 3,
                       GTO_INTERPRET_DEFAULT, byte3( 0 ) );
    PROPERTY_INSTANCE( Byte4Property, byte4, ByteLayout, 4,
                       GTO_INTERPRET_DEFAULT, byte4( 0 ) );
    PROPERTY_INSTANCE( Byte6Property, byte6, ByteLayout, 6,
                       GTO_INTERPRET_DEFAULT, byte6( 0 ) );
}

} // End namespace GtoContainer
