//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _RiGtoSet_h_
#define _RiGtoSet_h_

#include <TwkUtil/TwkUtilRegEx.h>
#include <RiGto/RiGtoObject.h>
#include <RiGto/RiGtoOnOffList.h>
#include <string>
#include <vector>

namespace RiGto {

// Just a list of objects that understands regular expressions.
class Set
{
protected:
    friend class DataBase;
    Set( const std::string &ref,
         const std::string &open,
         const std::string &close );
    
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
                    const OnOffList &offList2 ) const;

    const std::string &ref() const { return m_ref; }
    const std::string &open() const { return m_open; }
    const std::string &close() const { return m_close; }

private:
    std::string m_ref;
    std::string m_open;
    std::string m_close;
    
    std::vector<Object *> m_objects;
};

} // End namespace RiGto

#endif
