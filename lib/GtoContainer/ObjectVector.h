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

#ifndef _GtoContainer_ObjectVector_h_
#define _GtoContainer_ObjectVector_h_

//-*****************************************************************************
// The ObjectVector is just a big list of PropertyContainers. This is what you
// pass to GTOWriter, and what you receive from GTOReader.
//
// If you have a fancy scene class, you can extract the pointers out of this
// class for your scene, and similarly, just iterate over your scene to
// create an ObjectVector for writing.
//-*****************************************************************************

#include <GtoContainer/PropertyContainer.h>
#include <vector>
#include <algorithm>

namespace GtoContainer {

//-*****************************************************************************
// Note that deleting the ObjectVector does NOT delete the contents.
// There is a deleteContents operator for exactly that purpose.
//
// This is just derived from the STL vector, so you can use all of those
// member functions to traverse. The primary functions will be extracted here.
class ObjectVector : public std::vector<PropertyContainer *>
{
public:
    ObjectVector() : std::vector<PropertyContainer *>() {}

    // Destruction does NOT delete the contents.
    ~ObjectVector() {}

    // Delete the contents.
    void deleteContents();

    // Remove a PropertyContainer without deleting.
    // Removes all copies of it from this list.
    void removeWithoutDeleting( const PropertyContainer *pc );

    //-*************************************************************************
    // Find the first PropertyContainer that matches a particular type.
    template <class DerivedType>
    DerivedType *findFirstOfType();

    template <class DerivedType>
    const DerivedType *findFirstOfType() const;

    // Find all the PropertyContainers that match a particular type.
    // Appends them to the list.
    template <class DerivedType>
    void findAllOfType( std::vector<DerivedType *> &into );

    template <class DerivedType>
    void findAllOfType( std::vector<const DerivedType *> &into ) const;

    //-*************************************************************************
    // Find the first PropertyContainer that matches a particular protocol
    // Returns NULL if none found.
    // Protocol must match in name AND version. If you want to find all the
    // protocols that match for a range of versions, you'll have to parse the
    // collection yourself.
    PropertyContainer *findFirstOfProtocol( const Protocol &p );
    const PropertyContainer *findFirstOfProtocol( const Protocol &p ) const;

    // Find all property containers that match a particular protocol.
    void findAllOfProtocol( const Protocol &p,
                            std::vector<PropertyContainer *> &into );
    void findAllOfProtocol( const Protocol &p,
                            std::vector<const PropertyContainer *> &i ) const;

    //-*************************************************************************
    // Find the first PropertyContainer that matches a particular name
    PropertyContainer *findFirstOfName( const std::string &n );
    const PropertyContainer *findFirstOfName( const std::string &n ) const;

    // Find all property containers that match a particular name
    void findAllOfName( const std::string &p,
                        std::vector<PropertyContainer *> &into );
    void findAllOfName( const std::string &p,
                        std::vector<const PropertyContainer *> &i ) const;
    
    
};

//-*****************************************************************************
// TEMPLATE AND INLINE FUNCTIONS
//-*****************************************************************************

//-*****************************************************************************
template <class DTYPE>
DTYPE *ObjectVector::findFirstOfType()
{
    for ( iterator iter = begin(); iter != end(); ++iter )
    {
        DTYPE *dt = dynamic_cast<DTYPE *>( (*iter) );
        if ( dt != NULL )
        {
            return dt;
        }
    }
    return NULL;
}

//-*****************************************************************************
template <class DTYPE>
const DTYPE *ObjectVector::findFirstOfType() const
{
    for ( const_iterator iter = begin(); iter != end(); ++iter )
    {
        const DTYPE *dt = dynamic_cast<const DTYPE *>( (*iter) );
        if ( dt != NULL )
        {
            return dt;
        }
    }
    return NULL;
}

//-*****************************************************************************
template <class DTYPE>
void ObjectVector::findAllOfType( std::vector<DTYPE *> &into )
{
    for ( iterator iter = begin(); iter != end(); ++iter )
    {
        DTYPE *dt = dynamic_cast<DTYPE *>( (*iter) );
        if ( dt != NULL )
        {
            into.push_back( dt );
        }
    }
}

//-*****************************************************************************
template <class DTYPE>
void ObjectVector::findAllOfType( std::vector<const DTYPE *> &into ) const
{
    for ( const_iterator iter = begin(); iter != end(); ++iter )
    {
        const DTYPE *dt = dynamic_cast<const DTYPE *>( (*iter) );
        if ( dt != NULL )
        {
            into.push_back( dt );
        }
    }
}

} // End namespace GtoContainer

#endif
