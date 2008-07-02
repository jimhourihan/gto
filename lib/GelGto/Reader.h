//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _GelGtoReader_h_
#define _GelGtoReader_h_

#include <Gto/Reader.h>
#include <GelGto/Object.h>
#include <GelGto/Set.h>

namespace GelGto {

class Reader : public Gto::Reader
{
public:
    Reader( Set &set, ReaderPhase rp );
    ~Reader();

    // Decide if we can use stuff
    virtual Request object( const std::string &name,
                            const std::string &protocol,
                            const unsigned int protocolVersion,
                            const ObjectInfo &header );

    virtual Request component( const std::string &name,
                               const ComponentInfo &header );

    virtual Request property( const std::string &name,
                              const PropertyInfo &header );

    // Get actual data for stuff.
    virtual void *data( const PropertyInfo &,
                        size_t numBytes );

    virtual void dataRead( const PropertyInfo & );
    
    const char *stripNamePrefix( const std::string &name );

    void doneReading();

protected:
    Set &m_set;
    ReaderPhase m_readerPhase;
};

} // End namespace GelGto

#endif
