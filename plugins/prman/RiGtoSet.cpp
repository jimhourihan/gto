//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#include <RiGto/RiGtoSet.h>

using namespace TwkUtil;

namespace RiGto {

//******************************************************************************
Set::Set( const std::string &ref,
          const std::string &open,
          const std::string &close )
  : m_ref( ref ),
    m_open( open ),
    m_close( close )
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
                     const OnOffList &offList2 ) const
{
    std::vector<Object *>::const_iterator iter = m_objects.begin();
    for ( ; iter != m_objects.end(); ++iter )
    {
        if ( onList.has( (*iter)->name() ) &&
             onList2.has( (*iter)->name() ) &&
             ! offList.has( (*iter)->name() ) &&
             ! offList2.has( (*iter)->name() ) )
        {
            (*iter)->declareRi();
        }
    }
}

} // End namespace RiGto
