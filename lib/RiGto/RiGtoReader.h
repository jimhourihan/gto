//
// Copyright (C) 2004 Tweak Films
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
