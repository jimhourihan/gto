//******************************************************************************
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
//******************************************************************************

%define MODULE_DOCSTRING
"This module defines a generic GtoDB class.  This class can be used to
work with GTO data at an extremely high level, without the need to derive a new
reader class or change your workflow to accomodate the somewhat restrictive GTO
I/O routines.

The gtoDB module is intended to replace the older gtoContainer module.  It's a
lot faster, since it doesn't convert data into Python objects until needed.
It also doesn't try to be quite as fancy, so accessing objects, components,
and properties as if they were attributes is no longer supported.

Perhaps the best introduction to what gtoDB can do is
with a few examples.  Here's a simple one that prints the names of every
object, component, and property in a file:

    from gtoDB import *

    for object in GtoDB('foo.gto'):
        for component in object:
            for property in component:
                print object, component, property

Let's say that you want to check every object for the existence of a particular
component/property and print the stored data if it's there:

    for obj in GtoDB( 'foo.gto' ):
        if 'channels' in obj and 'diff' in obj['channels']:
            print obj['channels']['diff']()


Let's say we want to start fresh and make a really simple gto file:

    #!/usr/bin/env python
    
    from gtoDB import *

    gtoFile = GtoDB()

    obj = Object('myobject', 'objprotocol', 1)
    gtoFile.append(obj)

    comp = Component('mycomponent')
    obj.append(comp)
    
    comp.append(Property('intProp', range(1,11)))
    comp.append(Property('strProp', 'hello'))
    comp.append(Property('floatProp', (1.1, 2.2)))

    gtoFile.write('test.gto')


One very important note:  GtoDB maintains a separate C++ database of data in
memory (using Gto::RawData).  Objects, Components, and most importantly
Properties (and their data) are converted into Python objects on-demand. 
Also, each of these containers delete their contents when destroyed. 
Therefore, don't hang on to a Property instance, for example, after the GtoDB
object has gone out of scope.  The data-access methods on the Property object
will return a copy of the data that *is* safe to hang on to after the Property
itself is gone.

"
%enddef

// *****************************************************************************
%module(docstring=MODULE_DOCSTRING) gtoDB

%include "exception.i"
%include "std_vector.i"
%include "std_string.i"
%include "carrays.i"

%ignore "Header";
%ignore "ObjectHeader";
%ignore "ObjectHeader_v2";
%ignore "ComponentHeader";
%ignore "ComponentHeader_v2";
%ignore "PropertyHeader";
%ignore "PropertyHeader_v2";

%include "Gto/Header.h"

// *****************************************************************************

// *****************************************************************************
//
// Cast a C++ pointer back from it's swig/Python wrapper.  A bit ugly...
//
%define CAST_PYOBJECT_TO_C(TYPE, OBJECT, RESULT)                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(OBJECT, &vptr, SWIGTYPE_p_ ## TYPE, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    RESULT = reinterpret_cast<TYPE*>(vptr);                              
}
%enddef

%define CAST_C_TO_PYOBJECT(OBJECT, TYPE)
SWIG_NewPointerObj(SWIG_as_voidptr(OBJECT), SWIGTYPE_p_ ## TYPE, 0 );
%enddef

// Always use SWIG's built-in STL exception handling:
%exception { try { $action } SWIG_CATCH_STDEXCEPT }

typedef std::vector<std::string> Strings;
%template(Strings) std::vector<std::string>;

%include "Property.i"
%include "Component.i"
%include "Object.i"
%include "GtoDB.i"
