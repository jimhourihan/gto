//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

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
