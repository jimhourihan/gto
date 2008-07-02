//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _GelGtoPlugin_h_
#define _GelGtoPlugin_h_

#include <GelGto/DataBase.h>
#include <GelGto/Set.h>
#include <GelGto/OnOffList.h>
#include "gelatoapi.h"

namespace GelGto {

class Plugin
{
public:
    Plugin( DataBase &dataBase,
            const char *configStr,
            bool verbose = true );
    ~Plugin();

    void declareRi(GelatoAPI *_rend) const;
        
protected:
    DataBase &m_dataBase;
    const Set *m_set;
    OnOffList m_onList;
    OnOffList m_offList;
    OnOffList m_onList2;
    OnOffList m_offList2;
};

} // End namespace GelGto

#endif
