//
// Copyright (C) 2003 Tweak Films
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
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

