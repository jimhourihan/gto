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
