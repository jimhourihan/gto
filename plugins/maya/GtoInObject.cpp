//
// Copyright (C) 2003 Tweak Films
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
#include "version.h"

#include <maya/MGlobal.h>
#include <maya/MFnSet.h>
#include <maya/MFn.h>
#include <maya/MSelectionList.h>
#include <maya/MStringArray.h>
#include <maya/MFnTransform.h>
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
    if ( name == GTO_COMPONENT_OBJECT )
    {
        return Request( true, ( void * )OBJECT_C );
    }

    return Request( false );
}

//******************************************************************************
Request Object::property( const std::string &name,
                          void *componentData ) const
{
    if ( (( int )componentData) == OBJECT_C )
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

} // End namespace GtoIOPlugin
