//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _GelGtoDataBase_h_
#define _GelGtoDataBase_h_

#include <GelGto/Set.h>
#include <string>
#include <vector>

namespace GelGto {

class DataBase
{
public:
    DataBase();
    ~DataBase();
    
    const Set *set( const std::string &rest,
                    const std::string &open,
                    const std::string &close,
                    const char *stripPrefix = NULL );

    void destroyAll();
    
    void destroySet( const std::string &rest,
                     const std::string &open,
                     const std::string &close );

protected:
    std::vector<Set *> m_sets;
};

} // End namespace GelGto

#endif
