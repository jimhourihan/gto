//******************************************************************************
// Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __GTOMAYAATTRIBUTE_H__
#define __GTOMAYAATTRIBUTE_H__

#include <vector>
#include <string>
#include <maya/MObjectHandle.h>
#include <Gto/Writer.h>

#include "GtoAnimCurve.h"

namespace GtoIOPlugin {

class GtoMayaAttribute {
public:

    GtoMayaAttribute( MObjectHandle nodeObj, 
                      MObjectHandle attributeObj,
                      const char *gtoComponentName=NULL );
    ~GtoMayaAttribute();

    void writeGtoHeader( Gto::Writer *writer ) const;
    void writeGtoData( Gto::Writer *writer ) const;
    
    const std::string &componentName() const { return m_componentName; }
    const std::string &propertyName() const { return m_propertyName; }

    bool isAnimated() const { return m_animCurve != NULL; }
    const GtoAnimCurve* animCurve() const { return m_animCurve; }

    static bool canExport( MPlug &plug, int depth=0 );

    static bool sortByComponent(const GtoMayaAttribute* s1, 
                                const GtoMayaAttribute* s2 );

private:
    MObjectHandle m_nodeObj;
    MObjectHandle m_attrObj;

    Gto::DataType m_dataType;

    GtoAnimCurve* m_animCurve;

    std::string m_componentName;
    std::string m_propertyName;
    std::string m_interp;
};

typedef std::vector<GtoMayaAttribute *> GtoMayaAttributes;
typedef std::vector<const GtoMayaAttribute *> ConstGtoMayaAttributes;


} // End namespace GtoIOPlugin

#endif    // End #ifdef __GTOMAYAATTRIBUTE_H__
