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

#include <maya/MFnIntArrayData.h>
#include <maya/MIntArray.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MVectorArray.h>
#include <maya/MSelectionList.h>
#include <TwkMaya/Liberation.h>
#include "ParticleAttribute.h"

// *****************************************************************************
ParticleAttribute::ParticleAttribute( const std::string &attrName, 
                                      const std::string &objectName )
    : m_attrName( attrName ), m_objectName( objectName )
{
}


// *****************************************************************************
ParticleAttribute::~ParticleAttribute()
{
}


// *****************************************************************************
MPlug ParticleAttribute::attrPlug() const
{
    MStatus status = MS::kSuccess;
    MSelectionList slist;
    slist.add( m_objectName.c_str() );

    MObject obj;
    status = slist.getDependNode( 0, obj );
    CHECK( status );

    MFnDependencyNode dn( obj, &status );
    CHECK( status );
    
    MPlug attrPlug = dn.findPlug( m_attrName.c_str(), &status );
    CHECK( status );
    
    return attrPlug;
}


// *****************************************************************************
const std::string &ParticleAttribute::name() const
{
    return m_attrName;
}


// *****************************************************************************
MFnData::Type ParticleAttribute::type() const
{
    MStatus status = MS::kSuccess;

    MObject attrObj = attrPlug().attribute( &status );
    CHECK( status );

    MFnTypedAttribute attrTA( attrObj, &status );
    CHECK( status );

    MFnData::Type attrType = attrTA.attrType( &status );
    CHECK( status );

    return attrType;
}


// *****************************************************************************
size_t ParticleAttribute::numElements() const
{
    MStatus status;
    MObject data;
    status = attrPlug().getValue( data );
    CHECK( status );

    if( type() == MFnData::kDoubleArray )
    {
        MFnDoubleArrayData darrData( data, &status );
        CHECK( status );
        MDoubleArray darr = darrData.array( &status );
        CHECK( status );

        return darr.length();
    }
    else if( type() == MFnData::kIntArray )
    {
        MFnIntArrayData iarrData( data, &status );
        CHECK( status );
        MIntArray iarr = iarrData.array( &status );
        CHECK( status );

        return iarr.length();
    }
    else if( type() == MFnData::kVectorArray )
    {
        MFnVectorArrayData varrData( data, &status );
        CHECK( status );
        MVectorArray varr = varrData.array( &status );
        CHECK( status );

        return varr.length();
    }
    else
    {
        std::cerr << "Invalid data type: " << type() << std::endl;
        return 0;
    }
}


// *****************************************************************************
size_t ParticleAttribute::partsPerElement() const
{
    if( type() == MFnData::kVectorArray )
    {
        return 3;
    }
    return 1;        
}


// *****************************************************************************
void ParticleAttribute::writeGtoHeader( Gto::Writer *writer )
{
    Gto::DataType gtoType = Gto::Float;
    if( type() == MFnData::kIntArray || m_attrName == "id" )
    {
        gtoType = Gto::Int;
    }
    writer->property( m_attrName.c_str(), gtoType, 
                      numElements(), partsPerElement() );
}


// *****************************************************************************
void ParticleAttribute::writeGtoData( Gto::Writer *writer )
{
    MStatus status;
    MObject data;
    status = attrPlug().getValue( data );
    CHECK( status );

    if( type() == MFnData::kDoubleArray )
    {
        MFnDoubleArrayData darrData( data, &status );
        CHECK( status );
        MDoubleArray darr = darrData.array( &status );
        CHECK( status );
        
        // "id" is a special case; it's a kDoubleArray but
        // we want to export it as Ints
        if( m_attrName == "id" )
        {
            std::vector<int> ids( darr.length() );
            for( int i = 0; i < darr.length(); ++i )
            {
                ids[i] = (int)darr[i];
            }
            writer->propertyData( &ids.front() );
        }
        else
        {
            std::vector<float> floats( darr.length() );
            for( int i = 0; i < darr.length(); ++i )
            {
                floats[i] = (float)darr[i];
            }
            writer->propertyData( &floats.front() );
        }
    }
    else if( type() == MFnData::kIntArray )
    {
        MFnIntArrayData iarrData( data, &status );
        CHECK( status );
        MIntArray iarr = iarrData.array( &status );
        CHECK( status );
        std::vector<int> ints( iarr.length() );
        for( int i = 0; i < iarr.length(); ++i )
        {
            ints[i] = iarr[i];
        }
        writer->propertyData( &ints.front() );
    }
    else if( type() == MFnData::kVectorArray )
    {
        MFnVectorArrayData varrData( data, &status );
        CHECK( status );
        MVectorArray varr = varrData.array( &status );
        CHECK( status );
        std::vector<float> floats( varr.length() * 3 );
        for( int i = 0, f = 0; i < varr.length(); ++i )
        {
            floats[f++] = varr[i].x;
            floats[f++] = varr[i].y;
            floats[f++] = varr[i].z;
        }
        writer->propertyData( &floats.front() );
    }
    else
    {
        std::cerr << "Invalid data type: " << type() << std::endl;
    }
}
