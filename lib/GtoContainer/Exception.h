//-*****************************************************************************
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
//-*****************************************************************************

#ifndef _GtoContainer_Exception_h_
#define _GtoContainer_Exception_h_

#include <iostream>
#include <sstream>
#include <exception>
#include <string>

namespace GtoContainer {

//-*****************************************************************************
class Exception : protected std::string, public std::exception
{
public:
    Exception() throw() : std::string( "" ) {}
    Exception( const char *str ) throw() : std::string( str ) {}
    Exception( const Exception &exc ) throw() : std::string( exc.c_str() ) {}

    virtual ~Exception() throw() {}

    virtual const char *what() const throw() { return c_str(); }
};

//-*****************************************************************************
#define GTC_THROW( TEXT )                                 \
do                                                          \
{                                                           \
    std::stringstream sstr;                                 \
    sstr << TEXT;                                           \
    GtoContainer::Exception exc( sstr.str().c_str() );      \
    throw exc;                                              \
}                                                           \
while( 0 )

//-*****************************************************************************
#define GTC_EXC_DECLARE( EXC_TYPE, BASE_TYPE, TAG )	        \
class EXC_TYPE : public BASE_TYPE				\
{								\
public:								\
    EXC_TYPE() : BASE_TYPE( TAG ) {}				\
    EXC_TYPE( const char *str ) : BASE_TYPE( TAG )		\
    { this->append( str ); }					\
};


//-*****************************************************************************
GTC_EXC_DECLARE( ReadFailedExc, Exception, "read failed" );
GTC_EXC_DECLARE( BadPropertyTypeMatchExc, Exception, "bad property match");
GTC_EXC_DECLARE( NoPropertyExc, Exception, "no such property" );
GTC_EXC_DECLARE( UnexpectedExc, Exception, "unexpected program state" );
GTC_EXC_DECLARE( TypeMismatchExc, Exception,
                 "PropertyContainer type mismatch" );
GTC_EXC_DECLARE( InvalidDerefExc, Exception,
                 "invalid iterator dereference" );
    

} // End namespace GtoContainer

#endif
