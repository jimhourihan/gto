//
// Copyright (C) 2004 Tweak Films
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

#include <RiGtoStub/Stubs.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ri.h>

#define STUB_OSTR (*g_riOstr)
// #define STUB_OSTR (std::cout)
#define STUB_DO_RI (g_riOstr==NULL)

namespace RiGtoStub {

//******************************************************************************
static std::ostream *g_riOstr = NULL;
static std::ofstream *g_riFileOut = NULL;

//******************************************************************************
void Stub_RiBegin( const char *fileName )
{
    if ( g_riFileOut != NULL )
    {
        delete g_riFileOut;
        g_riFileOut = NULL;
    }

    if ( fileName != NULL )
    {
        g_riFileOut = new std::ofstream( fileName );
        g_riOstr = ( std::ostream * )g_riFileOut;
    }
    else
    {
        g_riOstr = &( std::cout );
    }
}

//******************************************************************************
void Stub_RiEnd()
{
    if ( g_riFileOut != NULL )
    {
        g_riFileOut->close();
        delete g_riFileOut;
        g_riFileOut = NULL;
    }
}

//******************************************************************************
void Stub_RiAttributeBegin()
{
    if ( STUB_DO_RI )
    {
        RiAttributeBegin();
    }
    else
    {
        STUB_OSTR << "AttributeBegin" << std::endl;
    }
}

//******************************************************************************
void Stub_RiAttributeEnd()
{
    if ( STUB_DO_RI )
    {
        RiAttributeEnd();
    }
    else
    {
        STUB_OSTR << "AttributeEnd" << std::endl;
    }
}

//******************************************************************************
void Stub_RiTransformBegin()
{
    if ( STUB_DO_RI )
    {
        RiTransformBegin();
    }
    else
    {
        STUB_OSTR << "TransformBegin" << std::endl;
    }
}

//******************************************************************************
void Stub_RiTransformEnd()
{
    if ( STUB_DO_RI )
    {
        RiTransformEnd();
    }
    else
    {
        STUB_OSTR << "TransformEnd" << std::endl;
    }
}

//******************************************************************************
void Stub_DeclareObjectName( const char *nameCstr )
{
    if ( STUB_DO_RI )
    {
        RiAttribute( "identifier",
                     ( RtToken )"name",
                     ( RtPointer )(&nameCstr),
                     NULL );
    }
    else
    {
        STUB_OSTR << "Attribute \"identifier\" \"string name\" [\""
                  << nameCstr << "\"]" << std::endl;
    }
}

//******************************************************************************
void Stub_DeclareObjectRefToWorld( Stub_RtMatrix &matrix )
{
    if ( STUB_DO_RI )
    {
        RiAttribute( ( RtToken )"user",
                     ( RtToken )"float refToWorld[16]",
                     ( RtPointer )matrix,
                     NULL );
    }
    else
    {
        STUB_OSTR << "Attribute \"user\" \"float refToWorld[16]\" [";
        float *m = ( float * )matrix;
        for ( size_t i = 0; i < 16; ++i )
        {
            STUB_OSTR << m[i] << " ";
        }
        STUB_OSTR << "]" << std::endl;
    }
}

//******************************************************************************
void Stub_RiMotionBegin( float open, float close )
{
    if ( STUB_DO_RI )
    {
        RiMotionBegin( 2, open, close );
    }
    else
    {
        STUB_OSTR << "MotionBegin [" << open << " " << close << "]"
                  << std::endl;
    }
}

//******************************************************************************
void Stub_RiMotionEnd()
{
    if ( STUB_DO_RI )
    {
        RiMotionEnd();
    }
    else
    {
        STUB_OSTR << "MotionEnd" << std::endl;
    }
}

//******************************************************************************
void Stub_RiConcatTransform( Stub_RtMatrix &matrix )
{
    if ( STUB_DO_RI )
    {
        RiConcatTransform( *( ( RtMatrix * )(&matrix) ) );
    }
    else
    {
        float *m = ( float * )matrix;
        STUB_OSTR << "ConcatTransform [ ";
        for ( size_t i = 0; i < 16; ++i )
        {
            STUB_OSTR << m[i] << " ";
        }
        STUB_OSTR << " ]" << std::endl;
    }
}

//******************************************************************************
void Stub_RiTransform( Stub_RtMatrix &matrix )
{
    if ( STUB_DO_RI )
    {
        RiTransform( *( ( RtMatrix * )(&matrix) ) );
    }
    else
    {
        float *m = ( float * )matrix;
        STUB_OSTR << "Transform [ ";
        for ( size_t i = 0; i < 16; ++i )
        {
            STUB_OSTR << m[i] << " ";
        }
        STUB_OSTR << " ]" << std::endl;
    }
}

//******************************************************************************
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
                        const float *positionsRef )
{
    const int orderU = degreeU + 1;
    const int orderV = degreeV + 1;
    const int nU = knotsUSize - orderU;
    const int nV = knotsVSize - orderV;
    
    const float *newKnotsU = knotsU;
    const float *newKnotsV = knotsV;

    RtToken tokens[2];
    RtPointer pointers[2];
    int numTokens = 0;

    // Positions cannot be NULL;
    assert( positions != NULL );
    tokens[numTokens] = ( RtToken )"Pw";
    pointers[numTokens] = ( RtPointer )positions;
    ++numTokens;

    // positionsRef can be NULL;
    if ( positionsRef != NULL )
    {
        tokens[numTokens] = ( RtToken )"vertex hpoint Pref";
        pointers[numTokens] = ( RtPointer )positionsRef;
        ++numTokens;
    }

    if ( STUB_DO_RI )
    {
        RiNuPatchV( nU, orderU, ( RtFloat * )newKnotsU, minU, maxU,
                    nV, orderV, ( RtFloat * )newKnotsV, minV, maxV,
                    numTokens, tokens, pointers );
    }
    else
    {
        // Do the U
        STUB_OSTR << "NuPatch " << nU << " " << orderU << "[ ";
        for ( size_t i = 0; i < knotsUSize; ++i )
        {
            STUB_OSTR << newKnotsU[i] << " " << std::flush;
        }

        STUB_OSTR << "] " << minU << " " << maxU << std::endl;

        // Do the V
        STUB_OSTR << nV << " " << orderV << "[ ";
        for ( size_t i = 0; i < knotsVSize; ++i )
        {
            STUB_OSTR << newKnotsV[i] << " " << std::flush;
        }
        
        STUB_OSTR << "] " << minV << " " << maxV << std::endl;

        // Do Pw
        STUB_OSTR << "\"Pw\" [ " << std::flush;
        size_t numPws = 4 * nU * nV;
        for ( size_t i = 0; i < numPws; ++i )
        {
            STUB_OSTR << positions[i] << " " << std::flush;
        }
        STUB_OSTR << "]" << std::endl;

        // Do Pref, if given
        if ( positionsRef != NULL )
        {
            STUB_OSTR << "\"vertex hpoint Pref\" [ " << std::flush;
            for ( size_t i = 0; i < numPws; ++i )
            {
                STUB_OSTR << positionsRef[i] << " " << std::flush;
            }
            STUB_OSTR << "]" << std::endl;
        }
    }
}

