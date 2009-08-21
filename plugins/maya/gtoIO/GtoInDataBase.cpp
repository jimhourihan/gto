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

