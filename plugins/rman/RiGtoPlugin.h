//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _RiGtoPlugin_h_
#define _RiGtoPlugin_h_

#include <RiGto/RiGtoDataBase.h>
#include <RiGto/RiGtoSet.h>
#include <RiGto/RiGtoOnOffList.h>

namespace RiGto {

class Plugin
{
public:
    Plugin( DataBase &dataBase,
            const char *configStr,
            bool verbose = true );
    ~Plugin();

    void declareRi() const;
        
protected:
    DataBase &m_dataBase;
    const Set *m_set;
    OnOffList m_onList;
    OnOffList m_offList;
    OnOffList m_onList2;
    OnOffList m_offList2;
};

} // End namespace RiGto

#endif
