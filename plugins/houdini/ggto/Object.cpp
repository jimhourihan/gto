//******************************************************************************
// Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
//******************************************************************************

#include <Gto/Protocols.h>
#include "Object.h"

namespace HGto {

// *****************************************************************************
Object::Object( std::string name, std::string protocol, int protoVersion ) :
    m_name( name ),
    m_protocol( protocol ),
    m_protoVersion( protoVersion )
{
    m_globalMatrix.makeIdentity();
    m_boundingBox.makeInfinite();
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
