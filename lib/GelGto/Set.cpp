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

#include <GelGto/Set.h>

using namespace TwkUtil;

namespace GelGto {

//******************************************************************************
Set::Set( const std::string &ref,
          const std::string &open,
          const std::string &close,
          const char *stripPrefix )
  : m_ref( ref ),
    m_open( open ),
    m_close( close ),
    m_stripPrefix( stripPrefix )
{
    // Nothing
}

//******************************************************************************
Set::~Set()
{
    std::vector<Object *>::iterator iter = m_objects.begin();
    for ( ; iter != m_objects.end(); ++iter )
    {
        delete (*iter);
    }
}

//******************************************************************************
void Set::doneReading( ReaderPhase rp )
{
    std::vector<Object *>::iterator iter = m_objects.begin();
    for ( ; iter != m_objects.end(); ++iter )
    {
        (*iter)->doneReading( rp );
    }
}

//******************************************************************************
Object *Set::object( const std::string &name )
{
    std::vector<Object *>::const_iterator iter = m_objects.begin();
    for ( ; iter != m_objects.end(); ++iter )
    {
        if ( (*iter)->name() == name )
        {
            return (*iter);
        }
    }

    return NULL;
}

//******************************************************************************
void Set::declareRi( const OnOffList &onList,
                     const OnOffList &offList,
                     const OnOffList &onList2,
                     const OnOffList &offList2,
                     GelatoAPI *_rend ) const
{
    std::vector<Object *>::const_iterator iter = m_objects.begin();
    for ( ; iter != m_objects.end(); ++iter )
    {
        if ( onList.has( (*iter)->name() ) &&
             onList2.has( (*iter)->name() ) &&
             ! offList.has( (*iter)->name() ) &&
             ! offList2.has( (*iter)->name() ) )
        {
            (*iter)->declareRi(_rend);
        }
    }
}

} // End namespace GelGto
