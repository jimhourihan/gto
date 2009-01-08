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

#ifndef _GtoContainer_Component_h_
#define _GtoContainer_Component_h_

#include <string>
#include <vector>

namespace GtoContainer {

//-*****************************************************************************
class Property;

//-*****************************************************************************
//  class Component
//
//  A Component is a group of Properties. An example would be "points"
//  which might contain the properties "position", "velocity", "mass",
//  etc. Typically, a component will have properties whose sizes are
//  the same. However, there is no enforcement of this.
//-*****************************************************************************

class Component
{
public:
    typedef std::vector<Property*> Container;
    typedef Container Properties;
    
    // Transposable components should have all properties of the same
    // size.
    explicit Component( const std::string &name, bool transposable = false );
    ~Component();
    
    // Returns the values passed into the constructor.
    bool isTransposable() const { return m_transposable; }
    const std::string &name() const { return m_name; }
    
    // Property management
    const Container& properties() const  { return m_properties; }
    Container & properties() { return m_properties; }

    void add( Property *p );
    void remove( Property *p );
    void remove( const std::string & );

    const Property *find( const std::string & ) const;
    Property *find( const std::string & );

    template <class T>
    T *property( const std::string &name );

    template <class T>
    const T *property( const std::string &name ) const;
    
    // resizing all properties in a component
    void resize( size_t s );
    
    // range deletion
    void erase( size_t start, size_t num );
    void eraseUnsorted( size_t start, size_t num );
    
    // resize to the first non-zero property size
    void resizeNonZero();
    
    // Copy everthing
    Component *copy() const;
    
    // Copy other to this
    void copy( const Component * );
    
    // Concatenate. Presumably the passed in component has all the
    // same properties as this one. However, its not an error to pass
    // in something that doesn't have all the same props.
    void concatenate( const Component * );
    
    // Archive
    bool isPersistent() const;
 
private:
    std::string		m_name;
    Container		m_properties;
    bool		m_transposable;
};

//-*****************************************************************************
// TEMPLATE AND INLINE FUNCTIONS
//-*****************************************************************************
template <class T>
inline T *Component::property( const std::string &name )
{
    return dynamic_cast<T*>( find( name ) );
}

//-*****************************************************************************
template<class T>
inline const T *Component::property( const std::string &name ) const
{
    return dynamic_cast<const T*>( find( name ) );
}

} // End namespace GtoContainer

#endif

