//
// Copyright (C) 2003 Tweak Films
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
