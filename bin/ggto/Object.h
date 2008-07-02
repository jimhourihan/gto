//******************************************************************************
// Copyright (c) 2001-2006 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <string>
#include <Gto/Writer.h>
#include <UT/UT_Matrix4.h>
#include <UT/UT_Vector.h>
#include <UT/UT_BoundingBox.h>

namespace HGto {

class Object {
public:
    Object( std::string name, std::string protocol, int protoVersion );
    virtual ~Object();

    virtual void writeHeader( Gto::Writer &writer );
    virtual void writeData( Gto::Writer &writer );

    UT_Matrix4 &globalMatrix() { return m_globalMatrix; }
    UT_BoundingBox &boundingBox() { return m_boundingBox; }

    std::string &name() { return m_name; }
    
//     virtual void writePointAttribHeader( Gto::Writer &writer, const GEO_Detail *gd );
//     virtual void writePointAttribData( Gto::Writer &writer, const GEO_Detail *gd );

protected:
    std::string m_name;
    std::string m_protocol;
    int m_protoVersion;

    UT_Matrix4 m_globalMatrix;
    UT_BoundingBox m_boundingBox;
};

}  //  End namespace HGto


#endif    // End #ifdef __OBJECT_H__


