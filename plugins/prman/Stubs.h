//******************************************************************************
// Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
//******************************************************************************

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




