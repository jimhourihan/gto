//******************************************************************************
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
//******************************************************************************

#include <string>
#include <Gto/RawData.h>

class GtoDB {

public:

    enum FileType
    {
        BinaryGTO,
        CompressedGTO,
        TextGTO
    };

    GtoDB();
    GtoDB(std::string filename);
    virtual ~GtoDB();

    void read(std::string filename);

    void write(std::string filename, FileType mode=CompressedGTO);

public:
    // Must be public so swig class extensions can access it
    Gto::RawDataBaseReader m_reader;
};
