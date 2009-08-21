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

#ifndef _RiGtoStubs_Stubs_h_
#define _RiGtoStubs_Stubs_h_

#include <sys/types.h>

namespace RiGtoStub {

typedef float Stub_RtMatrix[4][4];
typedef float Stub_RtBound[6];

void Stub_RiBegin( const char *fileName );
void Stub_RiEnd();

void Stub_RiAttributeBegin();
void Stub_RiAttributeEnd();

void Stub_RiTransformBegin();
void Stub_RiTransformEnd();

void Stub_DeclareObjectName( const char *name );
void Stub_DeclareObjectRefToWorld( Stub_RtMatrix &matrix );

void Stub_RiMotionBegin( float open, float close );
void Stub_RiMotionEnd();

void Stub_RiConcatTransform( Stub_RtMatrix &matrix );
void Stub_RiTransform( Stub_RtMatrix &matrix );

// From NURBS
void Stub_DeclareNURBS( const float *knotsU,
                        size_t knotsUSize,
                        int degreeU,
                        float minU,
                        float maxU,

                        const float *knotsV,
                        size_t knotsVSize,
                        int degreeV,
                        float minV,
                        float maxV,

                        const float *positions,
                        const float *positionsRef );

//******************************************************************************
void Stub_DeclarePoly( const int *numVerts,
                       size_t numVertsSize,
                       const int *indices,
                       const float *positions,
                       const float *positionsRef,
                       const float *sValues,
                       const float *tValues,
                       const float *normalValues );

//******************************************************************************
void Stub_DeclareSubd( const int *numVerts,
                       size_t numVertsSize,
                       const int *indices,
                       const float *positions,
                       const float *positionsRef,
                       const float *sValues,
                       const float *tValues );

//******************************************************************************
void Stub_DynamicLoad( const char *pluginName,
                       const char *cfgString,
                       Stub_RtBound &bounds );

// *****************************************************************************
void Stub_DeclareCurves( const char *degree,
                         int ncurves,
                         const int *nverts,
                         float constantWidth,  // used iff widths == NULL
                         const float *widths,   
                         const float *positions );

} // End namespace RiGtoStub

#endif




