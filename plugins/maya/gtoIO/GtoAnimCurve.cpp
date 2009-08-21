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
