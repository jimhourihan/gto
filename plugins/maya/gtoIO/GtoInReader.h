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
