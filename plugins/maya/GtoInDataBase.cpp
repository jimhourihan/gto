//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************
#include "version.h"

#include "GtoInDataBase.h"
#include "GtoInReader.h"

namespace GtoIOPlugin {

//******************************************************************************
DataBase::DataBase()
{
    // Nothing
}

//******************************************************************************
DataBase::~DataBase()
{
    std::vector<Set *>::iterator iter = m_sets.begin();
    for ( ; iter != m_sets.end(); ++iter )
    {
        delete (*iter);
    }
}

//******************************************************************************
Set *DataBase::set( const std::string &filename )
{
    Set *newSet = new Set( filename );
    m_sets.push_back( newSet );

    // Read GTO file
    {
        Reader reader( *newSet );
        if ( reader.open( filename.c_str() ) == false )
        {
            return NULL;
        }
    }

    return newSet;
}

//******************************************************************************
void DataBase::destroyAll()
{
    std::vector<Set *>::iterator iter = m_sets.begin();
    for ( ; iter != m_sets.end(); ++iter )
    {
        delete (*iter);
    }
    m_sets.erase( m_sets.begin(), m_sets.end() );
}

} // End namespace GtoIOPlugin

