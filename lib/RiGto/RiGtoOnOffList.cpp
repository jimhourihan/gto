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

#include <RiGto/RiGtoOnOffList.h>
#include <iostream>

namespace RiGto {
using namespace std;

// *****************************************************************************
static void tokenize( vector<string> &tokens,
                      const string &str,
                      string delimiters )
{
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (pos != string::npos || lastPos != string::npos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}


// *****************************************************************************
static string deglobSyntax( const char *pattern )
{
    string gpat( pattern );
    
    int lastFound = 0;
    while( gpat.find( ".", lastFound ) != gpat.npos )
    {
        gpat.replace( gpat.find( ".", lastFound ), 1, "\\." );
        lastFound = gpat.find( ".", lastFound ) + 1;
    }
    while( gpat.find( "?" ) != gpat.npos )
    {
        gpat.replace( gpat.find( "?" ), 1, "." );
    }

    lastFound = 0;
    while( gpat.find( "*", lastFound ) != gpat.npos )
    {
        gpat.replace( gpat.find( "*", lastFound ), 1, ".*" );
        lastFound = gpat.find( "*", lastFound ) + 1;
    }
    
    gpat = "^" + gpat + "$";
    
    return gpat;
}


// *****************************************************************************
void OnOffList::init( const char *inList )
{
    // Make temp string
    string lst( inList );
    
    // Split the regex into parts (or just one part if there are no '|' chars)
    vector<string> lstParts;
    tokenize( lstParts, lst, "|" );
    
    // Pre-compile a regex for each part
    for( int i = 0; i < lstParts.size(); ++i )
    {
        // Convert the regex into our glob-like syntax
        string globPat = deglobSyntax( lstParts[i].c_str() );
        
        regex_t preg;
        int status = regcomp( &preg, globPat.c_str(), 
                              REG_EXTENDED | REG_NOSUB );
        if( status != 0 )
        {
            char buf[1024];
            regerror( status, &preg, buf, 1023 );
            cerr << buf << ": " << lstParts[i] << endl;
        }
        else
        {
            m_regexList.push_back( preg );
        }
    }
    
}

// *****************************************************************************
OnOffList::~OnOffList()
{
}

// *****************************************************************************
bool OnOffList::has( const string &str ) const
{
    for( int i = 0; i < m_regexList.size(); ++i )
    {
        if( ! regexec( &(m_regexList[i]), str.c_str(), 0, NULL, 0 ) )
        {
            return true;
        }
    }
    return false;
}


}  //  End namespace RiGto
