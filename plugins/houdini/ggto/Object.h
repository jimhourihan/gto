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

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <string>
#include <Gto/Writer.h>
#include <TwkMath/TwkMathMat44.h>
#include <TwkMath/TwkMathBox.h>

namespace HGto {

class Object {
public:
    Object( std::string name, std::string protocol, int protoVersion );
    virtual ~Object();

    virtual void writeHeader( Gto::Writer &writer );
    virtual void writeData( Gto::Writer &writer );

    TwkMath::Mat44f &globalMatrix() { return m_globalMatrix; }
    TwkMath::Box2f &boundingBox() { return m_boundingBox; }

    std::string &name() { return m_name; }
    
//     virtual void writePointAttribHeader( Gto::Writer &writer, const GEO_Detail *gd );
//     virtual void writePointAttribData( Gto::Writer &writer, const GEO_Detail *gd );

protected:
    std::string m_name;
    std::string m_protocol;
    int m_protoVersion;

    TwkMath::Mat44f m_globalMatrix;
    TwkMath::Box2f m_boundingBox;
};

}  //  End namespace HGto


#endif    // End #ifdef __OBJECT_H__


