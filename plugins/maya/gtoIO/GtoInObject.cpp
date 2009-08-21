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
#include "version.h"

#include <maya/MGlobal.h>
#include <maya/MFnSet.h>
#include <maya/MFn.h>
#include <maya/MSelectionList.h>
#include <maya/MStringArray.h>
#include <maya/MFnTransform.h>
#include <maya/MPlug.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MTransformationMatrix.h>
#include "GtoInObject.h"


namespace GtoIOPlugin {
using namespace std;

string Object::m_filePrefix;

//******************************************************************************
Object::Object( const std::string &name, 
                const std::string &protocol, 
                const unsigned int protocolVersion )
  : m_name( name ), 
    m_protocol( protocol ),
    m_protocolVersion( protocolVersion ),
    m_parent( "" ),
    m_globalTransform(),
    m_localTransform(),
    m_mayaObject( MObject::kNullObj ),
    m_mayaParentObject( MObject::kNullObj ),
    m_wasRenamed( false )
{
}

//******************************************************************************
Object::~Object()
{
}

//******************************************************************************
Request Object::component( const std::string &name ) const
{
    if( name == GTO_COMPONENT_OBJECT )
    {
        return Request( true, ( void * )OBJECT_C );
    }
    else if( name == GTO_COMPONENT_CHANNELS )
    {
        return Request( true, ( void * )CHANNELS_C );
    }

    return Request( false );
}

//******************************************************************************
Request Object::property( const std::string &name,
                          void *componentData ) const
{
    if( (( int )componentData) == OBJECT_C )
    {
        if ( name == GTO_PROPERTY_GLOBAL_MATRIX )
        {
            return Request( true, ( void * )OBJECT_GLOBALMATRIX_P );
        }
        else if( name == GTO_PROPERTY_PARENT )
        {
            return Request( true, (void *)OBJECT_PARENT_P );
        }
    }
    else if(( int )componentData == CHANNELS_C )
    {
        return Request( true );
    }

    return Request( false );
}

// *****************************************************************************
void *Object::data( const PropertyInfo &pinfo, 
                    size_t bytes,
                    void *componentData,
                    void *propertyData )
{
    if( (int)componentData == OBJECT_C &&
        (int)propertyData == OBJECT_GLOBALMATRIX_P )
    {
        if ( pinfo.size != 1 || pinfo.width != 16 )
        {
            string str = "Wrong number of floats for transform in object: "
                         + m_name;
            MGlobal::displayWarning( str.c_str() );
            return NULL;
        }

        m_tmpFloatData.resize( 16 );
        return &m_tmpFloatData.front();
    }
    else if( (int)componentData == OBJECT_C &&
             (int)propertyData == OBJECT_PARENT_P )
    {
        return &m_tmpIntData;
    }
    else if( (int)componentData == CHANNELS_C )
    {
        return m_textureAssignmentIds;
    }

    return NULL;
}

// *****************************************************************************
void Object::dataRead( const PropertyInfo &pinfo,
                       void *componentData,
                       void *propertyData,
                       const StringTable &strings )
{
    if ( (( int )componentData) == OBJECT_C &&
         (( int )propertyData) == OBJECT_GLOBALMATRIX_P )
    {
        setTransform( &m_tmpFloatData.front() );
        m_tmpFloatData.clear();
    }
    else if( (int)componentData == OBJECT_C &&
             (int)propertyData == OBJECT_PARENT_P )
    {
        m_parent = strings[m_tmpIntData];
    }
    else if( (int)componentData == CHANNELS_C )
    {
        TextureChannel assignment;

        assignment.mappingType = strings[m_textureAssignmentIds[0]];
        assignment.texFilename = strings[m_textureAssignmentIds[1]];

        m_textureMappings[strings[pinfo.name]] = assignment;
    }
}

//******************************************************************************
void Object::setTransform( const float *transform )
{
    for( int row = 0, i = 0; row < 4; row++ )
    {
        for( int col = 0; col < 4; col++ )
        {
           m_globalTransform[row][col] = transform[i++];
        }
    }
}

// *****************************************************************************
void Object::computeLocalTransform( const Object *parent )
{
    if( m_protocol != GTO_PROTOCOL_TRANSFORM )
    {
        return;
    }
    if( parent == NULL )
    {
        m_localTransform = m_globalTransform;
    }
    else
    {
        m_localTransform = 
            (parent->globalTransform().inverse()) * m_globalTransform;
    }
}

// *****************************************************************************
void Object::addToDefaultSG()
{
    MStatus status;
    MSelectionList selList;

    MObject initialSG;
    MGlobal::getSelectionListByName( "initialShadingGroup", selList );
    status = selList.getDependNode( 0, initialSG );
    if( status != MS::kSuccess )
    {
        status.perror( "selList.getDependNode failed" );
        return;
    }

    MFnSet fnSG( initialSG );
    status = fnSG.addMember( m_mayaObject );
    if( status != MS::kSuccess )
    {
        status.perror( "fnSG.addMember() failed" );
        return;
    }
}

// *****************************************************************************
void Object::setName()
{
    string objName = m_name;
    
    // Check for existing name in scene
    MSelectionList lst;
    MGlobal::getSelectionListByName( objName.c_str(), lst );
    if( ! lst.isEmpty() )
    {
        // Add the filename as a prefix
        objName = m_filePrefix + "_" + objName;
    }
    lst.clear();
    MGlobal::getSelectionListByName( objName.c_str(), lst );
    if( ! lst.isEmpty() )
    {
        // If it's still not empty, let maya add a number
        int numPos = objName.find_last_not_of( "0123456789" );
        objName = objName.substr( 0, numPos + 1 ) + "#";
    }

    MFnDependencyNode dnObj( m_mayaObject );
    dnObj.setName( objName.c_str() );
    if( objName != m_name )
    {
        m_wasRenamed = true;
    }
}

// *****************************************************************************
void Object::addTextureChannelAttributes()
{
    MStatus status;
    MFnDependencyNode dn( m_mayaObject );
    
    TexChannelMap::const_iterator iter = m_textureMappings.begin();
    for( ; iter != m_textureMappings.end(); ++iter )
    {
        const std::string &channelName = (*iter).first;
        MString mappingType( (*iter).second.mappingType.c_str() );
        MString texFilename( (*iter).second.texFilename.c_str() );
        
        std::string attrName = "texChan_" + channelName;
        
        MFnTypedAttribute tAttr;
        tAttr.create( attrName.c_str(), channelName.c_str(), MFnData::kString,
                     &status );
        CHECK_MSTATUS( status );
        CHECK_MSTATUS( tAttr.setStorable( true ) );
        CHECK_MSTATUS( tAttr.setConnectable( true ) );
        CHECK_MSTATUS( tAttr.setArray( true ) );
        CHECK_MSTATUS( tAttr.setUsesArrayDataBuilder( true ) );
        CHECK_MSTATUS( dn.addAttribute( tAttr.object() ) );
        
        MPlug channelPlug = dn.findPlug( tAttr.object(), &status );
        CHECK_MSTATUS( status );

        MPlug mappingPlug = channelPlug.elementByLogicalIndex( 0, &status );        
        CHECK_MSTATUS( status );
        MPlug filenamePlug = channelPlug.elementByLogicalIndex( 1, &status );        
        CHECK_MSTATUS( status );
        
        CHECK_MSTATUS( mappingPlug.setValue( mappingType ) );
        CHECK_MSTATUS( filenamePlug.setValue( texFilename ) );
    }
}

} // End namespace GtoIOPlugin
