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

#include <iostream>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MMatrix.h>
#include <maya/MDagModifier.h>
#include <maya/MDGModifier.h>
#include <maya/MFnTransform.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>

#include "GtoInSet.h"

namespace GtoIOPlugin {
using namespace std;

//******************************************************************************
Set::Set( const std::string &fileName )
  : m_fileName( fileName )
{
    m_objects.clear();

    // Make the filename available to Objects for name collision resolution
    int start = fileName.rfind( '/' ) + 1;
    int end = fileName.find( '.' );
    string filePrefix = fileName.substr( start, end - start );
    Object::filePrefix( filePrefix );
}

//******************************************************************************
Set::~Set()
{
    std::vector<Object *>::iterator iter = m_objects.begin();
    for ( ; iter != m_objects.end(); ++iter )
    {
        delete (*iter);
    }
}

//******************************************************************************
Object *Set::object( const std::string &name )
{
    std::vector<Object *>::const_iterator iter = m_objects.begin();
    for ( ; iter != m_objects.end(); ++iter )
    {
        if ( (*iter)->name() == name )
        {
            return (*iter);
        }
    }

    return NULL;
}


// *****************************************************************************
void Set::addObject( Object *obj )
{
    m_objects.push_back( obj );
}

// *****************************************************************************
void Set::computeLocalTransforms()
{
    std::vector<Object *>::const_iterator iter = m_objects.begin();
    for ( ; iter != m_objects.end(); ++iter )
    {
        Object *parent = object( (*iter)->parent() );
        (*iter)->computeLocalTransform( parent );
    }
}

//******************************************************************************
void Set::declareMaya()
{
    std::vector<Object *>::const_iterator iter = m_objects.begin();
    for ( ; iter != m_objects.end(); ++iter )
    {
        Object *obj = (*iter);

        obj->declareMaya();
        
        if( obj->wasRenamed() )
        {
            MFnDependencyNode mobj( obj->mayaObject() );

            std::string oldName = obj->name();
            std::string newName = mobj.name().asChar();

            m_changedNames[oldName] = newName;

            std::string warningMsg = oldName;
            warningMsg += " was renamed to ";
            warningMsg += newName;
            MGlobal::displayWarning( warningMsg.c_str() );
        }
    }
}

// *****************************************************************************
void Set::reparentAll()
{
    MDagModifier dagMod;
    MDGModifier dgMod;

    std::vector<Object *>::const_iterator iter = m_objects.begin();
    for ( ; iter != m_objects.end(); ++iter )
    {
        MObject childObj = (*iter)->mayaObject();
        MObject currentParentObj = (*iter)->mayaParentObject();
        MObject newParentObj = MObject::kNullObj;
        MString parentName = (*iter)->parent().c_str();
        
        if( parentName == (*iter)->name().c_str() )
        {
            MString errMsg = (*iter)->name().c_str();
            errMsg += " has self as parent, discarding parent info";
            errMsg += "for this node.";
            MGlobal::displayWarning( errMsg );
            continue;
        }
        
        if( parentName == "" )
        {
            // This object isn't supposed to have a parent, 
            // so just leave it alone
            continue;
        }

        // Check to see if the parent node was renamed at some point
        if( m_changedNames[parentName.asChar()] != "" )
        {
            // If so, use the new name
            parentName = m_changedNames[parentName.asChar()].c_str();
        }
        
        // See if the object to which this object is supposed to be
        // parented exists in the scene...
        MStatus status;
        MSelectionList selList;
        MGlobal::getSelectionListByName( parentName, selList );
        if( ! selList.isEmpty() )
        {
            status = selList.getDependNode( 0, newParentObj );
            if( status != MS::kSuccess )
            {
                status.perror( "selList.getDependNode failed" );
                return;
            }
            
            // OK, so the real parent node exists in this scene...
            if( currentParentObj == MObject::kNullObj )
            {
                // The node doesn't already have a parent, so
                // just reparent it to the one we discovered
                dagMod.reparentNode( childObj, newParentObj );
            }
            else
            {
                // This node already has a parent, but it isn't 
                // correct.
                dagMod.reparentNode( childObj, newParentObj );
                dagMod.deleteNode( currentParentObj );
            }
            
        }
        else
        {
            // The node to which this object is supposed to be parented
            // does not exist in the scene.  If the object came with a 
            // globalMatrix property of its own, we'll create a transform
            // node with that matrix to be the missing parent.  If the object
            // does not have a globalMatrix property, we'll create an identity 
            // transform node to be the missing parent.
            string str = (*iter)->name() 
                         + string( " is missing parent node '" )
                         + string( parentName.asChar() )
                         + string( "', creating Identity Transform" );
            MGlobal::displayWarning( str.c_str() );
            
            MFnTransform transformNode;
            newParentObj = transformNode.create();
            transformNode.setName( parentName );
            dagMod.reparentNode( childObj, newParentObj );
        }
    }
}

// *****************************************************************************
//******************************************************************************
void Set::declareMayaDiff()
{
    MStatus status = MS::kSuccess;
    std::vector<Object *>::const_iterator iter = m_objects.begin();
    for ( ; iter != m_objects.end(); ++iter )
    {
        Object *obj = (*iter);
        
        MSelectionList slist;
        status = slist.add( obj->name().c_str() );
        if( ! status )
        {
            MGlobal::displayWarning( std::string( "No maya node matches: " 
                                         + obj->name() ).c_str() );
            return;
        }
        if( slist.length() > 1 )
        {
            MGlobal::displayWarning( std::string( "More than one maya node matches: " 
                                         + obj->name() ).c_str() );
        }

        MItSelectionList iter( slist );
        MObject o;
        iter.getDependNode( o );

        obj->mayaObject( o );
        obj->declareMayaDiff();
    }
}

} // End namespace GtoIOPlugin
