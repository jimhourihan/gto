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