//******************************************************************************
void Stub_DeclarePoly( const int *numVerts,
                       size_t numVertsSize,
                       const int *indices,
                       const float *positions,
                       const float *positionsRef,
                       const float *sValues,
                       const float *tValues,
                       const float *normalValues )
{
    if ( STUB_DO_RI )
    {
        // P
        // vertex point Pref
        RtToken tokens[5];
        RtPointer pointers[5];
        int numTokens = 0;

        // Must have positions
        assert( positions != NULL );
        tokens[numTokens] = ( RtToken )"P";
        pointers[numTokens] = ( RtPointer )positions;
        ++numTokens;
    
        // Normals are optional
        if( normalValues != NULL )
        {
            tokens[numTokens] = ( RtToken )"facevarying normal N";
            pointers[numTokens] = ( RtPointer )normalValues;
            ++numTokens;
        }

        // Ref positions are optional
        if ( positionsRef != NULL )
        {
            tokens[numTokens] = ( RtToken )"vertex point Pref";
            pointers[numTokens] = ( RtPointer )positionsRef;
            ++numTokens;
        }

        if ( sValues != NULL )
        {
            tokens[numTokens] = ( RtToken )"facevarying float s";
            pointers[numTokens] = ( RtPointer )sValues;
            ++numTokens;
        }

        if ( tValues != NULL )
        {
            tokens[numTokens] = ( RtToken )"facevarying float t";
            pointers[numTokens] = ( RtPointer )tValues;
            ++numTokens;
        }

        // Make the primitive.
        RiPointsPolygonsV( ( int )numVertsSize,
                           ( RtInt * )numVerts,
                           ( RtInt * )indices,
                           numTokens,
                           tokens,
                           pointers );
    }
    else
    {
        // Put the num verts per element
        size_t totalNumVerts = 0;
        STUB_OSTR << "PointsPolygons [ ";
        for ( size_t i = 0; i < numVertsSize; ++i )
        {
            size_t nv = numVerts[i];
            totalNumVerts += nv;
            STUB_OSTR << nv << " " << std::flush;
        }
        STUB_OSTR << "] " << std::endl;

        // Total num verts tells us how many
        // indices there are
        STUB_OSTR << "[ ";
        size_t maxIndex = 0;
        for ( size_t i = 0; i < totalNumVerts; ++i )
        {
            size_t index = indices[i];
            maxIndex = std::max( maxIndex, index );
            STUB_OSTR << index << " " << std::flush;
        }
        STUB_OSTR << "] " << std::endl;

        // Max index tells us how many P's there are
        STUB_OSTR << "\"P\" [ ";
        for ( size_t i = 0; i < ((maxIndex+1)*3); ++i )
        {
            STUB_OSTR << positions[i] << " " << std::flush;
        }
        STUB_OSTR << "] " << std::endl;

        // Do Pref, if it is there
        if ( positionsRef != NULL )
        {
            STUB_OSTR << "\"vertex point Pref\" [ ";
            for ( size_t i = 0; i < ((maxIndex+1)*3); ++i )
            {
                STUB_OSTR << positionsRef[i] << " " << std::flush;
            }
            STUB_OSTR << "] " << std::endl;
        }

        // Do normals, if they are there
        if ( normalValues != NULL )
        {
            // There are totalNumVerts*3 normal values,
            // because they're face varying.
            STUB_OSTR << "\"facevarying normal N\" [ ";
            for ( size_t i = 0; i < (totalNumVerts*3); ++i )
            {
                STUB_OSTR << normalValues[i] << " " << std::flush;
            }
            STUB_OSTR << "] " << std::endl;
        }

        // Do s, if they are there
        if ( sValues != NULL )
        {
            // There are totalNumVerts s values,
            // because they're face varying.
            STUB_OSTR << "\"facevarying float s\" [ ";
            for ( size_t i = 0; i < totalNumVerts; ++i )
            {
                STUB_OSTR << sValues[i] << " " << std::flush;
            }
            STUB_OSTR << "] " << std::endl;
        }

        // Do t, if they are there
        if ( tValues != NULL )
        {
            // There are totalNumVerts s values,
            // because they're face varying.
            STUB_OSTR << "\"facevarying float t\" [ ";
            for ( size_t i = 0; i < totalNumVerts; ++i )
            {
                STUB_OSTR << tValues[i] << " " << std::flush;
            }
            STUB_OSTR << "] " << std::endl;
        }
    }
}

