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

#ifndef _GtoContainer_Reader_h_
#define _GtoContainer_Reader_h_

#include <GtoContainer/PropertyContainer.h>
#include <GtoContainer/ObjectVector.h>

#ifdef None
#define __NONE_REP__ None
#undef None
#endif

#include <Gto/Reader.h>

#ifdef __NONE_REP__
#define None __NONE_REP__
#undef __NONE_REP__
#endif

#include <string>
#include <vector>

namespace GtoContainer {

//-*****************************************************************************
class Reader : protected Gto::Reader
{
public:
    //-*************************************************************************
    // Types
    //-*************************************************************************
    typedef Reader::Request                 Request;
    
    // Constructors
    Reader();
    
    virtual ~Reader();
    
    // There's only one call to this class that does anything
    // interesting. read will throw one of the GtoContainer exceptions if
    // something goes wrong.
    // If readIntoExistingObjects is true, the class will only read stuff
    // that's there to begin with.
    void read( const std::string &filename,
               ObjectVector &objects,
               bool readIntoExistingObjects = false );

    //-*************************************************************************
    //-*************************************************************************
    // INTERNAL STUFF
    //-*************************************************************************
    // Override this function if you want to do non-default
    // container creation based on the protocol.
    virtual PropertyContainer* newContainer( const Protocol &protocol );
    
    // Override this function if you want to make a specific type of
    // property given the name of a property, a protocol, and
    // component name.
    //
    // If this function returns NULL, the reader will use the default
    // property type.
    //
    // This mechanism allows for total override control, however it's
    // a heavy tool. The more regular thing would be add new
    // metaproperties below.
    virtual Property *newProperty( const std::string &name,
                                   const PropertyInfo & );

    virtual Request object( const std::string &name,
                            const std::string &protocol,
                            unsigned int protocolVersion,
                            const ObjectInfo &header );

    virtual Request component( const std::string &name,
                               const std::string &interp,
                               const ComponentInfo &header );

    virtual Request property( const std::string &name,
                              const std::string &interp,
                              const PropertyInfo &header);

    virtual void *data( const PropertyInfo &, size_t bytes );
    virtual void dataRead( const PropertyInfo & );
    // To accomodate some GTO base library implementations which have changed
    // the dataRead signature.
    virtual void dataRead( const PropertyInfo &p, char * ) { dataRead( p ); }

    // The reader uses reverse ordering with the meta properties.
    // The last ones are consulted first.
    // This behavior can be changed by overriding findMetaProperty
    void appendMetaProperty( MetaProperty *mp )
    { m_metaProperties.push_back( mp ); }

    virtual const MetaProperty *findMetaProperty( Layout lyt,
                                                  size_t width,
                                                  const std::string &nt ) const;

protected:
    MetaProperties      m_metaProperties;

private:
    bool                m_useExisting;
    ObjectVector*       m_objects;
    std::vector<int>    m_tempstrings;
};

} // End namespace GtoContainer

#endif
