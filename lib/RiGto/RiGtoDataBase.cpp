//
// Copyright (C) 2004 Tweak Films
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

#include <RiGto/RiGtoDataBase.h>
#include <RiGto/RiGtoException.h>
#include <RiGto/RiGtoReader.h>

namespace RiGto {

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
        if( (*iter) == NULL )
        {
            std::cerr << "WARNING: NULL set found in object database" 
                      << std::endl;
        }
        else
        {
            delete (*iter);
        }
    }
}

//******************************************************************************
const Set *DataBase::set( const std::string &ref,
                          const std::string &open,
                          const std::string &close )
{
    // Check to see if set was already built.
    std::vector<Set *>::iterator iter = m_sets.begin();
    for ( ; iter != m_sets.end(); ++iter )
    {
        if ( (*iter)->isMe( ref, open, close ) )
        {
            return (*iter);
        }
    }

    // If we get here, set didn't exist.
    Set *newSet = new Set( ref, open, close );
    m_sets.push_back( newSet );
    
    // Read reference file
    Reader reader( *newSet, READER_REF );
    if ( reader.open( ref.c_str() ) == false )
    {
        std::cerr << "ERROR: Couldn't open rest file '" 
                  << ref << "'" << std::endl;
        // Remove the mal-formed set
        m_sets.pop_back();
        return NULL;
    }
    newSet->doneReading( READER_REF );

    // Read open file
    if ( open != "" && open != "NULL" && open != ref )
    {
        Reader reader( *newSet, READER_OPEN );
        if ( reader.open( open.c_str() ) == false )
        {
            std::cerr << "ERROR: Couldn't open shutter-open file '" 
                      << open << "'" << std::endl;
            // Remove the mal-formed set
            m_sets.pop_back();
            return NULL;
        }
    }
    newSet->doneReading( READER_OPEN );

    // Read close file
    if ( open != "" && open != "NULL" &&
         close != "" && close != "NULL" && 
         open != ref && close != ref )
    {
        Reader reader( *newSet, READER_CLOSE );
        if ( reader.open( close.c_str() ) == false )
        {
            std::cerr << "ERROR: Couldn't open shutter-close file '" 
                      << close << "'" << std::endl;
            // Remove the mal-formed set
            m_sets.pop_back();
            return NULL;
        }
    }
    newSet->doneReading( READER_CLOSE );

    return newSet;
}

//******************************************************************************
void DataBase::destroyAll()
{
    std::vector<Set *>::iterator iter = m_sets.begin();
    for ( ; iter != m_sets.end(); ++iter )
    {
        if( (*iter) == NULL )
        {
            std::cerr << "WARNING: NULL set found in object database" 
                      << std::endl;
        }
        else
        {
            delete (*iter);
        }
    }
    m_sets.erase( m_sets.begin(), m_sets.end() );
}

//******************************************************************************
void DataBase::destroySet( const std::string &rest,
                           const std::string &open,
                           const std::string &close )
{
    std::vector<Set *>::iterator iter = m_sets.begin();
    for ( ; iter != m_sets.end(); ++iter )
    {
        if( (*iter) == NULL )
        {
            std::cerr << "WARNING: NULL set found in object database" 
                      << std::endl;
        }
        else
        {
            if ( (*iter)->isMe( rest, open, close ) )
            {
                // This is the one.
                delete (*iter);
                m_sets.erase( iter );
                return;
            }
        }
    }
}

} // End namespace RiGto

