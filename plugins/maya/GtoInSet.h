//******************************************************************************
// Copyright (c) 2001-2002 Tweak Inc. All rights reserved.
//******************************************************************************

#ifndef _GtoInSet_h_
#define _GtoInSet_h_

#include <string>
#include <map>
#include <vector>

#include "GtoInObject.h"

namespace GtoIOPlugin {

// Just a list of objects
class Set
{
protected:
    friend class DataBase;
    Set( const std::string &fileName );
    
    ~Set();

public:
    void addObject( Object *obj );

    Object *object( const std::string &name );

    void computeLocalTransforms();

    void declareMaya();
    
    void reparentAll();

private:
    std::string m_fileName;
    std::vector<Object *> m_objects;

    // Map of old->new name changes
    std::map<std::string, std::string> m_changedNames;
};

} // End namespace GtoIOPlugin

#endif
