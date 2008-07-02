//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#include <GelGto/Plugin.h>
#include <GelGto/Exception.h>
#include <iostream>
#include <gelatoapi.h>

#define GELGTO_VERSION_MAJOR 1
#define GELGTO_VERSION_MINOR 0
#define GELGTO_VERSION_PATCH 0


class GtoReader : public GelatoAPI::Generator 
{ 
public: 
    GtoReader(); 
    virtual ~GtoReader(); 
    virtual void run( GelatoAPI *renderer, const char *params ); 

private:
    static bool m_bannerPrinted;
    static GelGto::DataBase *m_globalDataBase;
};

bool GtoReader::m_bannerPrinted = false;
GelGto::DataBase *GtoReader::m_globalDataBase = NULL;


// *****************************************************************************
GtoReader::GtoReader()
{ 
    if( ! m_bannerPrinted )
    {
        printf( "Gelato GTO Plugin\n"
                "Copyright (c) 2002-2003, Tweak Inc. All rights reserved.\n"
                "Version %d.%d.%d, built on %s at %s\n",
                GELGTO_VERSION_MAJOR, GELGTO_VERSION_MINOR, 
                GELGTO_VERSION_PATCH, __DATE__ , __TIME__ );
        m_bannerPrinted = true;
    }
} 

// *****************************************************************************
GtoReader::~GtoReader()
{
} 


// *****************************************************************************
void GtoReader::run( GelatoAPI *renderer, const char *params )
{
    // Here we read "Gto" from filename, and make GelatoAPI calls 
    // to renderer in order to communicate the commands in the file 
    // to Gelato. 

    int verbosity = 0;
    renderer->GetAttribute("int verbosity", verbosity);
    if( verbosity >= 2 )
    {
        printf("gto.generator.so: reading '%s'.\n\n", params);
    }

    if( m_globalDataBase == NULL )
    {
        m_globalDataBase = new GelGto::DataBase();
    }

    try
    {
        GelGto::Plugin thePlugin( *m_globalDataBase, params, true );

        thePlugin.declareRi( renderer );
    }
    catch( std::exception &e )
    {
        std::cerr << "ERROR: gto.generator: " << e.what() << std::endl;
    }
} 

extern "C" 
{
    GELATO_EXPORT int generator_version = GelatoAPI::API_VERSION; 
    GELATO_EXPORT GelatoAPI::Generator *gto_generator_create( const char *command )
    { 
        return new GtoReader; 
    }
};
