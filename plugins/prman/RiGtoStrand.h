//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

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
        ELEMENTS_C
    };

    enum
    {
        POINTS_POSITION_P = Object::NEXT_P,
        ELEMENTS_TYPE_P,
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
