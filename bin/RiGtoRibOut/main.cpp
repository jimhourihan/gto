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

#include <RiGto/RiGtoPlugin.h>
#include <RiGto/RiGtoException.h>
#include <RiGtoStub/Stubs.h>
#include <iostream>

//******************************************************************************
using namespace RiGto;
using namespace RiGtoStub;

//******************************************************************************
void *ConvertParameters( const char *initialData )
{
    static DataBase *globalDataBase = NULL;

    void *ret = NULL;
    if ( globalDataBase == NULL )
    {
        globalDataBase = new DataBase();
    }

    ret = ( void * )( new Plugin( *globalDataBase,
                                  initialData, false ) );

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
    
    (( Plugin * )blindData)->declareRi();
}

//******************************************************************************
void Free( void *blindData )
{
    if( blindData == NULL )
    {
        TWK_FAKE_EXCEPTION( Exception, "Entropy gave a NULL pointer"
                            " to Free()!" );
    }    
    Plugin *plugin = ( Plugin * )blindData;
    delete plugin;
}

//******************************************************************************
int main( int argc, char *argv[] )
{
    if ( argc < 2 )
    {
        std::cout << "USAGE: " << argv[0] << " cfg cfg cfg ... "
                  << std::endl;
        return -1;
    }

    std::string cfg( "" );
    for ( size_t i = 1; i < argc; ++i )
    {
        cfg += std::string( argv[i] );
        cfg += std::string( " " );
    }
    // std::cout << "Using cfg string: " << cfg << std::endl;

    // Open out file
    // std::cout << "RiBegin" << std::endl;
    Stub_RiBegin( NULL );

    // Do the thing
    // std::cout << "Convert Parameters" << std::endl;
    void *data = ConvertParameters( cfg.c_str() );

    // std::cout << "Subdivide" << std::endl;
    Subdivide( data, 1.0f );

    // std::cout << "Free" << std::endl;
    Free( data );

    // Close the file.
    Stub_RiEnd();

    return 0;
}


