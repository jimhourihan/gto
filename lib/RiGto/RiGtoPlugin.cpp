//
// Copyright (C) 2004 Tweak Films
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

#include <RiGto/RiGtoPlugin.h>
#include <RiGto/RiGtoException.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <ctype.h>

namespace RiGto {

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
    static const int majorVersion = 1;
    static const int minorVersion = 17;
    static bool printed = false;

    if ( !printed && verbose )
    {
        printf( "RiGto Entropy Plugin\n"
                "Copyright (c) 2002-2003, Tweak Inc. All rights reserved.\n"
                "Version %d.%04d, built on %s at %s\n",
                majorVersion, minorVersion, __DATE__ , __TIME__ );
        printed = true;
    }
    
    std::string rest( "" );
    std::string open( "" );
    std::string close( "" );
    std::string onList( "" );
    std::string offList( "" );
    std::string onList2( "" );
    std::string offList2( "" );
    std::string flags( "" );
    int configStrPtr = 0;

    // Get strings
    getString( configStr, configStrPtr, rest );
    getString( configStr, configStrPtr, open );
    getString( configStr, configStrPtr, close );
    getString( configStr, configStrPtr, onList );
    getString( configStr, configStrPtr, offList );
    getString( configStr, configStrPtr, onList2 );
    getString( configStr, configStrPtr, offList2 );
    getString( configStr, configStrPtr, flags );

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
    m_set = m_dataBase.set( rest, open, close );

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
}

//******************************************************************************
Plugin::~Plugin()
{
    // Nothing!
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
void Plugin::declareRi() const
{
    if ( m_set != NULL )
    {
        m_set->declareRi( m_onList, m_offList, m_onList2, m_offList2 );
    }
}

} // End namespace RiGto

