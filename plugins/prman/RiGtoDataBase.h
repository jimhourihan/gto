//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _RiGtoDataBase_h_
#define _RiGtoDataBase_h_

#include <RiGto/RiGtoSet.h>
#include <string>
#include <vector>

namespace RiGto {

class DataBase
{
public:
    DataBase();
    ~DataBase();
    
    const Set *set( const std::string &rest,
                    const std::string &open,
                    const std::string &close );

    void destroyAll();
    
    void destroySet( const std::string &rest,
                     const std::string &open,
                     const std::string &close );

protected:
    std::vector<Set *> m_sets;
};

} // End namespace RiGto

#endif
