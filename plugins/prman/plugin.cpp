//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#include <RiGto/RiGtoPlugin.h>
#include <RiGto/RiGtoException.h>
#include <iostream>
#include <ri.h>

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
        TWK_FAKE_EXCEPTION( Exception, "Entropy gave NULL pointer"
                                       " to Subdivide()!" );
    }    
    
    (( RiGto::Plugin * )blindData)->declareRi();
}

//******************************************************************************
void Free( void *blindData )
{
    if( blindData == NULL )
    {
        TWK_FAKE_EXCEPTION( Exception, "Entropy gave a NULL pointer"
                                       " to Free()!" );
    }    
    RiGto::Plugin *plugin = ( RiGto::Plugin * )blindData;
    delete plugin;
}

} // End extern "C"
