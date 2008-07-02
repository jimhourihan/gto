//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _GelGtoSet_h_
#define _GelGtoSet_h_

#include <TwkUtil/TwkUtilRegEx.h>
#include <GelGto/Object.h>
#include <GelGto/OnOffList.h>
#include <string>
#include <vector>

#include "gelatoapi.h"

namespace GelGto {

// Just a list of objects that understands regular expressions.
class Set
{
protected:
    friend class DataBase;
    Set( const std::string &ref,
         const std::string &open,
         const std::string &close,
         const char *stripPrefix = NULL );
    
    ~Set();
    
    bool isMe( const std::string &ref,
               const std::string &open,
               const std::string &close ) const
    {
        return ( m_ref == ref &&
                 m_open == open &&
                 m_close == close );
    }

    void doneReading( ReaderPhase rp );

public:
    void addObject( Object *obj ) { m_objects.push_back( obj ); }

    Object *object( const std::string &name );

    void declareRi( const OnOffList &onList,
                    const OnOffList &offList,
                    const OnOffList &onList2,
                    const OnOffList &offList2,
                    GelatoAPI *_rend ) const;

    const std::string &ref() const { return m_ref; }
    const std::string &open() const { return m_open; }
    const std::string &close() const { return m_close; }
    const char *stripPrefix() const { return m_stripPrefix; }

private:
    std::string m_ref;
    std::string m_open;
    std::string m_close;
    const char *m_stripPrefix;
    
    std::vector<Object *> m_objects;
};

} // End namespace GelGto

#endif
