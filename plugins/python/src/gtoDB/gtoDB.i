//******************************************************************************
// Copyright (c) 2008 Tippett Studio. All rights reserved.
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

%define DOCSTRING
"Python interface to the GTO RawData C++ class."
%enddef

// *****************************************************************************
%module(docstring=DOCSTRING) gtoDB

%include "exception.i"
%include "std_vector.i"
%include "std_string.i"
%include "carrays.i"

%include "Header.h"

// *****************************************************************************

// *****************************************************************************
//
// Cast a C++ pointer back from it's swig/Python wrapper.  A bit ugly...
//
%define PYOBJECT_CAST(TYPE, OBJECT, RESULT)                              
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

%define CAST_PYOBJECT(OBJECT, TYPE)
SWIG_NewPointerObj(SWIG_as_voidptr(OBJECT), SWIGTYPE_p_ ## TYPE, 0 );
%enddef

// Always use SWIG's built-in STL exception handling:
%exception { try { $action } SWIG_CATCH_STDEXCEPT }

%include "Property.i"
%include "Component.i"
%include "Object.i"
%include "RawData.i"


