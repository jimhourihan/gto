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

#ifndef _GtoContainer_Utilities_h_
#define _GtoContainer_Utilities_h_

#include <algorithm>
#include <functional>

namespace GtoContainer {

//-*****************************************************************************
// Function remove
//
// This removes an element from a Container by erasing it. It does a
// linear search to find the element to remove.
template <class Container>
void 
remove( Container &container,
        typename Container::value_type value )
{
    typename Container::iterator i = std::find( container.begin(),
                                                container.end(),
                                                value );

    if ( i != container.end() )
    {
        container.erase( i );
    }
}

//-*****************************************************************************
// Function remove_unsorted
//
// This removes an element from a Container by copying the last
// element in the container on top of the element to be removed. It
// then erases the last element.
template <class Container>
void 
remove_unsorted( Container &container,
                 typename Container::value_type value )
{
    typename Container::iterator i = std::find( container.begin(),
                                                container.end(),
                                                value );

    if ( i != container.end() )
    {
        *i = container.back();
        container.erase( container.begin() + container.size() - 1 );
    }
}

//-*****************************************************************************
// Function delete_contents
//
// Deletes the contents of a container. Useful when the container
// holds pointers.
template <class Container>
inline void
delete_contents( Container &container )
{
    for ( typename Container::iterator i = container.begin(); 
          i != container.end(); 
          ++i )
    {
	delete *i;
    }
}

//-*****************************************************************************
// Function exists
//
// Returns true if the element exists inside the container false otherwise.
template <class Container>
inline bool
exists( const Container &container, 
        typename Container::value_type value )
{
    return std::find( container.begin(), 
		      container.end(), 
                      value ) != container.end();
}

//-*****************************************************************************
// Cast Adapters for std::transform. So transform can be used to copy
// from a container of one type to another.
template <class From, class To>
struct StaticPointerCast
{
    To* operator() (From* x) const { return static_cast<To*>(x); }
};

template <class From, class To>
struct DynamicPointerCast
{
    To* operator() (From* x) const { return dynamic_cast<To*>(x); }
};

template <class From, class To>
struct ReinterpertCast
{
    To operator() (From x) const { return reinterpret_cast<To>(x); }
};

template <class From, class To>
struct StaticCast
{
    To operator() (From x) const { return static_cast<To>(x); }
};

template <class From, class To>
struct DynamicCast
{
    To operator() (From x) const { return dynamic_cast<To>(x); }
};

//-*****************************************************************************
//
//  Predicates. (All end in _p)
//
template <class From, class To>
struct IsA_p
{
    bool operator() (From* s) const { return dynamic_cast<To*>(s) == 0; }
};

//
//  Full container versions of std algorithms
//

template <class Container, class UnaryFunction>
inline typename Container::iterator 
for_each (Container& c, UnaryFunction f)
{
    return std::for_each(c.begin(), c.end(), f);
}

template <class Container>
inline typename Container::iterator 
find (Container& c, const typename Container::value_type& v)
{
    return std::find(c.begin(), c.end(), v);
}

template <class Container>
inline void
sort (Container& c)
{
    std::sort(c.begin(), c.end());
}

template <class Container, class OutputIterator>
inline OutputIterator 
copy (Container& c, OutputIterator i)
{
    return std::copy(c.begin(), c.end(), i);
}

} // End namespace GtoContainer

#endif
