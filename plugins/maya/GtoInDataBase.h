//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _GtoInDataBase_h_
#define _GtoInDataBase_h_

#include "GtoInSet.h"
#include <string>
#include <vector>

namespace GtoIOPlugin {

class DataBase
{
public:
    DataBase();
    ~DataBase();
    
    Set *set( const std::string &filename );

    void destroyAll();
    
protected:
    std::vector<Set *> m_sets;
};

} // End namespace GtoIOPlugin

#endif
