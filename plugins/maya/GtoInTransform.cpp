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

#include <iostream>
#include <maya/MGlobal.h>
#include <maya/MMatrix.h>
#include <maya/MFnTransform.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MAnimUtil.h>
#include "GtoInTransform.h"

namespace GtoIOPlugin {
using namespace std;

//******************************************************************************
Transform::Transform( const std::string &name, 
                      const std::string &protocol,
                      const unsigned int protocolVersion )
  : Object( name, protocol, protocolVersion )
{
    // Nothing
}

//******************************************************************************
Transform::~Transform()
{
}

//******************************************************************************
void Transform::declareMaya()
{
    MFnTransform transformNode;
    m_mayaObject = transformNode.create();
    MMatrix mm;
    for( int row = 0; row < 4; ++row )
    {
        for( int col = 0; col < 4; ++col )
        {
            mm( row, col ) = m_localTransform[col][row];
        }
    }

    transformNode.set( MTransformationMatrix( mm ) );

    // Set misc stuff...
    setName();
}

//******************************************************************************
void Transform::declareMayaDiff()
{
    MFnTransform transformNode( m_mayaObject );

    MMatrix mm;
    for( int row = 0; row < 4; ++row )
    {
        for( int col = 0; col < 4; ++col )
        {
            mm( row, col ) = m_localTransform[col][row];
        }
    }

    transformNode.set( MTransformationMatrix( mm ) );
    
    MString melCmd = std::string( "setKeyframe " + name() + ".translate;" ).c_str();
    melCmd += std::string( "setKeyframe " + name() + ".rotate;" ).c_str();
    melCmd += std::string( "setKeyframe " + name() + ".scale;" ).c_str();
    MGlobal::executeCommand( melCmd );
}



} // End namespace GtoIOPlugin