//******************************************************************************
void Stub_DeclareSubd( const int *numVerts,
                       size_t numVertsSize,
                       const int *indices,
                       const float *positions,
                       const float *positionsRef,
                       const float *sValues,
                       const float *tValues )
{
    if ( STUB_DO_RI )
    {
        // P
        // vertex point Pref
        // facevarying float s
        // facevarying float t
        RtToken tokens[4];
        RtPointer pointers[4];
        int numTokens = 0;

        // Must have positions
        assert( positions != NULL );
        tokens[numTokens] = ( RtToken )"P";
        pointers[numTokens] = ( RtPointer )positions;
        ++numTokens;

        // Ref positions are optional
        if ( positionsRef != NULL )
        {
            tokens[numTokens] = ( RtToken )"vertex point Pref";
            pointers[numTokens] = ( RtPointer )positionsRef;
            ++numTokens;
        }

        if ( sValues != NULL )
        {
            tokens[numTokens] = ( RtToken )"facevarying float s";
            pointers[numTokens] = ( RtPointer )sValues;
            ++numTokens;
        }

        if ( tValues != NULL )
        {
            tokens[numTokens] = ( RtToken )"facevarying float t";
            pointers[numTokens] = ( RtPointer )tValues;
            ++numTokens;
        }

        // Make the primitive.
        RtToken tag = "interpolateboundary";
        RtInt numArgs[2];
        numArgs[0] = 0;
        numArgs[1] = 0;
        RtInt intArgs[1] = { 0 };
        RtFloat floatArgs[1] = { 0.0f };
        RiSubdivisionMeshV( ( RtToken )"catmull-clark",
                            ( int )numVertsSize,
                            ( RtInt * )numVerts,
                            ( RtInt * )indices,
                            1, // num tags
                            ( RtToken * )( &tag ),
                            numArgs,
                            intArgs,
                            floatArgs,
                            numTokens,
                            tokens,
                            pointers );
    }
    else
    {
        // Put the num verts per element
        size_t totalNumVerts = 0;
        STUB_OSTR << "SubdivisionMesh \"catmull-clark\" [ ";
        for ( size_t i = 0; i < numVertsSize; ++i )
        {
            size_t nv = numVerts[i];
            totalNumVerts += nv;
            STUB_OSTR << nv << " " << std::flush;
        }
        STUB_OSTR << "] " << std::endl;

        // Total num verts tells us how many
        // indices there are
        STUB_OSTR << "[ ";
        size_t maxIndex = 0;
        for ( size_t i = 0; i < totalNumVerts; ++i )
        {
            size_t index = indices[i];
            maxIndex = std::max( maxIndex, index );
            STUB_OSTR << index << " " << std::flush;
        }
        STUB_OSTR << "] " << std::endl;

        // After the indices, the other weirdness...
        // Tags
        STUB_OSTR << " [\"interpolateboundary\"] ";

        // Args, INTARGS, FLOATARGS,
        STUB_OSTR << " [0 0] [] [] " << std::endl;

        // Max index tells us how many P's there are
        STUB_OSTR << "\"P\" [ ";
        for ( size_t i = 0; i < ((maxIndex+1)*3); ++i )
        {
            STUB_OSTR << positions[i] << " " << std::flush;
        }
        STUB_OSTR << "] " << std::endl;

        // Do Pref, if it is there
        if ( positionsRef != NULL )
        {
            STUB_OSTR << "\"vertex point Pref\" [ ";
            for ( size_t i = 0; i < ((maxIndex+1)*3); ++i )
            {
                STUB_OSTR << positionsRef[i] << " " << std::flush;
            }
            STUB_OSTR << "] " << std::endl;
        }

        // Do s, if they are there
        if ( sValues != NULL )
        {
            // There are totalNumVerts s values,
            // because they're face varying.
            STUB_OSTR << "\"facevarying float s\" [ ";
            for ( size_t i = 0; i < totalNumVerts; ++i )
            {
                STUB_OSTR << sValues[i] << " " << std::flush;
            }
            STUB_OSTR << "] " << std::endl;
        }

        // Do t, if they are there
        if ( tValues != NULL )
        {
            // There are totalNumVerts s values,
            // because they're face varying.
            STUB_OSTR << "\"facevarying float t\" [ ";
            for ( size_t i = 0; i < totalNumVerts; ++i )
            {
                STUB_OSTR << tValues[i] << " " << std::flush;
            }
            STUB_OSTR << "] " << std::endl;
        }
    }
}

