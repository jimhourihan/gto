//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _GtoInObject_h_
#define _GtoInObject_h_
#include <maya/MObject.h>
#include <maya/MMatrix.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <Gto/Protocols.h>
#include <Gto/Reader.h>

namespace GtoIOPlugin {

typedef Gto::Reader::Request Request;
typedef Gto::Reader::PropertyInfo PropertyInfo;
typedef Gto::Reader::StringTable StringTable;

class Object
{
public:
    Object( const std::string &name, 
            const std::string &protocol,
            const unsigned int protocolVersion );
    virtual ~Object();

    const std::string &name() const { return m_name; }
    const std::string &parent() const { return m_parent; }
    const std::string &protocol() const { return m_protocol; }
    const MObject &mayaObject() const { return m_mayaObject; }
    const MObject &mayaParentObject() const { return m_mayaParentObject; }
    bool wasRenamed() const { return m_wasRenamed; }

    //**************************************************************************
    // READER STUFF
    enum
    {
        OBJECT_C = 1,
        NEXT_C
    };

    enum
    {
        OBJECT_GLOBALMATRIX_P = 1,
        OBJECT_PARENT_P,
        NEXT_P
    };
    
    virtual Request component( const std::string &name ) const;
    
    virtual Request property( const std::string &name,
                              void *componentData ) const;

    virtual void *data( const PropertyInfo &pinfo, 
                        size_t bytes,
                        void *componentData,
                        void *propertyData );

    virtual void dataRead( const PropertyInfo &pinfo,
                           void *componentData,
                           void *propertyData,
                           const StringTable &strings );

#if 0
    virtual void data( void *componentData,
                       void *propertyData,
                       const float *items,
                       size_t numItems,
                       size_t width);

    virtual void data( void *componentData,
                       void *propertyData,
                       const double *items,
                       size_t numItems,
                       size_t width);
 
    virtual void data( void *componentData,
                       void *propertyData,
                       const int *items,
                       size_t numItems,
                       size_t width);

    virtual void data( void *componentData,
                       void *propertyData,
                       const unsigned short *items,
                       size_t numItems,
                       size_t width);

    virtual void data( void *componentData,
                       void *propertyData,
                       const unsigned char *items,
                       size_t numItems,
                       size_t width);

    virtual void data( void *componentData,
                       void *propertyData,
                       const std::vector<std::string> &item,
                       size_t numItems,
                       size_t width);

#endif 

    virtual void declareMaya() {};
    
    void addToDefaultSG();
    
    void computeLocalTransform( const Object *parent );
    const MMatrix &globalTransform() const { return m_globalTransform; }

    static void filePrefix( std::string filePrefix )
                                     { m_filePrefix = filePrefix; }
    
protected:
    void setTransform( const float *transform );
    void setName();
    
protected:
    std::string m_name;
    std::string m_protocol;
    unsigned int m_protocolVersion;
    std::string m_parent;

    MMatrix m_globalTransform;
    MMatrix m_localTransform;

    MObject m_mayaObject;
    MObject m_mayaParentObject;
    bool m_wasRenamed;

    static std::string m_filePrefix;
    
    std::vector<float> m_tmpFloatData;
    int m_tmpIntData;
};

} // End namespace GtoIOPlugin

#endif

