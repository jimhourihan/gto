//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************
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
void *Object::component( const std::string &name ) const
{
    if ( name == GTO_COMPONENT_OBJECT )
    {
        return ( void * )OBJECT_C;
    }

    return 0;
}

//******************************************************************************
void *Object::property( const std::string &name,
                        void *componentData ) const
{
    if ( (( int )componentData) == OBJECT_C )
    {
        if ( name == GTO_PROPERTY_GLOBAL_MATRIX )
        {
            return ( void * )OBJECT_GLOBALMATRIX_P;
        }
        else if( name == GTO_PROPERTY_PARENT )
        {
            return (void *)OBJECT_PARENT_P;
        }
    }

    return 0;
}

//******************************************************************************
void Object::data( void *componentData,
                   void *propertyData,
                   const float *items,
                   size_t numItems,
                   size_t width)
{
    if ( (( int )componentData) == OBJECT_C &&
         (( int )propertyData) == OBJECT_GLOBALMATRIX_P )
    {
        if ( numItems != 1 || width != 16 )
        {
            string str = "Wrong number of floats for transform in object: "
                         + m_name;
            MGlobal::displayWarning( str.c_str() );
            return;
        }
        
        setTransform( items );
    }
}

//******************************************************************************
void Object::data( void *componentData,
                   void *propertyData,
                   const double *items,
                   size_t numItems,
                   size_t width)
{
    // Nothing
}

//******************************************************************************
void Object::data( void *componentData,
                   void *propertyData,
                   const int *items,
                   size_t numItems,
                   size_t width)
{
    // Nothing
}

//******************************************************************************
void Object::data( void *componentData,
                   void *propertyData,
                   const std::vector<std::string> &items,
                   size_t numItems,
                   size_t width)
{
    if ( (( int )componentData) == OBJECT_C &&
         (( int )propertyData) == OBJECT_PARENT_P )
    {
        m_parent = items[0];
    }
}

//******************************************************************************
void Object::data( void *componentData,
                   void *propertyData,
                   const unsigned short *items,
                   size_t numItems,
                   size_t width)
{
    // Nothing
}

//******************************************************************************
void Object::data( void *componentData,
                   void *propertyData,
                   const unsigned char *items,
                   size_t numItems,
                   size_t width)
{
    // Nothing
}

//******************************************************************************
void Object::setTransform( const float *transform )
{
    for (int row = 0, i=0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
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
