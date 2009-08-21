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

#ifndef __PARTICLEATTRIBUTE_H__
#define __PARTICLEATTRIBUTE_H__

#include <string>
#include <vector>
#include <maya/MPlug.h>
#include <maya/MFnTypedAttribute.h>
#include <Gto/Writer.h>

//
// A ParticleAttribute represents a Maya attribute plug of type kDoubleArray, 
// kIntArray, or kVectorArray that knows how to write itself into a gto file.
//
class ParticleAttribute
{
public:
    ParticleAttribute( const std::string &attrName, 
                       const std::string &objectName );
    ~ParticleAttribute();

    MPlug attrPlug() const;

    const std::string &name() const;
    MFnData::Type type() const;
    size_t numElements() const;
    size_t partsPerElement() const;
    
    void writeGtoHeader( Gto::Writer *writer );
    void writeGtoData( Gto::Writer *writer );

private:
    const std::string m_attrName;
    const std::string m_objectName;

};

typedef std::vector<ParticleAttribute *> ParticleAttributes;

#endif    // End #ifdef __PARTICLEATTRIBUTE_H__