// *****************************************************************************
void Stub_DeclareCurves( const char *degree,
                         int ncurves,
                         const int *nverts,
                         float constantWidth,
                         const float *widths,  
                         const float *positions )
{
    if ( STUB_DO_RI )
    {
        RtToken tokens[8];
        RtPointer pointers[8];
        int numTokens = 0;

        assert( positions != NULL );
        tokens[numTokens] = ( RtToken )"P";
        pointers[numTokens] = ( RtPointer )positions;
        ++numTokens;

        if( widths != NULL )
        {
            tokens[numTokens] = ( RtToken )"varying float width";
            pointers[numTokens] = ( RtPointer )widths;
            ++numTokens;

            // For some unknown reason, 'width' isn't properly passed
            // to the shader, so we make a duplicate set here
            tokens[numTokens] = ( RtToken )"varying float curvewidth";
            pointers[numTokens] = ( RtPointer )widths;
            ++numTokens;
        }
        else
        {
            tokens[numTokens] = ( RtToken )"constantwidth";
            pointers[numTokens] = ( RtPointer )&constantWidth;
            ++numTokens;

            // For some unknown reason, 'constantwidth' isn't properly passed
            // to the shader, so we make a duplicate here
            tokens[numTokens] = ( RtToken )"constant float curvewidth";
            pointers[numTokens] = ( RtPointer )&constantWidth;
            ++numTokens;
        }

        RiCurvesV( (RtToken)degree, 
                   ncurves, (RtInt*)nverts, 
                   "nonperiodic", 
                   numTokens, tokens, pointers );
    }
    else
    {
        STUB_OSTR << "Curves \"" << degree << "\" ";
        STUB_OSTR << "[";
        for( int p = 0; p < ncurves; ++p )
        {
            STUB_OSTR << nverts[p] << " ";
        }
        STUB_OSTR << "] \"nonperiodic\" ";
        if( widths != NULL )
        {
            STUB_OSTR << "\"varying float width\" [";
            for( int p = 0; p < ncurves * 2; ++p )
            {
                STUB_OSTR << widths[p] << " ";
            }
        }
        else
        {
            STUB_OSTR << "\"float constantwidth\" [" << constantWidth;
        }
        STUB_OSTR << "] \"P\" [";
        int vptr = 0;
        for( int p = 0; p < ncurves; ++p )
        {
            for( int v = 0; v < nverts[p]; ++v )
            {
                STUB_OSTR << positions[vptr++] << " ";
                STUB_OSTR << positions[vptr++] << " ";
                STUB_OSTR << positions[vptr++] << " ";
            }
        }
        STUB_OSTR << "]\n"; 
    }
}

