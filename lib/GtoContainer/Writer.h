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

#ifndef _GtoContainer_Writer_h_
#define _GtoContainer_Writer_h_

#include <Gto/Writer.h>
#include <string>
#include <vector>

namespace GtoContainer {

//-*****************************************************************************
// Forward Declarations
class Component;
class Property;
class PropertyContainer;
class ObjectVector;

//-*****************************************************************************
class Writer
{
public:
    typedef Gto::Writer::FileType FileType;

    Writer( const char *stamp = NULL );
    virtual ~Writer();

    // File types are Gto::BinaryGTO,
    //                Gto::CompressedGTO,
    //                Gto::TextGTO
    bool                    write( const char *filename, 
                                   const ObjectVector &ov, 
                                   FileType type = Gto::Writer::CompressedGTO );

private:
    void                    writeComponent( bool header, const Component * );
    void                    writeProperty( bool header, const Property * );

private:
    std::string             m_stamp;
    Gto::Writer             m_writer;

    std::vector<std::string> m_orderedStrings;
};

} // End namespace GtoContainer

#endif

