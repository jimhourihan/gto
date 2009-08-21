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
