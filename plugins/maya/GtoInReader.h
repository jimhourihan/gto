//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _GtoInReader_h_
#define _GtoInReader_h_

#include <Gto/Reader.h>
#include "GtoInSet.h"

namespace GtoIOPlugin {

typedef Gto::Reader::Request Request;

class Reader : public Gto::Reader
{
public:
    Reader( Set &set );
    ~Reader();

    // Decide if we can read the object, create new object instance if so
    virtual Request object( const std::string &name,
                            const std::string &protocol,
                            unsigned int protocolVersion,
                            const ObjectInfo &header );

    // Call component method of corresponding object
    virtual Request component( const std::string &name,
                               const ComponentInfo &header );

    // Call property method of corresponding object
    virtual Request property( const std::string &name,
                              const PropertyInfo &header );


    // Call data methods of corresponding object
    virtual void *data( const PropertyInfo &pinfo, size_t bytes );
    virtual void dataRead( const PropertyInfo &pinfo );

protected:
    Set &m_set;
};

} // End namespace GtoIOPlugin

#endif
