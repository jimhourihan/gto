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

#ifndef _RiGtoException_h_
#define _RiGtoException_h_

#include <stdlib.h>
#include <exception>
#include <iostream>

namespace RiGto {

//******************************************************************************
class Exception : public std::exception
{
public:
    Exception() throw() {}
    Exception( const char *str ) throw() : m_string( "RiGto::Exception: " ) {}
    virtual ~Exception() throw() {}

    virtual const char *what() const throw() { return m_string.c_str(); }
    
    const std::string &str() const throw() { return m_string; }
    std::string &str() throw() { return m_string; }

protected:
    std::string m_string;
};

#define TWK_FAKE_EXCEPTION( e, errorString )                \
        {                                                   \
            std::cerr << std::endl << "ERROR: RiGto "       \
                "Exception at line " << __LINE__ << " in "  \
                 << __FILE__ << ": " << errorString         \
                 << std::endl << std::endl ;                \
            if( ! getenv( "TWK_RI_CONTINUE_ON_ERROR" ) )    \
            {                                               \
                exit( 1 );                                  \
            }                                               \
        }

} // End namespace RiGto

#endif
