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

#include <RiGto/RiGtoPlugin.h>
#include <RiGto/RiGtoException.h>
#include <iostream>

extern "C" {

//******************************************************************************
void *ConvertParameters( const char *initialData )
{
    static RiGto::DataBase *globalDataBase = NULL;

    void *ret = NULL;
    if ( globalDataBase == NULL )
    {
        globalDataBase = new RiGto::DataBase();
    }

    ret = ( void * )( new RiGto::Plugin( *globalDataBase,
                                         initialData ) );

    if( ret == NULL )
    {
        TWK_FAKE_EXCEPTION( Exception, "Unable to create plugin instance" );
    }
    return ret;
}


//******************************************************************************
void Subdivide( void *blindData, float detailSize )
{
    if( blindData == NULL )
    {
        TWK_FAKE_EXCEPTION( Exception, "Renderer gave NULL pointer"
                                       " to Subdivide()!" );
    }    
    
    (( RiGto::Plugin * )blindData)->declareRi();
}

//******************************************************************************
void Free( void *blindData )
{
    if( blindData == NULL )
    {
        TWK_FAKE_EXCEPTION( Exception, "Renderer gave a NULL pointer"
                                       " to Free()!" );
    }    
    RiGto::Plugin *plugin = ( RiGto::Plugin * )blindData;
    delete plugin;
}

} // End extern "C"
