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

#include <GtoContainer/Reader.h>
#include <GtoContainer/ObjectVector.h>
#include <GtoContainer/StdProperties.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <exception>
#include <string>

//-*****************************************************************************
using namespace GtoContainer;

typedef PropertyContainer::Components Components;
typedef Component::Properties Properties;

//-*****************************************************************************
int main( int argc, char **argv )
{
    if( argc != 2 )
    {
        printf("GTO to GTOascii v 3.4\n");
        printf("\n");
        printf("Usage: %s [options]\n", argv[0]);
        printf("\n");
        printf("%%S     GTO file to review\n");
        printf("\n");
        return 0;
    }
    
    char *gtoFileName = argv[1];

    try
    {
        ObjectVector objs;

        Reader reader;
        reader.read( gtoFileName, objs );

        printf( "GTOa(4)\n\n" );

        for ( ObjectVector::iterator iter = objs.begin();
              iter != objs.end(); ++iter )
        {
            PropertyContainer *pc = (*iter);

            printf( "\n\"%s\" : %s(%d)\n"
                    "{\n",
                    pc->name().c_str(),
                    pc->protocol().protocol.c_str(),
                    ( int )pc->protocol().version );

            Components &comps = pc->components();

            for ( Components::iterator citer = comps.begin();
                  citer != comps.end(); ++citer )
            {
                Component *comp = (*citer);
                
                printf( "\t\"%s\"\n"
                        "\t{\n",
                        comp->name().c_str() );

                Properties &props = comp->properties();
                for ( Properties::iterator piter = props.begin();
                      piter != props.end(); ++piter )
                {
                    Property *prop = (*piter);

                    printf( "\t\t%s %s[%d][%d]\n",
                            layoutName( prop->layoutTrait() ).c_str(),
                            prop->name().c_str(),
                            ( int )( prop->widthTrait() ),
                            ( int )( prop->size() ) );
                }

                printf( "\t}\n\n" );

            }

            printf( "};\n" );

        }
    }
    catch ( std::exception &exc )
    {
        std::cerr << "EXCEPTION: " << exc.what() << std::endl;
        exit( -1 );
    }
    catch ( ... )
    {
        std::cerr << "UNKNOWN EXCEPTION." << std::endl;
        exit( -1 );
    }

    return 0;
}

