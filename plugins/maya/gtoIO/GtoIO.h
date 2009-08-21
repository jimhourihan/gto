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

#ifndef __GTOINCMD_H__
#define __GTOINCMD_H__

#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MPxFileTranslator.h>
#include <maya/MString.h>
#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>

namespace GtoIOPlugin {

class GtoIO : public MPxFileTranslator
{
public:
    GtoIO();

    static void *creator();
    
    virtual MStatus reader( const MFileObject &file, 
                            const MString &optionsString,
                            MPxFileTranslator::FileAccessMode mode );

    virtual MStatus writer( const MFileObject &file, 
                            const MString &optionsString, 
                            MPxFileTranslator::FileAccessMode mode );
  
    virtual bool haveReadMethod() const { return true; }
    virtual bool haveWriteMethod() const { return true; }
    
    virtual MString defaultExtension() const { return "gto"; }
    
    // Allow 'File...import' and 'File...Open'
    virtual bool canBeOpened() const { return true; }
    
    virtual MPxFileTranslator::MFileKind identifyFile( const MFileObject &file,
                                                       const char *magic,
                                                       short magicSize ) const;
    
protected:
};

MString replaceFrameCookies( const MString &in, int frame );

} // End namespace GtoIOPlugin

#endif    // End #ifdef __GTOINCMD_H__
