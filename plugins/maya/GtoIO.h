//
// Copyright (C) 2003 Tweak Films
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
