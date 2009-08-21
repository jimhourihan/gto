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

#include <iostream>
#include <unistd.h>
#include <arg.h>

#include <UT/UT_Vector4.h>
#include <GU/GU_Detail.h>
#include <GEO/GEO_TriMesh.h>
#include <GEO/GEO_PrimPoly.h>
#include <GEO/GEO_PrimNURBSurf.h>
#include <GEO/GEO_PrimPart.h>

#include <TwkDeploy/Deploy.h>
#include <Gto/Protocols.h>
#include <Gto/Header.h>
#include <Gto/Writer.h>
#include <Gto/RawData.h>

#include "Poly.h"
#include "NURBS.h"
#include "Particle.h"
#include "GtoAttribute.h"

#define GGTO_MAJOR_VERSION 1
#define GGTO_MINOR_VERSION 1
#define GGTO_PATCH_LEVEL 1


// *****************************************************************************
// Just in case it's ever needed
// *****************************************************************************
int loadConvFile( const char *filename, std::vector<std::string>& table )
{
    std::ifstream convFile( filename, std::ios::in );
    if( convFile.fail() )
    {
        std::cerr << "Failed to read " << filename << std::endl;
        return 0;
    }

    while( ! convFile.eof() )
    {
        std::string houdiniStr;
        std::string outsideStr;

        convFile >> houdiniStr >> outsideStr;

        // Ignores empty lines
        if (houdiniStr != "" && outsideStr != "")
        {
            table.push_back(houdiniStr);
            table.push_back(outsideStr);
        }
    }

    convFile.close();

    return 1;
}

// *****************************************************************************
int outputToGto( const char *filename, bool ascii, bool verbose )
{
    using namespace HGto;

    if( verbose ) std::cerr << "Loading from stdin..." << std::flush;
    GU_Detail gdp;
    if( gdp.load( std::cin ) < 0 )
    {
        std::cerr << "Unable to load input geometry" << std::endl;
        exit(1);
    }
    if( verbose ) std::cerr << "Done" << std::endl;


    if( verbose ) std::cerr << "Building GTO Data..." << std::flush;
    std::vector<Object *> Objects;
    Poly gtoPoly( "hPolyShape" ); // There is only ever 1 poly shape?

    if( verbose ) std::cerr << "Found " << gdp.primitives().entries() << " primitives." << std::endl;
    for( size_t i = 0; i < gdp.primitives().entries(); ++i )
    {
        const GEO_Primitive *prim = gdp.primitives()(i);

        if( const GEO_PrimPoly *poly = dynamic_cast<const GEO_PrimPoly *>( prim ) )
        {
            gtoPoly.addFace( poly );
        }
        else if( const GEO_PrimParticle *particle = dynamic_cast<const GEO_PrimParticle *>( prim ) )
        {
            Objects.push_back( new Particle( particle ) );
        }
//         else if( const GEO_PrimNURBSurf *nurbs = dynamic_cast<const GEO_PrimNURBSurf *>( prim ) )
//         {
//             Objects.push_back( new NURBS( nurbs ) );
//         }    
        else
        {
            if(verbose) std::cerr << "Unknown object: " << prim->getPrimitiveId() << std::endl;
        }
    }
    gtoPoly.getVertices( gdp.points() );
    if( verbose ) std::cerr << "Done" << std::endl;

    try
    {
        if( verbose ) std::cerr << "Writing GTO File..." << std::flush;
        Gto::Writer writer;
        writer.open( filename, ascii ? Gto::Writer::TextGTO : Gto::Writer::CompressedGTO );

        gtoPoly.writeHeader( writer );
        for( int i = 0; i < Objects.size(); ++i ) Objects[i]->writeHeader( writer );

        writer.beginData();

        gtoPoly.writeData( writer );
        for( int i = 0; i < Objects.size(); ++i ) Objects[i]->writeData( writer );

        writer.endData();

        writer.close();
        if( verbose ) std::cerr << "Done" << std::endl;
    }
    catch( std::exception &e )
    {
        std::cerr << "hgto::exception: " << e.what() << std::endl;
        exit(-1);
    }
    return 0;
}