//******************************************************************************
extern "C" {

void Stub_Internal_FreeStrings( RtPointer twoStringsV )
{
    RtString *twoStrings = ( RtString * )twoStringsV;
    free( ( void * )( twoStrings[0] ) );
    free( ( void * )( twoStrings[1] ) );
    free( twoStringsV );
}

} // End extern "C"

//******************************************************************************
void Stub_DynamicLoad( const char *pluginName,
                       const char *cfgString,
                       Stub_RtBound &bound )
{
    if ( STUB_DO_RI )
    {
        RtString *twoStrings =
            ( RtString * )malloc( 2 * sizeof( RtString ) );
        twoStrings[0] = strdup( pluginName );
        twoStrings[1] = strdup( cfgString );
        RtBound bnd = { bound[0], bound[1], bound[2],
                        bound[3], bound[4], bound[5] };
        RiProcedural( twoStrings, bnd, RiProcDynamicLoad,
                      Stub_Internal_FreeStrings );
    }
    else
    {
        STUB_OSTR << "Procedural \"DynamicLoad\" [\""
                  << pluginName << "\" \"" << cfgString
                  << "\"] ["
                  << bound[0] << " "
                  << bound[1] << " "
                  << bound[2] << " "
                  << bound[3] << " "
                  << bound[4] << " "
                  << bound[5] << "]" << std::endl;
    }
}

} // End namespace RiGtoStub







