//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _GtoInReader_h_
#define _GtoInReader_h_

#include <Gto/Reader.h>
#include "GtoInSet.h"

namespace GtoIOPlugin {

class Reader : public Gto::Reader
{
public:
    Reader( Set &set );
    ~Reader();

    // Decide if we can use stuff
    virtual void *object( const std::string &name,
                          const std::string &protocol,
                          unsigned int protocolVersion,
                          const ObjectInfo &header );

    virtual void *component( const std::string &name,
                             const ComponentInfo &header );

    virtual void *property( const std::string &name,
                            const PropertyInfo &header );

    // Get actual data for stuff.
    virtual void data( const PropertyInfo &,
                       const float *,
                       size_t numItems );

    virtual void data( const PropertyInfo &,
                       const double *,
                       size_t numItems );

    virtual void data( const PropertyInfo &,
                       const int *,
                       size_t numItems );

    virtual void data( const PropertyInfo &, 
                       const unsigned short *,
                       size_t numItems);

    virtual void data( const PropertyInfo &, 
                       const unsigned char *,
                       size_t numItems);

protected:
    Set &m_set;
};

} // End namespace GtoIOPlugin

#endif
