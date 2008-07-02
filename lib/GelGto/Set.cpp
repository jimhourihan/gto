//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

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
