//******************************************************************************
// Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
//******************************************************************************

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
