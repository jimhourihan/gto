//******************************************************************************
// Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
//******************************************************************************
#include <RiGto/RiGtoOnOffList.h>
#include <TwkUtil/TwkUtilRegEx.h>
#include <stl_ext/string_algo.h>
#include <iostream>

namespace RiGto {
using namespace std;
using namespace TwkUtil;

// *****************************************************************************
void OnOffList::init( const char *inList )
{
    // Make temp string
    string lst( inList );
    
    // Split the regex into parts (or just one part if there are no '|' chars)
    vector<string> lstParts;
    stl_ext::tokenize( lstParts, lst, "|" );
    
    // Pre-compile a regex for each part
    for( int i = 0; i < lstParts.size(); ++i )
    {
        // Convert the regex into our glob-like syntax
        string globPat = GlobEx::deglobSyntax( lstParts[i].c_str() );
        
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
