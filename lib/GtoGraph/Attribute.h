#ifndef __GtoGraph__Attribute__h__
#define __GtoGraph__Attribute__h__
//
// Copyright (C) 2008 Tweak Software
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
#include <string>
#include <vector>
#include <algorithm>

class Container;
class AttributeGroup;

/// ValueTraits describes properties of attribute values

enum Layout
{
    CompoundLayout,
    FloatLayout,
    IntLayout,
    BoolLayout,
    DoubleLayout,
    HalfLayout,
    ByteLayout,
    ShortLayout,
    StringLayout,
    NoLayout
};

template <typename T>
struct ValueTraits
{
    static T defaultValue() { return T(0); }
    static Layout layout()  { return NoLayout; }
    static size_t elementSize() { return sizeof(T); }
    static size_t width() { return 1; }
};


/// Attribute::Info is meta data describing characteristics of an Attribute

///
/// The Info struct can be shared among mutiple Attributes (a
/// reference count is held by the base class). The base class also
/// indicates whether the attribute should be persistent (saved in the
/// file) or not. Ultimately, Info is stored as an interp string in
/// the file.
///

class Info
{
  public:
    explicit Info(bool persistant = true) : m_persistant(persistant), m_ref(0) {}
    void ref() { m_ref++; }
    void unref() { m_ref--; }
    
  private:
    bool m_persistent;
    size_t m_ref;
};

/// Attribute is an untyped base class for TypedAttribute<> 

///
/// An attribute must have this class as the Base for I/O and use by
/// the Container and AttributeGroup classes. However, you can insert
/// an intermediate base class between Attribute and TypedAttribute<>.
/// For example, if the attribute is part of a dependancy graph with
/// evaluation, you could insert a base class with notification, a
/// dirty bit, and time.
///

class Attribute
{
  public:

    //
    //  Associated Types
    //

    //
    //  Constructors
    //

    explicit Attribute(const std::string&, Info* info = 0);

    const std::string& name() const { return m_name; }
    const Info* info() const { return m_info; }

    ///
    /// Raw Access functions point to contiguous memory. 
    ///

    virtual void* rawData() = 0;
    virtual const void* rawData() const = 0;
    virtual size_t rawDataSizeInBytes() const = 0;
    virtual Layout layout() const = 0;

  private:
    std::string m_name;
    Info*       m_info;
};


/// TypedAttribute<> adds typed state to Attribute

template <typename T, class Base = Attribute>
class TypedAttribute : public Base
{
  public:
    //
    //  Associated Types
    //

    typedef T value_type;

    //
    //  Constructors
    //

    explicit TypedAttribute<T,Base>(const std::string& name, Info* info = 0)
        : Base(name, info) {}

    T& value() { return m_value; }
    const T& value() const { return m_value; }

    ///
    /// Raw Access functions point to contiguous memory. Used
    /// primarily by File I/O at this level. This API turns any
    /// attribute into a C style array
    ///

    virtual void* rawData() { return &m_value; }
    virtual const void* rawData() const { return &m_value; }
    virtual size_t rawDataSizeInBytes() const { return sizeof(T); }
    virtual Layout layout() const { return ValueTraits<T>::layout(); }

  protected:
    T   m_value;
};

/// TypedArrayAttribute<> extends typed attribute to hold multiple values

///
/// This first iterator is probably restricted to std::vector<> as the
/// container class. In the future it may be reasonable to assign
/// other container types. Requirement: Base <: Attribute
///

template <typename T, class C = std::vector<T>, class Base = Attribute>
class TypedArrayAttribute : public TypedAttribute<C, Base>
{
  public:
    //
    //  Associated Types
    //

    typedef TypedAttribute<C,Base> TAttr;

    //
    //  Constructors
    //

    explicit TypedArrayAttribute<T,C,Base> (const std::string& name,
                                            size_t size=0,
                                            Info* info = 0)
        : TAttr(name, info) 
    { 
        TAttr::value().resize(size); 
        std::fill(TAttr::value().begin(),
                  TAttr::value().end(),
                  ValueTraits<T>::defaultValue());
    }

    ///
    /// Raw Access functions point to contiguous memory. Used by File
    /// I/O and vector streaming functions at the array level. This
    /// API assumes the container class stores its values
    /// continguously (as does vector).
    ///

    virtual void* rawData() { return &TAttr::value().front(); }
    virtual const void* rawData() const { return &TAttr::value().front(); }
    virtual size_t rawDataSizeInBytes() const { return sizeof(T) * TAttr::value().size(); }
};

/// Macro to define base types

///
/// If you have a Vector3<float> class for example, you could do:
/// GTO_DECLARE_ATTR(Vec3f, Vector3<float>, "vec3f", FloatLayout, 3, Vector3<float>(0,0,0))
/// which would declare two typedefs: Vec3fAttribute and Vec3fArrayAttribute.
///

#define GTO_DECLARE_ATTR(TYPENAME, TYPE, NAME, LAYOUT, WIDTH, DEFAULT_VALUE) \
template <>                                                             \
struct ValueTraits<TYPE>                                                \
{                                                                       \
    static TYPE defaultValue() { return DEFAULT_VALUE; }                \
    static Layout layout() { return LAYOUT; }                           \
    static size_t elementSize() { return sizeof(TYPE); }                \
    static size_t width() { return WIDTH; }                             \
    static std::string typeName() { return NAME; }                      \
};                                                                      \
                                                                        \
template <>                                                             \
struct ValueTraits<std::vector<TYPE> >                                  \
{                                                                       \
    static TYPE defaultValue() { return DEFAULT_VALUE; }                \
    static Layout layout() { return LAYOUT; }                           \
    static size_t elementSize() { return sizeof(TYPE); }                \
    static size_t width() { return WIDTH; }                             \
    static std::string typeName() { return NAME "[]"; }                 \
};                                                                      \
                                                                        \
typedef TypedAttribute<TYPE> TYPENAME ## Attribute;                     \
typedef TypedArrayAttribute<TYPE> TYPENAME ## ArrayAttribute

GTO_DECLARE_ATTR(Float, float, "float", FloatLayout, 1, 0);
GTO_DECLARE_ATTR(Int, int, "int", IntLayout, 1, 0);
GTO_DECLARE_ATTR(Bool, bool, "bool", BoolLayout, 1, false);
GTO_DECLARE_ATTR(Double, double, "double", DoubleLayout, 1, 0);
GTO_DECLARE_ATTR(Byte, unsigned char, "byte", ByteLayout, 1, 0);
GTO_DECLARE_ATTR(Short, short, "short", ShortLayout, 1, 0);
GTO_DECLARE_ATTR(String, std::string, "string", StringLayout, 1, "");

#ifdef GTO_SUPPORT_HALF
GTO_DECLARE_ATTR(Half, half, "half", HalfLayout, 1, 0);
#endif


#define GTO_ATTR_INTERP(INTERPSTRING, GTOTYPE, WIDTH, ATTRTYPE) \
add

#endif // __GtoGraph__Attribute__h__
