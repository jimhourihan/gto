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

#include <Gto/Protocols.h>
#include "Object.h"

namespace HGto {

// *****************************************************************************
Object::Object( std::string name, std::string protocol, int protoVersion ) :
    m_name( name ),
    m_protocol( protocol ),
    m_protoVersion( protoVersion )
{
    m_globalMatrix.identity();
    m_boundingBox.initBounds();
}


// *****************************************************************************
Object::~Object()
{
}


// *****************************************************************************
void Object::writeHeader( Gto::Writer &writer )
{
    writer.beginComponent( GTO_COMPONENT_OBJECT );
    writer.property( GTO_PROPERTY_GLOBAL_MATRIX, Gto::Float, 1, 16 );
    writer.property( GTO_PROPERTY_BOUNDINGBOX, Gto::Float, 1, 6 );
    writer.endComponent();
}


// *****************************************************************************
void Object::writeData( Gto::Writer &writer )
{
    writer.propertyData( (void *)&m_globalMatrix );
    writer.propertyData( (void *)&m_boundingBox );
}


}  //  End namespace HGto
