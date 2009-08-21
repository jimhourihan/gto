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

#include <GelGto/Plugin.h>
#include <GelGto/Exception.h>
//#include <GelGtoStub/Stubs.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <ctype.h>

using namespace TwkUtil;

namespace GelGto {

//******************************************************************************
static void getString( const char *configStr,
                       int &ptr,
                       std::string &into )
{
    // We must assume that configStr is zero terminated
    size_t len = strlen( configStr );

    // Eat preceding white space
    while ( ptr < len && isspace( configStr[ptr] ) )
    {
        ++ptr;
    }

    // Eat good characters
    while ( ptr < len && !isspace( configStr[ptr] ) )
    {
        into.append( 1, configStr[ptr] );
        ++ptr;
    }
}


//******************************************************************************
Plugin::Plugin( DataBase &dataBase,
                const char *configStr,
                bool verbose )
  : m_dataBase( dataBase ),
    m_set( NULL ),
    m_onList( "*" ),
    m_offList( "__ThisCouldNeverEverMatchAnythingNoMatterWhat__" ),
    m_onList2( "*" ),
    m_offList2( "__ThisCouldNeverEverMatchAnythingNoMatterWhat__" )
{
    std::string rest( "" );
    std::string open( "" );
    std::string close( "" );
    std::string onList( "" );
    std::string offList( "" );
    std::string onList2( "" );
    std::string offList2( "" );
    std::string stripPrefix( "" );
    int configStrPtr = 0;

    // Get strings
    getString( configStr, configStrPtr, rest );
    getString( configStr, configStrPtr, open );
    getString( configStr, configStrPtr, close );
    getString( configStr, configStrPtr, onList );
    getString( configStr, configStrPtr, offList );
    getString( configStr, configStrPtr, onList2 );
    getString( configStr, configStrPtr, offList2 );
    getString( configStr, configStrPtr, stripPrefix );

    // Check for FLUSH ALL
    if ( rest == "__FLUSH__" )
    {
        m_dataBase.destroyAll();
        return;
    }

    // Check for FLUSH
    if ( onList == "__FLUSH__" )
    {
        m_dataBase.destroySet( rest, open, close );
        return;
    }

    // Make stuff.
    m_set = m_dataBase.set( rest, open, close, stripPrefix.c_str() );

    if ( onList != "" && onList != "NULL" )
    {
        m_onList = OnOffList( onList );
    }

    if ( offList != "" && offList != "NULL" )
    {
        m_offList = OnOffList( offList );
    }

    if ( onList2 != "" && onList2 != "NULL" )
    {
        m_onList2 = OnOffList( onList2 );
    }

    if ( offList2 != "" && offList2 != "NULL" )
    {
        m_offList2 = OnOffList( offList2 );
    }
    
    if( getenv( "TWK_RI_DUMP_RIB" ) )
    {
        std::cerr << "WARNING: Dumping output to '" 
                  << getenv( "TWK_RI_DUMP_RIB" )
                  << "' for debugging.  NOTHING WILL RENDER!" << std::endl;
//        GelGtoStub::Stub_RiBegin( getenv( "TWK_RI_DUMP_RIB" ) );
    }
}

//******************************************************************************
Plugin::~Plugin()
{
    if( getenv( "TWK_RI_DUMP_RIB" ) )
    {
//        GelGtoStub::Stub_RiEnd();
    }

    if ( m_set != NULL )
    {
        bool nuke = false;
        const char *noCacheEnv = getenv( "TWK_RI_GTO_NO_CACHE" );
        if ( noCacheEnv != NULL &&
             !( strcmp( noCacheEnv, "0" ) == 0 ||
                strcmp( noCacheEnv, "FALSE" ) == 0 ||
                strcmp( noCacheEnv, "False" ) == 0 ||
                strcmp( noCacheEnv, "false" ) == 0 ) )
        {
            nuke = true;
        }
        
        if ( nuke )
        {
            m_dataBase.destroySet( m_set->ref(),
                                   m_set->open(),
                                   m_set->close() );
            m_set = NULL;
        }
    }
}

//******************************************************************************
void Plugin::declareRi(GelatoAPI *_rend) const
{
    if ( m_set != NULL )
    {
        m_set->declareRi( m_onList, m_offList, m_onList2, m_offList2, _rend );
    }
}

} // End namespace GelGto