// *****************************************************************************
int inputFromGto( const char *filename, bool ascii, bool verbose )
{
    using namespace Gto;

    GU_Detail gdp;
    
    RawDataBaseReader reader;
    
    if( verbose ) std::cerr << "Loading " << filename << "..." << std::flush;

    if( ! reader.open( filename ) )
    {
        std::cerr << "Unable to load input geometry" << std::endl;
        exit(1);
    }

    if( verbose ) std::cerr << "Done" << std::endl;
    if( verbose ) std::cerr << "Building objects..." << std::flush;

    const RawDataBase *db = reader.dataBase();
    for( int oi = 0; oi < db->objects.size(); ++oi )
    {
        const Object *o = db->objects[oi];

        if( o->protocol == GTO_PROTOCOL_PARTICLE ||
            o->protocol == "warped particle")
        {
            HGto::Particle particle( o->name );

            for( int ci = 0; ci < o->components.size(); ++ci )
            {
                const Component *c = o->components[ci];

                for( int pi = 0; pi < c->properties.size(); ++pi )
                {
                    Property *p = c->properties[pi];

                    if( c->name == GTO_COMPONENT_OBJECT )
                    {
                        if( p->name == GTO_PROPERTY_GLOBAL_MATRIX )
                        {
                            memcpy( &particle.globalMatrix(),
                                    p->floatData, 
                                    p->size * p->width * sizeof(float) );
                        }
                    }

                    if( c->name == GTO_COMPONENT_POINTS )
                    {
                        if( p->name == GTO_PROPERTY_POSITION )
                        {
                            particle.positions().resize( p->size );
                            memcpy( &particle.positions().front(), 
                                    p->floatData, 
                                    p->size * p->width * sizeof(float) );
                        }
                        else
                        {
                            // For all other properties
                            HGto::GtoAttribute *attr = NULL;

                            if( p->type == Gto::Float && p->width == 1)
                            {
                                attr = new HGto::FloatAttribute(p->name, p->size);
                                HGto::FloatAttribute *fattr = (HGto::FloatAttribute*)attr;

                                memcpy(&fattr->data().front(), 
                                       p->floatData, 
                                       sizeof( float ) * p->size);

                                particle.attributes().push_back(attr);
                            }
                            else if( p->type == Gto::Float && p->width == 3)
                            {
                                attr = new HGto::VectorAttribute(p->name, p->size);
                                HGto::VectorAttribute *vattr = (HGto::VectorAttribute*)attr;

                                memcpy(&vattr->data().front(), 
                                       p->floatData, 
                                       sizeof( float ) * p->size * 3);

                                particle.attributes().push_back(attr);
                            }
                            else if( p->type == Gto::Int && p->width == 1)
                            {
                                attr = new HGto::IntAttribute(p->name, p->size);
                                HGto::IntAttribute *iattr = (HGto::IntAttribute*)attr;

                                memcpy(&iattr->data().front(), 
                                       p->int32Data, 
                                       sizeof( int ) * p->size * 1);

                                particle.attributes().push_back(attr);
                            }
                        }
                    }
                }

            }

            particle.declareHoudini( gdp );
        }
        else if( o->protocol == GTO_PROTOCOL_POLYGON )
        {
            HGto::Poly polyObject( o->name );

            for( int ci = 0; ci < o->components.size(); ++ci )
            {
                const Component *c = o->components[ci];

                for( int pi = 0; pi < c->properties.size(); ++pi )
                {
                    const Property *p = c->properties[pi];

                    if( c->name == GTO_COMPONENT_OBJECT )
                    {
                        if( p->name == GTO_PROPERTY_GLOBAL_MATRIX )
                        {
                            memcpy( &polyObject.globalMatrix(),
                                    p->floatData, 
                                    p->size * p->width * sizeof(float) );
                        }
                    }

                    if( c->name == GTO_COMPONENT_POINTS )
                    {
                        if( p->name == GTO_PROPERTY_POSITION )
                        {
                            polyObject.positions().resize( p->size );
                            memcpy( &polyObject.positions().front(), 
                                    p->floatData, 
                                    p->size * p->width * sizeof(float) );
                        }
                    }

                    if( c->name == GTO_COMPONENT_ELEMENTS )
                    {
                        if( p->name == GTO_PROPERTY_TYPE )
                        {
                            polyObject.elementsType().resize( p->size );
                            memcpy( &polyObject.elementsType().front(),
                                    p->uint8Data, 
                                    p->size * p->width * sizeof(char) );
                        }
                        if( p->name == GTO_PROPERTY_SIZE )
                        {
                            polyObject.elementsSize().resize( p->size );
                            memcpy( &polyObject.elementsSize().front(),
                                    p->uint16Data, 
                                    p->size * p->width * sizeof(short) );
                        }
                    }
                    if( c->name == GTO_COMPONENT_INDICES )
                    {
                        if( p->name == GTO_PROPERTY_VERTEX )
                        {
                            polyObject.indicesVertex().resize( p->size );
                            memcpy( &polyObject.indicesVertex().front(),
                                    p->int32Data, 
                                    p->size * p->width * sizeof(int) );
                        }
                    }
                    if( c->name == GTO_COMPONENT_SMOOTHING )
                    {
                        if( p->name == GTO_PROPERTY_METHOD )
                        {
                            polyObject.smoothingMethod() = *(p->int32Data);
                        }
                    }
                }

            }

            polyObject.declareHoudini( gdp );
        }
    }
    if( verbose ) std::cerr << "Done" << std::endl;

    if( verbose ) std::cerr << "Writing to stdout..." << std::flush;
    gdp.save( std::cout, ! ascii );
    if( verbose ) std::cerr << "Done" << std::endl;

    return 0;
}




// *****************************************************************************
int main( int argc, char **argv )
{
    // Only show the banner if run directly from a terminal. 
    // (i.e. not a pipe to/from Houdini)
    if( isatty( STDIN_FILENO ) && isatty( STDOUT_FILENO ) )
    {
        TWK_DEPLOY_INITIALIZE(GGTO_MAJOR_VERSION,
                              GGTO_MINOR_VERSION,
                              GGTO_PATCH_LEVEL,
                              argc, argv,
                              __DATE__, __TIME__);

        TWK_DEPLOY_SHOW_PROGRAM_BANNER(cout, "Houdini GTO input/output converter");
        TWK_DEPLOY_SHOW_COPYRIGHT_BANNER(cout);
    }

    char *infilename = NULL;
    char *outfilename = NULL;
    int ascii = false;
    int verbose = false;

    if( arg_parse( argc, argv,
         "-i %S", &infilename, "Input Gto filename (output is stdout)",
         "-o %S", &outfilename, "Output Gto filename (input is stdin)",
         "-a", ARG_FLAG( &ascii ), "Make output ascii (geo/ascii gto)",
         "-v", ARG_FLAG( &verbose ), "Verbose",
         NULL ) < 0 )
    {
        exit(-1);
    }

    if( outfilename )
    {
        return outputToGto( outfilename, ascii, verbose );
    }
    else if( infilename )
    {
        return inputFromGto( infilename, ascii, verbose );
    }
    else
    {
        std::cout << "Run with -help for usage" << std::endl;
    }

    return 0;
}
