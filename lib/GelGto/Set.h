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
