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

