//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************
#include "version.h"

#include <iostream>
#include <maya/MMatrix.h>
#include <maya/MFnTransform.h>
#include <maya/MTransformationMatrix.h>
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


} // End namespace GtoIOPlugin
