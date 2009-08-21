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

#ifndef _RiGtoNURBS_h_
#define _RiGtoNURBS_h_

#include <RiGto/RiGtoObject.h>

namespace RiGto {

class NURBS : public Object
{
public:
    NURBS( const std::string &name, 
           const unsigned int protocolVersion,
           Reader *reader );
    virtual ~NURBS();

    //**************************************************************************
    // READER STUFF
    enum
    {
        POINTS_C = Object::NEXT_C,
        SURFACE_C
    };

    enum
    {
        POINTS_POSITION_P = Object::NEXT_P,
        POINTS_WEIGHT_P,
        SURFACE_DEGREE_P,
        SURFACE_UKNOTS_P,
        SURFACE_VKNOTS_P,
        SURFACE_URANGE_P,
        SURFACE_VRANGE_P
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

    virtual void doneReading( ReaderPhase rp );

protected:
    void *degreeData();
    void *knotsUData( size_t knotsUSize );
    void *knotsVData( size_t knotsVSize );
    void *rangeUData();
    void *rangeVData();
    void *weightsData( size_t weightsSize );
    void *positionsRefData( size_t positionsRefSize );
    void *positionsOpenData( size_t positionsOpenSize );
    void *positionsCloseData( size_t positionsCloseSize );

protected:
    virtual void internalDeclareRi() const;

    int m_degreeU;
    int m_degreeV;
    float *m_knotsU;
    size_t m_knotsUSize;
    float *m_knotsV;
    size_t m_knotsVSize;
    float m_minU;
    float m_maxU;
    float m_minV;
    float m_maxV;

    float *m_positionsRef;
    size_t m_positionsRefSize;
    
    float *m_positionsOpen;
    size_t m_positionsOpenSize;
    
    float *m_positionsClose;
    size_t m_positionsCloseSize;

    float *m_weights;
    size_t m_weightsSize;

    float *m_homogPosRef;
    float *m_homogPosOpen;
    float *m_homogPosClose;
};

} // End namespace RiGto

#endif
