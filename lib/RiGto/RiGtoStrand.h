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

#ifndef _RiGtoStrand_h_
#define _RiGtoStrand_h_

#include <RiGto/RiGtoObject.h>

namespace RiGto {

class Strand : public Object
{
public:
    Strand( const std::string &name, 
            const unsigned int protocolVersion,
            Reader *reader );
    virtual ~Strand();

    //**************************************************************************
    // READER STUFF
    enum
    {
        POINTS_C = Object::NEXT_C,
        STRAND_C,
        ELEMENTS_C
    };

    enum
    {
        POINTS_POSITION_P = Object::NEXT_P,
        STRAND_TYPE_P,
        STRAND_WIDTH_P,
        ELEMENTS_SIZE_P,
        ELEMENTS_WIDTH_P
    };

    virtual void *component( const std::string &name,
                             ReaderPhase rp ) const;

    virtual void *property( const std::string &name,
                            void *componentData,
                            ReaderPhase rp ) const;

    virtual void *data( void *componentData,
                        void *propertyData,
                        size_t numItems,
                        size_t itemWidth,
                        size_t numBytes,
                        ReaderPhase rp );

    virtual void dataRead( void *componentData,
                           void *propertyData,
                           ReaderPhase rp );

protected:
    void *typeData();
    void *sizeData( size_t sizesSize );
    void *constantWidthData();
    void *widthData( size_t widthsSize );
    void *positionsRefData( size_t positionsRefSize );
    void *positionsOpenData( size_t positionsOpenSize );
    void *positionsCloseData( size_t positionsCloseSize );

protected:
    virtual void internalDeclareRi() const;

    int m_typeStrId;
    std::string m_type;
    int *m_sizes;
    size_t m_sizesSize;

    float m_constantWidth;
    float *m_widths;
    size_t m_widthsSize;

    float *m_positionsRef;
    size_t m_positionsRefSize;
    
    float *m_positionsOpen;
    size_t m_positionsOpenSize;
    
    float *m_positionsClose;
    size_t m_positionsCloseSize;
};

} // End namespace RiGto

#endif
