//******************************************************************************
// Copyright (c) 2001-2007 Tweak Inc. All rights reserved.
//******************************************************************************

#include "GtoAnimCurve.h"
#include <Gto/Header.h>

namespace GtoIOPlugin {

// *****************************************************************************
GtoAnimCurve::GtoAnimCurve( const char *name, const char *interp ) :
    m_interp( interp ),
    m_flags( FLAG_NONE ),
    m_preInfinity( INF_CONSTANT ),
    m_postInfinity( INF_CONSTANT ),
    m_lastKey( NULL ),
    m_name( name )
{
}

std::string GtoAnimCurve::infinityStr( InfinityType inf ) const
{
    switch( inf )
    {
    case INF_CONSTANT: return "constant";
    case INF_LINEAR: return "linear";
    case INF_CYCLE: return "cycle";
    case INF_CYCLERELATIVE: return "cyclerelative";
    case INF_OSCILLATE: return "oscillate";
    default: return "unknown";
    };
}

// *****************************************************************************
void GtoAnimCurve::writeGtoHeader( Gto::Writer &writer ) const
{
    std::string interp = "hermite";
    if( isWeighted() ) interp = "bezier";
    
    interp += " pre-" + infinityStr( m_preInfinity );
    interp += " post-" + infinityStr( m_postInfinity );
    
    if( ! m_interp.empty() )
    {
        interp += " " + m_interp;
    }
    
    writer.property( m_name.c_str(), Gto::Double, 
                     m_keys.size(), 6, interp.c_str() );
}


// *****************************************************************************
void GtoAnimCurve::writeGtoData( Gto::Writer &writer ) const
{
    writer.propertyData( &m_keys.front() );
}


}  //  End namespace GtoIOPlugin
