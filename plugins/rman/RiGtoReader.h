//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _RiGtoReader_h_
#define _RiGtoReader_h_

#include <Gto/Reader.h>
#include <RiGto/RiGtoObject.h>
#include <RiGto/RiGtoSet.h>

namespace RiGto {

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

    void doneReading();

protected:
    Set &m_set;
    ReaderPhase m_readerPhase;
};

} // End namespace RiGto

#endif
