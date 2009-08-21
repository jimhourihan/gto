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

#ifndef __GTOATTRIBUTE_H__
#define __GTOATTRIBUTE_H__

namespace HGto {

// *****************************************************************************
class GtoAttribute {
public:
    GtoAttribute( std::string name, size_t size ) : m_name( name ), m_size( size ) {}
    virtual ~GtoAttribute() {}

    std::string &name() { return m_name; }

    virtual void writeHeader( Gto::Writer &writer ) = 0;
    virtual void writeData( Gto::Writer &writer ) = 0;

protected:
    std::string m_name;
    size_t m_size;
};


// *****************************************************************************
template <typename T>
class TypedGtoAttribute : public GtoAttribute
{
public:
    TypedGtoAttribute( std::string name, size_t size );
    virtual ~TypedGtoAttribute() {}

    std::vector<T> &data() { return m_data; }

    virtual void writeHeader( Gto::Writer &writer );
    virtual void writeData( Gto::Writer &writer ) { writer.propertyData( (void *)&m_data.front() ); }

private:
    std::vector<T> m_data;
};


// *****************************************************************************
template<typename T>
TypedGtoAttribute<T>::TypedGtoAttribute( std::string name, size_t size )
    : GtoAttribute( name, size )
{
    m_data.resize( size );
}

// *****************************************************************************
template <> 
inline void TypedGtoAttribute<float>::writeHeader( Gto::Writer &writer )
{
    writer.property( name().c_str(), Gto::Float, m_data.size(), 1 );
}

// *****************************************************************************
template <> 
inline void TypedGtoAttribute<int>::writeHeader( Gto::Writer &writer )
{
    writer.property( name().c_str(), Gto::Int, m_data.size(), 1 );
}

// *****************************************************************************
template <> 
inline void TypedGtoAttribute<UT_Vector3>::writeHeader( Gto::Writer &writer )
{
    writer.property( name().c_str(), Gto::Float, m_data.size(), 3 );
}

typedef TypedGtoAttribute<float> FloatAttribute;
typedef TypedGtoAttribute<int> IntAttribute;
typedef TypedGtoAttribute<std::string> StringAttribute;
typedef TypedGtoAttribute<UT_Vector3> VectorAttribute;

typedef std::vector<GtoAttribute *> GtoAttributes;

}  //  End namespace HGto

#endif    // End #ifdef __GTOATTRIBUTE_H__
