//******************************************************************************
// Copyright (c) 2001-2003 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __RIGTOONOFFLIST_H__
#define __RIGTOONOFFLIST_H__

#include <vector>
#include <string>
#include <regex.h>

// Implements a very efficient regular expression based on/off list

namespace RiGto {

class OnOffList
{
public:
    OnOffList( const std::string &lst )
    {
        init( lst.c_str() );
    }
    OnOffList( const char *lst )
    {
        init( lst );
    }
    ~OnOffList();

    bool has( const std::string &str ) const;
    bool has( const char *str ) const { return has( std::string( str ) ); }

protected:
    void init( const char *str );

private:
    std::vector<regex_t> m_regexList;
};




}  //  End namespace RiGto

#endif    // End #ifdef __RIGTOONOFFLIST_H__

