//******************************************************************************
// Copyright (c) 2002 Tweak Inc. 
// All rights reserved.
//******************************************************************************

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

} // End namespace GtoIOPlugin

#endif    // End #ifdef __GTOINCMD_H__
