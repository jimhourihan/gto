#ifndef __GtoGraph__Imath__h__
#define __GtoGraph__Imath__h__
//
// Copyright (C) 2008 Tweak Software
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
#include <Imath/ImathVec.h>
#include <Imath/ImathColor.h>
#include <Imath/ImathQuat.h>
#include <Imath/ImathBox.h>
#include <GtoGraph/Attribute.h>

/// Delcare the most useful Imath Attributes

GTO_DECLARE_ATTR(V2f, V2f, "V2f", FloatLayout, 2, V2f());
GTO_DECLARE_ATTR(V3f, V3f, "V3f", FloatLayout, 3, V3f());
GTO_DECLARE_ATTR(V2i, V2i, "V2i", IntLayout, 2, V2i());
GTO_DECLARE_ATTR(V3i, V3i, "V3i", IntLayout, 3, V3i());
GTO_DECLARE_ATTR(V2d, V2d, "V2d", DoubleLayout, 2, V2d());
GTO_DECLARE_ATTR(V3d, V3d, "V3d", DoubleLayout, 3, V3d());
GTO_DECLARE_ATTR(V2s, V2s, "V2s", ShortLayout, 2, V2s());
GTO_DECLARE_ATTR(V3s, V3s, "V3s", ShortLayout, 3, V3s());

GTO_DECLARE_ATTR(C3f, C3f, "C3f", FloatLayout, 3, C3f());
GTO_DECLARE_ATTR(C4f, C4f, "C4f", FloatLayout, 4, C4f());
GTO_DECLARE_ATTR(C3h, C3h, "C3h", HalfLayout, 3, C3h());
GTO_DECLARE_ATTR(C4h, C4h, "C4h", HalfLayout, 4, C4h());
GTO_DECLARE_ATTR(C3c, C3c, "C3c", ByteLayout, 3, C3c());
GTO_DECLARE_ATTR(C4c, C4c, "C4c", ByteLayout, 4, C4c());

GTO_DECLARE_ATTR(M33f, M33f, "M33f", FloatLayout, 9, M33f());
GTO_DECLARE_ATTR(M44f, M44f, "M44f", FloatLayout, 16, M44f());
GTO_DECLARE_ATTR(M33d, M33d, "M33d", DoubleLayout, 9, M33d());
GTO_DECLARE_ATTR(M44d, M44d, "M44d", DoubleLayout, 16, M44d());

GTO_DECLARE_ATTR(Quatf, Quatf, "Quatf", FloatLayout, 4, Quatf());
GTO_DECLARE_ATTR(Quatd, Quatd, "Quatd", DoubleLayout, 4, Quatf());

GTO_DECLARE_ATTR(Box2f, Box2f, "Box2f", FloatLayout, 4, Box2f());
GTO_DECLARE_ATTR(Box3f, Box3f, "Box3f", FloatLayout, 6, Box3f());
GTO_DECLARE_ATTR(Box2d, Box2d, "Box2d", DoubleLayout, 4, Box2d());
GTO_DECLARE_ATTR(Box3d, Box3d, "Box3d", DoubleLayout, 6, Box3d());

#endif // __GtoGraph__Imath__h__
