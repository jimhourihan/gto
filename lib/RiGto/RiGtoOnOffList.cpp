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
