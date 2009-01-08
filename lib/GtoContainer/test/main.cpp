//-*****************************************************************************
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
//-*****************************************************************************

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

