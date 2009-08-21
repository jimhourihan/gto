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

#ifndef _GelGtoStrand_h_
#define _GelGtoStrand_h_

#include <GelGto/Object.h>

namespace GelGto {

class Strand : public Object
{
public:
    Strand( const std::string &name, 
            const unsigned int protocolVersion,
            Gto::Reader *reader );
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
        ELEMENTS_WIDTH_P,
        ELEMENTS_STROOT_P
    };

    virtual void *component( const std::string &name,
                             ReaderPhase rp ) const;

    virtual void *property( const std::string &name,
                            void *componentData,
                            ReaderPhase rp,
                            const PropertyInfo &pinfo ) const;

    virtual void *data( void *componentData,
                        void *propertyData,
                        size_t numItems,
                        size_t itemWidth,
                        size_t numBytes,
                        ReaderPhase rp,
                        const PropertyInfo &pinfo );

    virtual void dataRead( void *componentData,
                           void *propertyData,
                           ReaderPhase rp,
                           const PropertyInfo &pinfo );

protected:
    void *typeData();
    void *sizeData( size_t sizesSize );
    
    void *strandWidthData();
    void *elementsWidthData( size_t widthsSize );
    void *elementsRootSTData( size_t rootSTSize );
    
    void *positionsRefData( size_t positionsRefSize );
    void *positionsOpenData( size_t positionsOpenSize );
    void *positionsCloseData( size_t positionsCloseSize );

protected:
    virtual void internalDeclareRi(GelatoAPI *_rend) const;

    int m_typeStrId;
    std::string m_type;
    
    int *m_sizes;
    size_t m_sizesSize;

    float m_strandWidth[2];

    float *m_elementsWidth;
    size_t m_elementsWidthSize;

    float *m_positionsRef;
    size_t m_positionsRefSize;
    
    float *m_positionsOpen;
    size_t m_positionsOpenSize;
    
    float *m_positionsClose;
    size_t m_positionsCloseSize;

    float *m_elementsRootST;
    size_t m_elementsRootSTSize;
};

} // End namespace GelGto

#endif
