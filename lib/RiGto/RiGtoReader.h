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
