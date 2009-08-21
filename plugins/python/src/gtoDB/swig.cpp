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

#define SWIGPYTHON
#define SWIG_PYTHON_DIRECTOR_NO_VTABLE

#ifdef __cplusplus
template<class T> class SwigValueWrapper {
    T *tt;
public:
    SwigValueWrapper() : tt(0) { }
    SwigValueWrapper(const SwigValueWrapper<T>& rhs) : tt(new T(*rhs.tt)) { }
    SwigValueWrapper(const T& t) : tt(new T(t)) { }
    ~SwigValueWrapper() { delete tt; } 
    SwigValueWrapper& operator=(const T& t) { delete tt; tt = new T(t); return *this; }
    operator T&() const { return *tt; }
    T *operator&() { return tt; }
private:
    SwigValueWrapper& operator=(const SwigValueWrapper<T>& rhs);
};
#endif

/* -----------------------------------------------------------------------------
 *  This section contains generic SWIG labels for method/variable
 *  declarations/attributes, and other compiler dependent labels.
 * ----------------------------------------------------------------------------- */

/* template workaround for compilers that cannot correctly implement the C++ standard */
#ifndef SWIGTEMPLATEDISAMBIGUATOR
# if defined(__SUNPRO_CC)
#   if (__SUNPRO_CC <= 0x560)
#     define SWIGTEMPLATEDISAMBIGUATOR template
#   else
#     define SWIGTEMPLATEDISAMBIGUATOR 
#   endif
# else
#   define SWIGTEMPLATEDISAMBIGUATOR 
# endif
#endif

/* inline attribute */
#ifndef SWIGINLINE
# if defined(__cplusplus) || (defined(__GNUC__) && !defined(__STRICT_ANSI__))
#   define SWIGINLINE inline
# else
#   define SWIGINLINE
# endif
#endif

/* attribute recognised by some compilers to avoid 'unused' warnings */
#ifndef SWIGUNUSED
# if defined(__GNUC__)
#   if !(defined(__cplusplus)) || (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#     define SWIGUNUSED __attribute__ ((__unused__)) 
#   else
#     define SWIGUNUSED
#   endif
# elif defined(__ICC)
#   define SWIGUNUSED __attribute__ ((__unused__)) 
# else
#   define SWIGUNUSED 
# endif
#endif

#ifndef SWIGUNUSEDPARM
# ifdef __cplusplus
#   define SWIGUNUSEDPARM(p)
# else
#   define SWIGUNUSEDPARM(p) p SWIGUNUSED 
# endif
#endif

/* internal SWIG method */
#ifndef SWIGINTERN
# define SWIGINTERN static SWIGUNUSED
#endif

/* internal inline SWIG method */
#ifndef SWIGINTERNINLINE
# define SWIGINTERNINLINE SWIGINTERN SWIGINLINE
#endif

/* exporting methods */
#if (__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#  ifndef GCC_HASCLASSVISIBILITY
#    define GCC_HASCLASSVISIBILITY
#  endif
#endif

#ifndef SWIGEXPORT
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   if defined(STATIC_LINKED)
#     define SWIGEXPORT
#   else
#     define SWIGEXPORT __declspec(dllexport)
#   endif
# else
#   if defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
#     define SWIGEXPORT __attribute__ ((visibility("default")))
#   else
#     define SWIGEXPORT
#   endif
# endif
#endif

/* calling conventions for Windows */
#ifndef SWIGSTDCALL
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define SWIGSTDCALL __stdcall
# else
#   define SWIGSTDCALL
# endif 
#endif

/* Deal with Microsoft's attempt at deprecating C standard runtime functions */
#if !defined(SWIG_NO_CRT_SECURE_NO_DEPRECATE) && defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
# define _CRT_SECURE_NO_DEPRECATE
#endif


/* Python.h has to appear first */
#include <Python.h>

/* -----------------------------------------------------------------------------
 * swigrun.swg
 *
 * This file contains generic CAPI SWIG runtime support for pointer
 * type checking.
 * ----------------------------------------------------------------------------- */

/* This should only be incremented when either the layout of swig_type_info changes,
   or for whatever reason, the runtime changes incompatibly */
#define SWIG_RUNTIME_VERSION "3"

/* define SWIG_TYPE_TABLE_NAME as "SWIG_TYPE_TABLE" */
#ifdef SWIG_TYPE_TABLE
# define SWIG_QUOTE_STRING(x) #x
# define SWIG_EXPAND_AND_QUOTE_STRING(x) SWIG_QUOTE_STRING(x)
# define SWIG_TYPE_TABLE_NAME SWIG_EXPAND_AND_QUOTE_STRING(SWIG_TYPE_TABLE)
#else
# define SWIG_TYPE_TABLE_NAME
#endif

/*
  You can use the SWIGRUNTIME and SWIGRUNTIMEINLINE macros for
  creating a static or dynamic library from the swig runtime code.
  In 99.9% of the cases, swig just needs to declare them as 'static'.
  
  But only do this if is strictly necessary, ie, if you have problems
  with your compiler or so.
*/

#ifndef SWIGRUNTIME
# define SWIGRUNTIME SWIGINTERN
#endif

#ifndef SWIGRUNTIMEINLINE
# define SWIGRUNTIMEINLINE SWIGRUNTIME SWIGINLINE
#endif

/*  Generic buffer size */
#ifndef SWIG_BUFFER_SIZE
# define SWIG_BUFFER_SIZE 1024
#endif

/* Flags for pointer conversions */
#define SWIG_POINTER_DISOWN        0x1

/* Flags for new pointer objects */
#define SWIG_POINTER_OWN           0x1


/* 
   Flags/methods for returning states.
   
   The swig conversion methods, as ConvertPtr, return and integer 
   that tells if the conversion was successful or not. And if not,
   an error code can be returned (see swigerrors.swg for the codes).
   
   Use the following macros/flags to set or process the returning
   states.
   
   In old swig versions, you usually write code as:

     if (SWIG_ConvertPtr(obj,vptr,ty.flags) != -1) {
       // success code
     } else {
       //fail code
     }

   Now you can be more explicit as:

    int res = SWIG_ConvertPtr(obj,vptr,ty.flags);
    if (SWIG_IsOK(res)) {
      // success code
    } else {
      // fail code
    }

   that seems to be the same, but now you can also do

    Type *ptr;
    int res = SWIG_ConvertPtr(obj,(void **)(&ptr),ty.flags);
    if (SWIG_IsOK(res)) {
      // success code
      if (SWIG_IsNewObj(res) {
        ...
	delete *ptr;
      } else {
        ...
      }
    } else {
      // fail code
    }
    
   I.e., now SWIG_ConvertPtr can return new objects and you can
   identify the case and take care of the deallocation. Of course that
   requires also to SWIG_ConvertPtr to return new result values, as

      int SWIG_ConvertPtr(obj, ptr,...) {         
        if (<obj is ok>) {			       
          if (<need new object>) {		       
            *ptr = <ptr to new allocated object>; 
            return SWIG_NEWOBJ;		       
          } else {				       
            *ptr = <ptr to old object>;	       
            return SWIG_OLDOBJ;		       
          } 				       
        } else {				       
          return SWIG_BADOBJ;		       
        }					       
      }

   Of course, returning the plain '0(success)/-1(fail)' still works, but you can be
   more explicit by returning SWIG_BADOBJ, SWIG_ERROR or any of the
   swig errors code.

   Finally, if the SWIG_CASTRANK_MODE is enabled, the result code
   allows to return the 'cast rank', for example, if you have this

       int food(double)
       int fooi(int);

   and you call
 
      food(1)   // cast rank '1'  (1 -> 1.0)
      fooi(1)   // cast rank '0'

   just use the SWIG_AddCast()/SWIG_CheckState()


 */
#define SWIG_OK                    (0) 
#define SWIG_ERROR                 (-1)
#define SWIG_IsOK(r)               (r >= 0)
#define SWIG_ArgError(r)           ((r != SWIG_ERROR) ? r : SWIG_TypeError)  

/* The CastRankLimit says how many bits are used for the cast rank */
#define SWIG_CASTRANKLIMIT         (1 << 8)
/* The NewMask denotes the object was created (using new/malloc) */
#define SWIG_NEWOBJMASK            (SWIG_CASTRANKLIMIT  << 1)
/* The TmpMask is for in/out typemaps that use temporal objects */
#define SWIG_TMPOBJMASK            (SWIG_NEWOBJMASK << 1)
/* Simple returning values */
#define SWIG_BADOBJ                (SWIG_ERROR)
#define SWIG_OLDOBJ                (SWIG_OK)
#define SWIG_NEWOBJ                (SWIG_OK | SWIG_NEWOBJMASK)
#define SWIG_TMPOBJ                (SWIG_OK | SWIG_TMPOBJMASK)
/* Check, add and del mask methods */
#define SWIG_AddNewMask(r)         (SWIG_IsOK(r) ? (r | SWIG_NEWOBJMASK) : r)
#define SWIG_DelNewMask(r)         (SWIG_IsOK(r) ? (r & ~SWIG_NEWOBJMASK) : r)
#define SWIG_IsNewObj(r)           (SWIG_IsOK(r) && (r & SWIG_NEWOBJMASK))
#define SWIG_AddTmpMask(r)         (SWIG_IsOK(r) ? (r | SWIG_TMPOBJMASK) : r)
#define SWIG_DelTmpMask(r)         (SWIG_IsOK(r) ? (r & ~SWIG_TMPOBJMASK) : r)
#define SWIG_IsTmpObj(r)           (SWIG_IsOK(r) && (r & SWIG_TMPOBJMASK))


/* Cast-Rank Mode */
#if defined(SWIG_CASTRANK_MODE)
#  ifndef SWIG_TypeRank
#    define SWIG_TypeRank             unsigned long
#  endif
#  ifndef SWIG_MAXCASTRANK            /* Default cast allowed */
#    define SWIG_MAXCASTRANK          (2)
#  endif
#  define SWIG_CASTRANKMASK          ((SWIG_CASTRANKLIMIT) -1)
#  define SWIG_CastRank(r)           (r & SWIG_CASTRANKMASK)
SWIGINTERNINLINE int SWIG_AddCast(int r) { 
  return SWIG_IsOK(r) ? ((SWIG_CastRank(r) < SWIG_MAXCASTRANK) ? (r + 1) : SWIG_ERROR) : r;
}
SWIGINTERNINLINE int SWIG_CheckState(int r) { 
  return SWIG_IsOK(r) ? SWIG_CastRank(r) + 1 : 0; 
}
#else /* no cast-rank mode */
#  define SWIG_AddCast
#  define SWIG_CheckState(r) (SWIG_IsOK(r) ? 1 : 0)
#endif




#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *(*swig_converter_func)(void *);
typedef struct swig_type_info *(*swig_dycast_func)(void **);

/* Structure to store inforomation on one type */
typedef struct swig_type_info {
  const char             *name;			/* mangled name of this type */
  const char             *str;			/* human readable name of this type */
  swig_dycast_func        dcast;		/* dynamic cast function down a hierarchy */
  struct swig_cast_info  *cast;			/* linked list of types that can cast into this type */
  void                   *clientdata;		/* language specific type data */
  int                    owndata;		/* flag if the structure owns the clientdata */
} swig_type_info;

/* Structure to store a type and conversion function used for casting */
typedef struct swig_cast_info {
  swig_type_info         *type;			/* pointer to type that is equivalent to this type */
  swig_converter_func     converter;		/* function to cast the void pointers */
  struct swig_cast_info  *next;			/* pointer to next cast in linked list */
  struct swig_cast_info  *prev;			/* pointer to the previous cast */
} swig_cast_info;

/* Structure used to store module information
 * Each module generates one structure like this, and the runtime collects
 * all of these structures and stores them in a circularly linked list.*/
typedef struct swig_module_info {
  swig_type_info         **types;		/* Array of pointers to swig_type_info structures that are in this module */
  size_t                 size;		        /* Number of types in this module */
  struct swig_module_info *next;		/* Pointer to next element in circularly linked list */
  swig_type_info         **type_initial;	/* Array of initially generated type structures */
  swig_cast_info         **cast_initial;	/* Array of initially generated casting structures */
  void                    *clientdata;		/* Language specific module data */
} swig_module_info;

/* 
  Compare two type names skipping the space characters, therefore
  "char*" == "char *" and "Class<int>" == "Class<int >", etc.

  Return 0 when the two name types are equivalent, as in
  strncmp, but skipping ' '.
*/
SWIGRUNTIME int
SWIG_TypeNameComp(const char *f1, const char *l1,
		  const char *f2, const char *l2) {
  for (;(f1 != l1) && (f2 != l2); ++f1, ++f2) {
    while ((*f1 == ' ') && (f1 != l1)) ++f1;
    while ((*f2 == ' ') && (f2 != l2)) ++f2;
    if (*f1 != *f2) return (*f1 > *f2) ? 1 : -1;
  }
  return (l1 - f1) - (l2 - f2);
}

/*
  Check type equivalence in a name list like <name1>|<name2>|...
  Return 0 if not equal, 1 if equal
*/
SWIGRUNTIME int
SWIG_TypeEquiv(const char *nb, const char *tb) {
  int equiv = 0;
  const char* te = tb + strlen(tb);
  const char* ne = nb;
  while (!equiv && *ne) {
    for (nb = ne; *ne; ++ne) {
      if (*ne == '|') break;
    }
    equiv = (SWIG_TypeNameComp(nb, ne, tb, te) == 0) ? 1 : 0;
    if (*ne) ++ne;
  }
  return equiv;
}

/*
  Check type equivalence in a name list like <name1>|<name2>|...
  Return 0 if equal, -1 if nb < tb, 1 if nb > tb
*/
SWIGRUNTIME int
SWIG_TypeCompare(const char *nb, const char *tb) {
  int equiv = 0;
  const char* te = tb + strlen(tb);
  const char* ne = nb;
  while (!equiv && *ne) {
    for (nb = ne; *ne; ++ne) {
      if (*ne == '|') break;
    }
    equiv = (SWIG_TypeNameComp(nb, ne, tb, te) == 0) ? 1 : 0;
    if (*ne) ++ne;
  }
  return equiv;
}


/* think of this as a c++ template<> or a scheme macro */
#define SWIG_TypeCheck_Template(comparison, ty)         \
  if (ty) {                                             \
    swig_cast_info *iter = ty->cast;                    \
    while (iter) {                                      \
      if (comparison) {                                 \
        if (iter == ty->cast) return iter;              \
        /* Move iter to the top of the linked list */   \
        iter->prev->next = iter->next;                  \
        if (iter->next)                                 \
          iter->next->prev = iter->prev;                \
        iter->next = ty->cast;                          \
        iter->prev = 0;                                 \
        if (ty->cast) ty->cast->prev = iter;            \
        ty->cast = iter;                                \
        return iter;                                    \
      }                                                 \
      iter = iter->next;                                \
    }                                                   \
  }                                                     \
  return 0

/*
  Check the typename
*/
SWIGRUNTIME swig_cast_info *
SWIG_TypeCheck(const char *c, swig_type_info *ty) {
  SWIG_TypeCheck_Template(strcmp(iter->type->name, c) == 0, ty);
}

/* Same as previous function, except strcmp is replaced with a pointer comparison */
SWIGRUNTIME swig_cast_info *
SWIG_TypeCheckStruct(swig_type_info *from, swig_type_info *into) {
  SWIG_TypeCheck_Template(iter->type == from, into);
}

/*
  Cast a pointer up an inheritance hierarchy
*/
SWIGRUNTIMEINLINE void *
SWIG_TypeCast(swig_cast_info *ty, void *ptr) {
  return ((!ty) || (!ty->converter)) ? ptr : (*ty->converter)(ptr);
}

/* 
   Dynamic pointer casting. Down an inheritance hierarchy
*/
SWIGRUNTIME swig_type_info *
SWIG_TypeDynamicCast(swig_type_info *ty, void **ptr) {
  swig_type_info *lastty = ty;
  if (!ty || !ty->dcast) return ty;
  while (ty && (ty->dcast)) {
    ty = (*ty->dcast)(ptr);
    if (ty) lastty = ty;
  }
  return lastty;
}

/*
  Return the name associated with this type
*/
SWIGRUNTIMEINLINE const char *
SWIG_TypeName(const swig_type_info *ty) {
  return ty->name;
}

/*
  Return the pretty name associated with this type,
  that is an unmangled type name in a form presentable to the user.
*/
SWIGRUNTIME const char *
SWIG_TypePrettyName(const swig_type_info *type) {
  /* The "str" field contains the equivalent pretty names of the
     type, separated by vertical-bar characters.  We choose
     to print the last name, as it is often (?) the most
     specific. */
  if (!type) return NULL;
  if (type->str != NULL) {
    const char *last_name = type->str;
    const char *s;
    for (s = type->str; *s; s++)
      if (*s == '|') last_name = s+1;
    return last_name;
  }
  else
    return type->name;
}

/* 
   Set the clientdata field for a type
*/
SWIGRUNTIME void
SWIG_TypeClientData(swig_type_info *ti, void *clientdata) {
  swig_cast_info *cast = ti->cast;
  /* if (ti->clientdata == clientdata) return; */
  ti->clientdata = clientdata;
  
  while (cast) {
    if (!cast->converter) {
      swig_type_info *tc = cast->type;
      if (!tc->clientdata) {
	SWIG_TypeClientData(tc, clientdata);
      }
    }    
    cast = cast->next;
  }
}
SWIGRUNTIME void
SWIG_TypeNewClientData(swig_type_info *ti, void *clientdata) {
  SWIG_TypeClientData(ti, clientdata);
  ti->owndata = 1;
}
  
/*
  Search for a swig_type_info structure only by mangled name
  Search is a O(log #types)
  
  We start searching at module start, and finish searching when start == end.  
  Note: if start == end at the beginning of the function, we go all the way around
  the circular list.
*/
SWIGRUNTIME swig_type_info *
SWIG_MangledTypeQueryModule(swig_module_info *start, 
                            swig_module_info *end, 
		            const char *name) {
  swig_module_info *iter = start;
  do {
    if (iter->size) {
      register size_t l = 0;
      register size_t r = iter->size - 1;
      do {
	/* since l+r >= 0, we can (>> 1) instead (/ 2) */
	register size_t i = (l + r) >> 1; 
	const char *iname = iter->types[i]->name;
	if (iname) {
	  register int compare = strcmp(name, iname);
	  if (compare == 0) {	    
	    return iter->types[i];
	  } else if (compare < 0) {
	    if (i) {
	      r = i - 1;
	    } else {
	      break;
	    }
	  } else if (compare > 0) {
	    l = i + 1;
	  }
	} else {
	  break; /* should never happen */
	}
      } while (l <= r);
    }
    iter = iter->next;
  } while (iter != end);
  return 0;
}

/*
  Search for a swig_type_info structure for either a mangled name or a human readable name.
  It first searches the mangled names of the types, which is a O(log #types)
  If a type is not found it then searches the human readable names, which is O(#types).
  
  We start searching at module start, and finish searching when start == end.  
  Note: if start == end at the beginning of the function, we go all the way around
  the circular list.
*/
SWIGRUNTIME swig_type_info *
SWIG_TypeQueryModule(swig_module_info *start, 
                     swig_module_info *end, 
		     const char *name) {
  /* STEP 1: Search the name field using binary search */
  swig_type_info *ret = SWIG_MangledTypeQueryModule(start, end, name);
  if (ret) {
    return ret;
  } else {
    /* STEP 2: If the type hasn't been found, do a complete search
       of the str field (the human readable name) */
    swig_module_info *iter = start;
    do {
      register size_t i = 0;
      for (; i < iter->size; ++i) {
	if (iter->types[i]->str && (SWIG_TypeEquiv(iter->types[i]->str, name)))
	  return iter->types[i];
      }
      iter = iter->next;
    } while (iter != end);
  }
  
  /* neither found a match */
  return 0;
}

/* 
   Pack binary data into a string
*/
SWIGRUNTIME char *
SWIG_PackData(char *c, void *ptr, size_t sz) {
  static const char hex[17] = "0123456789abcdef";
  register const unsigned char *u = (unsigned char *) ptr;
  register const unsigned char *eu =  u + sz;
  for (; u != eu; ++u) {
    register unsigned char uu = *u;
    *(c++) = hex[(uu & 0xf0) >> 4];
    *(c++) = hex[uu & 0xf];
  }
  return c;
}

/* 
   Unpack binary data from a string
*/
SWIGRUNTIME const char *
SWIG_UnpackData(const char *c, void *ptr, size_t sz) {
  register unsigned char *u = (unsigned char *) ptr;
  register const unsigned char *eu = u + sz;
  for (; u != eu; ++u) {
    register char d = *(c++);
    register unsigned char uu;
    if ((d >= '0') && (d <= '9'))
      uu = ((d - '0') << 4);
    else if ((d >= 'a') && (d <= 'f'))
      uu = ((d - ('a'-10)) << 4);
    else 
      return (char *) 0;
    d = *(c++);
    if ((d >= '0') && (d <= '9'))
      uu |= (d - '0');
    else if ((d >= 'a') && (d <= 'f'))
      uu |= (d - ('a'-10));
    else 
      return (char *) 0;
    *u = uu;
  }
  return c;
}

/* 
   Pack 'void *' into a string buffer.
*/
SWIGRUNTIME char *
SWIG_PackVoidPtr(char *buff, void *ptr, const char *name, size_t bsz) {
  char *r = buff;
  if ((2*sizeof(void *) + 2) > bsz) return 0;
  *(r++) = '_';
  r = SWIG_PackData(r,&ptr,sizeof(void *));
  if (strlen(name) + 1 > (bsz - (r - buff))) return 0;
  strcpy(r,name);
  return buff;
}

SWIGRUNTIME const char *
SWIG_UnpackVoidPtr(const char *c, void **ptr, const char *name) {
  if (*c != '_') {
    if (strcmp(c,"NULL") == 0) {
      *ptr = (void *) 0;
      return name;
    } else {
      return 0;
    }
  }
  return SWIG_UnpackData(++c,ptr,sizeof(void *));
}

SWIGRUNTIME char *
SWIG_PackDataName(char *buff, void *ptr, size_t sz, const char *name, size_t bsz) {
  char *r = buff;
  size_t lname = (name ? strlen(name) : 0);
  if ((2*sz + 2 + lname) > bsz) return 0;
  *(r++) = '_';
  r = SWIG_PackData(r,ptr,sz);
  if (lname) {
    strncpy(r,name,lname+1);
  } else {
    *r = 0;
  }
  return buff;
}

SWIGRUNTIME const char *
SWIG_UnpackDataName(const char *c, void *ptr, size_t sz, const char *name) {
  if (*c != '_') {
    if (strcmp(c,"NULL") == 0) {
      memset(ptr,0,sz);
      return name;
    } else {
      return 0;
    }
  }
  return SWIG_UnpackData(++c,ptr,sz);
}

#ifdef __cplusplus
}
#endif

/*  Errors in SWIG */
#define  SWIG_UnknownError    	   -1 
#define  SWIG_IOError        	   -2 
#define  SWIG_RuntimeError   	   -3 
#define  SWIG_IndexError     	   -4 
#define  SWIG_TypeError      	   -5 
#define  SWIG_DivisionByZero 	   -6 
#define  SWIG_OverflowError  	   -7 
#define  SWIG_SyntaxError    	   -8 
#define  SWIG_ValueError     	   -9 
#define  SWIG_SystemError    	   -10
#define  SWIG_AttributeError 	   -11
#define  SWIG_MemoryError    	   -12 
#define  SWIG_NullReferenceError   -13




/* Add PyOS_snprintf for old Pythons */
#if PY_VERSION_HEX < 0x02020000
# if defined(_MSC_VER) || defined(__BORLANDC__) || defined(_WATCOM)
#  define PyOS_snprintf _snprintf
# else
#  define PyOS_snprintf snprintf
# endif
#endif

/* A crude PyString_FromFormat implementation for old Pythons */
#if PY_VERSION_HEX < 0x02020000

#ifndef SWIG_PYBUFFER_SIZE
# define SWIG_PYBUFFER_SIZE 1024
#endif

static PyObject *
PyString_FromFormat(const char *fmt, ...) {
  va_list ap;
  char buf[SWIG_PYBUFFER_SIZE * 2];
  int res;
  va_start(ap, fmt);
  res = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  return (res < 0 || res >= (int)sizeof(buf)) ? 0 : PyString_FromString(buf);
}
#endif

/* Add PyObject_Del for old Pythons */
#if PY_VERSION_HEX < 0x01060000
# define PyObject_Del(op) PyMem_DEL((op))
#endif
#ifndef PyObject_DEL
# define PyObject_DEL PyObject_Del
#endif

/* A crude PyExc_StopIteration exception for old Pythons */
#if PY_VERSION_HEX < 0x02020000
# ifndef PyExc_StopIteration
#  define PyExc_StopIteration PyExc_RuntimeError
# endif
# ifndef PyObject_GenericGetAttr
#  define PyObject_GenericGetAttr 0
# endif
#endif
/* Py_NotImplemented is defined in 2.1 and up. */
#if PY_VERSION_HEX < 0x02010000
# ifndef Py_NotImplemented
#  define Py_NotImplemented PyExc_RuntimeError
# endif
#endif


/* A crude PyString_AsStringAndSize implementation for old Pythons */
#if PY_VERSION_HEX < 0x02010000
# ifndef PyString_AsStringAndSize
#  define PyString_AsStringAndSize(obj, s, len) {*s = PyString_AsString(obj); *len = *s ? strlen(*s) : 0;}
# endif
#endif

/* PySequence_Size for old Pythons */
#if PY_VERSION_HEX < 0x02000000
# ifndef PySequence_Size
#  define PySequence_Size PySequence_Length
# endif
#endif


/* PyBool_FromLong for old Pythons */
#if PY_VERSION_HEX < 0x02030000
static
PyObject *PyBool_FromLong(long ok)
{
  PyObject *result = ok ? Py_True : Py_False;
  Py_INCREF(result);
  return result;
}
#endif

/* Py_ssize_t for old Pythons */
/* This code is as recommended by: */
/* http://www.python.org/dev/peps/pep-0353/#conversion-guidelines */
#if PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN)
typedef int Py_ssize_t;
# define PY_SSIZE_T_MAX INT_MAX
# define PY_SSIZE_T_MIN INT_MIN
#endif

/* -----------------------------------------------------------------------------
 * error manipulation
 * ----------------------------------------------------------------------------- */

SWIGRUNTIME PyObject*
SWIG_Python_ErrorType(int code) {
  PyObject* type = 0;
  switch(code) {
  case SWIG_MemoryError:
    type = PyExc_MemoryError;
    break;
  case SWIG_IOError:
    type = PyExc_IOError;
    break;
  case SWIG_RuntimeError:
    type = PyExc_RuntimeError;
    break;
  case SWIG_IndexError:
    type = PyExc_IndexError;
    break;
  case SWIG_TypeError:
    type = PyExc_TypeError;
    break;
  case SWIG_DivisionByZero:
    type = PyExc_ZeroDivisionError;
    break;
  case SWIG_OverflowError:
    type = PyExc_OverflowError;
    break;
  case SWIG_SyntaxError:
    type = PyExc_SyntaxError;
    break;
  case SWIG_ValueError:
    type = PyExc_ValueError;
    break;
  case SWIG_SystemError:
    type = PyExc_SystemError;
    break;
  case SWIG_AttributeError:
    type = PyExc_AttributeError;
    break;
  default:
    type = PyExc_RuntimeError;
  }
  return type;
}


SWIGRUNTIME void
SWIG_Python_AddErrorMsg(const char* mesg)
{
  PyObject *type = 0;
  PyObject *value = 0;
  PyObject *traceback = 0;

  if (PyErr_Occurred()) PyErr_Fetch(&type, &value, &traceback);
  if (value) {
    PyObject *old_str = PyObject_Str(value);
    PyErr_Clear();
    Py_XINCREF(type);
    PyErr_Format(type, "%s %s", PyString_AsString(old_str), mesg);
    Py_DECREF(old_str);
    Py_DECREF(value);
  } else {
    PyErr_Format(PyExc_RuntimeError, mesg);
  }
}



#if defined(SWIG_PYTHON_NO_THREADS)
#  if defined(SWIG_PYTHON_THREADS)
#    undef SWIG_PYTHON_THREADS
#  endif
#endif
#if defined(SWIG_PYTHON_THREADS) /* Threading support is enabled */
#  if !defined(SWIG_PYTHON_USE_GIL) && !defined(SWIG_PYTHON_NO_USE_GIL)
#    if (PY_VERSION_HEX >= 0x02030000) /* For 2.3 or later, use the PyGILState calls */
#      define SWIG_PYTHON_USE_GIL
#    endif
#  endif
#  if defined(SWIG_PYTHON_USE_GIL) /* Use PyGILState threads calls */
#    ifndef SWIG_PYTHON_INITIALIZE_THREADS
#     define SWIG_PYTHON_INITIALIZE_THREADS  PyEval_InitThreads() 
#    endif
#    ifdef __cplusplus /* C++ code */
       class SWIG_Python_Thread_Block {
         bool status;
         PyGILState_STATE state;
       public:
         void end() { if (status) { PyGILState_Release(state); status = false;} }
         SWIG_Python_Thread_Block() : status(true), state(PyGILState_Ensure()) {}
         ~SWIG_Python_Thread_Block() { end(); }
       };
       class SWIG_Python_Thread_Allow {
         bool status;
         PyThreadState *save;
       public:
         void end() { if (status) { PyEval_RestoreThread(save); status = false; }}
         SWIG_Python_Thread_Allow() : status(true), save(PyEval_SaveThread()) {}
         ~SWIG_Python_Thread_Allow() { end(); }
       };
#      define SWIG_PYTHON_THREAD_BEGIN_BLOCK   SWIG_Python_Thread_Block _swig_thread_block
#      define SWIG_PYTHON_THREAD_END_BLOCK     _swig_thread_block.end()
#      define SWIG_PYTHON_THREAD_BEGIN_ALLOW   SWIG_Python_Thread_Allow _swig_thread_allow
#      define SWIG_PYTHON_THREAD_END_ALLOW     _swig_thread_allow.end()
#    else /* C code */
#      define SWIG_PYTHON_THREAD_BEGIN_BLOCK   PyGILState_STATE _swig_thread_block = PyGILState_Ensure()
#      define SWIG_PYTHON_THREAD_END_BLOCK     PyGILState_Release(_swig_thread_block)
#      define SWIG_PYTHON_THREAD_BEGIN_ALLOW   PyThreadState *_swig_thread_allow = PyEval_SaveThread()
#      define SWIG_PYTHON_THREAD_END_ALLOW     PyEval_RestoreThread(_swig_thread_allow)
#    endif
#  else /* Old thread way, not implemented, user must provide it */
#    if !defined(SWIG_PYTHON_INITIALIZE_THREADS)
#      define SWIG_PYTHON_INITIALIZE_THREADS
#    endif
#    if !defined(SWIG_PYTHON_THREAD_BEGIN_BLOCK)
#      define SWIG_PYTHON_THREAD_BEGIN_BLOCK
#    endif
#    if !defined(SWIG_PYTHON_THREAD_END_BLOCK)
#      define SWIG_PYTHON_THREAD_END_BLOCK
#    endif
#    if !defined(SWIG_PYTHON_THREAD_BEGIN_ALLOW)
#      define SWIG_PYTHON_THREAD_BEGIN_ALLOW
#    endif
#    if !defined(SWIG_PYTHON_THREAD_END_ALLOW)
#      define SWIG_PYTHON_THREAD_END_ALLOW
#    endif
#  endif
#else /* No thread support */
#  define SWIG_PYTHON_INITIALIZE_THREADS
#  define SWIG_PYTHON_THREAD_BEGIN_BLOCK
#  define SWIG_PYTHON_THREAD_END_BLOCK
#  define SWIG_PYTHON_THREAD_BEGIN_ALLOW
#  define SWIG_PYTHON_THREAD_END_ALLOW
#endif

/* -----------------------------------------------------------------------------
 * Python API portion that goes into the runtime
 * ----------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#if 0
} /* cc-mode */
#endif
#endif

/* -----------------------------------------------------------------------------
 * Constant declarations
 * ----------------------------------------------------------------------------- */

/* Constant Types */
#define SWIG_PY_POINTER 4
#define SWIG_PY_BINARY  5

/* Constant information structure */
typedef struct swig_const_info {
  int type;
  char *name;
  long lvalue;
  double dvalue;
  void   *pvalue;
  swig_type_info **ptype;
} swig_const_info;

#ifdef __cplusplus
#if 0
{ /* cc-mode */
#endif
}
#endif


/* -----------------------------------------------------------------------------
 * See the LICENSE file for information on copyright, usage and redistribution
 * of SWIG, and the README file for authors - http://www.swig.org/release.html.
 *
 * pyrun.swg
 *
 * This file contains the runtime support for Python modules
 * and includes code for managing global variables and pointer
 * type checking.
 *
 * ----------------------------------------------------------------------------- */

/* Common SWIG API */

/* for raw pointers */
#define SWIG_Python_ConvertPtr(obj, pptr, type, flags)  SWIG_Python_ConvertPtrAndOwn(obj, pptr, type, flags, 0)
#define SWIG_ConvertPtr(obj, pptr, type, flags)         SWIG_Python_ConvertPtr(obj, pptr, type, flags)
#define SWIG_ConvertPtrAndOwn(obj,pptr,type,flags,own)  SWIG_Python_ConvertPtrAndOwn(obj, pptr, type, flags, own)
#define SWIG_NewPointerObj(ptr, type, flags)            SWIG_Python_NewPointerObj(ptr, type, flags)
#define SWIG_CheckImplicit(ty)                          SWIG_Python_CheckImplicit(ty) 
#define SWIG_AcquirePtr(ptr, src)                       SWIG_Python_AcquirePtr(ptr, src)
#define swig_owntype                                    int

/* for raw packed data */
#define SWIG_ConvertPacked(obj, ptr, sz, ty)            SWIG_Python_ConvertPacked(obj, ptr, sz, ty)
#define SWIG_NewPackedObj(ptr, sz, type)                SWIG_Python_NewPackedObj(ptr, sz, type)

/* for class or struct pointers */
#define SWIG_ConvertInstance(obj, pptr, type, flags)    SWIG_ConvertPtr(obj, pptr, type, flags)
#define SWIG_NewInstanceObj(ptr, type, flags)           SWIG_NewPointerObj(ptr, type, flags)

/* for C or C++ function pointers */
#define SWIG_ConvertFunctionPtr(obj, pptr, type)        SWIG_Python_ConvertFunctionPtr(obj, pptr, type)
#define SWIG_NewFunctionPtrObj(ptr, type)               SWIG_Python_NewPointerObj(ptr, type, 0)

/* for C++ member pointers, ie, member methods */
#define SWIG_ConvertMember(obj, ptr, sz, ty)            SWIG_Python_ConvertPacked(obj, ptr, sz, ty)
#define SWIG_NewMemberObj(ptr, sz, type)                SWIG_Python_NewPackedObj(ptr, sz, type)


/* Runtime API */

#define SWIG_GetModule(clientdata)                      SWIG_Python_GetModule()
#define SWIG_SetModule(clientdata, pointer)             SWIG_Python_SetModule(pointer)
#define SWIG_NewClientData(obj)                         PySwigClientData_New(obj)

#define SWIG_SetErrorObj                                SWIG_Python_SetErrorObj                            
#define SWIG_SetErrorMsg                        	SWIG_Python_SetErrorMsg				   
#define SWIG_ErrorType(code)                    	SWIG_Python_ErrorType(code)                        
#define SWIG_Error(code, msg)            		SWIG_Python_SetErrorMsg(SWIG_ErrorType(code), msg) 
#define SWIG_fail                        		goto fail					   


/* Runtime API implementation */

/* Error manipulation */

SWIGINTERN void 
SWIG_Python_SetErrorObj(PyObject *errtype, PyObject *obj) {
  SWIG_PYTHON_THREAD_BEGIN_BLOCK; 
  PyErr_SetObject(errtype, obj);
  Py_DECREF(obj);
  SWIG_PYTHON_THREAD_END_BLOCK;
}

SWIGINTERN void 
SWIG_Python_SetErrorMsg(PyObject *errtype, const char *msg) {
  SWIG_PYTHON_THREAD_BEGIN_BLOCK;
  PyErr_SetString(errtype, (char *) msg);
  SWIG_PYTHON_THREAD_END_BLOCK;
}

#define SWIG_Python_Raise(obj, type, desc)  SWIG_Python_SetErrorObj(SWIG_Python_ExceptionType(desc), obj)

/* Set a constant value */

SWIGINTERN void
SWIG_Python_SetConstant(PyObject *d, const char *name, PyObject *obj) {   
  PyDict_SetItemString(d, (char*) name, obj);
  Py_DECREF(obj);                            
}

/* Append a value to the result obj */

SWIGINTERN PyObject*
SWIG_Python_AppendOutput(PyObject* result, PyObject* obj) {
#if !defined(SWIG_PYTHON_OUTPUT_TUPLE)
  if (!result) {
    result = obj;
  } else if (result == Py_None) {
    Py_DECREF(result);
    result = obj;
  } else {
    if (!PyList_Check(result)) {
      PyObject *o2 = result;
      result = PyList_New(1);
      PyList_SetItem(result, 0, o2);
    }
    PyList_Append(result,obj);
    Py_DECREF(obj);
  }
  return result;
#else
  PyObject*   o2;
  PyObject*   o3;
  if (!result) {
    result = obj;
  } else if (result == Py_None) {
    Py_DECREF(result);
    result = obj;
  } else {
    if (!PyTuple_Check(result)) {
      o2 = result;
      result = PyTuple_New(1);
      PyTuple_SET_ITEM(result, 0, o2);
    }
    o3 = PyTuple_New(1);
    PyTuple_SET_ITEM(o3, 0, obj);
    o2 = result;
    result = PySequence_Concat(o2, o3);
    Py_DECREF(o2);
    Py_DECREF(o3);
  }
  return result;
#endif
}

/* Unpack the argument tuple */

SWIGINTERN int
SWIG_Python_UnpackTuple(PyObject *args, const char *name, int min, int max, PyObject **objs)
{
  if (!args) {
    if (!min && !max) {
      return 1;
    } else {
      PyErr_Format(PyExc_TypeError, "%s expected %s%d arguments, got none", 
		   name, (min == max ? "" : "at least "), min);
      return 0;
    }
  }  
  if (!PyTuple_Check(args)) {
    PyErr_SetString(PyExc_SystemError, "UnpackTuple() argument list is not a tuple");
    return 0;
  } else {
    register int l = PyTuple_GET_SIZE(args);
    if (l < min) {
      PyErr_Format(PyExc_TypeError, "%s expected %s%d arguments, got %d", 
		   name, (min == max ? "" : "at least "), min, l);
      return 0;
    } else if (l > max) {
      PyErr_Format(PyExc_TypeError, "%s expected %s%d arguments, got %d", 
		   name, (min == max ? "" : "at most "), max, l);
      return 0;
    } else {
      register int i;
      for (i = 0; i < l; ++i) {
	objs[i] = PyTuple_GET_ITEM(args, i);
      }
      for (; l < max; ++l) {
	objs[l] = 0;
      }
      return i + 1;
    }    
  }
}

/* A functor is a function object with one single object argument */
#if PY_VERSION_HEX >= 0x02020000
#define SWIG_Python_CallFunctor(functor, obj)	        PyObject_CallFunctionObjArgs(functor, obj, NULL);
#else
#define SWIG_Python_CallFunctor(functor, obj)	        PyObject_CallFunction(functor, "O", obj);
#endif

/*
  Helper for static pointer initialization for both C and C++ code, for example
  static PyObject *SWIG_STATIC_POINTER(MyVar) = NewSomething(...);
*/
#ifdef __cplusplus
#define SWIG_STATIC_POINTER(var)  var
#else
#define SWIG_STATIC_POINTER(var)  var = 0; if (!var) var
#endif

/* -----------------------------------------------------------------------------
 * Pointer declarations
 * ----------------------------------------------------------------------------- */

/* Flags for new pointer objects */
#define SWIG_POINTER_NOSHADOW       (SWIG_POINTER_OWN      << 1)
#define SWIG_POINTER_NEW            (SWIG_POINTER_NOSHADOW | SWIG_POINTER_OWN)

#define SWIG_POINTER_IMPLICIT_CONV  (SWIG_POINTER_DISOWN   << 1)

#ifdef __cplusplus
extern "C" {
#if 0
} /* cc-mode */
#endif
#endif

/*  How to access Py_None */
#if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#  ifndef SWIG_PYTHON_NO_BUILD_NONE
#    ifndef SWIG_PYTHON_BUILD_NONE
#      define SWIG_PYTHON_BUILD_NONE
#    endif
#  endif
#endif

#ifdef SWIG_PYTHON_BUILD_NONE
#  ifdef Py_None
#   undef Py_None
#   define Py_None SWIG_Py_None()
#  endif
SWIGRUNTIMEINLINE PyObject * 
_SWIG_Py_None(void)
{
  PyObject *none = Py_BuildValue((char*)"");
  Py_DECREF(none);
  return none;
}
SWIGRUNTIME PyObject * 
SWIG_Py_None(void)
{
  static PyObject *SWIG_STATIC_POINTER(none) = _SWIG_Py_None();
  return none;
}
#endif

/* The python void return value */

SWIGRUNTIMEINLINE PyObject * 
SWIG_Py_Void(void)
{
  PyObject *none = Py_None;
  Py_INCREF(none);
  return none;
}

/* PySwigClientData */

typedef struct {
  PyObject *klass;
  PyObject *newraw;
  PyObject *newargs;
  PyObject *destroy;
  int delargs;
  int implicitconv;
} PySwigClientData;

SWIGRUNTIMEINLINE int 
SWIG_Python_CheckImplicit(swig_type_info *ty)
{
  PySwigClientData *data = (PySwigClientData *)ty->clientdata;
  return data ? data->implicitconv : 0;
}

SWIGRUNTIMEINLINE PyObject *
SWIG_Python_ExceptionType(swig_type_info *desc) {
  PySwigClientData *data = desc ? (PySwigClientData *) desc->clientdata : 0;
  PyObject *klass = data ? data->klass : 0;
  return (klass ? klass : PyExc_RuntimeError);
}


SWIGRUNTIME PySwigClientData * 
PySwigClientData_New(PyObject* obj)
{
  if (!obj) {
    return 0;
  } else {
    PySwigClientData *data = (PySwigClientData *)malloc(sizeof(PySwigClientData));
    /* the klass element */
    data->klass = obj;
    Py_INCREF(data->klass);
    /* the newraw method and newargs arguments used to create a new raw instance */
    if (PyClass_Check(obj)) {
      data->newraw = 0;
      data->newargs = obj;
      Py_INCREF(obj);
    } else {
#if (PY_VERSION_HEX < 0x02020000)
      data->newraw = 0;
#else
      data->newraw = PyObject_GetAttrString(data->klass, (char *)"__new__");
#endif
      if (data->newraw) {
	Py_INCREF(data->newraw);
	data->newargs = PyTuple_New(1);
	PyTuple_SetItem(data->newargs, 0, obj);
      } else {
	data->newargs = obj;
      }
      Py_INCREF(data->newargs);
    }
    /* the destroy method, aka as the C++ delete method */
    data->destroy = PyObject_GetAttrString(data->klass, (char *)"__swig_destroy__");
    if (PyErr_Occurred()) {
      PyErr_Clear();
      data->destroy = 0;
    }
    if (data->destroy) {
      int flags;
      Py_INCREF(data->destroy);
      flags = PyCFunction_GET_FLAGS(data->destroy);
#ifdef METH_O
      data->delargs = !(flags & (METH_O));
#else
      data->delargs = 0;
#endif
    } else {
      data->delargs = 0;
    }
    data->implicitconv = 0;
    return data;
  }
}

SWIGRUNTIME void 
PySwigClientData_Del(PySwigClientData* data)
{
  Py_XDECREF(data->newraw);
  Py_XDECREF(data->newargs);
  Py_XDECREF(data->destroy);
}

/* =============== PySwigObject =====================*/

typedef struct {
  PyObject_HEAD
  void *ptr;
  swig_type_info *ty;
  int own;
  PyObject *next;
} PySwigObject;

SWIGRUNTIME PyObject *
PySwigObject_long(PySwigObject *v)
{
  return PyLong_FromVoidPtr(v->ptr);
}

SWIGRUNTIME PyObject *
PySwigObject_format(const char* fmt, PySwigObject *v)
{
  PyObject *res = NULL;
  PyObject *args = PyTuple_New(1);
  if (args) {
    if (PyTuple_SetItem(args, 0, PySwigObject_long(v)) == 0) {
      PyObject *ofmt = PyString_FromString(fmt);
      if (ofmt) {
	res = PyString_Format(ofmt,args);
	Py_DECREF(ofmt);
      }
      Py_DECREF(args);
    }
  }
  return res;
}

SWIGRUNTIME PyObject *
PySwigObject_oct(PySwigObject *v)
{
  return PySwigObject_format("%o",v);
}

SWIGRUNTIME PyObject *
PySwigObject_hex(PySwigObject *v)
{
  return PySwigObject_format("%x",v);
}

SWIGRUNTIME PyObject *
#ifdef METH_NOARGS
PySwigObject_repr(PySwigObject *v)
#else
PySwigObject_repr(PySwigObject *v, PyObject *args)
#endif
{
  const char *name = SWIG_TypePrettyName(v->ty);
  PyObject *hex = PySwigObject_hex(v);    
  PyObject *repr = PyString_FromFormat("<Swig Object of type '%s' at 0x%s>", name, PyString_AsString(hex));
  Py_DECREF(hex);
  if (v->next) {
#ifdef METH_NOARGS
    PyObject *nrep = PySwigObject_repr((PySwigObject *)v->next);
#else
    PyObject *nrep = PySwigObject_repr((PySwigObject *)v->next, args);
#endif
    PyString_ConcatAndDel(&repr,nrep);
  }
  return repr;  
}

SWIGRUNTIME int
PySwigObject_print(PySwigObject *v, FILE *fp, int SWIGUNUSEDPARM(flags))
{
#ifdef METH_NOARGS
  PyObject *repr = PySwigObject_repr(v);
#else
  PyObject *repr = PySwigObject_repr(v, NULL);
#endif
  if (repr) {
    fputs(PyString_AsString(repr), fp);
    Py_DECREF(repr);
    return 0; 
  } else {
    return 1; 
  }
}

SWIGRUNTIME PyObject *
PySwigObject_str(PySwigObject *v)
{
  char result[SWIG_BUFFER_SIZE];
  return SWIG_PackVoidPtr(result, v->ptr, v->ty->name, sizeof(result)) ?
    PyString_FromString(result) : 0;
}

SWIGRUNTIME int
PySwigObject_compare(PySwigObject *v, PySwigObject *w)
{
  void *i = v->ptr;
  void *j = w->ptr;
  return (i < j) ? -1 : ((i > j) ? 1 : 0);
}

SWIGRUNTIME PyTypeObject* _PySwigObject_type(void);

SWIGRUNTIME PyTypeObject*
PySwigObject_type(void) {
  static PyTypeObject *SWIG_STATIC_POINTER(type) = _PySwigObject_type();
  return type;
}

SWIGRUNTIMEINLINE int
PySwigObject_Check(PyObject *op) {
  return ((op)->ob_type == PySwigObject_type())
    || (strcmp((op)->ob_type->tp_name,"PySwigObject") == 0);
}

SWIGRUNTIME PyObject *
PySwigObject_New(void *ptr, swig_type_info *ty, int own);

SWIGRUNTIME void
PySwigObject_dealloc(PyObject *v)
{
  PySwigObject *sobj = (PySwigObject *) v;
  PyObject *next = sobj->next;
  if (sobj->own) {
    swig_type_info *ty = sobj->ty;
    PySwigClientData *data = ty ? (PySwigClientData *) ty->clientdata : 0;
    PyObject *destroy = data ? data->destroy : 0;
    if (destroy) {
      /* destroy is always a VARARGS method */
      PyObject *res;
      if (data->delargs) {
	/* we need to create a temporal object to carry the destroy operation */
	PyObject *tmp = PySwigObject_New(sobj->ptr, ty, 0);
	res = SWIG_Python_CallFunctor(destroy, tmp);
	Py_DECREF(tmp);
      } else {
	PyCFunction meth = PyCFunction_GET_FUNCTION(destroy);
	PyObject *mself = PyCFunction_GET_SELF(destroy);
	res = ((*meth)(mself, v));
      }
      Py_XDECREF(res);
    } else {
      const char *name = SWIG_TypePrettyName(ty);
#if !defined(SWIG_PYTHON_SILENT_MEMLEAK)
      printf("swig/python detected a memory leak of type '%s', no destructor found.\n", name);
#endif
    }
  } 
  Py_XDECREF(next);
  PyObject_DEL(v);
}

SWIGRUNTIME PyObject* 
PySwigObject_append(PyObject* v, PyObject* next)
{
  PySwigObject *sobj = (PySwigObject *) v;
#ifndef METH_O
  PyObject *tmp = 0;
  if (!PyArg_ParseTuple(next,(char *)"O:append", &tmp)) return NULL;
  next = tmp;
#endif
  if (!PySwigObject_Check(next)) {
    return NULL;
  }
  sobj->next = next;
  Py_INCREF(next);
  return SWIG_Py_Void();
}

SWIGRUNTIME PyObject* 
#ifdef METH_NOARGS
PySwigObject_next(PyObject* v)
#else
PySwigObject_next(PyObject* v, PyObject *SWIGUNUSEDPARM(args))
#endif
{
  PySwigObject *sobj = (PySwigObject *) v;
  if (sobj->next) {    
    Py_INCREF(sobj->next);
    return sobj->next;
  } else {
    return SWIG_Py_Void();
  }
}

SWIGINTERN PyObject*
#ifdef METH_NOARGS
PySwigObject_disown(PyObject *v)
#else
PySwigObject_disown(PyObject* v, PyObject *SWIGUNUSEDPARM(args))
#endif
{
  PySwigObject *sobj = (PySwigObject *)v;
  sobj->own = 0;
  return SWIG_Py_Void();
}

SWIGINTERN PyObject*
#ifdef METH_NOARGS
PySwigObject_acquire(PyObject *v)
#else
PySwigObject_acquire(PyObject* v, PyObject *SWIGUNUSEDPARM(args))
#endif
{
  PySwigObject *sobj = (PySwigObject *)v;
  sobj->own = SWIG_POINTER_OWN;
  return SWIG_Py_Void();
}

SWIGINTERN PyObject*
PySwigObject_own(PyObject *v, PyObject *args)
{
  PyObject *val = 0;
#if (PY_VERSION_HEX < 0x02020000)
  if (!PyArg_ParseTuple(args,(char *)"|O:own",&val))
#else
  if (!PyArg_UnpackTuple(args, (char *)"own", 0, 1, &val)) 
#endif
    {
      return NULL;
    } 
  else
    {
      PySwigObject *sobj = (PySwigObject *)v;
      PyObject *obj = PyBool_FromLong(sobj->own);
      if (val) {
#ifdef METH_NOARGS
	if (PyObject_IsTrue(val)) {
	  PySwigObject_acquire(v);
	} else {
	  PySwigObject_disown(v);
	}
#else
	if (PyObject_IsTrue(val)) {
	  PySwigObject_acquire(v,args);
	} else {
	  PySwigObject_disown(v,args);
	}
#endif
      } 
      return obj;
    }
}

#ifdef METH_O
static PyMethodDef
swigobject_methods[] = {
  {(char *)"disown",  (PyCFunction)PySwigObject_disown,  METH_NOARGS,  (char *)"releases ownership of the pointer"},
  {(char *)"acquire", (PyCFunction)PySwigObject_acquire, METH_NOARGS,  (char *)"aquires ownership of the pointer"},
  {(char *)"own",     (PyCFunction)PySwigObject_own,     METH_VARARGS, (char *)"returns/sets ownership of the pointer"},
  {(char *)"append",  (PyCFunction)PySwigObject_append,  METH_O,       (char *)"appends another 'this' object"},
  {(char *)"next",    (PyCFunction)PySwigObject_next,    METH_NOARGS,  (char *)"returns the next 'this' object"},
  {(char *)"__repr__",(PyCFunction)PySwigObject_repr,    METH_NOARGS,  (char *)"returns object representation"},
  {0, 0, 0, 0}  
};
#else
static PyMethodDef
swigobject_methods[] = {
  {(char *)"disown",  (PyCFunction)PySwigObject_disown,  METH_VARARGS,  (char *)"releases ownership of the pointer"},
  {(char *)"acquire", (PyCFunction)PySwigObject_acquire, METH_VARARGS,  (char *)"aquires ownership of the pointer"},
  {(char *)"own",     (PyCFunction)PySwigObject_own,     METH_VARARGS,  (char *)"returns/sets ownership of the pointer"},
  {(char *)"append",  (PyCFunction)PySwigObject_append,  METH_VARARGS,  (char *)"appends another 'this' object"},
  {(char *)"next",    (PyCFunction)PySwigObject_next,    METH_VARARGS,  (char *)"returns the next 'this' object"},
  {(char *)"__repr__",(PyCFunction)PySwigObject_repr,   METH_VARARGS,  (char *)"returns object representation"},
  {0, 0, 0, 0}  
};
#endif

#if PY_VERSION_HEX < 0x02020000
SWIGINTERN PyObject *
PySwigObject_getattr(PySwigObject *sobj,char *name)
{
  return Py_FindMethod(swigobject_methods, (PyObject *)sobj, name);
}
#endif

SWIGRUNTIME PyTypeObject*
_PySwigObject_type(void) {
  static char swigobject_doc[] = "Swig object carries a C/C++ instance pointer";
  
  static PyNumberMethods PySwigObject_as_number = {
    (binaryfunc)0, /*nb_add*/
    (binaryfunc)0, /*nb_subtract*/
    (binaryfunc)0, /*nb_multiply*/
    (binaryfunc)0, /*nb_divide*/
    (binaryfunc)0, /*nb_remainder*/
    (binaryfunc)0, /*nb_divmod*/
    (ternaryfunc)0,/*nb_power*/
    (unaryfunc)0,  /*nb_negative*/
    (unaryfunc)0,  /*nb_positive*/
    (unaryfunc)0,  /*nb_absolute*/
    (inquiry)0,    /*nb_nonzero*/
    0,		   /*nb_invert*/
    0,		   /*nb_lshift*/
    0,		   /*nb_rshift*/
    0,		   /*nb_and*/
    0,		   /*nb_xor*/
    0,		   /*nb_or*/
    (coercion)0,   /*nb_coerce*/
    (unaryfunc)PySwigObject_long, /*nb_int*/
    (unaryfunc)PySwigObject_long, /*nb_long*/
    (unaryfunc)0,                 /*nb_float*/
    (unaryfunc)PySwigObject_oct,  /*nb_oct*/
    (unaryfunc)PySwigObject_hex,  /*nb_hex*/
#if PY_VERSION_HEX >= 0x02020000
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 /* nb_inplace_add -> nb_inplace_true_divide */ 
#elif PY_VERSION_HEX >= 0x02000000
    0,0,0,0,0,0,0,0,0,0,0 /* nb_inplace_add -> nb_inplace_or */
#endif
  };

  static PyTypeObject pyswigobject_type;  
  static int type_init = 0;
  if (!type_init) {
    const PyTypeObject tmp
      = {
	PyObject_HEAD_INIT(NULL)
	0,				    /* ob_size */
	(char *)"PySwigObject",		    /* tp_name */
	sizeof(PySwigObject),		    /* tp_basicsize */
	0,			            /* tp_itemsize */
	(destructor)PySwigObject_dealloc,   /* tp_dealloc */
	(printfunc)PySwigObject_print,	    /* tp_print */
#if PY_VERSION_HEX < 0x02020000
	(getattrfunc)PySwigObject_getattr,  /* tp_getattr */ 
#else
	(getattrfunc)0,			    /* tp_getattr */ 
#endif
	(setattrfunc)0,			    /* tp_setattr */ 
	(cmpfunc)PySwigObject_compare,	    /* tp_compare */ 
	(reprfunc)PySwigObject_repr,	    /* tp_repr */    
	&PySwigObject_as_number,	    /* tp_as_number */
	0,				    /* tp_as_sequence */
	0,				    /* tp_as_mapping */
	(hashfunc)0,			    /* tp_hash */
	(ternaryfunc)0,			    /* tp_call */
	(reprfunc)PySwigObject_str,	    /* tp_str */
	PyObject_GenericGetAttr,            /* tp_getattro */
	0,				    /* tp_setattro */
	0,		                    /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,	            /* tp_flags */
	swigobject_doc, 	            /* tp_doc */        
	0,                                  /* tp_traverse */
	0,                                  /* tp_clear */
	0,                                  /* tp_richcompare */
	0,                                  /* tp_weaklistoffset */
#if PY_VERSION_HEX >= 0x02020000
	0,                                  /* tp_iter */
	0,                                  /* tp_iternext */
	swigobject_methods,		    /* tp_methods */ 
	0,			            /* tp_members */
	0,				    /* tp_getset */	    	
	0,			            /* tp_base */	        
	0,				    /* tp_dict */	    	
	0,				    /* tp_descr_get */  	
	0,				    /* tp_descr_set */  	
	0,				    /* tp_dictoffset */ 	
	0,				    /* tp_init */	    	
	0,				    /* tp_alloc */	    	
	0,			            /* tp_new */	    	
	0,	                            /* tp_free */	   
        0,                                  /* tp_is_gc */  
	0,				    /* tp_bases */   
	0,				    /* tp_mro */
	0,				    /* tp_cache */   
 	0,				    /* tp_subclasses */
	0,				    /* tp_weaklist */
#endif
#if PY_VERSION_HEX >= 0x02030000
	0,                                  /* tp_del */
#endif
#ifdef COUNT_ALLOCS
	0,0,0,0                             /* tp_alloc -> tp_next */
#endif
      };
    pyswigobject_type = tmp;
    pyswigobject_type.ob_type = &PyType_Type;
    type_init = 1;
  }
  return &pyswigobject_type;
}

SWIGRUNTIME PyObject *
PySwigObject_New(void *ptr, swig_type_info *ty, int own)
{
  PySwigObject *sobj = PyObject_NEW(PySwigObject, PySwigObject_type());
  if (sobj) {
    sobj->ptr  = ptr;
    sobj->ty   = ty;
    sobj->own  = own;
    sobj->next = 0;
  }
  return (PyObject *)sobj;
}

/* -----------------------------------------------------------------------------
 * Implements a simple Swig Packed type, and use it instead of string
 * ----------------------------------------------------------------------------- */

typedef struct {
  PyObject_HEAD
  void *pack;
  swig_type_info *ty;
  size_t size;
} PySwigPacked;

SWIGRUNTIME int
PySwigPacked_print(PySwigPacked *v, FILE *fp, int SWIGUNUSEDPARM(flags))
{
  char result[SWIG_BUFFER_SIZE];
  fputs("<Swig Packed ", fp); 
  if (SWIG_PackDataName(result, v->pack, v->size, 0, sizeof(result))) {
    fputs("at ", fp); 
    fputs(result, fp); 
  }
  fputs(v->ty->name,fp); 
  fputs(">", fp);
  return 0; 
}
  
SWIGRUNTIME PyObject *
PySwigPacked_repr(PySwigPacked *v)
{
  char result[SWIG_BUFFER_SIZE];
  if (SWIG_PackDataName(result, v->pack, v->size, 0, sizeof(result))) {
    return PyString_FromFormat("<Swig Packed at %s%s>", result, v->ty->name);
  } else {
    return PyString_FromFormat("<Swig Packed %s>", v->ty->name);
  }  
}

SWIGRUNTIME PyObject *
PySwigPacked_str(PySwigPacked *v)
{
  char result[SWIG_BUFFER_SIZE];
  if (SWIG_PackDataName(result, v->pack, v->size, 0, sizeof(result))){
    return PyString_FromFormat("%s%s", result, v->ty->name);
  } else {
    return PyString_FromString(v->ty->name);
  }  
}

SWIGRUNTIME int
PySwigPacked_compare(PySwigPacked *v, PySwigPacked *w)
{
  size_t i = v->size;
  size_t j = w->size;
  int s = (i < j) ? -1 : ((i > j) ? 1 : 0);
  return s ? s : strncmp((char *)v->pack, (char *)w->pack, 2*v->size);
}

SWIGRUNTIME PyTypeObject* _PySwigPacked_type(void);

SWIGRUNTIME PyTypeObject*
PySwigPacked_type(void) {
  static PyTypeObject *SWIG_STATIC_POINTER(type) = _PySwigPacked_type();
  return type;
}

SWIGRUNTIMEINLINE int
PySwigPacked_Check(PyObject *op) {
  return ((op)->ob_type == _PySwigPacked_type()) 
    || (strcmp((op)->ob_type->tp_name,"PySwigPacked") == 0);
}

SWIGRUNTIME void
PySwigPacked_dealloc(PyObject *v)
{
  if (PySwigPacked_Check(v)) {
    PySwigPacked *sobj = (PySwigPacked *) v;
    free(sobj->pack);
  }
  PyObject_DEL(v);
}

SWIGRUNTIME PyTypeObject*
_PySwigPacked_type(void) {
  static char swigpacked_doc[] = "Swig object carries a C/C++ instance pointer";
  static PyTypeObject pyswigpacked_type;
  static int type_init = 0;  
  if (!type_init) {
    const PyTypeObject tmp
      = {
	PyObject_HEAD_INIT(NULL)
	0,				    /* ob_size */	
	(char *)"PySwigPacked",		    /* tp_name */	
	sizeof(PySwigPacked),		    /* tp_basicsize */	
	0,				    /* tp_itemsize */	
	(destructor)PySwigPacked_dealloc,   /* tp_dealloc */	
	(printfunc)PySwigPacked_print,	    /* tp_print */   	
	(getattrfunc)0,			    /* tp_getattr */ 	
	(setattrfunc)0,			    /* tp_setattr */ 	
	(cmpfunc)PySwigPacked_compare,	    /* tp_compare */ 	
	(reprfunc)PySwigPacked_repr,	    /* tp_repr */    	
	0,	                            /* tp_as_number */	
	0,				    /* tp_as_sequence */
	0,				    /* tp_as_mapping */	
	(hashfunc)0,			    /* tp_hash */	
	(ternaryfunc)0,			    /* tp_call */	
	(reprfunc)PySwigPacked_str,	    /* tp_str */	
	PyObject_GenericGetAttr,            /* tp_getattro */
	0,				    /* tp_setattro */
	0,		                    /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,	            /* tp_flags */
	swigpacked_doc, 	            /* tp_doc */
	0,                                  /* tp_traverse */
	0,                                  /* tp_clear */
	0,                                  /* tp_richcompare */
	0,                                  /* tp_weaklistoffset */
#if PY_VERSION_HEX >= 0x02020000
	0,                                  /* tp_iter */
	0,                                  /* tp_iternext */
	0,		                    /* tp_methods */ 
	0,			            /* tp_members */
	0,				    /* tp_getset */	    	
	0,			            /* tp_base */	        
	0,				    /* tp_dict */	    	
	0,				    /* tp_descr_get */  	
	0,				    /* tp_descr_set */  	
	0,				    /* tp_dictoffset */ 	
	0,				    /* tp_init */	    	
	0,				    /* tp_alloc */	    	
	0,			            /* tp_new */	    	
	0, 	                            /* tp_free */	   
        0,                                  /* tp_is_gc */  
	0,				    /* tp_bases */   
	0,				    /* tp_mro */
	0,				    /* tp_cache */   
 	0,				    /* tp_subclasses */
	0,				    /* tp_weaklist */
#endif
#if PY_VERSION_HEX >= 0x02030000
	0,                                  /* tp_del */
#endif
#ifdef COUNT_ALLOCS
	0,0,0,0                             /* tp_alloc -> tp_next */
#endif
      };
    pyswigpacked_type = tmp;
    pyswigpacked_type.ob_type = &PyType_Type;
    type_init = 1;
  }
  return &pyswigpacked_type;
}

SWIGRUNTIME PyObject *
PySwigPacked_New(void *ptr, size_t size, swig_type_info *ty)
{
  PySwigPacked *sobj = PyObject_NEW(PySwigPacked, PySwigPacked_type());
  if (sobj) {
    void *pack = malloc(size);
    if (pack) {
      memcpy(pack, ptr, size);
      sobj->pack = pack;
      sobj->ty   = ty;
      sobj->size = size;
    } else {
      PyObject_DEL((PyObject *) sobj);
      sobj = 0;
    }
  }
  return (PyObject *) sobj;
}

SWIGRUNTIME swig_type_info *
PySwigPacked_UnpackData(PyObject *obj, void *ptr, size_t size)
{
  if (PySwigPacked_Check(obj)) {
    PySwigPacked *sobj = (PySwigPacked *)obj;
    if (sobj->size != size) return 0;
    memcpy(ptr, sobj->pack, size);
    return sobj->ty;
  } else {
    return 0;
  }
}

/* -----------------------------------------------------------------------------
 * pointers/data manipulation
 * ----------------------------------------------------------------------------- */

SWIGRUNTIMEINLINE PyObject *
_SWIG_This(void)
{
  return PyString_FromString("this");
}

SWIGRUNTIME PyObject *
SWIG_This(void)
{
  static PyObject *SWIG_STATIC_POINTER(swig_this) = _SWIG_This();
  return swig_this;
}

/* #define SWIG_PYTHON_SLOW_GETSET_THIS */

SWIGRUNTIME PySwigObject *
SWIG_Python_GetSwigThis(PyObject *pyobj) 
{
  if (PySwigObject_Check(pyobj)) {
    return (PySwigObject *) pyobj;
  } else {
    PyObject *obj = 0;
#if (!defined(SWIG_PYTHON_SLOW_GETSET_THIS) && (PY_VERSION_HEX >= 0x02030000))
    if (PyInstance_Check(pyobj)) {
      obj = _PyInstance_Lookup(pyobj, SWIG_This());      
    } else {
      PyObject **dictptr = _PyObject_GetDictPtr(pyobj);
      if (dictptr != NULL) {
	PyObject *dict = *dictptr;
	obj = dict ? PyDict_GetItem(dict, SWIG_This()) : 0;
      } else {
#ifdef PyWeakref_CheckProxy
	if (PyWeakref_CheckProxy(pyobj)) {
	  PyObject *wobj = PyWeakref_GET_OBJECT(pyobj);
	  return wobj ? SWIG_Python_GetSwigThis(wobj) : 0;
	}
#endif
	obj = PyObject_GetAttr(pyobj,SWIG_This());
	if (obj) {
	  Py_DECREF(obj);
	} else {
	  if (PyErr_Occurred()) PyErr_Clear();
	  return 0;
	}
      }
    }
#else
    obj = PyObject_GetAttr(pyobj,SWIG_This());
    if (obj) {
      Py_DECREF(obj);
    } else {
      if (PyErr_Occurred()) PyErr_Clear();
      return 0;
    }
#endif
    if (obj && !PySwigObject_Check(obj)) {
      /* a PyObject is called 'this', try to get the 'real this'
	 PySwigObject from it */ 
      return SWIG_Python_GetSwigThis(obj);
    }
    return (PySwigObject *)obj;
  }
}

/* Acquire a pointer value */

SWIGRUNTIME int
SWIG_Python_AcquirePtr(PyObject *obj, int own) {
  if (own) {
    PySwigObject *sobj = SWIG_Python_GetSwigThis(obj);
    if (sobj) {
      int oldown = sobj->own;
      sobj->own = own;
      return oldown;
    }
  }
  return 0;
}

/* Convert a pointer value */

SWIGRUNTIME int
SWIG_Python_ConvertPtrAndOwn(PyObject *obj, void **ptr, swig_type_info *ty, int flags, int *own) {
  if (!obj) return SWIG_ERROR;
  if (obj == Py_None) {
    if (ptr) *ptr = 0;
    return SWIG_OK;
  } else {
    PySwigObject *sobj = SWIG_Python_GetSwigThis(obj);
    while (sobj) {
      void *vptr = sobj->ptr;
      if (ty) {
	swig_type_info *to = sobj->ty;
	if (to == ty) {
	  /* no type cast needed */
	  if (ptr) *ptr = vptr;
	  break;
	} else {
	  swig_cast_info *tc = SWIG_TypeCheck(to->name,ty);
	  if (!tc) {
	    sobj = (PySwigObject *)sobj->next;
	  } else {
	    if (ptr) *ptr = SWIG_TypeCast(tc,vptr);
	    break;
	  }
	}
      } else {
	if (ptr) *ptr = vptr;
	break;
      }
    }
    if (sobj) {
      if (own) *own = sobj->own;
      if (flags & SWIG_POINTER_DISOWN) {
	sobj->own = 0;
      }
      return SWIG_OK;
    } else {
      int res = SWIG_ERROR;
      if (flags & SWIG_POINTER_IMPLICIT_CONV) {
	PySwigClientData *data = ty ? (PySwigClientData *) ty->clientdata : 0;
	if (data && !data->implicitconv) {
	  PyObject *klass = data->klass;
	  if (klass) {
	    PyObject *impconv;
	    data->implicitconv = 1; /* avoid recursion and call 'explicit' constructors*/
	    impconv = SWIG_Python_CallFunctor(klass, obj);
	    data->implicitconv = 0;
	    if (PyErr_Occurred()) {
	      PyErr_Clear();
	      impconv = 0;
	    }
	    if (impconv) {
	      PySwigObject *iobj = SWIG_Python_GetSwigThis(impconv);
	      if (iobj) {
		void *vptr;
		res = SWIG_Python_ConvertPtrAndOwn((PyObject*)iobj, &vptr, ty, 0, 0);
		if (SWIG_IsOK(res)) {
		  if (ptr) {
		    *ptr = vptr;
		    /* transfer the ownership to 'ptr' */
		    iobj->own = 0;
		    res = SWIG_AddCast(res);
		    res = SWIG_AddNewMask(res);
		  } else {
		    res = SWIG_AddCast(res);		    
		  }
		}
	      }
	      Py_DECREF(impconv);
	    }
	  }
	}
      }
      return res;
    }
  }
}

/* Convert a function ptr value */

SWIGRUNTIME int
SWIG_Python_ConvertFunctionPtr(PyObject *obj, void **ptr, swig_type_info *ty) {
  if (!PyCFunction_Check(obj)) {
    return SWIG_ConvertPtr(obj, ptr, ty, 0);
  } else {
    void *vptr = 0;
    
    /* here we get the method pointer for callbacks */
    const char *doc = (((PyCFunctionObject *)obj) -> m_ml -> ml_doc);
    const char *desc = doc ? strstr(doc, "swig_ptr: ") : 0;
    if (desc) {
      desc = ty ? SWIG_UnpackVoidPtr(desc + 10, &vptr, ty->name) : 0;
      if (!desc) return SWIG_ERROR;
    }
    if (ty) {
      swig_cast_info *tc = SWIG_TypeCheck(desc,ty);
      if (!tc) return SWIG_ERROR;
      *ptr = SWIG_TypeCast(tc,vptr);
    } else {
      *ptr = vptr;
    }
    return SWIG_OK;
  }
}

/* Convert a packed value value */

SWIGRUNTIME int
SWIG_Python_ConvertPacked(PyObject *obj, void *ptr, size_t sz, swig_type_info *ty) {
  swig_type_info *to = PySwigPacked_UnpackData(obj, ptr, sz);
  if (!to) return SWIG_ERROR;
  if (ty) {
    if (to != ty) {
      /* check type cast? */
      swig_cast_info *tc = SWIG_TypeCheck(to->name,ty);
      if (!tc) return SWIG_ERROR;
    }
  }
  return SWIG_OK;
}  

/* -----------------------------------------------------------------------------
 * Create a new pointer object
 * ----------------------------------------------------------------------------- */

/*
  Create a new instance object, whitout calling __init__, and set the
  'this' attribute.
*/

SWIGRUNTIME PyObject* 
SWIG_Python_NewShadowInstance(PySwigClientData *data, PyObject *swig_this)
{
#if (PY_VERSION_HEX >= 0x02020000)
  PyObject *inst = 0;
  PyObject *newraw = data->newraw;
  if (newraw) {
    inst = PyObject_Call(newraw, data->newargs, NULL);
    if (inst) {
#if !defined(SWIG_PYTHON_SLOW_GETSET_THIS)
      PyObject **dictptr = _PyObject_GetDictPtr(inst);
      if (dictptr != NULL) {
	PyObject *dict = *dictptr;
	if (dict == NULL) {
	  dict = PyDict_New();
	  *dictptr = dict;
	  PyDict_SetItem(dict, SWIG_This(), swig_this);
	}
      }
#else
      PyObject *key = SWIG_This();
      PyObject_SetAttr(inst, key, swig_this);
#endif
    }
  } else {
    PyObject *dict = PyDict_New();
    PyDict_SetItem(dict, SWIG_This(), swig_this);
    inst = PyInstance_NewRaw(data->newargs, dict);
    Py_DECREF(dict);
  }
  return inst;
#else
#if (PY_VERSION_HEX >= 0x02010000)
  PyObject *inst;
  PyObject *dict = PyDict_New();
  PyDict_SetItem(dict, SWIG_This(), swig_this);
  inst = PyInstance_NewRaw(data->newargs, dict);
  Py_DECREF(dict);
  return (PyObject *) inst;
#else
  PyInstanceObject *inst = PyObject_NEW(PyInstanceObject, &PyInstance_Type);
  if (inst == NULL) {
    return NULL;
  }
  inst->in_class = (PyClassObject *)data->newargs;
  Py_INCREF(inst->in_class);
  inst->in_dict = PyDict_New();
  if (inst->in_dict == NULL) {
    Py_DECREF(inst);
    return NULL;
  }
#ifdef Py_TPFLAGS_HAVE_WEAKREFS
  inst->in_weakreflist = NULL;
#endif
#ifdef Py_TPFLAGS_GC
  PyObject_GC_Init(inst);
#endif
  PyDict_SetItem(inst->in_dict, SWIG_This(), swig_this);
  return (PyObject *) inst;
#endif
#endif
}

SWIGRUNTIME void
SWIG_Python_SetSwigThis(PyObject *inst, PyObject *swig_this)
{
 PyObject *dict;
#if (PY_VERSION_HEX >= 0x02020000) && !defined(SWIG_PYTHON_SLOW_GETSET_THIS)
 PyObject **dictptr = _PyObject_GetDictPtr(inst);
 if (dictptr != NULL) {
   dict = *dictptr;
   if (dict == NULL) {
     dict = PyDict_New();
     *dictptr = dict;
   }
   PyDict_SetItem(dict, SWIG_This(), swig_this);
   return;
 }
#endif
 dict = PyObject_GetAttrString(inst, (char*)"__dict__");
 PyDict_SetItem(dict, SWIG_This(), swig_this);
 Py_DECREF(dict);
} 


SWIGINTERN PyObject *
SWIG_Python_InitShadowInstance(PyObject *args) {
  PyObject *obj[2];
  if (!SWIG_Python_UnpackTuple(args,(char*)"swiginit", 2, 2, obj)) {
    return NULL;
  } else {
    PySwigObject *sthis = SWIG_Python_GetSwigThis(obj[0]);
    if (sthis) {
      PySwigObject_append((PyObject*) sthis, obj[1]);
    } else {
      SWIG_Python_SetSwigThis(obj[0], obj[1]);
    }
    return SWIG_Py_Void();
  }
}

/* Create a new pointer object */

SWIGRUNTIME PyObject *
SWIG_Python_NewPointerObj(void *ptr, swig_type_info *type, int flags) {
  if (!ptr) {
    return SWIG_Py_Void();
  } else {
    int own = (flags & SWIG_POINTER_OWN) ? SWIG_POINTER_OWN : 0;
    PyObject *robj = PySwigObject_New(ptr, type, own);
    PySwigClientData *clientdata = type ? (PySwigClientData *)(type->clientdata) : 0;
    if (clientdata && !(flags & SWIG_POINTER_NOSHADOW)) {
      PyObject *inst = SWIG_Python_NewShadowInstance(clientdata, robj);
      if (inst) {
	Py_DECREF(robj);
	robj = inst;
      }
    }
    return robj;
  }
}

/* Create a new packed object */

SWIGRUNTIMEINLINE PyObject *
SWIG_Python_NewPackedObj(void *ptr, size_t sz, swig_type_info *type) {
  return ptr ? PySwigPacked_New((void *) ptr, sz, type) : SWIG_Py_Void();
}

/* -----------------------------------------------------------------------------*
 *  Get type list 
 * -----------------------------------------------------------------------------*/

#ifdef SWIG_LINK_RUNTIME
void *SWIG_ReturnGlobalTypeList(void *);
#endif

SWIGRUNTIME swig_module_info *
SWIG_Python_GetModule(void) {
  static void *type_pointer = (void *)0;
  /* first check if module already created */
  if (!type_pointer) {
#ifdef SWIG_LINK_RUNTIME
    type_pointer = SWIG_ReturnGlobalTypeList((void *)0);
#else
    type_pointer = PyCObject_Import((char*)"swig_runtime_data" SWIG_RUNTIME_VERSION,
				    (char*)"type_pointer" SWIG_TYPE_TABLE_NAME);
    if (PyErr_Occurred()) {
      PyErr_Clear();
      type_pointer = (void *)0;
    }
#endif
  }
  return (swig_module_info *) type_pointer;
}

#if PY_MAJOR_VERSION < 2
/* PyModule_AddObject function was introduced in Python 2.0.  The following function
   is copied out of Python/modsupport.c in python version 2.3.4 */
SWIGINTERN int
PyModule_AddObject(PyObject *m, char *name, PyObject *o)
{
  PyObject *dict;
  if (!PyModule_Check(m)) {
    PyErr_SetString(PyExc_TypeError,
		    "PyModule_AddObject() needs module as first arg");
    return SWIG_ERROR;
  }
  if (!o) {
    PyErr_SetString(PyExc_TypeError,
		    "PyModule_AddObject() needs non-NULL value");
    return SWIG_ERROR;
  }
  
  dict = PyModule_GetDict(m);
  if (dict == NULL) {
    /* Internal error -- modules must have a dict! */
    PyErr_Format(PyExc_SystemError, "module '%s' has no __dict__",
		 PyModule_GetName(m));
    return SWIG_ERROR;
  }
  if (PyDict_SetItemString(dict, name, o))
    return SWIG_ERROR;
  Py_DECREF(o);
  return SWIG_OK;
}
#endif

SWIGRUNTIME void
SWIG_Python_DestroyModule(void *vptr)
{
  swig_module_info *swig_module = (swig_module_info *) vptr;
  swig_type_info **types = swig_module->types;
  size_t i;
  for (i =0; i < swig_module->size; ++i) {
    swig_type_info *ty = types[i];
    if (ty->owndata) {
      PySwigClientData *data = (PySwigClientData *) ty->clientdata;
      if (data) PySwigClientData_Del(data);
    }
  }
  Py_DECREF(SWIG_This());
}

SWIGRUNTIME void
SWIG_Python_SetModule(swig_module_info *swig_module) {
  static PyMethodDef swig_empty_runtime_method_table[] = { {NULL, NULL, 0, NULL} };/* Sentinel */

  PyObject *module = Py_InitModule((char*)"swig_runtime_data" SWIG_RUNTIME_VERSION,
				   swig_empty_runtime_method_table);
  PyObject *pointer = PyCObject_FromVoidPtr((void *) swig_module, SWIG_Python_DestroyModule);
  if (pointer && module) {
    PyModule_AddObject(module, (char*)"type_pointer" SWIG_TYPE_TABLE_NAME, pointer);
  } else {
    Py_XDECREF(pointer);
  }
}

/* The python cached type query */
SWIGRUNTIME PyObject *
SWIG_Python_TypeCache(void) {
  static PyObject *SWIG_STATIC_POINTER(cache) = PyDict_New();
  return cache;
}

SWIGRUNTIME swig_type_info *
SWIG_Python_TypeQuery(const char *type)
{
  PyObject *cache = SWIG_Python_TypeCache();
  PyObject *key = PyString_FromString(type); 
  PyObject *obj = PyDict_GetItem(cache, key);
  swig_type_info *descriptor;
  if (obj) {
    descriptor = (swig_type_info *) PyCObject_AsVoidPtr(obj);
  } else {
    swig_module_info *swig_module = SWIG_Python_GetModule();
    descriptor = SWIG_TypeQueryModule(swig_module, swig_module, type);
    if (descriptor) {
      obj = PyCObject_FromVoidPtr(descriptor, NULL);
      PyDict_SetItem(cache, key, obj);
      Py_DECREF(obj);
    }
  }
  Py_DECREF(key);
  return descriptor;
}

/* 
   For backward compatibility only
*/
#define SWIG_POINTER_EXCEPTION  0
#define SWIG_arg_fail(arg)      SWIG_Python_ArgFail(arg)
#define SWIG_MustGetPtr(p, type, argnum, flags)  SWIG_Python_MustGetPtr(p, type, argnum, flags)

SWIGRUNTIME int
SWIG_Python_AddErrMesg(const char* mesg, int infront)
{
  if (PyErr_Occurred()) {
    PyObject *type = 0;
    PyObject *value = 0;
    PyObject *traceback = 0;
    PyErr_Fetch(&type, &value, &traceback);
    if (value) {
      PyObject *old_str = PyObject_Str(value);
      Py_XINCREF(type);
      PyErr_Clear();
      if (infront) {
	PyErr_Format(type, "%s %s", mesg, PyString_AsString(old_str));
      } else {
	PyErr_Format(type, "%s %s", PyString_AsString(old_str), mesg);
      }
      Py_DECREF(old_str);
    }
    return 1;
  } else {
    return 0;
  }
}
  
SWIGRUNTIME int
SWIG_Python_ArgFail(int argnum)
{
  if (PyErr_Occurred()) {
    /* add information about failing argument */
    char mesg[256];
    PyOS_snprintf(mesg, sizeof(mesg), "argument number %d:", argnum);
    return SWIG_Python_AddErrMesg(mesg, 1);
  } else {
    return 0;
  }
}

SWIGRUNTIMEINLINE const char *
PySwigObject_GetDesc(PyObject *self)
{
  PySwigObject *v = (PySwigObject *)self;
  swig_type_info *ty = v ? v->ty : 0;
  return ty ? ty->str : (char*)"";
}

SWIGRUNTIME void
SWIG_Python_TypeError(const char *type, PyObject *obj)
{
  if (type) {
#if defined(SWIG_COBJECT_TYPES)
    if (obj && PySwigObject_Check(obj)) {
      const char *otype = (const char *) PySwigObject_GetDesc(obj);
      if (otype) {
	PyErr_Format(PyExc_TypeError, "a '%s' is expected, 'PySwigObject(%s)' is received",
		     type, otype);
	return;
      }
    } else 
#endif      
    {
      const char *otype = (obj ? obj->ob_type->tp_name : 0); 
      if (otype) {
	PyObject *str = PyObject_Str(obj);
	const char *cstr = str ? PyString_AsString(str) : 0;
	if (cstr) {
	  PyErr_Format(PyExc_TypeError, "a '%s' is expected, '%s(%s)' is received",
		       type, otype, cstr);
	} else {
	  PyErr_Format(PyExc_TypeError, "a '%s' is expected, '%s' is received",
		       type, otype);
	}
	Py_XDECREF(str);
	return;
      }
    }   
    PyErr_Format(PyExc_TypeError, "a '%s' is expected", type);
  } else {
    PyErr_Format(PyExc_TypeError, "unexpected type is received");
  }
}


/* Convert a pointer value, signal an exception on a type mismatch */
SWIGRUNTIME void *
SWIG_Python_MustGetPtr(PyObject *obj, swig_type_info *ty, int argnum, int flags) {
  void *result;
  if (SWIG_Python_ConvertPtr(obj, &result, ty, flags) == -1) {
    PyErr_Clear();
    if (flags & SWIG_POINTER_EXCEPTION) {
      SWIG_Python_TypeError(SWIG_TypePrettyName(ty), obj);
      SWIG_Python_ArgFail(argnum);
    }
  }
  return result;
}


#ifdef __cplusplus
#if 0
{ /* cc-mode */
#endif
}
#endif



#define SWIG_exception_fail(code, msg) do { SWIG_Error(code, msg); SWIG_fail; } while(0) 

#define SWIG_contract_assert(expr, msg) if (!(expr)) { SWIG_Error(SWIG_RuntimeError, msg); SWIG_fail; } else 



  #define SWIG_exception(code, msg) do { SWIG_Error(code, msg); SWIG_fail;; } while(0) 


/* -------- TYPES TABLE (BEGIN) -------- */

#define SWIGTYPE_p_Component swig_types[0]
#define SWIGTYPE_p_Components swig_types[1]
#define SWIGTYPE_p_GtoDB swig_types[2]
#define SWIGTYPE_p_Object swig_types[3]
#define SWIGTYPE_p_Objects swig_types[4]
#define SWIGTYPE_p_Properties swig_types[5]
#define SWIGTYPE_p_Property swig_types[6]
#define SWIGTYPE_p_allocator_type swig_types[7]
#define SWIGTYPE_p_char swig_types[8]
#define SWIGTYPE_p_difference_type swig_types[9]
#define SWIGTYPE_p_double swig_types[10]
#define SWIGTYPE_p_float swig_types[11]
#define SWIGTYPE_p_int swig_types[12]
#define SWIGTYPE_p_p_PyObject swig_types[13]
#define SWIGTYPE_p_size_type swig_types[14]
#define SWIGTYPE_p_std__invalid_argument swig_types[15]
#define SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t swig_types[16]
#define SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t__allocator_type swig_types[17]
#define SWIGTYPE_p_swig__PySwigIterator swig_types[18]
#define SWIGTYPE_p_unsigned_char swig_types[19]
#define SWIGTYPE_p_unsigned_int swig_types[20]
#define SWIGTYPE_p_unsigned_short swig_types[21]
#define SWIGTYPE_p_value_type swig_types[22]
static swig_type_info *swig_types[24];
static swig_module_info swig_module = {swig_types, 23, 0, 0, 0, 0};
#define SWIG_TypeQuery(name) SWIG_TypeQueryModule(&swig_module, &swig_module, name)
#define SWIG_MangledTypeQuery(name) SWIG_MangledTypeQueryModule(&swig_module, &swig_module, name)

/* -------- TYPES TABLE (END) -------- */

#if (PY_VERSION_HEX <= 0x02000000)
# if !defined(SWIG_PYTHON_CLASSIC)
#  error "This python version requires swig to be run with the '-classic' option"
# endif
#endif

/*-----------------------------------------------
              @(target):= _gtoDB.so
  ------------------------------------------------*/
#define SWIG_init    init_gtoDB

#define SWIG_name    "_gtoDB"

#define SWIGVERSION 0x010331 
#define SWIG_VERSION SWIGVERSION


#define SWIG_as_voidptr(a) const_cast< void * >(static_cast< const void * >(a)) 
#define SWIG_as_voidptrptr(a) ((void)SWIG_as_voidptr(*a),reinterpret_cast< void** >(a)) 


#include <stdexcept>


namespace swig {
  class PyObject_ptr {
  protected:
    PyObject *_obj;

  public:
    PyObject_ptr() :_obj(0)
    {
    }

    PyObject_ptr(const PyObject_ptr& item) : _obj(item._obj)
    {
      Py_XINCREF(_obj);      
    }
    
    PyObject_ptr(PyObject *obj, bool initial_ref = true) :_obj(obj)
    {
      if (initial_ref) Py_XINCREF(_obj);
    }
    
    PyObject_ptr & operator=(const PyObject_ptr& item) 
    {
      Py_XINCREF(item._obj);
      Py_XDECREF(_obj);
      _obj = item._obj;
      return *this;      
    }
    
    ~PyObject_ptr() 
    {
      Py_XDECREF(_obj);
    }
    
    operator PyObject *() const
    {
      return _obj;
    }

    PyObject *operator->() const
    {
      return _obj;
    }
  };
}


namespace swig {
  struct PyObject_var : PyObject_ptr {
    PyObject_var(PyObject* obj = 0) : PyObject_ptr(obj, false) { }
    
    PyObject_var & operator = (PyObject* obj)
    {
      Py_XDECREF(_obj);
      _obj = obj;
      return *this;      
    }
  };
}


#include <iostream>


#include <stdexcept>


  
#if defined(__SUNPRO_CC) && defined(_RWSTD_VER)
#  define SWIG_STD_NOASSIGN_STL
#  define SWIG_STD_NOINSERT_TEMPLATE_STL
#  define SWIG_STD_NOITERATOR_TRAITS_STL
#endif

#if defined(__GNUC__)
#  if __GNUC__ == 2 && __GNUC_MINOR <= 96
#     define SWIG_STD_NOMODERN_STL
#  endif
#endif




#include <string>
#include <stdexcept>

  
namespace swig {
  struct stop_iteration {
  };

  struct PySwigIterator {
  private:
    PyObject_ptr _seq;

  protected:
    PySwigIterator(PyObject *seq) : _seq(seq)
    {
    }
      
  public:
    virtual ~PySwigIterator() {}

    // Access iterator method, required by Python
    virtual PyObject *value() const = 0;

    // Forward iterator method, required by Python
    virtual PySwigIterator *incr(size_t n = 1) = 0;
    
    // Backward iterator method, very common in C++, but not required in Python
    virtual PySwigIterator *decr(size_t n = 1)
    {
      throw stop_iteration();
    }

    // Random access iterator methods, but not required in Python
    virtual ptrdiff_t distance(const PySwigIterator &x) const
    {
      throw std::invalid_argument("operation not supported");
    }

    virtual bool equal (const PySwigIterator &x) const
    {
      throw std::invalid_argument("operation not supported");
    }
    
    // C++ common/needed methods
    virtual PySwigIterator *copy() const = 0;

    PyObject *next()
    {
      PyObject *obj = value();
      incr();
      return obj;
    }

    PyObject *previous()
    {
      decr();
      return value();
    }

    PySwigIterator *advance(ptrdiff_t n)
    {
      return  (n > 0) ?  incr(n) : decr(-n);
    }
      
    bool operator == (const PySwigIterator& x)  const
    {
      return equal(x);
    }
      
    bool operator != (const PySwigIterator& x) const
    {
      return ! operator==(x);
    }
      
    PySwigIterator& operator += (ptrdiff_t n)
    {
      return *advance(n);
    }

    PySwigIterator& operator -= (ptrdiff_t n)
    {
      return *advance(-n);
    }
      
    PySwigIterator* operator + (ptrdiff_t n) const
    {
      return copy()->advance(n);
    }

    PySwigIterator* operator - (ptrdiff_t n) const
    {
      return copy()->advance(-n);
    }
      
    ptrdiff_t operator - (const PySwigIterator& x) const
    {
      return x.distance(*this);
    }
      
    static swig_type_info* descriptor() {
      static int init = 0;
      static swig_type_info* desc = 0;
      if (!init) {
	desc = SWIG_TypeQuery("swig::PySwigIterator *");
	init = 1;
      }	
      return desc;
    }    
  };
}


SWIGINTERN int
SWIG_AsVal_double (PyObject *obj, double *val)
{
  int res = SWIG_TypeError;
  if (PyFloat_Check(obj)) {
    if (val) *val = PyFloat_AsDouble(obj);
    return SWIG_OK;
  } else if (PyInt_Check(obj)) {
    if (val) *val = PyInt_AsLong(obj);
    return SWIG_OK;
  } else if (PyLong_Check(obj)) {
    double v = PyLong_AsDouble(obj);
    if (!PyErr_Occurred()) {
      if (val) *val = v;
      return SWIG_OK;
    } else {
      PyErr_Clear();
    }
  }
#ifdef SWIG_PYTHON_CAST_MODE
  {
    int dispatch = 0;
    double d = PyFloat_AsDouble(obj);
    if (!PyErr_Occurred()) {
      if (val) *val = d;
      return SWIG_AddCast(SWIG_OK);
    } else {
      PyErr_Clear();
    }
    if (!dispatch) {
      long v = PyLong_AsLong(obj);
      if (!PyErr_Occurred()) {
	if (val) *val = v;
	return SWIG_AddCast(SWIG_AddCast(SWIG_OK));
      } else {
	PyErr_Clear();
      }
    }
  }
#endif
  return res;
}


#include <float.h>


#include <math.h>


SWIGINTERNINLINE int
SWIG_CanCastAsInteger(double *d, double min, double max) {
  double x = *d;
  if ((min <= x && x <= max)) {
   double fx = floor(x);
   double cx = ceil(x);
   double rd =  ((x - fx) < 0.5) ? fx : cx; /* simple rint */
   if ((errno == EDOM) || (errno == ERANGE)) {
     errno = 0;
   } else {
     double summ, reps, diff;
     if (rd < x) {
       diff = x - rd;
     } else if (rd > x) {
       diff = rd - x;
     } else {
       return 1;
     }
     summ = rd + x;
     reps = diff/summ;
     if (reps < 8*DBL_EPSILON) {
       *d = rd;
       return 1;
     }
   }
  }
  return 0;
}


SWIGINTERN int
SWIG_AsVal_unsigned_SS_long (PyObject *obj, unsigned long *val) 
{
  if (PyInt_Check(obj)) {
    long v = PyInt_AsLong(obj);
    if (v >= 0) {
      if (val) *val = v;
      return SWIG_OK;
    } else {
      return SWIG_OverflowError;
    }
  } else if (PyLong_Check(obj)) {
    unsigned long v = PyLong_AsUnsignedLong(obj);
    if (!PyErr_Occurred()) {
      if (val) *val = v;
      return SWIG_OK;
    } else {
      PyErr_Clear();
    }
  }
#ifdef SWIG_PYTHON_CAST_MODE
  {
    int dispatch = 0;
    unsigned long v = PyLong_AsUnsignedLong(obj);
    if (!PyErr_Occurred()) {
      if (val) *val = v;
      return SWIG_AddCast(SWIG_OK);
    } else {
      PyErr_Clear();
    }
    if (!dispatch) {
      double d;
      int res = SWIG_AddCast(SWIG_AsVal_double (obj,&d));
      if (SWIG_IsOK(res) && SWIG_CanCastAsInteger(&d, 0, ULONG_MAX)) {
	if (val) *val = (unsigned long)(d);
	return res;
      }
    }
  }
#endif
  return SWIG_TypeError;
}


SWIGINTERNINLINE int
SWIG_AsVal_size_t (PyObject * obj, size_t *val)
{
  unsigned long v;
  int res = SWIG_AsVal_unsigned_SS_long (obj, val ? &v : 0);
  if (SWIG_IsOK(res) && val) *val = static_cast< size_t >(v);
  return res;
}


  #define SWIG_From_long   PyInt_FromLong 


SWIGINTERNINLINE PyObject *
SWIG_From_ptrdiff_t  (ptrdiff_t value)
{    
  return SWIG_From_long  (static_cast< long >(value));
}


SWIGINTERNINLINE PyObject*
  SWIG_From_bool  (bool value)
{
  return PyBool_FromLong(value ? 1 : 0);
}


SWIGINTERN int
SWIG_AsVal_long (PyObject *obj, long* val)
{
  if (PyInt_Check(obj)) {
    if (val) *val = PyInt_AsLong(obj);
    return SWIG_OK;
  } else if (PyLong_Check(obj)) {
    long v = PyLong_AsLong(obj);
    if (!PyErr_Occurred()) {
      if (val) *val = v;
      return SWIG_OK;
    } else {
      PyErr_Clear();
    }
  }
#ifdef SWIG_PYTHON_CAST_MODE
  {
    int dispatch = 0;
    long v = PyInt_AsLong(obj);
    if (!PyErr_Occurred()) {
      if (val) *val = v;
      return SWIG_AddCast(SWIG_OK);
    } else {
      PyErr_Clear();
    }
    if (!dispatch) {
      double d;
      int res = SWIG_AddCast(SWIG_AsVal_double (obj,&d));
      if (SWIG_IsOK(res) && SWIG_CanCastAsInteger(&d, LONG_MIN, LONG_MAX)) {
	if (val) *val = (long)(d);
	return res;
      }
    }
  }
#endif
  return SWIG_TypeError;
}


SWIGINTERNINLINE int
SWIG_AsVal_ptrdiff_t (PyObject * obj, ptrdiff_t *val)
{
  long v;
  int res = SWIG_AsVal_long (obj, val ? &v : 0);
  if (SWIG_IsOK(res) && val) *val = static_cast< ptrdiff_t >(v);
  return res;
}


#include <stdexcept>


#include <algorithm>


#include <vector>


#include <string>


SWIGINTERNINLINE PyObject *
SWIG_From_int  (int value)
{    
  return SWIG_From_long  (value);
}


namespace swig {  
  template <class Type>
  struct noconst_traits {
    typedef Type noconst_type;
  };

  template <class Type>
  struct noconst_traits<const Type> {
    typedef Type noconst_type;
  };

  /*
    type categories
  */
  struct pointer_category { };  
  struct value_category { };

  /*
    General traits that provides type_name and type_info
  */
  template <class Type> struct traits { };

  template <class Type>
  inline const char* type_name() {
    return traits<typename noconst_traits<Type >::noconst_type >::type_name();
  }

  template <class Type> 
  struct traits_info {
    static swig_type_info *type_query(std::string name) {
      name += " *";
      return SWIG_TypeQuery(name.c_str());
    }    
    static swig_type_info *type_info() {
      static swig_type_info *info = type_query(type_name<Type>());
      return info;
    }
  };

  template <class Type>
  inline swig_type_info *type_info() {
    return traits_info<Type>::type_info();
  }

  /*
    Partial specialization for pointers
  */
  template <class Type> struct traits <Type *> {
    typedef pointer_category category;
    static std::string make_ptr_name(const char* name) {
      std::string ptrname = name;
      ptrname += " *";
      return ptrname;
    }    
    static const char* type_name() {
      static std::string name = make_ptr_name(swig::type_name<Type>());
      return name.c_str();
    }
  };

  template <class Type, class Category> 
  struct traits_as { };
 
  template <class Type, class Category> 
  struct traits_check { };

}


namespace swig {  
  /*
    Traits that provides the from method
  */
  template <class Type> struct traits_from_ptr {
    static PyObject *from(Type *val, int owner = 0) {
      return SWIG_NewPointerObj(val, type_info<Type>(), owner);
    }
  };

  template <class Type> struct traits_from {
    static PyObject *from(const Type& val) {
      return traits_from_ptr<Type>::from(new Type(val), 1);
    }
  };

  template <class Type> struct traits_from<Type *> {
    static PyObject *from(Type* val) {
      return traits_from_ptr<Type>::from(val, 0);
    }
  };

  template <class Type> struct traits_from<const Type *> {
    static PyObject *from(const Type* val) {
      return traits_from_ptr<Type>::from(const_cast<Type*>(val), 0);
    }
  };


  template <class Type>
  inline PyObject *from(const Type& val) {
    return traits_from<Type>::from(val);
  }

  template <class Type>
  inline PyObject *from_ptr(Type* val, int owner) {
    return traits_from_ptr<Type>::from(val, owner);
  }

  /*
    Traits that provides the asval/as/check method
  */
  template <class Type>
  struct traits_asptr {   
    static int asptr(PyObject *obj, Type **val) {
      Type *p;
      int res = (SWIG_ConvertPtr(obj, (void**)&p, type_info<Type>(), 0) == SWIG_OK) ? SWIG_OLDOBJ : 0;
      if (SWIG_IsOK(res)) {
	if (val) *val = p;
      }
      return res;
    }
  }; 

  template <class Type>
  inline int asptr(PyObject *obj, Type **vptr) {
    return traits_asptr<Type>::asptr(obj, vptr);
  }

  template <class Type> 
  struct traits_asval {
    static int asval(PyObject *obj, Type *val) {
      if (val) {
	Type *p = 0;
	int res = traits_asptr<Type>::asptr(obj, &p);
	if (!SWIG_IsOK(res)) return res;	
	if (p) {
	  typedef typename noconst_traits<Type>::noconst_type noconst_type;
	  *(const_cast<noconst_type*>(val)) = *p;
	  if (SWIG_IsNewObj(res)){
	    delete p;
	    res = SWIG_DelNewMask(res);
	  }
	  return res;
	} else {
	  return SWIG_ERROR;
	}
      } else {
	return traits_asptr<Type>::asptr(obj, (Type **)(0));
      }
    }
  };

  template <class Type> struct traits_asval<Type*> {
    static int asval(PyObject *obj, Type **val) {
      if (val) {
        typedef typename noconst_traits<Type>::noconst_type noconst_type;
        noconst_type *p = 0;
        int res = traits_asptr<noconst_type>::asptr(obj,  &p);
        if (SWIG_IsOK(res)) {
          *(const_cast<noconst_type**>(val)) = p;
	}
	return res;
      } else {
	return traits_asptr<Type>::asptr(obj, (Type **)(0));
      }
    }
  };
  
  template <class Type>
  inline int asval(PyObject *obj, Type *val) {
    return traits_asval<Type>::asval(obj, val);
  }

  template <class Type> 
  struct traits_as<Type, value_category> {
    static Type as(PyObject *obj, bool throw_error) {
      Type v;
      int res = asval(obj, &v);
      if (!obj || !SWIG_IsOK(res)) {
	if (!PyErr_Occurred()) {
	  SWIG_Error(SWIG_TypeError,  swig::type_name<Type>());
	}
	if (throw_error) throw std::invalid_argument("bad type");
      }
      return v;
    }
  };

  template <class Type> 
  struct traits_as<Type, pointer_category> {
    static Type as(PyObject *obj, bool throw_error) {
      Type *v = 0;      
      int res = (obj ? traits_asptr<Type>::asptr(obj, &v) : SWIG_ERROR);
      if (SWIG_IsOK(res) && v) {
	if (SWIG_IsNewObj(res)) {
	  Type r(*v);
	  delete v;
	  return r;
	} else {
	  return *v;
	}
      } else {
	// Uninitialized return value, no Type() constructor required.
	static Type *v_def = (Type*) malloc(sizeof(Type));
	if (!PyErr_Occurred()) {
	  SWIG_Error(SWIG_TypeError,  swig::type_name<Type>());
	}
	if (throw_error) throw std::invalid_argument("bad type");
	memset(v_def,0,sizeof(Type));
	return *v_def;
      }
    }
  };

  template <class Type> 
  struct traits_as<Type*, pointer_category> {
    static Type* as(PyObject *obj, bool throw_error) {
      Type *v = 0;      
      int res = (obj ? traits_asptr<Type>::asptr(obj, &v) : SWIG_ERROR);
      if (SWIG_IsOK(res)) {
	return v;
      } else {
	if (!PyErr_Occurred()) {
	  SWIG_Error(SWIG_TypeError,  swig::type_name<Type>());
	}
	if (throw_error) throw std::invalid_argument("bad type");
	return 0;
      }
    }
  };
    
  template <class Type>
  inline Type as(PyObject *obj, bool te = false) {
    return traits_as<Type, typename traits<Type>::category>::as(obj, te);
  }

  template <class Type> 
  struct traits_check<Type, value_category> {
    static bool check(PyObject *obj) {
      int res = obj ? asval(obj, (Type *)(0)) : SWIG_ERROR;
      return SWIG_IsOK(res) ? true : false;
    }
  };

  template <class Type> 
  struct traits_check<Type, pointer_category> {
    static bool check(PyObject *obj) {
      int res = obj ? asptr(obj, (Type **)(0)) : SWIG_ERROR;
      return SWIG_IsOK(res) ? true : false;
    }
  };

  template <class Type>
  inline bool check(PyObject *obj) {
    return traits_check<Type, typename traits<Type>::category>::check(obj);
  }
}


#include <functional>

namespace std {
  template <>
  struct less <PyObject *>: public binary_function<PyObject *, PyObject *, bool>
  {
    bool
    operator()(PyObject * v, PyObject *w) const
    { 
      bool res;
      SWIG_PYTHON_THREAD_BEGIN_BLOCK;
      res = PyObject_Compare(v, w) < 0;
      SWIG_PYTHON_THREAD_END_BLOCK;
      return res;
    }
  };

  template <>
  struct less <swig::PyObject_ptr>: public binary_function<swig::PyObject_ptr, swig::PyObject_ptr, bool>
  {
    bool
    operator()(const swig::PyObject_ptr& v, const swig::PyObject_ptr& w) const
    {
      return std::less<PyObject *>()(v, w);
    }
  };

  template <>
  struct less <swig::PyObject_var>: public binary_function<swig::PyObject_var, swig::PyObject_var, bool>
  {
    bool
    operator()(const swig::PyObject_var& v, const swig::PyObject_var& w) const
    {
      return std::less<PyObject *>()(v, w);
    }
  };

}

namespace swig {
  template <> struct traits<PyObject *> {
    typedef value_category category;
    static const char* type_name() { return "PyObject *"; }
  };  

  template <>  struct traits_asval<PyObject * > {   
    typedef PyObject * value_type;
    static int asval(PyObject *obj, value_type *val) {
      if (val) *val = obj;
      return SWIG_OK;
    }
  };

  template <> 
  struct traits_check<PyObject *, value_category> {
    static bool check(PyObject *) {
      return true;
    }
  };

  template <>  struct traits_from<PyObject *> {
    typedef PyObject * value_type;
    static PyObject *from(const value_type& val) {
      Py_XINCREF(val);
      return val;
    }
  };
  
}

namespace swig {
  inline size_t
  check_index(ptrdiff_t i, size_t size, bool insert = false) {
    if ( i < 0 ) {
      if ((size_t) (-i) <= size)
	return (size_t) (i + size);
    } else if ( (size_t) i < size ) {
      return (size_t) i;
    } else if (insert && ((size_t) i == size)) {
      return size;
    }
    
    throw std::out_of_range("index out of range");
  }

  inline size_t
  slice_index(ptrdiff_t i, size_t size) {
    if ( i < 0 ) {
      if ((size_t) (-i) <= size) {
	return (size_t) (i + size);
      } else {
	throw std::out_of_range("index out of range");
      }
    } else {
      return ( (size_t) i < size ) ? ((size_t) i) : size;
    }
  }

  template <class Sequence, class Difference>
  inline typename Sequence::iterator
  getpos(Sequence* self, Difference i)  {
    typename Sequence::iterator pos = self->begin();
    std::advance(pos, check_index(i,self->size()));
    return pos;
  }

  template <class Sequence, class Difference>
  inline typename Sequence::const_iterator
  cgetpos(const Sequence* self, Difference i)  {
    typename Sequence::const_iterator pos = self->begin();
    std::advance(pos, check_index(i,self->size()));
    return pos;
  }

  template <class Sequence, class Difference>
  inline Sequence*
  getslice(const Sequence* self, Difference i, Difference j) {
    typename Sequence::size_type size = self->size();
    typename Sequence::size_type ii = swig::check_index(i, size);
    typename Sequence::size_type jj = swig::slice_index(j, size);

    if (jj > ii) {
      typename Sequence::const_iterator vb = self->begin();
      typename Sequence::const_iterator ve = self->begin();
      std::advance(vb,ii);
      std::advance(ve,jj);
      return new Sequence(vb, ve);
    } else {
      return new Sequence();
    }
  }

  template <class Sequence, class Difference, class InputSeq>
  inline void
  setslice(Sequence* self, Difference i, Difference j, const InputSeq& v) {
    typename Sequence::size_type size = self->size();
    typename Sequence::size_type ii = swig::check_index(i, size, true);
    typename Sequence::size_type jj = swig::slice_index(j, size);
    if (jj < ii) jj = ii;
    size_t ssize = jj - ii;
    if (ssize <= v.size()) {
      typename Sequence::iterator sb = self->begin();
      typename InputSeq::const_iterator vmid = v.begin();
      std::advance(sb,ii);
      std::advance(vmid, jj - ii);
      self->insert(std::copy(v.begin(), vmid, sb), vmid, v.end());
    } else {
      typename Sequence::iterator sb = self->begin();
      typename Sequence::iterator se = self->begin();
      std::advance(sb,ii);
      std::advance(se,jj);
      self->erase(sb,se);
      self->insert(sb, v.begin(), v.end());
    }
  }

  template <class Sequence, class Difference>
  inline void
  delslice(Sequence* self, Difference i, Difference j) {
    typename Sequence::size_type size = self->size();
    typename Sequence::size_type ii = swig::check_index(i, size, true);
    typename Sequence::size_type jj = swig::slice_index(j, size);
    if (jj > ii) {
      typename Sequence::iterator sb = self->begin();
      typename Sequence::iterator se = self->begin();
      std::advance(sb,ii);
      std::advance(se,jj);
      self->erase(sb,se);
    }
  }
}


#if !defined(SWIG_STD_NOITERATOR_TRAITS_STL)
#include <iterator>
#else
namespace std  {
  template <class Iterator>
  struct iterator_traits {
    typedef ptrdiff_t difference_type;
    typedef typename Iterator::value_type value_type;
  };

#if defined(__SUNPRO_CC) && defined(_RWSTD_VER)
  template <class Iterator, class Category,class T, class Reference, class Pointer, class Distance>
  struct iterator_traits<__reverse_bi_iterator<Iterator,Category,T,Reference,Pointer,Distance> > {
    typedef Distance difference_type;
    typedef T value_type;
  };
#endif  

  template <class T>
  struct iterator_traits<T*> {
    typedef T value_type;
    typedef ptrdiff_t difference_type;
  };

  template<typename _InputIterator>
  inline typename iterator_traits<_InputIterator>::difference_type
  distance(_InputIterator __first, _InputIterator __last)
  {
    typename iterator_traits<_InputIterator>::difference_type __n = 0;
    while (__first != __last) {
      ++__first; ++__n;
    }
    return __n;
  }

} 
#endif


namespace swig {
  template<typename OutIterator>
  class PySwigIterator_T :  public PySwigIterator
  {
  public:
    typedef OutIterator out_iterator;
    typedef typename std::iterator_traits<out_iterator>::value_type value_type;    
    typedef PySwigIterator_T<out_iterator> self_type;

    PySwigIterator_T(out_iterator curr, PyObject *seq)
      : PySwigIterator(seq), current(curr)
    {
    }

    const out_iterator& get_current() const
    {
      return current;
    }

    
    bool equal (const PySwigIterator &iter) const
    {
      const self_type *iters = dynamic_cast<const self_type *>(&iter);
      if (iters) {
	return (current == iters->get_current());
      } else {
	throw std::invalid_argument("bad iterator type");
      }
    }
    
    ptrdiff_t distance(const PySwigIterator &iter) const
    {
      const self_type *iters = dynamic_cast<const self_type *>(&iter);
      if (iters) {
	return std::distance(current, iters->get_current());
      } else {
	throw std::invalid_argument("bad iterator type");
      }
    }    
    
  protected:
    out_iterator current;
  };
  
  template <class ValueType>
  struct from_oper 
  {
    typedef const ValueType& argument_type;
    typedef PyObject *result_type;
    result_type operator()(argument_type v) const
    {
      return swig::from(v);
    }
  };

  template<typename OutIterator, 
	   typename ValueType = typename std::iterator_traits<OutIterator>::value_type,
	   typename FromOper = from_oper<ValueType> >
  class PySwigIteratorOpen_T :  public PySwigIterator_T<OutIterator>
  {
  public:
    FromOper from;
    typedef OutIterator out_iterator;
    typedef ValueType value_type;
    typedef PySwigIterator_T<out_iterator>  base;
    typedef PySwigIteratorOpen_T<OutIterator, ValueType, FromOper> self_type;
    
    PySwigIteratorOpen_T(out_iterator curr, PyObject *seq)
      : PySwigIterator_T<OutIterator>(curr, seq)
    {
    }
    
    PyObject *value() const {
      return from(static_cast<const value_type&>(*(base::current)));
    }
    
    PySwigIterator *copy() const
    {
      return new self_type(*this);
    }

    PySwigIterator *incr(size_t n = 1)
    {
      while (n--) {
	++base::current;
      }
      return this;
    }

    PySwigIterator *decr(size_t n = 1)
    {
      while (n--) {
	--base::current;
      }
      return this;
    }
  };

  template<typename OutIterator, 
	   typename ValueType = typename std::iterator_traits<OutIterator>::value_type,
	   typename FromOper = from_oper<ValueType> >
  class PySwigIteratorClosed_T :  public PySwigIterator_T<OutIterator>
  {
  public:
    FromOper from;
    typedef OutIterator out_iterator;
    typedef ValueType value_type;
    typedef PySwigIterator_T<out_iterator>  base;    
    typedef PySwigIteratorClosed_T<OutIterator, ValueType, FromOper> self_type;
    
    PySwigIteratorClosed_T(out_iterator curr, out_iterator first, out_iterator last, PyObject *seq)
      : PySwigIterator_T<OutIterator>(curr, seq), begin(first), end(last)
    {
    }
    
    PyObject *value() const {
      if (base::current == end) {
	throw stop_iteration();
      } else {
	return from(static_cast<const value_type&>(*(base::current)));
      }
    }
    
    PySwigIterator *copy() const
    {
      return new self_type(*this);
    }

    PySwigIterator *incr(size_t n = 1)
    {
      while (n--) {
	if (base::current == end) {
	  throw stop_iteration();
	} else {
	  ++base::current;
	}
      }
      return this;
    }

    PySwigIterator *decr(size_t n = 1)
    {
      while (n--) {
	if (base::current == begin) {
	  throw stop_iteration();
	} else {
	  --base::current;
	}
      }
      return this;
    }

  private:
    out_iterator begin;
    out_iterator end;
  };

  template<typename OutIter>
  inline PySwigIterator*
  make_output_iterator(const OutIter& current, const OutIter& begin,const OutIter& end, PyObject *seq = 0)
  {
    return new PySwigIteratorClosed_T<OutIter>(current, begin, end, seq);
  }

  template<typename OutIter>
  inline PySwigIterator*
  make_output_iterator(const OutIter& current, PyObject *seq = 0)
  {
    return new PySwigIteratorOpen_T<OutIter>(current, seq);
  }
}


namespace swig
{
  template <class T>
  struct PySequence_Ref
  {
    PySequence_Ref(PyObject* seq, int index)
      : _seq(seq), _index(index)
    {
    }
    
    operator T () const
    {
      swig::PyObject_var item = PySequence_GetItem(_seq, _index);
      try {
	return swig::as<T>(item, true);
      } catch (std::exception& e) {
	char msg[1024];
	sprintf(msg, "in sequence element %d ", _index);
	if (!PyErr_Occurred()) {
	  SWIG_Error(SWIG_TypeError,  swig::type_name<T>());
	}
	SWIG_Python_AddErrorMsg(msg);
	SWIG_Python_AddErrorMsg(e.what());
	throw;
      }
    }

    PySequence_Ref& operator=(const T& v)
    {
      PySequence_SetItem(_seq, _index, swig::from<T>(v));
      return *this;
    }

  private:
    PyObject* _seq;
    int _index;
  };

  template <class T>
  struct PySequence_ArrowProxy
  {
    PySequence_ArrowProxy(const T& x): m_value(x) {}
    const T* operator->() const { return &m_value; }
    operator const T*() const { return &m_value; }
    T m_value;
  };

  template <class T, class Reference >
  struct PySequence_InputIterator
  {
    typedef PySequence_InputIterator<T, Reference > self;

    typedef std::random_access_iterator_tag iterator_category;
    typedef Reference reference;
    typedef T value_type;
    typedef T* pointer;
    typedef int difference_type;

    PySequence_InputIterator()
    {
    }

    PySequence_InputIterator(PyObject* seq, int index)
      : _seq(seq), _index(index)
    {
    }

    reference operator*() const
    {
      return reference(_seq, _index);
    }

    PySequence_ArrowProxy<T>
    operator->() const {
      return PySequence_ArrowProxy<T>(operator*());
    }

    bool operator==(const self& ri) const
    {
      return (_index == ri._index) && (_seq == ri._seq);
    }

    bool operator!=(const self& ri) const
    {
      return !(operator==(ri));
    }

    self& operator ++ ()
    {
      ++_index;
      return *this;
    }

    self& operator -- ()
    {
      --_index;
      return *this;
    }

    self& operator += (difference_type n)
    {
      _index += n;
      return *this;
    }

    self operator +(difference_type n) const
    {
      return self(_seq, _index + n);
    }

    self& operator -= (difference_type n)
    {
      _index -= n;
      return *this;
    }

    self operator -(difference_type n) const
    {
      return self(_seq, _index - n);
    }

    difference_type operator - (const self& ri) const
    {
      return _index - ri._index;
    }

    bool operator < (const self& ri) const
    {
      return _index < ri._index;
    }

    reference
    operator[](difference_type n) const
    {
      return reference(_seq, _index + n);
    }

  private:
    PyObject* _seq;
    difference_type _index;
  };

  template <class T>
  struct PySequence_Cont
  {
    typedef PySequence_Ref<T> reference;
    typedef const PySequence_Ref<T> const_reference;
    typedef T value_type;
    typedef T* pointer;
    typedef int difference_type;
    typedef int size_type;
    typedef const pointer const_pointer;
    typedef PySequence_InputIterator<T, reference> iterator;
    typedef PySequence_InputIterator<T, const_reference> const_iterator;

    PySequence_Cont(PyObject* seq) : _seq(0)
    {
      if (!PySequence_Check(seq)) {
	throw std::invalid_argument("a sequence is expected");
      }
      _seq = seq;
      Py_INCREF(_seq);
    }

    ~PySequence_Cont()
    {
      if (_seq) Py_DECREF(_seq);
    }

    size_type size() const
    {
      return PySequence_Size(_seq);
    }

    bool empty() const
    {
      return size() == 0;
    }

    iterator begin()
    {
      return iterator(_seq, 0);
    }

    const_iterator begin() const
    {
      return const_iterator(_seq, 0);
    }

    iterator end()
    {
      return iterator(_seq, size());
    }

    const_iterator end() const
    {
      return const_iterator(_seq, size());
    }

    reference operator[](difference_type n)
    {
      return reference(_seq, n);
    }

    const_reference operator[](difference_type n)  const
    {
      return const_reference(_seq, n);
    }

    bool check(bool set_err = true) const
    {
      int s = size();
      for (int i = 0; i < s; ++i) {
	swig::PyObject_var item = PySequence_GetItem(_seq, i);
	if (!swig::check<value_type>(item)) {
	  if (set_err) {
	    char msg[1024];
	    sprintf(msg, "in sequence element %d", i);
	    SWIG_Error(SWIG_RuntimeError, msg);
	  }
	  return false;
	}
      }
      return true;
    }

  private:
    PyObject* _seq;
  };

}


SWIGINTERN swig_type_info*
SWIG_pchar_descriptor(void)
{
  static int init = 0;
  static swig_type_info* info = 0;
  if (!init) {
    info = SWIG_TypeQuery("_p_char");
    init = 1;
  }
  return info;
}


SWIGINTERN int
SWIG_AsCharPtrAndSize(PyObject *obj, char** cptr, size_t* psize, int *alloc)
{
  if (PyString_Check(obj)) {
    char *cstr; Py_ssize_t len;
    PyString_AsStringAndSize(obj, &cstr, &len);
    if (cptr)  {
      if (alloc) {
	/* 
	   In python the user should not be able to modify the inner
	   string representation. To warranty that, if you define
	   SWIG_PYTHON_SAFE_CSTRINGS, a new/copy of the python string
	   buffer is always returned.

	   The default behavior is just to return the pointer value,
	   so, be careful.
	*/ 
#if defined(SWIG_PYTHON_SAFE_CSTRINGS)
	if (*alloc != SWIG_OLDOBJ) 
#else
	if (*alloc == SWIG_NEWOBJ) 
#endif
	  {
	    *cptr = reinterpret_cast< char* >(memcpy((new char[len + 1]), cstr, sizeof(char)*(len + 1)));
	    *alloc = SWIG_NEWOBJ;
	  }
	else {
	  *cptr = cstr;
	  *alloc = SWIG_OLDOBJ;
	}
      } else {
	*cptr = PyString_AsString(obj);
      }
    }
    if (psize) *psize = len + 1;
    return SWIG_OK;
  } else {
    swig_type_info* pchar_descriptor = SWIG_pchar_descriptor();
    if (pchar_descriptor) {
      void* vptr = 0;
      if (SWIG_ConvertPtr(obj, &vptr, pchar_descriptor, 0) == SWIG_OK) {
	if (cptr) *cptr = (char *) vptr;
	if (psize) *psize = vptr ? (strlen((char *)vptr) + 1) : 0;
	if (alloc) *alloc = SWIG_OLDOBJ;
	return SWIG_OK;
      }
    }
  }
  return SWIG_TypeError;
}


SWIGINTERN int
SWIG_AsPtr_std_string (PyObject * obj, std::string **val) 
{
  char* buf = 0 ; size_t size = 0; int alloc = SWIG_OLDOBJ;
  if (SWIG_IsOK((SWIG_AsCharPtrAndSize(obj, &buf, &size, &alloc)))) {
    if (buf) {
      if (val) *val = new std::string(buf, size - 1);
      if (alloc == SWIG_NEWOBJ) delete[] buf;
      return SWIG_NEWOBJ;
    } else {
      if (val) *val = 0;
      return SWIG_OLDOBJ;
    }
  } else {
    static int init = 0;
    static swig_type_info* descriptor = 0;
    if (!init) {
      descriptor = SWIG_TypeQuery("std::string" " *");
      init = 1;
    }
    if (descriptor) {
      std::string *vptr;
      int res = SWIG_ConvertPtr(obj, (void**)&vptr, descriptor, 0);
      if (SWIG_IsOK(res) && val) *val = vptr;
      return res;
    }
  }
  return SWIG_ERROR;
}


SWIGINTERN int
SWIG_AsVal_std_string (PyObject * obj, std::string *val)
{
  std::string* v = (std::string *) 0;
  int res = SWIG_AsPtr_std_string (obj, &v);
  if (!SWIG_IsOK(res)) return res;
  if (v) {
    if (val) *val = *v;
    if (SWIG_IsNewObj(res)) {
      delete v;
      res = SWIG_DelNewMask(res);
    }
    return res;
  }
  return SWIG_ERROR;
}


SWIGINTERNINLINE PyObject *
SWIG_FromCharPtrAndSize(const char* carray, size_t size)
{
  if (carray) {
    if (size > INT_MAX) {
      swig_type_info* pchar_descriptor = SWIG_pchar_descriptor();
      return pchar_descriptor ? 
	SWIG_NewPointerObj(const_cast< char * >(carray), pchar_descriptor, 0) : SWIG_Py_Void();
    } else {
      return PyString_FromStringAndSize(carray, static_cast< int >(size));
    }
  } else {
    return SWIG_Py_Void();
  }
}


SWIGINTERNINLINE PyObject *
SWIG_From_std_string  (const std::string& s)
{
  if (s.size()) {
    return SWIG_FromCharPtrAndSize(s.data(), s.size());
  } else {
    return SWIG_FromCharPtrAndSize(s.c_str(), 0);
  }
}


namespace swig {
  template <> struct traits<std::string > {
    typedef value_category category;
    static const char* type_name() { return"std::string"; }
  };  
  template <>  struct traits_asval<std::string > {   
    typedef std::string value_type;
    static int asval(PyObject *obj, value_type *val) { 
      return SWIG_AsVal_std_string (obj, val);
    }
  };
  template <>  struct traits_from<std::string > {
    typedef std::string value_type;
    static PyObject *from(const value_type& val) {
      return SWIG_From_std_string  (val);
    }
  };
}


namespace swig {
  template <class PySeq, class Seq>
  inline void
  assign(const PySeq& pyseq, Seq* seq) {
#ifdef SWIG_STD_NOASSIGN_STL
    typedef typename PySeq::value_type value_type;
    typename PySeq::const_iterator it = pyseq.begin();
    for (;it != pyseq.end(); ++it) {
      seq->insert(seq->end(),(value_type)(*it));
    }
#else
    seq->assign(pyseq.begin(), pyseq.end());
#endif
  }

  template <class Seq, class T = typename Seq::value_type >
  struct traits_asptr_stdseq {
    typedef Seq sequence;
    typedef T value_type;

    static int asptr(PyObject *obj, sequence **seq) {
      if (PySequence_Check(obj)) {
	try {
	  PySequence_Cont<value_type> pyseq(obj);
	  if (seq) {
	    sequence *pseq = new sequence();
	    assign(pyseq, pseq);
	    *seq = pseq;
	    return SWIG_NEWOBJ;
	  } else {
	    return pyseq.check() ? SWIG_OK : SWIG_ERROR;
	  }
	} catch (std::exception& e) {
	  if (seq) {
	    if (!PyErr_Occurred()) {
	      PyErr_SetString(PyExc_TypeError, e.what());
	    }
	  }
	  return SWIG_ERROR;
	}
      } else {
	sequence *p;
	if (SWIG_ConvertPtr(obj,(void**)&p,
			    swig::type_info<sequence>(),0) == SWIG_OK) {
	  if (seq) *seq = p;
	  return SWIG_OLDOBJ;
	}
      }
      return SWIG_ERROR;
    }
  };

  template <class Seq, class T = typename Seq::value_type >
  struct traits_from_stdseq {
    typedef Seq sequence;
    typedef T value_type;
    typedef typename Seq::size_type size_type;
    typedef typename sequence::const_iterator const_iterator;

    static PyObject *from(const sequence& seq) {






      size_type size = seq.size();
      if (size <= (size_type)INT_MAX) {
	PyObject *obj = PyTuple_New((int)size);
	int i = 0;
	for (const_iterator it = seq.begin();
	     it != seq.end(); ++it, ++i) {
	  PyTuple_SetItem(obj,i,swig::from<value_type>(*it));
	}
	return obj;
      } else {
	PyErr_SetString(PyExc_OverflowError,"sequence size not valid in python");
	return NULL;
      }
    }
  };
}


  namespace swig {
    template <class T>
    struct traits_asptr<std::vector<T> >  {
      static int asptr(PyObject *obj, std::vector<T> **vec) {
	return traits_asptr_stdseq<std::vector<T> >::asptr(obj, vec);
      }
    };
    
    template <class T>
    struct traits_from<std::vector<T> > {
      static PyObject *from(const std::vector<T>& vec) {
	return traits_from_stdseq<std::vector<T> >::from(vec);
      }
    };
  }


      namespace swig {
	template <>  struct traits<std::vector<std::string, std::allocator<std::string > > > {
	  typedef pointer_category category;
	  static const char* type_name() {
	    return "std::vector<" "std::string" "," "std::allocator<std::string >" " >";
	  }
	};
      }
    
SWIGINTERN swig::PySwigIterator *std_vector_Sl_std_string_Sg__iterator(std::vector<std::string > *self,PyObject **PYTHON_SELF){
      return swig::make_output_iterator(self->begin(), self->begin(), self->end(), *PYTHON_SELF);
    }
SWIGINTERN bool std_vector_Sl_std_string_Sg____nonzero__(std::vector<std::string > const *self){
      return !(self->empty());
    }
SWIGINTERN std::vector<std::string >::size_type std_vector_Sl_std_string_Sg____len__(std::vector<std::string > const *self){
      return self->size();
    }

SWIGINTERNINLINE PyObject* 
SWIG_From_unsigned_SS_long  (unsigned long value)
{
  return (value > LONG_MAX) ?
    PyLong_FromUnsignedLong(value) : PyInt_FromLong(static_cast< long >(value)); 
}


SWIGINTERNINLINE PyObject *
SWIG_From_size_t  (size_t value)
{    
  return SWIG_From_unsigned_SS_long  (static_cast< unsigned long >(value));
}

SWIGINTERN std::vector<std::string >::value_type std_vector_Sl_std_string_Sg__pop(std::vector<std::string > *self){
      if (self->size() == 0)
	throw std::out_of_range("pop from empty container");
      std::vector<std::string,std::allocator<std::string > >::value_type x = self->back();
      self->pop_back();
      return x;
    }
SWIGINTERN std::vector<std::string,std::allocator<std::string > > *std_vector_Sl_std_string_Sg____getslice__(std::vector<std::string > *self,std::vector<std::string >::difference_type i,std::vector<std::string >::difference_type j){
      return swig::getslice(self, i, j);
    }
SWIGINTERN void std_vector_Sl_std_string_Sg____setslice__(std::vector<std::string > *self,std::vector<std::string >::difference_type i,std::vector<std::string >::difference_type j,std::vector<std::string,std::allocator<std::string > > const &v){
      swig::setslice(self, i, j, v);
    }
SWIGINTERN void std_vector_Sl_std_string_Sg____delslice__(std::vector<std::string > *self,std::vector<std::string >::difference_type i,std::vector<std::string >::difference_type j){
      swig::delslice(self, i, j);
    }
SWIGINTERN void std_vector_Sl_std_string_Sg____delitem__(std::vector<std::string > *self,std::vector<std::string >::difference_type i){
      self->erase(swig::getpos(self,i));
    }
SWIGINTERN std::vector<std::string >::value_type const &std_vector_Sl_std_string_Sg____getitem__(std::vector<std::string > const *self,std::vector<std::string >::difference_type i){
      return *(swig::cgetpos(self, i));
    }
SWIGINTERN void std_vector_Sl_std_string_Sg____setitem__(std::vector<std::string > *self,std::vector<std::string >::difference_type i,std::vector<std::string >::value_type const &x){
      *(swig::getpos(self,i)) = x;
    }
SWIGINTERN void std_vector_Sl_std_string_Sg__append(std::vector<std::string > *self,std::vector<std::string >::value_type const &x){
      self->push_back(x);
    }
  // Begin verbatim

#include <stdexcept>
#include <Gto/Header.h>
#include <Gto/Reader.h>
#include <Gto/RawData.h>
#include "GtoDB.h"

using namespace Gto;


// *****************************************************************************
//
// Utility function, not exposed to Python.  Build a python representation of
// one item in a given property.  If property width is > 1, result will be
// a tuple.
//
PyObject *gtoDB_getItem(const Property* prop, long index)
{
    if(prop->width == 1)
    {
        switch(prop->type)
        {
        case Gto::String:
            return PyString_FromString(prop->stringData[index].c_str());
        case Gto::Int:
            return PyInt_FromLong(prop->int32Data[index]);
        case Gto::Float:
            return PyFloat_FromDouble(prop->floatData[index]); 
        case Gto::Double:
            return PyFloat_FromDouble(prop->doubleData[index]); 
        case Gto::Byte:
            return PyInt_FromLong(prop->uint8Data[index]); 
        case Gto::Short:
            return PyInt_FromLong(prop->uint16Data[index]); 
        default:
            PyErr_SetString(PyExc_IndexError, "Unhandled GTO data type");
            return NULL;
        }
    }
    else
    {
        PyObject *tuple = PyTuple_New(prop->width);
        for( size_t w = 0; w < prop->width; ++w )
        {
            const size_t di = ( index * prop->width ) + w;
            PyObject *item = NULL;
            switch(prop->type)
            {
            case Gto::String:
                item = PyString_FromString(prop->stringData[di].c_str());
                break;
            case Gto::Int:
                item = PyInt_FromLong(prop->int32Data[di]);
                break;
            case Gto::Float:
                item = PyFloat_FromDouble(prop->floatData[di]);
                break;
            case Gto::Double:
                item = PyFloat_FromDouble(prop->doubleData[di]);
                break;
            case Gto::Byte:
                item = PyInt_FromLong(prop->uint8Data[di]);
                break;
            case Gto::Short:
                item = PyInt_FromLong(prop->uint16Data[di]);
                break;
            default:
                PyErr_SetString(PyExc_IndexError, "Unhandled GTO data type");
                return NULL;
            }
            PyTuple_SetItem(tuple, w, item);
        }
        return tuple;
    }

    throw std::runtime_error("GTO Property unknown failure");
}


// *****************************************************************************
//
// Utility function, not exposed to Python.  Build a python representation of
// a slice of items in a given property.  Always returns a tuple.
//
PyObject *gtoDB_getSlice(const Property* prop, long start, long stop, 
                   long step, long size)
{
    PyObject *tuple = PyTuple_New(size);
    for(long dataIndex = start, tupleIndex = 0; 
        dataIndex < stop; 
        tupleIndex++, dataIndex += step)
    {
        PyObject *item = gtoDB_getItem(prop, dataIndex);
        PyTuple_SetItem(tuple, tupleIndex, item);
    }
    return tuple;
}


// *****************************************************************************
//
// Utility function, not exposed to Python.  Set one data item in the given
// property.  Number of items in the value must match the property's width.
//
void setItem(const Property* prop, long index, PyObject *value)
{
    if(PySequence_Check(value) && (! PyString_Check(value)))
    {
        const size_t valueSize = PySequence_Size(value);
        if(valueSize != prop->width)
        {
            throw std::invalid_argument("GTO Property width mismatch");
        }
        for( size_t w = 0; w < prop->width; ++w )
        {
            const size_t di = ( index * prop->width ) + w;
            switch(prop->type)
            {
            case Gto::String:
                prop->stringData[di] = PyString_AsString(PySequence_GetItem(value, w));
                break;
            case Gto::Int:
                prop->int32Data[di] = PyInt_AsLong(PySequence_GetItem(value, w));
                break;
            case Gto::Float:
                prop->floatData[di] = PyFloat_AsDouble(PySequence_GetItem(value, w));
                break;
            case Gto::Double:
                prop->doubleData[di] = PyFloat_AsDouble(PySequence_GetItem(value, w));
                break;
            case Gto::Byte:
                prop->uint8Data[di] = PyInt_AsLong(PySequence_GetItem(value, w));
                break;
            case Gto::Short:
                prop->uint16Data[di] = PyInt_AsLong(PySequence_GetItem(value, w));
                break;
            default:
                throw std::invalid_argument("Unhandled GTO data type");
            }
        }
    }
    else if (prop->width == 1)
    {
        switch(prop->type)
        {
        case Gto::String:
            prop->stringData[index] = PyString_AsString(value);
            break;
        case Gto::Int:
            prop->int32Data[index] = PyInt_AsLong(value);
            break;
        case Gto::Float:
            prop->floatData[index] = PyFloat_AsDouble(value);
            break;
        case Gto::Double:
            prop->doubleData[index] = PyFloat_AsDouble(value);
            break;
        case Gto::Byte:
            prop->uint8Data[index] = PyInt_AsLong(value);
            break;
        case Gto::Short:
            prop->uint16Data[index] = PyInt_AsLong(value);
            break;
        default:
            throw std::invalid_argument("Unhandled GTO data type");
        }
    }
    else
    {
        throw std::invalid_argument("GTO Property unknown failure");
    }
}


// *****************************************************************************
//
// Utility function, not exposed to Python.  Set a slice of values on the given
// property.  Number of items in the value must match the size of the slice.
//
void setSlice(const Property* prop, long start, long stop, 
                   long step, long size, PyObject *value)
{
    if(PySequence_Check(value) && (! PyString_Check(value)))
    {
        const size_t valueSize = PySequence_Size(value);
        if(valueSize != size)
        {
            throw std::invalid_argument("GTO Property value count mismatch");
        }
        for(size_t dataIndex = start, valueIndex = 0; 
                   dataIndex < stop; dataIndex += step, ++valueIndex )
        {
            setItem(prop, dataIndex, PySequence_GetItem(value, valueIndex));
        }
    }
    else if( size == 1 )
    {
        setItem(prop, start, value);
    }
}


// *****************************************************************************
//
// Utility function, not exposed to Python.  Choose a GTO data type from the 
// given Python value.
//
inline Gto::DataType gtoTypeFromPyObject(PyObject *o)
{
    if( PyString_Check(o) )
    {
        return Gto::String;
    }
    else if( PyInt_Check(o) || PyLong_Check(o) || PyBool_Check(o) )
    {
        return Gto::Int;
    }
    else if( PyFloat_Check(o) )
    {
        return Gto::Float;
    }
    else
    {
        return Gto::ErrorType;
    }
}


// *****************************************************************************
//
// Utility function, not exposed to Python.  Create a new GTO Property object,
// infering the type, size, and width from the given Python data.
//
Property* createPropertyFromPyObject(const std::string name, 
                                     const std::string interp, 
                                     PyObject *data)
{
    size_t size = 1;
    size_t width = 1;
    Gto::DataType type = Gto::ErrorType;
    if( PySequence_Check(data) && (! PyString_Check(data)) )
    {
        size = PySequence_Length(data);
        if( size == 0 )
        {
            throw std::invalid_argument("GTO cannot infer data type from empty sequence");
        }
        PyObject *firstItem = PySequence_GetItem(data, 0);
        if( PySequence_Check(firstItem) && (! PyString_Check(firstItem)) )
        {
            width = PySequence_Length(firstItem);
            type = gtoTypeFromPyObject(PySequence_GetItem(firstItem, 0));
        }
        else
        {
            type = gtoTypeFromPyObject(PySequence_GetItem(data, 0));
        }
    }
    else
    {
        type = gtoTypeFromPyObject(data);
    }
    if( type == Gto::ErrorType)
    {
        throw std::invalid_argument("GTO cannot store data of this type");
    }
    Property *prop = new Property( name, interp, type, size, width, true );
    setSlice(prop, 0, size, 1, size, data);
    return prop;
}




#include <limits.h>
#ifndef LLONG_MIN
# define LLONG_MIN	LONG_LONG_MIN
#endif
#ifndef LLONG_MAX
# define LLONG_MAX	LONG_LONG_MAX
#endif
#ifndef ULLONG_MAX
# define ULLONG_MAX	ULONG_LONG_MAX
#endif


SWIGINTERN int
SWIG_AsVal_int (PyObject * obj, int *val)
{
  long v;
  int res = SWIG_AsVal_long (obj, &v);
  if (SWIG_IsOK(res)) {
    if ((v < INT_MIN || v > INT_MAX)) {
      return SWIG_OverflowError;
    } else {
      if (val) *val = static_cast< int >(v);
    }
  }  
  return res;
}

SWIGINTERN Property *new_Property__SWIG_0(std::string const name,std::string const interp,Gto::DataType type,size_t size,size_t width=1){
        if( type >= NumberOfDataTypes )
        {
            throw std::invalid_argument("GTO Property invalid data type");
        }
        return new Property( name, interp, type, size, width, true ); 
    }
SWIGINTERN Property *new_Property__SWIG_2(std::string const name,Gto::DataType type,size_t size,size_t width=1){
        if( type >= NumberOfDataTypes )
        {
            throw std::invalid_argument("GTO Property invalid data type");
        }
        return new Property( name, type, size, width, true ); 
    }
SWIGINTERN Property *new_Property__SWIG_4(std::string const name,std::string const interp,PyObject *data){
        return createPropertyFromPyObject(name, interp, data);
    }
SWIGINTERN Property *new_Property__SWIG_5(std::string const name,PyObject *data){
        return createPropertyFromPyObject(name, "", data);
    }
SWIGINTERN char *Property___repr__(Property *self){
        static const char typeStr[][16] = { "int", "float", "double", "half",
                                            "string", "boolean", "short", 
                                            "byte" };
        static char tmp[256];
        snprintf(tmp, 256, "<GTO Property %s[%u][%u] '%s' at %p>", 
                 typeStr[self->type], 
                 self->width,
                 self->size, 
                 self->name.c_str(),
                 self
                );
        return tmp;
    }

SWIGINTERNINLINE PyObject * 
SWIG_FromCharPtr(const char *cptr)
{ 
  return SWIG_FromCharPtrAndSize(cptr, (cptr ? strlen(cptr) : 0));
}

SWIGINTERN long Property___cmp__(Property *self,PyObject *other){
        Property *otherProp = NULL;
        /*@SWIG:CAST_PYOBJECT_TO_C@*/                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(other, &vptr, SWIGTYPE_p_Property, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    otherProp = reinterpret_cast<Property*>(vptr);                              
}
/*@SWIG@*/;
        if(self == otherProp)
        {
            return 0;
        }
        else if(otherProp->name > self->name)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
SWIGINTERN long Property___len__(Property *self){
        return long(self->size);
    }
SWIGINTERN PyObject *Property___call__(Property *self){
        if( self->size == 1 )
        {
            return gtoDB_getItem(self, 0);
        }
        else
        {
            return gtoDB_getSlice(self, 0, self->size, 1, self->size);
        }
    }
SWIGINTERN PyObject *Property___getitem__(Property *self,PyObject *key){
        if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, self->size, 
                                              &start, &stop, &step, &slicelength);
            if(result != 0)
            {
                PyErr_SetString(PyExc_IndexError, "GTO Property invalid slice");
                return NULL;
            }
            return gtoDB_getSlice(self, start, stop, step, slicelength);
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = self->size - index - 2;
            }
            if(index < 0 || index >= self->size)
            {
                PyErr_SetString(PyExc_IndexError, "GTO Property index out of range");
                return NULL;
            }
            return gtoDB_getItem(self, index);
        }
        else if( (self->size > 1) && PyTuple_Check(key) && (PyTuple_Size(key) == 2) )
        {
            PyObject* xObj = PyTuple_GetItem(key, 0);
            PyObject* yObj = PyTuple_GetItem(key, 1);
            if(PyInt_Check(xObj) && PyInt_Check(yObj))
            {
                long xindex = PyInt_AS_LONG(xObj);
                long yindex = PyInt_AS_LONG(yObj);
                if(xindex < 0)
                {
                    xindex = self->size - xindex - 2;
                }
                if(xindex < 0 || xindex >= self->size)
                {
                    PyErr_SetString(PyExc_IndexError, "GTO Property index out of range");
                    return NULL;
                }

                if(yindex < 0)
                {
                    yindex = self->width - yindex - 2;
                }
                if(yindex < 0 || yindex >= self->width)
                {
                    PyErr_SetString(PyExc_IndexError, "GTO Property index out of range");
                    return NULL;
                }
                PyObject *tuple = gtoDB_getItem(self, xindex);
                assert(PyTuple_Check(tuple));
                assert(yindex < PyTuple_Size(tuple));

                PyObject *result = PyTuple_GetItem(tuple, yindex);
                Py_INCREF(result);
                return result;
            }
        }

        PyErr_SetString(PyExc_IndexError, "GTO Property Invalid index type");
        return NULL;
    }
SWIGINTERN void Property___setitem__(Property *self,PyObject *key,PyObject *value){
        if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = self->size - index - 2;
            }
            if(index < 0 || index >= self->size)
            {
                throw std::out_of_range("GTO Property index out of range");
            }
            setItem(self, index, value);
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, self->size, 
                                              &start, &stop, &step, &slicelength);
            if(result != 0)
            {
                throw std::out_of_range("GTO Property invalid slice");
            }
            setSlice(self, start, stop, step, slicelength, value);
        }
        else
        {
            throw std::invalid_argument("GTO Property Invalid index type");
        }
    }
SWIGINTERN bool Property___contains__(Property *self,PyObject *item){
        if(PyString_Check(item) && (self->type == Gto::String))
        {
            const char *itemstr = PyString_AS_STRING(item);
            for(size_t i = 0; i < self->size; ++i )
            {
                if( self->stringData[i] == itemstr )
                {
                    return true;
                }
            }
        }
        else if(PyInt_Check(item))
        {
            if( self->type == Gto::Int )
            {
                const int32 itemVal = PyInt_AS_LONG(item);
                for(size_t i = 0; i < self->size; ++i )
                {
                    if( self->int32Data[i] == itemVal ) return true;
                }
            }
            else if( self->type == Gto::Short )
            {
                const uint16 itemVal = PyInt_AS_LONG(item);
                for(size_t i = 0; i < self->size; ++i )
                {
                    if( self->uint16Data[i] == itemVal ) return true;
                }
            }
            else if( self->type == Gto::Byte )
            {
                const uint16 itemVal = PyInt_AS_LONG(item);
                for(size_t i = 0; i < self->size; ++i )
                {
                    if( self->uint8Data[i] == itemVal ) return true;
                }
            }
        }
        else if(PyFloat_Check(item))
        {
            if( self->type == Gto::Float )
            {
                const float itemVal = PyFloat_AS_DOUBLE(item);
                for(size_t i = 0; i < self->size; ++i )
                {
                    if( self->floatData[i] == itemVal ) return true;
                }
            }
            else if( self->type == Gto::Double )
            {
                const double itemVal = PyFloat_AS_DOUBLE(item);
                for(size_t i = 0; i < self->size; ++i )
                {
                    if( self->doubleData[i] == itemVal ) return true;
                }
            }
//             else if( self->type == Gto::Half )
//             {
//                 const half itemVal = PyFloat_AS_DOUBLE(item);
//                 for(size_t i = 0; i < self->size; ++i )
//                 {
//                     if( self->halfData[i] == itemVal ) return true;
//                 }
//             }
        }
        else if(PySequence_Check(item) && (! PyString_Check(item)))
        {
            if(PySequence_Length(item) != self->width) return false;
            if( self->type == Gto::String )
            {
                for(size_t i = 0; i < self->size; ++i )
                {
                    bool match = true;
                    for( size_t w = 0; w < self->width; ++w )
                    {
                        const size_t di = ( i * self->width ) + w;
                        const char *v = PyString_AsString(PySequence_GetItem(item, w));
                        if( self->stringData[di] != v )
                        {
                            match = false;
                            break;
                        }
                    }
                    if( match ) return true;
                }
            }
            else if( self->type == Gto::Int )
            {
                for(size_t i = 0; i < self->size; ++i )
                {
                    bool match = true;
                    for( size_t w = 0; w < self->width; ++w )
                    {
                        const size_t di = ( i * self->width ) + w;
                        const int32 v = PyInt_AsLong(PySequence_GetItem(item, w));
                        if( self->int32Data[di] != v )
                        {
                            match = false;
                            break;
                        }
                    }
                    if( match ) return true;
                }
            }
            else if( self->type == Gto::Short )
            {
                for(size_t i = 0; i < self->size; ++i )
                {
                    bool match = true;
                    for( size_t w = 0; w < self->width; ++w )
                    {
                        const size_t di = ( i * self->width ) + w;
                        const uint16 v = PyInt_AsLong(PySequence_GetItem(item, w));
                        if( self->uint16Data[di] != v )
                        {
                            match = false;
                            break;
                        }
                    }
                    if( match ) return true;
                }
            }
            else if( self->type == Gto::Byte )
            {
                for(size_t i = 0; i < self->size; ++i )
                {
                    bool match = true;
                    for( size_t w = 0; w < self->width; ++w )
                    {
                        const size_t di = ( i * self->width ) + w;
                        const uint8 v = PyInt_AsLong(PySequence_GetItem(item, w));
                        if( self->uint8Data[di] != v )
                        {
                            match = false;
                            break;
                        }
                    }
                    if( match ) return true;
                }
            }
            else if( self->type == Gto::Float )
            {
                for(size_t i = 0; i < self->size; ++i )
                {
                    bool match = true;
                    for( size_t w = 0; w < self->width; ++w )
                    {
                        const size_t di = ( i * self->width ) + w;
                        const float v = PyFloat_AsDouble(PySequence_GetItem(item, w));
                        if( self->floatData[di] != v )
                        {
                            match = false;
                            break;
                        }
                    }
                    if( match ) return true;
                }
            }
            else if( self->type == Gto::Double )
            {
                for(size_t i = 0; i < self->size; ++i )
                {
                    bool match = true;
                    for( size_t w = 0; w < self->width; ++w )
                    {
                        const size_t di = ( i * self->width ) + w;
                        const double v = PyFloat_AsDouble(PySequence_GetItem(item, w));
                        if( self->floatData[di] != v )
                        {
                            match = false;
                            break;
                        }
                    }
                    if( match ) return true;
                }
            }

        }

        return false;
    }

SWIGINTERN int
SWIG_AsVal_unsigned_SS_short (PyObject * obj, unsigned short *val)
{
  unsigned long v;
  int res = SWIG_AsVal_unsigned_SS_long (obj, &v);
  if (SWIG_IsOK(res)) {
    if ((v > USHRT_MAX)) {
      return SWIG_OverflowError;
    } else {
      if (val) *val = static_cast< unsigned short >(v);
    }
  }  
  return res;
}


SWIGINTERNINLINE PyObject *
SWIG_From_unsigned_SS_short  (unsigned short value)
{    
  return SWIG_From_unsigned_SS_long  (value);
}

SWIGINTERN Component *new_Component__SWIG_2(std::string const name,std::string const interp){
        return new Component(name, interp, 0);
    }
SWIGINTERN Component *new_Component__SWIG_3(std::string const name){
        return new Component(name, "", 0);
    }
SWIGINTERN char *Component___repr__(Component *self){
        static char tmp[256];
        snprintf(tmp, 256, "<GTO Component '%s' at %p>", 
                 self->name.c_str(),
                 self
                );
        return tmp;
    }
SWIGINTERN long Component___len__(Component *self){
        return long(self->properties.size());
    }
SWIGINTERN long Component___cmp__(Component *self,PyObject *other){
        Component *otherComp = NULL;
        /*@SWIG:CAST_PYOBJECT_TO_C@*/                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(other, &vptr, SWIGTYPE_p_Component, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    otherComp = reinterpret_cast<Component*>(vptr);                              
}
/*@SWIG@*/;
        if(self == otherComp)
        {
            return 0;
        }
        else if(otherComp->name > self->name)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
SWIGINTERN PyObject *Component___getitem__(Component *self,PyObject *key){
        if( PyString_Check(key) )
        {
            const char *keystr = PyString_AS_STRING(key);
            for(size_t i = 0; i < self->properties.size(); ++i )
            {
                Gto::Property *p = self->properties[i];
                if( p->name == keystr )
                {
                    return /*@SWIG:CAST_C_TO_PYOBJECT@*/
SWIG_NewPointerObj(SWIG_as_voidptr(p), SWIGTYPE_p_Property, 0 );
/*@SWIG@*/;
                }
            }
            throw std::out_of_range("GTO Component KeyError");
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = self->properties.size() - index - 2;
            }
            if(index < 0 || index >= self->properties.size())
            {
                throw std::out_of_range("GTO Component index out of range");
            }
            return /*@SWIG:CAST_C_TO_PYOBJECT@*/
SWIG_NewPointerObj(SWIG_as_voidptr(self->properties[index]), SWIGTYPE_p_Property, 0 );
/*@SWIG@*/;
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, 
                                              self->properties.size(), 
                                              &start, &stop, &step, &slicelength);
            if(result != 0)
            {
                PyErr_SetString(PyExc_IndexError, "GTO Component invalid slice");
                return NULL;
            }
            PyObject *tuple = PyTuple_New(slicelength);
            for(size_t dataIndex = start, tupleIndex = 0;
                dataIndex < stop; dataIndex += step, ++tupleIndex)
            {
                Property *p = self->properties[dataIndex];
                PyObject *po = /*@SWIG:CAST_C_TO_PYOBJECT@*/
SWIG_NewPointerObj(SWIG_as_voidptr(p), SWIGTYPE_p_Property, 0 );
/*@SWIG@*/;
                PyTuple_SetItem(tuple, tupleIndex, po);
            }
            return tuple;
        }
        throw std::invalid_argument("GTO Component Invalid index type");
    }
SWIGINTERN void Component___setitem__(Component *self,PyObject *key,PyObject *value){
        Property *prop = NULL;
        /*@SWIG:CAST_PYOBJECT_TO_C@*/                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(value, &vptr, SWIGTYPE_p_Property, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    prop = reinterpret_cast<Property*>(vptr);                              
}
/*@SWIG@*/;

        if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = self->properties.size() - index - 2;
            }
            if(index < 0 || index >= self->properties.size())
            {
                throw std::out_of_range("GTO Component index out of range");
            }
            for(size_t i = 0; i < self->properties.size(); ++i)
            {
                if(self->properties[i]->name == prop->name)
                {
                    throw std::invalid_argument("GTO Component duplicate property name");
                }
            }
            self->properties[index] = prop;
            Py_INCREF(value);
        }
        else
        {
            throw std::invalid_argument("GTO Component Invalid index type");
        }
    }
SWIGINTERN void Component___delitem__(Component *self,PyObject *key){
        if( PyString_Check(key) )
        {
            const char *keystr = PyString_AS_STRING(key);
            Properties::iterator piter = self->properties.begin();
            for(; piter != self->properties.end(); ++piter)
            {
                if( (*piter)->name == keystr )
                {
                    self->properties.erase(piter);
                    return;
                }
            }
            throw std::out_of_range("GTO Component KeyError");
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = self->properties.size() - index - 2;
            }
            if(index < 0 || index >= self->properties.size())
            {
                throw std::out_of_range("GTO Component index out of range");
            }
            self->properties.erase(self->properties.begin() + index);
            return;
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, 
                                              self->properties.size(), &start,
                                              &stop, &step, &slicelength);
            if(result != 0)
            {
                throw std::invalid_argument("GTO Component Invalid slice");
            }
            size_t numErased = 0;
            for( size_t i = start; i < stop; i += step )
            {
                self->properties.erase(self->properties.begin() + i - numErased);
                ++numErased;
            }
            return;
        }
        throw std::invalid_argument("GTO Component Invalid index type");
    }
SWIGINTERN bool Component___contains__(Component *self,PyObject *item){
        if( PyString_Check(item) )
        {
            const char *itemstr = PyString_AS_STRING(item);
            for(size_t i = 0; i < self->properties.size(); ++i )
            {
                if( self->properties[i]->name == itemstr )
                {
                    return true;
                }
            }
        }
        return false;
    }
SWIGINTERN PyObject *Component_keys(Component *self){
        PyObject *tuple = PyTuple_New(self->properties.size());
        for(size_t i = 0; i < self->properties.size(); ++i)
        {
            PyObject *name = PyString_FromString(self->properties[i]->name.c_str());
            PyTuple_SetItem(tuple, i, name);
        }
        return tuple;
    }
SWIGINTERN Properties Component_values(Component *self){
        return self->properties;
    }
SWIGINTERN PyObject *Component_items(Component *self){
        PyObject *tuple = PyTuple_New(self->properties.size());
        for(size_t i = 0; i < self->properties.size(); ++i)
        {
            PyObject *subtuple = PyTuple_New(2);
            PyObject *name = PyString_FromString(self->properties[i]->name.c_str());
            PyObject *pyProp = /*@SWIG:CAST_C_TO_PYOBJECT@*/
SWIG_NewPointerObj(SWIG_as_voidptr(self->properties[i]), SWIGTYPE_p_Property, 0 );
/*@SWIG@*/;
            PyTuple_SetItem(subtuple, 0, name);
            PyTuple_SetItem(subtuple, 1, pyProp);
            PyTuple_SetItem(tuple, i, subtuple);
        }
        return tuple;
    }
SWIGINTERN void Component_append(Component *self,PyObject *item){
        Property *prop = NULL;
        /*@SWIG:CAST_PYOBJECT_TO_C@*/                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(item, &vptr, SWIGTYPE_p_Property, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    prop = reinterpret_cast<Property*>(vptr);                              
}
/*@SWIG@*/;

        for(size_t i = 0; i < self->properties.size(); ++i)
        {
            if( prop->name == self->properties[i]->name )
            {
                throw std::invalid_argument("GTO Component duplicate property name");
            }
        }
        Py_INCREF(item);
        self->properties.push_back(prop);
    }
SWIGINTERN void Component_extend(Component *self,PyObject *sequence){
        if(PySequence_Check(sequence) && (! PyString_Check(sequence)))
        {
            for(size_t i = 0; i < PySequence_Length(sequence); ++i )
            {
                PyObject *pyProp = PySequence_GetItem(sequence, i);
                Property *prop = NULL;
                /*@SWIG:CAST_PYOBJECT_TO_C@*/                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(pyProp, &vptr, SWIGTYPE_p_Property, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    prop = reinterpret_cast<Property*>(vptr);                              
}
/*@SWIG@*/;

                for(size_t i = 0; i < self->properties.size(); ++i)
                {
                    if( prop->name == self->properties[i]->name )
                    {
                        throw std::invalid_argument("GTO Component duplicate property name");
                    }
                }
                Py_INCREF(pyProp);
                self->properties.push_back(prop);
            }
        }
        else
        {
            throw std::invalid_argument("GTO Component invalid sequence");
        }
    }

SWIGINTERN int
SWIG_AsVal_unsigned_SS_int (PyObject * obj, unsigned int *val)
{
  unsigned long v;
  int res = SWIG_AsVal_unsigned_SS_long (obj, &v);
  if (SWIG_IsOK(res)) {
    if ((v > UINT_MAX)) {
      return SWIG_OverflowError;
    } else {
      if (val) *val = static_cast< unsigned int >(v);
    }
  }  
  return res;
}


SWIGINTERNINLINE PyObject *
SWIG_From_unsigned_SS_int  (unsigned int value)
{    
  return SWIG_From_unsigned_SS_long  (value);
}

SWIGINTERN char *Object___repr__(Object *self){
        static char tmp[256];
        snprintf(tmp, 256, "<GTO Object '%s' protocol '%s' v%d at %p>", 
                 self->name.c_str(),
                 self->protocol.c_str(),
                 self->protocolVersion,
                 self
                );
        return tmp;
    }
SWIGINTERN long Object___len__(Object *self){
        return long(self->components.size());
    }
SWIGINTERN long Object___cmp__(Object *self,PyObject *other){
        Object *otherObj = NULL;
        /*@SWIG:CAST_PYOBJECT_TO_C@*/                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(other, &vptr, SWIGTYPE_p_Object, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    otherObj = reinterpret_cast<Object*>(vptr);                              
}
/*@SWIG@*/;
        if(self == otherObj)
        {
            return 0;
        }
        else if(otherObj->name > self->name)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
SWIGINTERN PyObject *Object___getitem__(Object *self,PyObject *key){
        if( PyString_Check (key) )
        {
            const char *keystr = PyString_AS_STRING(key);
            for(size_t i = 0; i < self->components.size(); ++i )
            {
                Gto::Component *c = self->components[i];
                if( c->name == keystr )
                {
                    return /*@SWIG:CAST_C_TO_PYOBJECT@*/
SWIG_NewPointerObj(SWIG_as_voidptr(c), SWIGTYPE_p_Component, 0 );
/*@SWIG@*/;
                }
            }
            throw std::out_of_range("GTO Object KeyError");
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = self->components.size() - index - 2;
            }
            if(index < 0 || index >= self->components.size())
            {
                throw std::out_of_range("GTO Object index out of range");
            }
            return /*@SWIG:CAST_C_TO_PYOBJECT@*/
SWIG_NewPointerObj(SWIG_as_voidptr(self->components[index]), SWIGTYPE_p_Component, 0 );
/*@SWIG@*/;
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, 
                                              self->components.size(), 
                                              &start, &stop, &step, &slicelength);

            std::cerr << "start: " << start << std::endl;
            std::cerr << "stop: " << stop << std::endl;
            std::cerr << "step: " << step << std::endl;
            std::cerr << "slicelen: " << slicelength << std::endl;

            if(result != 0)
            {
                PyErr_SetString(PyExc_IndexError, "GTO Object invalid slice");
                return NULL;
            }
            PyObject *tuple = PyTuple_New(slicelength);
            for(size_t dataIndex = start, tupleIndex = 0;
                dataIndex < stop; dataIndex += step, ++tupleIndex)
            {
                Component *c = self->components[dataIndex];
                PyObject *co = /*@SWIG:CAST_C_TO_PYOBJECT@*/
SWIG_NewPointerObj(SWIG_as_voidptr(c), SWIGTYPE_p_Component, 0 );
/*@SWIG@*/;
                PyTuple_SetItem(tuple, tupleIndex, co);
            }
            return tuple;
        }
        throw std::invalid_argument("GTO Object Invalid index type");
    }
SWIGINTERN void Object___setitem__(Object *self,PyObject *key,PyObject *value){
        Component *comp = NULL;
        /*@SWIG:CAST_PYOBJECT_TO_C@*/                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(value, &vptr, SWIGTYPE_p_Component, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    comp = reinterpret_cast<Component*>(vptr);                              
}
/*@SWIG@*/;

        if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = self->components.size() - index - 2;
            }
            if(index < 0 || index >= self->components.size())
            {
                throw std::out_of_range("GTO Object index out of range");
            }
            for(size_t i = 0; i < self->components.size(); ++i)
            {
                if(self->components[i]->name == comp->name)
                {
                    throw std::invalid_argument("GTO Object duplicate component name");
                }
            }
            self->components[index] = comp;
            Py_INCREF(value);
        }
        else
        {
            throw std::invalid_argument("GTO Object Invalid index type");
        }
    }
SWIGINTERN void Object___delitem__(Object *self,PyObject *key){
        if( PyString_Check(key) )
        {
            const char *keystr = PyString_AS_STRING(key);
            Components::iterator citer = self->components.begin();
            for(; citer != self->components.end(); ++citer)
            {
                if( (*citer)->name == keystr )
                {
                    self->components.erase(citer);
                    return;
                }
            }
            throw std::out_of_range("GTO Object KeyError");
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = self->components.size() - index - 2;
            }
            if(index < 0 || index >= self->components.size())
            {
                throw std::out_of_range("GTO Object index out of range");
            }
            self->components.erase(self->components.begin() + index);
            return;
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, 
                                              self->components.size(), &start,
                                              &stop, &step, &slicelength);
            if(result != 0)
            {
                throw std::invalid_argument("GTO Object Invalid slice");
            }
            size_t numErased = 0;
            for( size_t i = start; i < stop; i += step )
            {
                self->components.erase(self->components.begin() + i - numErased);
                ++numErased;
            }
            return;
        }
        throw std::invalid_argument("GTO Object Invalid index type");
    }
SWIGINTERN bool Object___contains__(Object *self,PyObject *item){
        if( PyString_Check(item) )
        {
            const char *itemstr = PyString_AS_STRING(item);
            for(size_t i = 0; i < self->components.size(); ++i )
            {
                if( self->components[i]->name == itemstr )
                {
                    return true;
                }
            }
        }
        return false;
    }
SWIGINTERN PyObject *Object_keys(Object *self){
        PyObject *tuple = PyTuple_New(self->components.size());
        for(size_t i = 0; i < self->components.size(); ++i)
        {
            PyObject *name = PyString_FromString(self->components[i]->name.c_str());
            PyTuple_SetItem(tuple, i, name);
        }
        return tuple;
    }
SWIGINTERN Components Object_values(Object *self){
        return self->components;
    }
SWIGINTERN void Object_append(Object *self,PyObject *item){
        Component *comp = NULL;
        /*@SWIG:CAST_PYOBJECT_TO_C@*/                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(item, &vptr, SWIGTYPE_p_Component, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    comp = reinterpret_cast<Component*>(vptr);                              
}
/*@SWIG@*/;

        for(size_t i = 0; i < self->components.size(); ++i)
        {
            if( comp->name == self->components[i]->name )
            {
                throw std::invalid_argument("GTO Object duplicate component name");
            }
        }
        Py_INCREF(item);
        self->components.push_back(comp);
    }
SWIGINTERN void Object_extend(Object *self,PyObject *sequence){
        if(PySequence_Check(sequence) && (! PyString_Check(sequence)))
        {
            for(size_t i = 0; i < PySequence_Length(sequence); ++i )
            {
                PyObject *pyComp = PySequence_GetItem(sequence, i);
                Component *comp = NULL;
                /*@SWIG:CAST_PYOBJECT_TO_C@*/                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(pyComp, &vptr, SWIGTYPE_p_Component, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    comp = reinterpret_cast<Component*>(vptr);                              
}
/*@SWIG@*/;

                for(size_t i = 0; i < self->components.size(); ++i)
                {
                    if( comp->name == self->components[i]->name )
                    {
                        throw std::invalid_argument("GTO Object duplicate component name");
                    }
                }
                Py_INCREF(pyComp);
                self->components.push_back(comp);
            }
        }
        else
        {
            throw std::invalid_argument("GTO Object invalid sequence");
        }
    }
SWIGINTERN long GtoDB___len__(GtoDB *self){
        RawDataBase *db = self->m_reader.dataBase();
        return long(db->objects.size());
    }
SWIGINTERN PyObject *GtoDB___getitem__(GtoDB *self,PyObject *key){
        RawDataBase *db = self->m_reader.dataBase();

        if( PyString_Check(key) )
        {
            const char *keystr = PyString_AS_STRING(key);
            for(size_t i = 0; i < db->objects.size(); ++i )
            {
                Gto::Object *o = db->objects[i];
                if( o->name == keystr )
                {
                    return /*@SWIG:CAST_C_TO_PYOBJECT@*/
SWIG_NewPointerObj(SWIG_as_voidptr(o), SWIGTYPE_p_Object, 0 );
/*@SWIG@*/;
                }
            }
            throw std::out_of_range("GTO DB KeyError");
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = db->objects.size() - index - 2;
            }
            if(index < 0 || index >= db->objects.size())
            {
                throw std::out_of_range("GTO DB index out of range");
            }
            return /*@SWIG:CAST_C_TO_PYOBJECT@*/
SWIG_NewPointerObj(SWIG_as_voidptr(db->objects[index]), SWIGTYPE_p_Object, 0 );
/*@SWIG@*/;
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, db->objects.size(), 
                                              &start, &stop, &step, &slicelength);
            if(result != 0)
            {
                PyErr_SetString(PyExc_IndexError, "GTO DB invalid slice");
                return NULL;
            }
            PyObject *tuple = PyTuple_New(slicelength);
            for(size_t dataIndex = start, tupleIndex = 0;
                dataIndex < stop; dataIndex += step, ++tupleIndex)
            {
                Object *o = db->objects[dataIndex];
                PyObject *po = /*@SWIG:CAST_C_TO_PYOBJECT@*/
SWIG_NewPointerObj(SWIG_as_voidptr(o), SWIGTYPE_p_Object, 0 );
/*@SWIG@*/;
                PyTuple_SetItem(tuple, tupleIndex, po);
            }
            return tuple;
        }
        throw std::invalid_argument("GTO DB Invalid index type");
    }
SWIGINTERN void GtoDB___setitem__(GtoDB *self,PyObject *key,PyObject *value){
        RawDataBase *db = self->m_reader.dataBase();

        Object *obj = NULL;
        /*@SWIG:CAST_PYOBJECT_TO_C@*/                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(value, &vptr, SWIGTYPE_p_Object, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    obj = reinterpret_cast<Object*>(vptr);                              
}
/*@SWIG@*/;

        if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = db->objects.size() - index - 2;
            }
            if(index < 0 || index >= db->objects.size())
            {
                throw std::out_of_range("GTO DB index out of range");
            }
            for(size_t i = 0; i < db->objects.size(); ++i)
            {
                if(db->objects[i]->name == obj->name)
                {
                    throw std::invalid_argument("GTO DB duplicate object name");
                }
            }
            db->objects[index] = obj;
            Py_INCREF(value);
        }
        else
        {
            throw std::invalid_argument("GTO DB Invalid index type");
        }
    }
SWIGINTERN void GtoDB___delitem__(GtoDB *self,PyObject *key){
        RawDataBase *db = self->m_reader.dataBase();

        if( PyString_Check(key) )
        {
            const char *keystr = PyString_AS_STRING(key);
            Objects::iterator oiter = db->objects.begin();
            for(; oiter != db->objects.end(); ++oiter)
            {
                if( (*oiter)->name == keystr )
                {
                    db->objects.erase(oiter);
                    return;
                }
            }
            throw std::out_of_range("GTO DB KeyError");
        }
        else if( PyInt_Check(key) )
        {
            long index = PyInt_AS_LONG(key);
            if(index < 0)
            {
                index = db->objects.size() - index - 2;
            }
            if(index < 0 || index >= db->objects.size())
            {
                throw std::out_of_range("GTO DB index out of range");
            }
            db->objects.erase(db->objects.begin() + index);
            return;
        }
        else if( PySlice_Check(key) )
        {
            Py_ssize_t start, stop, step, slicelength;
            int result = PySlice_GetIndicesEx((PySliceObject*)key, 
                                              db->objects.size(), &start,
                                              &stop, &step, &slicelength);
            if(result != 0)
            {
                throw std::invalid_argument("GTO DB Invalid slice");
            }
            size_t numErased = 0;
            for( size_t i = start; i < stop; i += step )
            {
                db->objects.erase(db->objects.begin() + i - numErased);
                ++numErased;
            }
            return;
        }
        throw std::invalid_argument("GTO DB Invalid index type");
    }
SWIGINTERN bool GtoDB___contains__(GtoDB *self,PyObject *item){
        RawDataBase *db = self->m_reader.dataBase();

        if( PyString_Check(item) )
        {
            const char *itemstr = PyString_AS_STRING(item);
            for(size_t i = 0; i < db->objects.size(); ++i )
            {
                if( db->objects[i]->name == itemstr )
                {
                    return true;
                }
            }
        }
        return false;
    }
SWIGINTERN PyObject *GtoDB_keys(GtoDB *self){
        RawDataBase *db = self->m_reader.dataBase();

        PyObject *tuple = PyTuple_New(db->objects.size());
        for(size_t i = 0; i < db->objects.size(); ++i)
        {
            PyObject *name = PyString_FromString(db->objects[i]->name.c_str());
            PyTuple_SetItem(tuple, i, name);
        }
        return tuple;
    }
SWIGINTERN Objects GtoDB_values(GtoDB *self){
        RawDataBase *db = self->m_reader.dataBase();
        return db->objects;
    }
SWIGINTERN PyObject *GtoDB_items(GtoDB *self){
        RawDataBase *db = self->m_reader.dataBase();

        PyObject *tuple = PyTuple_New(db->objects.size());
        for(size_t i = 0; i < db->objects.size(); ++i)
        {
            PyObject *subtuple = PyTuple_New(2);
            PyObject *name = PyString_FromString(db->objects[i]->name.c_str());
            PyObject *po = /*@SWIG:CAST_C_TO_PYOBJECT@*/
SWIG_NewPointerObj(SWIG_as_voidptr(db->objects[i]), SWIGTYPE_p_Object, 0 );
/*@SWIG@*/;
            PyTuple_SetItem(subtuple, 0, name);
            PyTuple_SetItem(subtuple, 1, po);
            PyTuple_SetItem(tuple, i, subtuple);
        }
        return tuple;
    }
SWIGINTERN void GtoDB_append(GtoDB *self,PyObject *item){
        RawDataBase *db = self->m_reader.dataBase();

        Object *obj = NULL;
        /*@SWIG:CAST_PYOBJECT_TO_C@*/                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(item, &vptr, SWIGTYPE_p_Object, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    obj = reinterpret_cast<Object*>(vptr);                              
}
/*@SWIG@*/;

        for(size_t i = 0; i < db->objects.size(); ++i)
        {
            if( obj->name == db->objects[i]->name )
            {
                throw std::invalid_argument("GTO DB duplicate object name");
            }
        }
        Py_INCREF(item);
        db->objects.push_back(obj);
    }
SWIGINTERN void GtoDB_extend(GtoDB *self,PyObject *sequence){
        if(PySequence_Check(sequence) && (! PyString_Check(sequence)))
        {
            RawDataBase *db = self->m_reader.dataBase();
            for(size_t i = 0; i < PySequence_Length(sequence); ++i )
            {
                PyObject *pyObj = PySequence_GetItem(sequence, i);
                Object *obj = NULL;
                /*@SWIG:CAST_PYOBJECT_TO_C@*/                              
{                                                                        
    void *vptr = NULL;                                                   
    int status = SWIG_ConvertPtr(pyObj, &vptr, SWIGTYPE_p_Object, 0 );
    if( ! SWIG_IsOK(status) )                                            
    {                                                                    
        throw std::invalid_argument("Invalid type");                     
    }                                                                    
    obj = reinterpret_cast<Object*>(vptr);                              
}
/*@SWIG@*/;

                for(size_t i = 0; i < db->objects.size(); ++i)
                {
                    if( obj->name == db->objects[i]->name )
                    {
                        throw std::invalid_argument("GtoDB duplicate object name");
                    }
                }
                Py_INCREF(pyObj);
                db->objects.push_back(obj);
            }
        }
        else
        {
            throw std::invalid_argument("GtoDB invalid sequence");
        }
    }
SWIGINTERN void GtoDB_clear(GtoDB *self){
        RawDataBase *db = self->m_reader.dataBase();
        db->objects.clear();
        db->strings.clear();
    }
#ifdef __cplusplus
extern "C" {
#endif
SWIGINTERN PyObject *_wrap_delete_PySwigIterator(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:delete_PySwigIterator",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, SWIG_POINTER_DISOWN |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "delete_PySwigIterator" "', argument " "1"" of type '" "swig::PySwigIterator *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); delete arg1; resultobj = SWIG_Py_Void(); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_value(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:PySwigIterator_value",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator_value" "', argument " "1"" of type '" "swig::PySwigIterator const *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); try {
    result = (PyObject *)((swig::PySwigIterator const *)arg1)->value();}  catch(swig::stop_iteration &_e) { { (void)_e;
      SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void()); SWIG_fail; }  }  resultobj = result; return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_incr__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) {
  PyObject *resultobj = 0; swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; size_t arg2 ;
  swig::PySwigIterator *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; size_t val2 ; int ecode2 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:PySwigIterator_incr",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator_incr" "', argument " "1"" of type '" "swig::PySwigIterator *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); ecode2 = SWIG_AsVal_size_t(obj1, &val2); if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "PySwigIterator_incr" "', argument " "2"" of type '" "size_t""'"); }
    arg2 = static_cast< size_t >(val2); try { result = (swig::PySwigIterator *)(arg1)->incr(arg2);} 
  catch(swig::stop_iteration &_e) { { (void)_e; SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void()); SWIG_fail; }  } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_incr__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) {
  PyObject *resultobj = 0; swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; swig::PySwigIterator *result = 0 ;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:PySwigIterator_incr",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator_incr" "', argument " "1"" of type '" "swig::PySwigIterator *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); try { result = (swig::PySwigIterator *)(arg1)->incr();} 
  catch(swig::stop_iteration &_e) { { (void)_e; SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void()); SWIG_fail; }  } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_incr(PyObject *self, PyObject *args) { int argc; PyObject *argv[3]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 2); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 1) { int _v; void *vptr = 0;
    int res = SWIG_ConvertPtr(argv[0], &vptr, SWIGTYPE_p_swig__PySwigIterator, 0); _v = SWIG_CheckState(res); if (_v) {
      return _wrap_PySwigIterator_incr__SWIG_1(self, args);}  }  if (argc == 2) { int _v; void *vptr = 0;
    int res = SWIG_ConvertPtr(argv[0], &vptr, SWIGTYPE_p_swig__PySwigIterator, 0); _v = SWIG_CheckState(res); if (_v) { {
        int res = SWIG_AsVal_size_t(argv[1], NULL); _v = SWIG_CheckState(res); }  if (_v) {
        return _wrap_PySwigIterator_incr__SWIG_0(self, args);}  }  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'PySwigIterator_incr'.\n  Possible C/C++ prototypes are:\n    incr(size_t)\n    incr()\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_decr__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) {
  PyObject *resultobj = 0; swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; size_t arg2 ;
  swig::PySwigIterator *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; size_t val2 ; int ecode2 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:PySwigIterator_decr",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator_decr" "', argument " "1"" of type '" "swig::PySwigIterator *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); ecode2 = SWIG_AsVal_size_t(obj1, &val2); if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "PySwigIterator_decr" "', argument " "2"" of type '" "size_t""'"); }
    arg2 = static_cast< size_t >(val2); try { result = (swig::PySwigIterator *)(arg1)->decr(arg2);} 
  catch(swig::stop_iteration &_e) { { (void)_e; SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void()); SWIG_fail; }  } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_decr__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) {
  PyObject *resultobj = 0; swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; swig::PySwigIterator *result = 0 ;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:PySwigIterator_decr",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator_decr" "', argument " "1"" of type '" "swig::PySwigIterator *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); try { result = (swig::PySwigIterator *)(arg1)->decr();} 
  catch(swig::stop_iteration &_e) { { (void)_e; SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void()); SWIG_fail; }  } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_decr(PyObject *self, PyObject *args) { int argc; PyObject *argv[3]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 2); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 1) { int _v; void *vptr = 0;
    int res = SWIG_ConvertPtr(argv[0], &vptr, SWIGTYPE_p_swig__PySwigIterator, 0); _v = SWIG_CheckState(res); if (_v) {
      return _wrap_PySwigIterator_decr__SWIG_1(self, args);}  }  if (argc == 2) { int _v; void *vptr = 0;
    int res = SWIG_ConvertPtr(argv[0], &vptr, SWIGTYPE_p_swig__PySwigIterator, 0); _v = SWIG_CheckState(res); if (_v) { {
        int res = SWIG_AsVal_size_t(argv[1], NULL); _v = SWIG_CheckState(res); }  if (_v) {
        return _wrap_PySwigIterator_decr__SWIG_0(self, args);}  }  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'PySwigIterator_decr'.\n  Possible C/C++ prototypes are:\n    decr(size_t)\n    decr()\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_distance(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; swig::PySwigIterator *arg2 = 0 ; ptrdiff_t result; void *argp1 = 0 ;
  int res1 = 0 ; void *argp2 = 0 ; int res2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:PySwigIterator_distance",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator_distance" "', argument " "1"" of type '" "swig::PySwigIterator const *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1);
  res2 = SWIG_ConvertPtr(obj1, &argp2, SWIGTYPE_p_swig__PySwigIterator,  0  | 0); if (!SWIG_IsOK(res2)) {
    SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "PySwigIterator_distance" "', argument " "2"" of type '" "swig::PySwigIterator const &""'");  }
   if (!argp2) {
    SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "PySwigIterator_distance" "', argument " "2"" of type '" "swig::PySwigIterator const &""'"); }
   arg2 = reinterpret_cast< swig::PySwigIterator * >(argp2); try {
    result = ((swig::PySwigIterator const *)arg1)->distance((swig::PySwigIterator const &)*arg2);} 
  catch(std::invalid_argument &_e) {
    SWIG_Python_Raise(SWIG_NewPointerObj((new std::invalid_argument(static_cast< const std::invalid_argument& >(_e))),SWIGTYPE_p_std__invalid_argument,SWIG_POINTER_OWN), "std::invalid_argument", SWIGTYPE_p_std__invalid_argument); SWIG_fail; }
   resultobj = SWIG_From_ptrdiff_t(static_cast< ptrdiff_t >(result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_equal(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; swig::PySwigIterator *arg2 = 0 ; bool result; void *argp1 = 0 ;
  int res1 = 0 ; void *argp2 = 0 ; int res2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:PySwigIterator_equal",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator_equal" "', argument " "1"" of type '" "swig::PySwigIterator const *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1);
  res2 = SWIG_ConvertPtr(obj1, &argp2, SWIGTYPE_p_swig__PySwigIterator,  0  | 0); if (!SWIG_IsOK(res2)) {
    SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "PySwigIterator_equal" "', argument " "2"" of type '" "swig::PySwigIterator const &""'");  }
   if (!argp2) {
    SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "PySwigIterator_equal" "', argument " "2"" of type '" "swig::PySwigIterator const &""'"); }
   arg2 = reinterpret_cast< swig::PySwigIterator * >(argp2); try {
    result = (bool)((swig::PySwigIterator const *)arg1)->equal((swig::PySwigIterator const &)*arg2);} 
  catch(std::invalid_argument &_e) {
    SWIG_Python_Raise(SWIG_NewPointerObj((new std::invalid_argument(static_cast< const std::invalid_argument& >(_e))),SWIGTYPE_p_std__invalid_argument,SWIG_POINTER_OWN), "std::invalid_argument", SWIGTYPE_p_std__invalid_argument); SWIG_fail; }
   resultobj = SWIG_From_bool(static_cast< bool >(result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_copy(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; swig::PySwigIterator *result = 0 ; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:PySwigIterator_copy",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator_copy" "', argument " "1"" of type '" "swig::PySwigIterator const *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1);
  result = (swig::PySwigIterator *)((swig::PySwigIterator const *)arg1)->copy();
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_swig__PySwigIterator, SWIG_POINTER_OWN |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_next(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:PySwigIterator_next",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator_next" "', argument " "1"" of type '" "swig::PySwigIterator *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); try { result = (PyObject *)(arg1)->next();} 
  catch(swig::stop_iteration &_e) { { (void)_e; SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void()); SWIG_fail; }  } 
  resultobj = result; return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_previous(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:PySwigIterator_previous",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator_previous" "', argument " "1"" of type '" "swig::PySwigIterator *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); try { result = (PyObject *)(arg1)->previous();} 
  catch(swig::stop_iteration &_e) { { (void)_e; SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void()); SWIG_fail; }  } 
  resultobj = result; return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator_advance(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; ptrdiff_t arg2 ; swig::PySwigIterator *result = 0 ;
  void *argp1 = 0 ; int res1 = 0 ; ptrdiff_t val2 ; int ecode2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:PySwigIterator_advance",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator_advance" "', argument " "1"" of type '" "swig::PySwigIterator *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); ecode2 = SWIG_AsVal_ptrdiff_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "PySwigIterator_advance" "', argument " "2"" of type '" "ptrdiff_t""'"); }
    arg2 = static_cast< ptrdiff_t >(val2); try { result = (swig::PySwigIterator *)(arg1)->advance(arg2);} 
  catch(swig::stop_iteration &_e) { { (void)_e; SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void()); SWIG_fail; }  } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator___eq__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; swig::PySwigIterator *arg2 = 0 ; bool result; void *argp1 = 0 ;
  int res1 = 0 ; void *argp2 = 0 ; int res2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:PySwigIterator___eq__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator___eq__" "', argument " "1"" of type '" "swig::PySwigIterator const *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1);
  res2 = SWIG_ConvertPtr(obj1, &argp2, SWIGTYPE_p_swig__PySwigIterator,  0  | 0); if (!SWIG_IsOK(res2)) {
    SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "PySwigIterator___eq__" "', argument " "2"" of type '" "swig::PySwigIterator const &""'");  }
   if (!argp2) {
    SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "PySwigIterator___eq__" "', argument " "2"" of type '" "swig::PySwigIterator const &""'"); }
   arg2 = reinterpret_cast< swig::PySwigIterator * >(argp2);
  result = (bool)((swig::PySwigIterator const *)arg1)->operator ==((swig::PySwigIterator const &)*arg2);
  resultobj = SWIG_From_bool(static_cast< bool >(result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator___ne__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; swig::PySwigIterator *arg2 = 0 ; bool result; void *argp1 = 0 ;
  int res1 = 0 ; void *argp2 = 0 ; int res2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:PySwigIterator___ne__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator___ne__" "', argument " "1"" of type '" "swig::PySwigIterator const *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1);
  res2 = SWIG_ConvertPtr(obj1, &argp2, SWIGTYPE_p_swig__PySwigIterator,  0  | 0); if (!SWIG_IsOK(res2)) {
    SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "PySwigIterator___ne__" "', argument " "2"" of type '" "swig::PySwigIterator const &""'");  }
   if (!argp2) {
    SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "PySwigIterator___ne__" "', argument " "2"" of type '" "swig::PySwigIterator const &""'"); }
   arg2 = reinterpret_cast< swig::PySwigIterator * >(argp2);
  result = (bool)((swig::PySwigIterator const *)arg1)->operator !=((swig::PySwigIterator const &)*arg2);
  resultobj = SWIG_From_bool(static_cast< bool >(result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator___iadd__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; ptrdiff_t arg2 ; swig::PySwigIterator *result = 0 ;
  void *argp1 = 0 ; int res1 = 0 ; ptrdiff_t val2 ; int ecode2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:PySwigIterator___iadd__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, SWIG_POINTER_DISOWN |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator___iadd__" "', argument " "1"" of type '" "swig::PySwigIterator *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); ecode2 = SWIG_AsVal_ptrdiff_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "PySwigIterator___iadd__" "', argument " "2"" of type '" "ptrdiff_t""'"); }
    arg2 = static_cast< ptrdiff_t >(val2); try { { swig::PySwigIterator &_result_ref = (arg1)->operator +=(arg2);
      result = (swig::PySwigIterator *) &_result_ref; } }  catch(swig::stop_iteration &_e) { { (void)_e;
      SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void()); SWIG_fail; }  } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_swig__PySwigIterator, SWIG_POINTER_OWN |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator___isub__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; ptrdiff_t arg2 ; swig::PySwigIterator *result = 0 ;
  void *argp1 = 0 ; int res1 = 0 ; ptrdiff_t val2 ; int ecode2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:PySwigIterator___isub__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, SWIG_POINTER_DISOWN |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator___isub__" "', argument " "1"" of type '" "swig::PySwigIterator *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); ecode2 = SWIG_AsVal_ptrdiff_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "PySwigIterator___isub__" "', argument " "2"" of type '" "ptrdiff_t""'"); }
    arg2 = static_cast< ptrdiff_t >(val2); try { { swig::PySwigIterator &_result_ref = (arg1)->operator -=(arg2);
      result = (swig::PySwigIterator *) &_result_ref; } }  catch(swig::stop_iteration &_e) { { (void)_e;
      SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void()); SWIG_fail; }  } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_swig__PySwigIterator, SWIG_POINTER_OWN |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator___add__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; ptrdiff_t arg2 ; swig::PySwigIterator *result = 0 ;
  void *argp1 = 0 ; int res1 = 0 ; ptrdiff_t val2 ; int ecode2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:PySwigIterator___add__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator___add__" "', argument " "1"" of type '" "swig::PySwigIterator const *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); ecode2 = SWIG_AsVal_ptrdiff_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "PySwigIterator___add__" "', argument " "2"" of type '" "ptrdiff_t""'"); }
    arg2 = static_cast< ptrdiff_t >(val2); try {
    result = (swig::PySwigIterator *)((swig::PySwigIterator const *)arg1)->operator +(arg2);}  catch(swig::stop_iteration &_e) {
    { (void)_e; SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void()); SWIG_fail; }  } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_swig__PySwigIterator, SWIG_POINTER_OWN |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator___sub____SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) {
  PyObject *resultobj = 0; swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; ptrdiff_t arg2 ;
  swig::PySwigIterator *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; ptrdiff_t val2 ; int ecode2 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:PySwigIterator___sub__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator___sub__" "', argument " "1"" of type '" "swig::PySwigIterator const *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1); ecode2 = SWIG_AsVal_ptrdiff_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "PySwigIterator___sub__" "', argument " "2"" of type '" "ptrdiff_t""'"); }
    arg2 = static_cast< ptrdiff_t >(val2); try {
    result = (swig::PySwigIterator *)((swig::PySwigIterator const *)arg1)->operator -(arg2);}  catch(swig::stop_iteration &_e) {
    { (void)_e; SWIG_SetErrorObj(PyExc_StopIteration, SWIG_Py_Void()); SWIG_fail; }  } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_swig__PySwigIterator, SWIG_POINTER_OWN |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator___sub____SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) {
  PyObject *resultobj = 0; swig::PySwigIterator *arg1 = (swig::PySwigIterator *) 0 ; swig::PySwigIterator *arg2 = 0 ;
  ptrdiff_t result; void *argp1 = 0 ; int res1 = 0 ; void *argp2 = 0 ; int res2 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:PySwigIterator___sub__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_swig__PySwigIterator, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "PySwigIterator___sub__" "', argument " "1"" of type '" "swig::PySwigIterator const *""'");  }
   arg1 = reinterpret_cast< swig::PySwigIterator * >(argp1);
  res2 = SWIG_ConvertPtr(obj1, &argp2, SWIGTYPE_p_swig__PySwigIterator,  0  | 0); if (!SWIG_IsOK(res2)) {
    SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "PySwigIterator___sub__" "', argument " "2"" of type '" "swig::PySwigIterator const &""'");  }
   if (!argp2) {
    SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "PySwigIterator___sub__" "', argument " "2"" of type '" "swig::PySwigIterator const &""'"); }
   arg2 = reinterpret_cast< swig::PySwigIterator * >(argp2);
  result = ((swig::PySwigIterator const *)arg1)->operator -((swig::PySwigIterator const &)*arg2);
  resultobj = SWIG_From_ptrdiff_t(static_cast< ptrdiff_t >(result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_PySwigIterator___sub__(PyObject *self, PyObject *args) { int argc; PyObject *argv[3]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 2); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 2) { int _v; void *vptr = 0;
    int res = SWIG_ConvertPtr(argv[0], &vptr, SWIGTYPE_p_swig__PySwigIterator, 0); _v = SWIG_CheckState(res); if (_v) {
      int res = SWIG_ConvertPtr(argv[1], 0, SWIGTYPE_p_swig__PySwigIterator, 0); _v = SWIG_CheckState(res); if (_v) {
        return _wrap_PySwigIterator___sub____SWIG_1(self, args);}  }  }  if (argc == 2) { int _v; void *vptr = 0;
    int res = SWIG_ConvertPtr(argv[0], &vptr, SWIGTYPE_p_swig__PySwigIterator, 0); _v = SWIG_CheckState(res); if (_v) { {
        int res = SWIG_AsVal_ptrdiff_t(argv[1], NULL); _v = SWIG_CheckState(res); }  if (_v) {
        return _wrap_PySwigIterator___sub____SWIG_0(self, args);}  }  }  fail: Py_INCREF(Py_NotImplemented);
  return Py_NotImplemented; }
SWIGINTERN PyObject *PySwigIterator_swigregister(PyObject *SWIGUNUSEDPARM(self), PyObject *args) {
  PyObject *obj;
  if (!PyArg_ParseTuple(args,(char*)"O|swigregister", &obj)) return NULL;
  SWIG_TypeNewClientData(SWIGTYPE_p_swig__PySwigIterator, SWIG_NewClientData(obj));
  return SWIG_Py_Void();
}

SWIGINTERN PyObject *_wrap_Strings_iterator(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; PyObject **arg2 = (PyObject **) 0 ;
  swig::PySwigIterator *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; arg2 = &obj0;
  if (!PyArg_ParseTuple(args,(char *)"O:Strings_iterator",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_iterator" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try {
      result = (swig::PySwigIterator *)std_vector_Sl_std_string_Sg__iterator(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_swig__PySwigIterator, SWIG_POINTER_OWN |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings___nonzero__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; bool result; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:Strings___nonzero__",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings___nonzero__" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try {
      result = (bool)std_vector_Sl_std_string_Sg____nonzero__((std::vector<std::string > const *)arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_bool(static_cast< bool >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings___len__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::size_type result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Strings___len__",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings___len__" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try {
      result = std_vector_Sl_std_string_Sg____len__((std::vector<std::string > const *)arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_size_t(static_cast< size_t >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_pop(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::value_type result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:Strings_pop",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_pop" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try { try { result = std_vector_Sl_std_string_Sg__pop(arg1);}
       catch(std::out_of_range &_e) { SWIG_exception_fail(SWIG_IndexError, (&_e)->what()); }  }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_From_std_string(static_cast< std::string >(result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings___getslice__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::difference_type arg2 ;
  std::vector<std::string >::difference_type arg3 ; std::vector<std::string,std::allocator<std::string > > *result = 0 ;
  void *argp1 = 0 ; int res1 = 0 ; ptrdiff_t val2 ; int ecode2 = 0 ; ptrdiff_t val3 ; int ecode3 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:Strings___getslice__",&obj0,&obj1,&obj2)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings___getslice__" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); ecode2 = SWIG_AsVal_ptrdiff_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "Strings___getslice__" "', argument " "2"" of type '" "std::vector<std::string >::difference_type""'"); }
    arg2 = static_cast< std::vector<std::string >::difference_type >(val2); ecode3 = SWIG_AsVal_ptrdiff_t(obj2, &val3);
  if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "Strings___getslice__" "', argument " "3"" of type '" "std::vector<std::string >::difference_type""'"); }
    arg3 = static_cast< std::vector<std::string >::difference_type >(val3); { try { try {
        result = (std::vector<std::string,std::allocator<std::string > > *)std_vector_Sl_std_string_Sg____getslice__(arg1,arg2,arg3);}
       catch(std::out_of_range &_e) { SWIG_exception_fail(SWIG_IndexError, (&_e)->what()); }  }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, SWIG_POINTER_OWN |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings___setslice__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::difference_type arg2 ;
  std::vector<std::string >::difference_type arg3 ; std::vector<std::string,std::allocator<std::string > > *arg4 = 0 ;
  void *argp1 = 0 ; int res1 = 0 ; ptrdiff_t val2 ; int ecode2 = 0 ; ptrdiff_t val3 ; int ecode3 = 0 ; int res4 = SWIG_OLDOBJ ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ; PyObject * obj3 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOOO:Strings___setslice__",&obj0,&obj1,&obj2,&obj3)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings___setslice__" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); ecode2 = SWIG_AsVal_ptrdiff_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "Strings___setslice__" "', argument " "2"" of type '" "std::vector<std::string >::difference_type""'"); }
    arg2 = static_cast< std::vector<std::string >::difference_type >(val2); ecode3 = SWIG_AsVal_ptrdiff_t(obj2, &val3);
  if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "Strings___setslice__" "', argument " "3"" of type '" "std::vector<std::string >::difference_type""'"); }
    arg3 = static_cast< std::vector<std::string >::difference_type >(val3); {
    std::vector<std::string,std::allocator<std::string > > *ptr = (std::vector<std::string,std::allocator<std::string > > *)0;
    res4 = swig::asptr(obj3, &ptr); if (!SWIG_IsOK(res4)) {
      SWIG_exception_fail(SWIG_ArgError(res4), "in method '" "Strings___setslice__" "', argument " "4"" of type '" "std::vector<std::string,std::allocator<std::string > > const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Strings___setslice__" "', argument " "4"" of type '" "std::vector<std::string,std::allocator<std::string > > const &""'"); }
     arg4 = ptr; }  { try { try {
        std_vector_Sl_std_string_Sg____setslice__(arg1,arg2,arg3,(std::vector<std::string,std::allocator<std::string > > const &)*arg4);}
       catch(std::out_of_range &_e) { SWIG_exception_fail(SWIG_IndexError, (&_e)->what()); }  catch(std::invalid_argument &_e) {
        SWIG_exception_fail(SWIG_ValueError, (&_e)->what()); }  }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res4)) delete arg4;
  return resultobj; fail: if (SWIG_IsNewObj(res4)) delete arg4; return NULL; }
SWIGINTERN PyObject *_wrap_Strings___delslice__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::difference_type arg2 ;
  std::vector<std::string >::difference_type arg3 ; void *argp1 = 0 ; int res1 = 0 ; ptrdiff_t val2 ; int ecode2 = 0 ;
  ptrdiff_t val3 ; int ecode3 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:Strings___delslice__",&obj0,&obj1,&obj2)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings___delslice__" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); ecode2 = SWIG_AsVal_ptrdiff_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "Strings___delslice__" "', argument " "2"" of type '" "std::vector<std::string >::difference_type""'"); }
    arg2 = static_cast< std::vector<std::string >::difference_type >(val2); ecode3 = SWIG_AsVal_ptrdiff_t(obj2, &val3);
  if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "Strings___delslice__" "', argument " "3"" of type '" "std::vector<std::string >::difference_type""'"); }
    arg3 = static_cast< std::vector<std::string >::difference_type >(val3); { try { try {
        std_vector_Sl_std_string_Sg____delslice__(arg1,arg2,arg3);}  catch(std::out_of_range &_e) {
        SWIG_exception_fail(SWIG_IndexError, (&_e)->what()); }  }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings___delitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::difference_type arg2 ;
  void *argp1 = 0 ; int res1 = 0 ; ptrdiff_t val2 ; int ecode2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Strings___delitem__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings___delitem__" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); ecode2 = SWIG_AsVal_ptrdiff_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "Strings___delitem__" "', argument " "2"" of type '" "std::vector<std::string >::difference_type""'"); }
    arg2 = static_cast< std::vector<std::string >::difference_type >(val2); { try { try {
        std_vector_Sl_std_string_Sg____delitem__(arg1,arg2);}  catch(std::out_of_range &_e) {
        SWIG_exception_fail(SWIG_IndexError, (&_e)->what()); }  }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings___getitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::difference_type arg2 ;
  std::vector<std::string >::value_type *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; ptrdiff_t val2 ; int ecode2 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Strings___getitem__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings___getitem__" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); ecode2 = SWIG_AsVal_ptrdiff_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "Strings___getitem__" "', argument " "2"" of type '" "std::vector<std::string >::difference_type""'"); }
    arg2 = static_cast< std::vector<std::string >::difference_type >(val2); { try { try { {
          std::vector<std::string >::value_type const &_result_ref = std_vector_Sl_std_string_Sg____getitem__((std::vector<std::string > const *)arg1,arg2);
          result = (std::vector<std::string >::value_type *) &_result_ref; } }  catch(std::out_of_range &_e) {
        SWIG_exception_fail(SWIG_IndexError, (&_e)->what()); }  }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_From_std_string(static_cast< std::string >(*result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings___setitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::difference_type arg2 ;
  std::vector<std::string >::value_type *arg3 = 0 ; void *argp1 = 0 ; int res1 = 0 ; ptrdiff_t val2 ; int ecode2 = 0 ;
  int res3 = SWIG_OLDOBJ ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:Strings___setitem__",&obj0,&obj1,&obj2)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings___setitem__" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); ecode2 = SWIG_AsVal_ptrdiff_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "Strings___setitem__" "', argument " "2"" of type '" "std::vector<std::string >::difference_type""'"); }
    arg2 = static_cast< std::vector<std::string >::difference_type >(val2); { std::string *ptr = (std::string *)0;
    res3 = SWIG_AsPtr_std_string(obj2, &ptr); if (!SWIG_IsOK(res3)) {
      SWIG_exception_fail(SWIG_ArgError(res3), "in method '" "Strings___setitem__" "', argument " "3"" of type '" "std::vector<std::string >::value_type const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Strings___setitem__" "', argument " "3"" of type '" "std::vector<std::string >::value_type const &""'"); }
     arg3 = ptr; }  { try { try { std_vector_Sl_std_string_Sg____setitem__(arg1,arg2,(std::string const &)*arg3);} 
      catch(std::out_of_range &_e) { SWIG_exception_fail(SWIG_IndexError, (&_e)->what()); }  }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res3)) delete arg3;
  return resultobj; fail: if (SWIG_IsNewObj(res3)) delete arg3; return NULL; }
SWIGINTERN PyObject *_wrap_Strings_append(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::value_type *arg2 = 0 ;
  void *argp1 = 0 ; int res1 = 0 ; int res2 = SWIG_OLDOBJ ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Strings_append",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_append" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { std::string *ptr = (std::string *)0;
    res2 = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res2)) {
      SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "Strings_append" "', argument " "2"" of type '" "std::vector<std::string >::value_type const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Strings_append" "', argument " "2"" of type '" "std::vector<std::string >::value_type const &""'"); }
     arg2 = ptr; }  { try { std_vector_Sl_std_string_Sg__append(arg1,(std::string const &)*arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res2)) delete arg2;
  return resultobj; fail: if (SWIG_IsNewObj(res2)) delete arg2; return NULL; }
SWIGINTERN PyObject *_wrap_new_Strings__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *result = 0 ; if (!PyArg_ParseTuple(args,(char *)":new_Strings")) SWIG_fail; { try {
      result = (std::vector<std::string > *)new std::vector<std::string >(); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, SWIG_POINTER_NEW |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_new_Strings__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = 0 ; std::vector<std::string > *result = 0 ; int res1 = SWIG_OLDOBJ ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:new_Strings",&obj0)) SWIG_fail; {
    std::vector<std::string,std::allocator<std::string > > *ptr = (std::vector<std::string,std::allocator<std::string > > *)0;
    res1 = swig::asptr(obj0, &ptr); if (!SWIG_IsOK(res1)) {
      SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "new_Strings" "', argument " "1"" of type '" "std::vector<std::string > const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "new_Strings" "', argument " "1"" of type '" "std::vector<std::string > const &""'"); }
     arg1 = ptr; }  { try {
      result = (std::vector<std::string > *)new std::vector<std::string >((std::vector<std::string > const &)*arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, SWIG_POINTER_NEW |  0 );
  if (SWIG_IsNewObj(res1)) delete arg1; return resultobj; fail: if (SWIG_IsNewObj(res1)) delete arg1; return NULL; }
SWIGINTERN PyObject *_wrap_Strings_empty(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; bool result; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:Strings_empty",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_empty" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try {
      result = (bool)((std::vector<std::string > const *)arg1)->empty(); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_bool(static_cast< bool >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_size(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::size_type result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:Strings_size",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_size" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try {
      result = ((std::vector<std::string > const *)arg1)->size(); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_size_t(static_cast< size_t >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_clear(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Strings_clear",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_clear" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try { (arg1)->clear(); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_swap(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string > *arg2 = 0 ; void *argp1 = 0 ;
  int res1 = 0 ; void *argp2 = 0 ; int res2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Strings_swap",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_swap" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1);
  res2 = SWIG_ConvertPtr(obj1, &argp2, SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t,  0 );
  if (!SWIG_IsOK(res2)) {
    SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "Strings_swap" "', argument " "2"" of type '" "std::vector<std::string > &""'");  }
   if (!argp2) {
    SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Strings_swap" "', argument " "2"" of type '" "std::vector<std::string > &""'"); }
   arg2 = reinterpret_cast< std::vector<std::string > * >(argp2); { try { (arg1)->swap(*arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_get_allocator(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; SwigValueWrapper<std::allocator<std::string > > result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Strings_get_allocator",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_get_allocator" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try {
      result = ((std::vector<std::string > const *)arg1)->get_allocator(); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj((new std::vector<std::string >::allocator_type(static_cast< const std::vector<std::string >::allocator_type& >(result))), SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t__allocator_type, SWIG_POINTER_OWN |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_begin__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::iterator result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:Strings_begin",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_begin" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try { result = (arg1)->begin(); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(swig::make_output_iterator(static_cast< const std::vector<std::string >::iterator & >(result)),
  swig::PySwigIterator::descriptor(),SWIG_POINTER_OWN); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_begin__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::const_iterator result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:Strings_begin",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_begin" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try {
      result = ((std::vector<std::string > const *)arg1)->begin(); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(swig::make_output_iterator(static_cast< const std::vector<std::string >::const_iterator & >(result)),
  swig::PySwigIterator::descriptor(),SWIG_POINTER_OWN); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_begin(PyObject *self, PyObject *args) { int argc; PyObject *argv[2]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 1); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 1) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { return _wrap_Strings_begin__SWIG_0(self, args);}  }  if (argc == 1) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { return _wrap_Strings_begin__SWIG_1(self, args);}  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'Strings_begin'.\n  Possible C/C++ prototypes are:\n    begin()\n    begin()\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_Strings_end__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::iterator result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:Strings_end",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_end" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try { result = (arg1)->end(); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(swig::make_output_iterator(static_cast< const std::vector<std::string >::iterator & >(result)),
  swig::PySwigIterator::descriptor(),SWIG_POINTER_OWN); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_end__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::const_iterator result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:Strings_end",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_end" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try {
      result = ((std::vector<std::string > const *)arg1)->end(); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(swig::make_output_iterator(static_cast< const std::vector<std::string >::const_iterator & >(result)),
  swig::PySwigIterator::descriptor(),SWIG_POINTER_OWN); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_end(PyObject *self, PyObject *args) { int argc; PyObject *argv[2]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 1); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 1) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { return _wrap_Strings_end__SWIG_0(self, args);}  }  if (argc == 1) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { return _wrap_Strings_end__SWIG_1(self, args);}  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'Strings_end'.\n  Possible C/C++ prototypes are:\n    end()\n    end()\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_Strings_rbegin__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::reverse_iterator result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Strings_rbegin",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_rbegin" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try { result = (arg1)->rbegin(); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(swig::make_output_iterator(static_cast< const std::vector<std::string >::reverse_iterator & >(result)),
  swig::PySwigIterator::descriptor(),SWIG_POINTER_OWN); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_rbegin__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::const_reverse_iterator result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Strings_rbegin",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_rbegin" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try {
      result = ((std::vector<std::string > const *)arg1)->rbegin(); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(swig::make_output_iterator(static_cast< const std::vector<std::string >::const_reverse_iterator & >(result)),
  swig::PySwigIterator::descriptor(),SWIG_POINTER_OWN); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_rbegin(PyObject *self, PyObject *args) { int argc; PyObject *argv[2]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 1); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 1) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { return _wrap_Strings_rbegin__SWIG_0(self, args);}  }  if (argc == 1) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { return _wrap_Strings_rbegin__SWIG_1(self, args);}  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'Strings_rbegin'.\n  Possible C/C++ prototypes are:\n    rbegin()\n    rbegin()\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_Strings_rend__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::reverse_iterator result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:Strings_rend",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_rend" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try { result = (arg1)->rend(); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(swig::make_output_iterator(static_cast< const std::vector<std::string >::reverse_iterator & >(result)),
  swig::PySwigIterator::descriptor(),SWIG_POINTER_OWN); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_rend__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::const_reverse_iterator result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:Strings_rend",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_rend" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try {
      result = ((std::vector<std::string > const *)arg1)->rend(); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(swig::make_output_iterator(static_cast< const std::vector<std::string >::const_reverse_iterator & >(result)),
  swig::PySwigIterator::descriptor(),SWIG_POINTER_OWN); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_rend(PyObject *self, PyObject *args) { int argc; PyObject *argv[2]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 1); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 1) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { return _wrap_Strings_rend__SWIG_0(self, args);}  }  if (argc == 1) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { return _wrap_Strings_rend__SWIG_1(self, args);}  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'Strings_rend'.\n  Possible C/C++ prototypes are:\n    rend()\n    rend()\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_new_Strings__SWIG_2(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string >::size_type arg1 ; std::vector<std::string > *result = 0 ; size_t val1 ; int ecode1 = 0 ;
  PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:new_Strings",&obj0)) SWIG_fail;
  ecode1 = SWIG_AsVal_size_t(obj0, &val1); if (!SWIG_IsOK(ecode1)) {
    SWIG_exception_fail(SWIG_ArgError(ecode1), "in method '" "new_Strings" "', argument " "1"" of type '" "std::vector<std::string >::size_type""'"); }
    arg1 = static_cast< std::vector<std::string >::size_type >(val1); { try {
      result = (std::vector<std::string > *)new std::vector<std::string >(arg1); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, SWIG_POINTER_NEW |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_pop_back(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Strings_pop_back",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_pop_back" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try { (arg1)->pop_back(); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_resize__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::size_type arg2 ;
  void *argp1 = 0 ; int res1 = 0 ; size_t val2 ; int ecode2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Strings_resize",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_resize" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); ecode2 = SWIG_AsVal_size_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "Strings_resize" "', argument " "2"" of type '" "std::vector<std::string >::size_type""'"); }
    arg2 = static_cast< std::vector<std::string >::size_type >(val2); { try { (arg1)->resize(arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_erase__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::iterator arg2 ;
  std::vector<std::string >::iterator result; void *argp1 = 0 ; int res1 = 0 ; swig::PySwigIterator *iter2 = 0 ; int res2 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:Strings_erase",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_erase" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1);
  res2 = SWIG_ConvertPtr(obj1, SWIG_as_voidptrptr(&iter2), swig::PySwigIterator::descriptor(), 0);
  if (!SWIG_IsOK(res2) || !iter2) {
    SWIG_exception_fail(SWIG_ArgError(SWIG_TypeError), "in method '" "Strings_erase" "', argument " "2"" of type '" "std::vector<std::string >::iterator""'"); }
   else {
    swig::PySwigIterator_T<std::vector<std::string >::iterator > *iter_t = dynamic_cast<swig::PySwigIterator_T<std::vector<std::string >::iterator > *>(iter2);
    if (iter_t) { arg2 = iter_t->get_current(); }  else {
      SWIG_exception_fail(SWIG_ArgError(SWIG_TypeError), "in method '" "Strings_erase" "', argument " "2"" of type '" "std::vector<std::string >::iterator""'"); }
     }  { try { result = (arg1)->erase(arg2); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(swig::make_output_iterator(static_cast< const std::vector<std::string >::iterator & >(result)),
  swig::PySwigIterator::descriptor(),SWIG_POINTER_OWN); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_erase__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::iterator arg2 ;
  std::vector<std::string >::iterator arg3 ; std::vector<std::string >::iterator result; void *argp1 = 0 ; int res1 = 0 ;
  swig::PySwigIterator *iter2 = 0 ; int res2 ; swig::PySwigIterator *iter3 = 0 ; int res3 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:Strings_erase",&obj0,&obj1,&obj2)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_erase" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1);
  res2 = SWIG_ConvertPtr(obj1, SWIG_as_voidptrptr(&iter2), swig::PySwigIterator::descriptor(), 0);
  if (!SWIG_IsOK(res2) || !iter2) {
    SWIG_exception_fail(SWIG_ArgError(SWIG_TypeError), "in method '" "Strings_erase" "', argument " "2"" of type '" "std::vector<std::string >::iterator""'"); }
   else {
    swig::PySwigIterator_T<std::vector<std::string >::iterator > *iter_t = dynamic_cast<swig::PySwigIterator_T<std::vector<std::string >::iterator > *>(iter2);
    if (iter_t) { arg2 = iter_t->get_current(); }  else {
      SWIG_exception_fail(SWIG_ArgError(SWIG_TypeError), "in method '" "Strings_erase" "', argument " "2"" of type '" "std::vector<std::string >::iterator""'"); }
     }  res3 = SWIG_ConvertPtr(obj2, SWIG_as_voidptrptr(&iter3), swig::PySwigIterator::descriptor(), 0);
  if (!SWIG_IsOK(res3) || !iter3) {
    SWIG_exception_fail(SWIG_ArgError(SWIG_TypeError), "in method '" "Strings_erase" "', argument " "3"" of type '" "std::vector<std::string >::iterator""'"); }
   else {
    swig::PySwigIterator_T<std::vector<std::string >::iterator > *iter_t = dynamic_cast<swig::PySwigIterator_T<std::vector<std::string >::iterator > *>(iter3);
    if (iter_t) { arg3 = iter_t->get_current(); }  else {
      SWIG_exception_fail(SWIG_ArgError(SWIG_TypeError), "in method '" "Strings_erase" "', argument " "3"" of type '" "std::vector<std::string >::iterator""'"); }
     }  { try { result = (arg1)->erase(arg2,arg3); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(swig::make_output_iterator(static_cast< const std::vector<std::string >::iterator & >(result)),
  swig::PySwigIterator::descriptor(),SWIG_POINTER_OWN); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_erase(PyObject *self, PyObject *args) { int argc; PyObject *argv[4]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 3); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 2) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { swig::PySwigIterator *iter = 0;
      int res = SWIG_ConvertPtr(argv[1], SWIG_as_voidptrptr(&iter), swig::PySwigIterator::descriptor(), 0);
      _v = (SWIG_IsOK(res) && iter && (dynamic_cast<swig::PySwigIterator_T<std::vector<std::string >::iterator > *>(iter) != 0));
      if (_v) { return _wrap_Strings_erase__SWIG_0(self, args);}  }  }  if (argc == 3) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { swig::PySwigIterator *iter = 0;
      int res = SWIG_ConvertPtr(argv[1], SWIG_as_voidptrptr(&iter), swig::PySwigIterator::descriptor(), 0);
      _v = (SWIG_IsOK(res) && iter && (dynamic_cast<swig::PySwigIterator_T<std::vector<std::string >::iterator > *>(iter) != 0));
      if (_v) { swig::PySwigIterator *iter = 0;
        int res = SWIG_ConvertPtr(argv[2], SWIG_as_voidptrptr(&iter), swig::PySwigIterator::descriptor(), 0);
        _v = (SWIG_IsOK(res) && iter && (dynamic_cast<swig::PySwigIterator_T<std::vector<std::string >::iterator > *>(iter) != 0));
        if (_v) { return _wrap_Strings_erase__SWIG_1(self, args);}  }  }  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'Strings_erase'.\n  Possible C/C++ prototypes are:\n    erase(std::vector<std::string >::iterator)\n    erase(std::vector<std::string >::iterator,std::vector<std::string >::iterator)\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_new_Strings__SWIG_3(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string >::size_type arg1 ; std::vector<std::string >::value_type *arg2 = 0 ;
  std::vector<std::string > *result = 0 ; size_t val1 ; int ecode1 = 0 ; int res2 = SWIG_OLDOBJ ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:new_Strings",&obj0,&obj1)) SWIG_fail;
  ecode1 = SWIG_AsVal_size_t(obj0, &val1); if (!SWIG_IsOK(ecode1)) {
    SWIG_exception_fail(SWIG_ArgError(ecode1), "in method '" "new_Strings" "', argument " "1"" of type '" "std::vector<std::string >::size_type""'"); }
    arg1 = static_cast< std::vector<std::string >::size_type >(val1); { std::string *ptr = (std::string *)0;
    res2 = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res2)) {
      SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "new_Strings" "', argument " "2"" of type '" "std::vector<std::string >::value_type const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "new_Strings" "', argument " "2"" of type '" "std::vector<std::string >::value_type const &""'"); }
     arg2 = ptr; }  { try {
      result = (std::vector<std::string > *)new std::vector<std::string >(arg1,(std::vector<std::string >::value_type const &)*arg2); }
        catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, SWIG_POINTER_NEW |  0 );
  if (SWIG_IsNewObj(res2)) delete arg2; return resultobj; fail: if (SWIG_IsNewObj(res2)) delete arg2; return NULL; }
SWIGINTERN PyObject *_wrap_new_Strings(PyObject *self, PyObject *args) { int argc; PyObject *argv[3]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 2); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 0) { return _wrap_new_Strings__SWIG_0(self, args);}  if (argc == 1) {
    int _v; { int res = SWIG_AsVal_size_t(argv[0], NULL); _v = SWIG_CheckState(res); }  if (_v) {
      return _wrap_new_Strings__SWIG_2(self, args);}  }  if (argc == 1) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { return _wrap_new_Strings__SWIG_1(self, args);}  }  if (argc == 2) { int _v; {
      int res = SWIG_AsVal_size_t(argv[0], NULL); _v = SWIG_CheckState(res); }  if (_v) {
      int res = SWIG_AsPtr_std_string(argv[1], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
        return _wrap_new_Strings__SWIG_3(self, args);}  }  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'new_Strings'.\n  Possible C/C++ prototypes are:\n    std::vector<(std::string)>()\n    std::vector<(std::string)>(std::vector<std::string > const &)\n    std::vector<(std::string)>(std::vector<std::string >::size_type)\n    std::vector<(std::string)>(std::vector<std::string >::size_type,std::vector<std::string >::value_type const &)\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_Strings_push_back(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::value_type *arg2 = 0 ;
  void *argp1 = 0 ; int res1 = 0 ; int res2 = SWIG_OLDOBJ ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Strings_push_back",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_push_back" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { std::string *ptr = (std::string *)0;
    res2 = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res2)) {
      SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "Strings_push_back" "', argument " "2"" of type '" "std::vector<std::string >::value_type const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Strings_push_back" "', argument " "2"" of type '" "std::vector<std::string >::value_type const &""'"); }
     arg2 = ptr; }  { try { (arg1)->push_back((std::vector<std::string >::value_type const &)*arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res2)) delete arg2;
  return resultobj; fail: if (SWIG_IsNewObj(res2)) delete arg2; return NULL; }
SWIGINTERN PyObject *_wrap_Strings_front(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::value_type *result = 0 ;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:Strings_front",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_front" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try { {
        std::vector<std::string >::value_type const &_result_ref = ((std::vector<std::string > const *)arg1)->front();
        result = (std::vector<std::string >::value_type *) &_result_ref; }  }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_From_std_string(static_cast< std::string >(*result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_back(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::value_type *result = 0 ;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; if (!PyArg_ParseTuple(args,(char *)"O:Strings_back",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_back" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try { {
        std::vector<std::string >::value_type const &_result_ref = ((std::vector<std::string > const *)arg1)->back();
        result = (std::vector<std::string >::value_type *) &_result_ref; }  }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_From_std_string(static_cast< std::string >(*result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_assign(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::size_type arg2 ;
  std::vector<std::string >::value_type *arg3 = 0 ; void *argp1 = 0 ; int res1 = 0 ; size_t val2 ; int ecode2 = 0 ;
  int res3 = SWIG_OLDOBJ ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:Strings_assign",&obj0,&obj1,&obj2)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_assign" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); ecode2 = SWIG_AsVal_size_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "Strings_assign" "', argument " "2"" of type '" "std::vector<std::string >::size_type""'"); }
    arg2 = static_cast< std::vector<std::string >::size_type >(val2); { std::string *ptr = (std::string *)0;
    res3 = SWIG_AsPtr_std_string(obj2, &ptr); if (!SWIG_IsOK(res3)) {
      SWIG_exception_fail(SWIG_ArgError(res3), "in method '" "Strings_assign" "', argument " "3"" of type '" "std::vector<std::string >::value_type const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Strings_assign" "', argument " "3"" of type '" "std::vector<std::string >::value_type const &""'"); }
     arg3 = ptr; }  { try { (arg1)->assign(arg2,(std::vector<std::string >::value_type const &)*arg3); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res3)) delete arg3;
  return resultobj; fail: if (SWIG_IsNewObj(res3)) delete arg3; return NULL; }
SWIGINTERN PyObject *_wrap_Strings_resize__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::size_type arg2 ;
  std::vector<std::string >::value_type *arg3 = 0 ; void *argp1 = 0 ; int res1 = 0 ; size_t val2 ; int ecode2 = 0 ;
  int res3 = SWIG_OLDOBJ ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:Strings_resize",&obj0,&obj1,&obj2)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_resize" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); ecode2 = SWIG_AsVal_size_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "Strings_resize" "', argument " "2"" of type '" "std::vector<std::string >::size_type""'"); }
    arg2 = static_cast< std::vector<std::string >::size_type >(val2); { std::string *ptr = (std::string *)0;
    res3 = SWIG_AsPtr_std_string(obj2, &ptr); if (!SWIG_IsOK(res3)) {
      SWIG_exception_fail(SWIG_ArgError(res3), "in method '" "Strings_resize" "', argument " "3"" of type '" "std::vector<std::string >::value_type const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Strings_resize" "', argument " "3"" of type '" "std::vector<std::string >::value_type const &""'"); }
     arg3 = ptr; }  { try { (arg1)->resize(arg2,(std::vector<std::string >::value_type const &)*arg3); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res3)) delete arg3;
  return resultobj; fail: if (SWIG_IsNewObj(res3)) delete arg3; return NULL; }
SWIGINTERN PyObject *_wrap_Strings_resize(PyObject *self, PyObject *args) { int argc; PyObject *argv[4]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 3); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 2) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { { int res = SWIG_AsVal_size_t(argv[1], NULL); _v = SWIG_CheckState(res); }  if (_v) {
        return _wrap_Strings_resize__SWIG_0(self, args);}  }  }  if (argc == 3) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { { int res = SWIG_AsVal_size_t(argv[1], NULL); _v = SWIG_CheckState(res); }  if (_v) {
        int res = SWIG_AsPtr_std_string(argv[2], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
          return _wrap_Strings_resize__SWIG_1(self, args);}  }  }  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'Strings_resize'.\n  Possible C/C++ prototypes are:\n    resize(std::vector<std::string >::size_type)\n    resize(std::vector<std::string >::size_type,std::vector<std::string >::value_type const &)\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_Strings_insert__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::iterator arg2 ;
  std::vector<std::string >::value_type *arg3 = 0 ; std::vector<std::string >::iterator result; void *argp1 = 0 ; int res1 = 0 ;
  swig::PySwigIterator *iter2 = 0 ; int res2 ; int res3 = SWIG_OLDOBJ ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  PyObject * obj2 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OOO:Strings_insert",&obj0,&obj1,&obj2)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_insert" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1);
  res2 = SWIG_ConvertPtr(obj1, SWIG_as_voidptrptr(&iter2), swig::PySwigIterator::descriptor(), 0);
  if (!SWIG_IsOK(res2) || !iter2) {
    SWIG_exception_fail(SWIG_ArgError(SWIG_TypeError), "in method '" "Strings_insert" "', argument " "2"" of type '" "std::vector<std::string >::iterator""'"); }
   else {
    swig::PySwigIterator_T<std::vector<std::string >::iterator > *iter_t = dynamic_cast<swig::PySwigIterator_T<std::vector<std::string >::iterator > *>(iter2);
    if (iter_t) { arg2 = iter_t->get_current(); }  else {
      SWIG_exception_fail(SWIG_ArgError(SWIG_TypeError), "in method '" "Strings_insert" "', argument " "2"" of type '" "std::vector<std::string >::iterator""'"); }
     }  { std::string *ptr = (std::string *)0; res3 = SWIG_AsPtr_std_string(obj2, &ptr); if (!SWIG_IsOK(res3)) {
      SWIG_exception_fail(SWIG_ArgError(res3), "in method '" "Strings_insert" "', argument " "3"" of type '" "std::vector<std::string >::value_type const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Strings_insert" "', argument " "3"" of type '" "std::vector<std::string >::value_type const &""'"); }
     arg3 = ptr; }  { try { result = (arg1)->insert(arg2,(std::vector<std::string >::value_type const &)*arg3); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(swig::make_output_iterator(static_cast< const std::vector<std::string >::iterator & >(result)),
  swig::PySwigIterator::descriptor(),SWIG_POINTER_OWN); if (SWIG_IsNewObj(res3)) delete arg3; return resultobj; fail:
  if (SWIG_IsNewObj(res3)) delete arg3; return NULL; }
SWIGINTERN PyObject *_wrap_Strings_insert__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::iterator arg2 ;
  std::vector<std::string >::size_type arg3 ; std::vector<std::string >::value_type *arg4 = 0 ; void *argp1 = 0 ; int res1 = 0 ;
  swig::PySwigIterator *iter2 = 0 ; int res2 ; size_t val3 ; int ecode3 = 0 ; int res4 = SWIG_OLDOBJ ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; PyObject * obj2 = 0 ; PyObject * obj3 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOOO:Strings_insert",&obj0,&obj1,&obj2,&obj3)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_insert" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1);
  res2 = SWIG_ConvertPtr(obj1, SWIG_as_voidptrptr(&iter2), swig::PySwigIterator::descriptor(), 0);
  if (!SWIG_IsOK(res2) || !iter2) {
    SWIG_exception_fail(SWIG_ArgError(SWIG_TypeError), "in method '" "Strings_insert" "', argument " "2"" of type '" "std::vector<std::string >::iterator""'"); }
   else {
    swig::PySwigIterator_T<std::vector<std::string >::iterator > *iter_t = dynamic_cast<swig::PySwigIterator_T<std::vector<std::string >::iterator > *>(iter2);
    if (iter_t) { arg2 = iter_t->get_current(); }  else {
      SWIG_exception_fail(SWIG_ArgError(SWIG_TypeError), "in method '" "Strings_insert" "', argument " "2"" of type '" "std::vector<std::string >::iterator""'"); }
     }  ecode3 = SWIG_AsVal_size_t(obj2, &val3); if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "Strings_insert" "', argument " "3"" of type '" "std::vector<std::string >::size_type""'"); }
    arg3 = static_cast< std::vector<std::string >::size_type >(val3); { std::string *ptr = (std::string *)0;
    res4 = SWIG_AsPtr_std_string(obj3, &ptr); if (!SWIG_IsOK(res4)) {
      SWIG_exception_fail(SWIG_ArgError(res4), "in method '" "Strings_insert" "', argument " "4"" of type '" "std::vector<std::string >::value_type const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Strings_insert" "', argument " "4"" of type '" "std::vector<std::string >::value_type const &""'"); }
     arg4 = ptr; }  { try { (arg1)->insert(arg2,arg3,(std::vector<std::string >::value_type const &)*arg4); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res4)) delete arg4;
  return resultobj; fail: if (SWIG_IsNewObj(res4)) delete arg4; return NULL; }
SWIGINTERN PyObject *_wrap_Strings_insert(PyObject *self, PyObject *args) { int argc; PyObject *argv[5]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 4); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 3) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { swig::PySwigIterator *iter = 0;
      int res = SWIG_ConvertPtr(argv[1], SWIG_as_voidptrptr(&iter), swig::PySwigIterator::descriptor(), 0);
      _v = (SWIG_IsOK(res) && iter && (dynamic_cast<swig::PySwigIterator_T<std::vector<std::string >::iterator > *>(iter) != 0));
      if (_v) { int res = SWIG_AsPtr_std_string(argv[2], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
          return _wrap_Strings_insert__SWIG_0(self, args);}  }  }  }  if (argc == 4) { int _v;
    int res = swig::asptr(argv[0], (std::vector<std::string,std::allocator<std::string > >**)(0)); _v = SWIG_CheckState(res);
    if (_v) { swig::PySwigIterator *iter = 0;
      int res = SWIG_ConvertPtr(argv[1], SWIG_as_voidptrptr(&iter), swig::PySwigIterator::descriptor(), 0);
      _v = (SWIG_IsOK(res) && iter && (dynamic_cast<swig::PySwigIterator_T<std::vector<std::string >::iterator > *>(iter) != 0));
      if (_v) { { int res = SWIG_AsVal_size_t(argv[2], NULL); _v = SWIG_CheckState(res); }  if (_v) {
          int res = SWIG_AsPtr_std_string(argv[3], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
            return _wrap_Strings_insert__SWIG_1(self, args);}  }  }  }  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'Strings_insert'.\n  Possible C/C++ prototypes are:\n    insert(std::vector<std::string >::iterator,std::vector<std::string >::value_type const &)\n    insert(std::vector<std::string >::iterator,std::vector<std::string >::size_type,std::vector<std::string >::value_type const &)\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_Strings_reserve(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::size_type arg2 ;
  void *argp1 = 0 ; int res1 = 0 ; size_t val2 ; int ecode2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Strings_reserve",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_reserve" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); ecode2 = SWIG_AsVal_size_t(obj1, &val2);
  if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "Strings_reserve" "', argument " "2"" of type '" "std::vector<std::string >::size_type""'"); }
    arg2 = static_cast< std::vector<std::string >::size_type >(val2); { try { (arg1)->reserve(arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Strings_capacity(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; std::vector<std::string >::size_type result;
  void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Strings_capacity",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0 |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Strings_capacity" "', argument " "1"" of type '" "std::vector<std::string > const *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try {
      result = ((std::vector<std::string > const *)arg1)->capacity(); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_size_t(static_cast< size_t >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_delete_Strings(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::vector<std::string > *arg1 = (std::vector<std::string > *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:delete_Strings",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, SWIG_POINTER_DISOWN |  0 );
  if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "delete_Strings" "', argument " "1"" of type '" "std::vector<std::string > *""'");  }
   arg1 = reinterpret_cast< std::vector<std::string > * >(argp1); { try { delete arg1; }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *Strings_swigregister(PyObject *SWIGUNUSEDPARM(self), PyObject *args) {
  PyObject *obj;
  if (!PyArg_ParseTuple(args,(char*)"O|swigregister", &obj)) return NULL;
  SWIG_TypeNewClientData(SWIGTYPE_p_std__vectorTstd__string_std__allocatorTstd__string_t_t, SWIG_NewClientData(obj));
  return SWIG_Py_Void();
}

SWIGINTERN PyObject *_wrap_Property_name_set(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; std::string *arg2 = 0 ; void *argp1 = 0 ; int res1 = 0 ; int res2 = SWIG_OLDOBJ ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:Property_name_set",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property_name_set" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); { std::string *ptr = (std::string *)0;
    res2 = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res2)) {
      SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "Property_name_set" "', argument " "2"" of type '" "std::string const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Property_name_set" "', argument " "2"" of type '" "std::string const &""'"); }
     arg2 = ptr; }  if (arg1) (arg1)->name = *arg2; resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res2)) delete arg2;
  return resultobj; fail: if (SWIG_IsNewObj(res2)) delete arg2; return NULL; }
SWIGINTERN PyObject *_wrap_Property_name_get(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; std::string *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Property_name_get",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property_name_get" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); { std::string const &_result_ref =  ((arg1)->name);
    result = (std::string *) &_result_ref; }  resultobj = SWIG_From_std_string(static_cast< std::string >(*result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Property_interp_set(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; std::string *arg2 = 0 ; void *argp1 = 0 ; int res1 = 0 ; int res2 = SWIG_OLDOBJ ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Property_interp_set",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property_interp_set" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); { std::string *ptr = (std::string *)0;
    res2 = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res2)) {
      SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "Property_interp_set" "', argument " "2"" of type '" "std::string const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Property_interp_set" "', argument " "2"" of type '" "std::string const &""'"); }
     arg2 = ptr; }  if (arg1) (arg1)->interp = *arg2; resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res2)) delete arg2;
  return resultobj; fail: if (SWIG_IsNewObj(res2)) delete arg2; return NULL; }
SWIGINTERN PyObject *_wrap_Property_interp_get(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; std::string *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Property_interp_get",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property_interp_get" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); { std::string const &_result_ref =  ((arg1)->interp);
    result = (std::string *) &_result_ref; }  resultobj = SWIG_From_std_string(static_cast< std::string >(*result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Property_size_get(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; size_t result; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Property_size_get",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property_size_get" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); result =  ((arg1)->size);
  resultobj = SWIG_From_size_t(static_cast< size_t >(result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Property_width_get(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; size_t result; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Property_width_get",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property_width_get" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); result =  ((arg1)->width);
  resultobj = SWIG_From_size_t(static_cast< size_t >(result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Property_type_get(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; Gto::DataType result; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Property_type_get",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property_type_get" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); result = (Gto::DataType) ((arg1)->type);
  resultobj = SWIG_From_int(static_cast< int >(result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_new_Property__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::string arg1 ; std::string arg2 ; Gto::DataType arg3 ; size_t arg4 ; size_t arg5 ; Property *result = 0 ; int val3 ;
  int ecode3 = 0 ; size_t val4 ; int ecode4 = 0 ; size_t val5 ; int ecode5 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  PyObject * obj2 = 0 ; PyObject * obj3 = 0 ; PyObject * obj4 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOOOO:new_Property",&obj0,&obj1,&obj2,&obj3,&obj4)) SWIG_fail; {
    std::string *ptr = (std::string *)0; int res = SWIG_AsPtr_std_string(obj0, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_Property" "', argument " "1"" of type '" "std::string const""'");  }
     arg1 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  { std::string *ptr = (std::string *)0;
    int res = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_Property" "', argument " "2"" of type '" "std::string const""'");  }
     arg2 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  ecode3 = SWIG_AsVal_int(obj2, &val3); if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "new_Property" "', argument " "3"" of type '" "Gto::DataType""'"); }
    arg3 = static_cast< Gto::DataType >(val3); ecode4 = SWIG_AsVal_size_t(obj3, &val4); if (!SWIG_IsOK(ecode4)) {
    SWIG_exception_fail(SWIG_ArgError(ecode4), "in method '" "new_Property" "', argument " "4"" of type '" "size_t""'"); }  
  arg4 = static_cast< size_t >(val4); ecode5 = SWIG_AsVal_size_t(obj4, &val5); if (!SWIG_IsOK(ecode5)) {
    SWIG_exception_fail(SWIG_ArgError(ecode5), "in method '" "new_Property" "', argument " "5"" of type '" "size_t""'"); }  
  arg5 = static_cast< size_t >(val5); { try { result = (Property *)new_Property__SWIG_0(arg1,arg2,arg3,arg4,arg5); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_Property, SWIG_POINTER_NEW |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_new_Property__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::string arg1 ; std::string arg2 ; Gto::DataType arg3 ; size_t arg4 ; Property *result = 0 ; int val3 ; int ecode3 = 0 ;
  size_t val4 ; int ecode4 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ; PyObject * obj3 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOOO:new_Property",&obj0,&obj1,&obj2,&obj3)) SWIG_fail; {
    std::string *ptr = (std::string *)0; int res = SWIG_AsPtr_std_string(obj0, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_Property" "', argument " "1"" of type '" "std::string const""'");  }
     arg1 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  { std::string *ptr = (std::string *)0;
    int res = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_Property" "', argument " "2"" of type '" "std::string const""'");  }
     arg2 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  ecode3 = SWIG_AsVal_int(obj2, &val3); if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "new_Property" "', argument " "3"" of type '" "Gto::DataType""'"); }
    arg3 = static_cast< Gto::DataType >(val3); ecode4 = SWIG_AsVal_size_t(obj3, &val4); if (!SWIG_IsOK(ecode4)) {
    SWIG_exception_fail(SWIG_ArgError(ecode4), "in method '" "new_Property" "', argument " "4"" of type '" "size_t""'"); }  
  arg4 = static_cast< size_t >(val4); { try { result = (Property *)new_Property__SWIG_0(arg1,arg2,arg3,arg4); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_Property, SWIG_POINTER_NEW |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_new_Property__SWIG_2(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::string arg1 ; Gto::DataType arg2 ; size_t arg3 ; size_t arg4 ; Property *result = 0 ; int val2 ; int ecode2 = 0 ;
  size_t val3 ; int ecode3 = 0 ; size_t val4 ; int ecode4 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  PyObject * obj2 = 0 ; PyObject * obj3 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOOO:new_Property",&obj0,&obj1,&obj2,&obj3)) SWIG_fail; {
    std::string *ptr = (std::string *)0; int res = SWIG_AsPtr_std_string(obj0, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_Property" "', argument " "1"" of type '" "std::string const""'");  }
     arg1 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  ecode2 = SWIG_AsVal_int(obj1, &val2); if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "new_Property" "', argument " "2"" of type '" "Gto::DataType""'"); }
    arg2 = static_cast< Gto::DataType >(val2); ecode3 = SWIG_AsVal_size_t(obj2, &val3); if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "new_Property" "', argument " "3"" of type '" "size_t""'"); }  
  arg3 = static_cast< size_t >(val3); ecode4 = SWIG_AsVal_size_t(obj3, &val4); if (!SWIG_IsOK(ecode4)) {
    SWIG_exception_fail(SWIG_ArgError(ecode4), "in method '" "new_Property" "', argument " "4"" of type '" "size_t""'"); }  
  arg4 = static_cast< size_t >(val4); { try { result = (Property *)new_Property__SWIG_2(arg1,arg2,arg3,arg4); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_Property, SWIG_POINTER_NEW |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_new_Property__SWIG_3(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::string arg1 ; Gto::DataType arg2 ; size_t arg3 ; Property *result = 0 ; int val2 ; int ecode2 = 0 ; size_t val3 ;
  int ecode3 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:new_Property",&obj0,&obj1,&obj2)) SWIG_fail; { std::string *ptr = (std::string *)0;
    int res = SWIG_AsPtr_std_string(obj0, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_Property" "', argument " "1"" of type '" "std::string const""'");  }
     arg1 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  ecode2 = SWIG_AsVal_int(obj1, &val2); if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "new_Property" "', argument " "2"" of type '" "Gto::DataType""'"); }
    arg2 = static_cast< Gto::DataType >(val2); ecode3 = SWIG_AsVal_size_t(obj2, &val3); if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "new_Property" "', argument " "3"" of type '" "size_t""'"); }  
  arg3 = static_cast< size_t >(val3); { try { result = (Property *)new_Property__SWIG_2(arg1,arg2,arg3); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_Property, SWIG_POINTER_NEW |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_new_Property__SWIG_4(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::string arg1 ; std::string arg2 ; PyObject *arg3 = (PyObject *) 0 ; Property *result = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:new_Property",&obj0,&obj1,&obj2)) SWIG_fail; { std::string *ptr = (std::string *)0;
    int res = SWIG_AsPtr_std_string(obj0, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_Property" "', argument " "1"" of type '" "std::string const""'");  }
     arg1 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  { std::string *ptr = (std::string *)0;
    int res = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_Property" "', argument " "2"" of type '" "std::string const""'");  }
     arg2 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  arg3 = obj2; { try {
      result = (Property *)new_Property__SWIG_4(arg1,arg2,arg3); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_Property, SWIG_POINTER_NEW |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_new_Property__SWIG_5(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::string arg1 ; PyObject *arg2 = (PyObject *) 0 ; Property *result = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:new_Property",&obj0,&obj1)) SWIG_fail; { std::string *ptr = (std::string *)0;
    int res = SWIG_AsPtr_std_string(obj0, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_Property" "', argument " "1"" of type '" "std::string const""'");  }
     arg1 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  arg2 = obj1; { try {
      result = (Property *)new_Property__SWIG_5(arg1,arg2); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_Property, SWIG_POINTER_NEW |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_new_Property(PyObject *self, PyObject *args) { int argc; PyObject *argv[6]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 5); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 2) { int _v;
    int res = SWIG_AsPtr_std_string(argv[0], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) { _v = (argv[1] != 0);
      if (_v) { return _wrap_new_Property__SWIG_5(self, args);}  }  }  if (argc == 3) { int _v;
    int res = SWIG_AsPtr_std_string(argv[0], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) { {
        int res = SWIG_AsVal_int(argv[1], NULL); _v = SWIG_CheckState(res); }  if (_v) { {
          int res = SWIG_AsVal_size_t(argv[2], NULL); _v = SWIG_CheckState(res); }  if (_v) {
          return _wrap_new_Property__SWIG_3(self, args);}  }  }  }  if (argc == 3) { int _v;
    int res = SWIG_AsPtr_std_string(argv[0], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
      int res = SWIG_AsPtr_std_string(argv[1], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) { _v = (argv[2] != 0);
        if (_v) { return _wrap_new_Property__SWIG_4(self, args);}  }  }  }  if (argc == 4) { int _v;
    int res = SWIG_AsPtr_std_string(argv[0], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) { {
        int res = SWIG_AsVal_int(argv[1], NULL); _v = SWIG_CheckState(res); }  if (_v) { {
          int res = SWIG_AsVal_size_t(argv[2], NULL); _v = SWIG_CheckState(res); }  if (_v) { {
            int res = SWIG_AsVal_size_t(argv[3], NULL); _v = SWIG_CheckState(res); }  if (_v) {
            return _wrap_new_Property__SWIG_2(self, args);}  }  }  }  }  if (argc == 4) { int _v;
    int res = SWIG_AsPtr_std_string(argv[0], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
      int res = SWIG_AsPtr_std_string(argv[1], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) { {
          int res = SWIG_AsVal_int(argv[2], NULL); _v = SWIG_CheckState(res); }  if (_v) { {
            int res = SWIG_AsVal_size_t(argv[3], NULL); _v = SWIG_CheckState(res); }  if (_v) {
            return _wrap_new_Property__SWIG_1(self, args);}  }  }  }  }  if (argc == 5) { int _v;
    int res = SWIG_AsPtr_std_string(argv[0], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
      int res = SWIG_AsPtr_std_string(argv[1], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) { {
          int res = SWIG_AsVal_int(argv[2], NULL); _v = SWIG_CheckState(res); }  if (_v) { {
            int res = SWIG_AsVal_size_t(argv[3], NULL); _v = SWIG_CheckState(res); }  if (_v) { {
              int res = SWIG_AsVal_size_t(argv[4], NULL); _v = SWIG_CheckState(res); }  if (_v) {
              return _wrap_new_Property__SWIG_0(self, args);}  }  }  }  }  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'new_Property'.\n  Possible C/C++ prototypes are:\n    Property(std::string const,std::string const,Gto::DataType,size_t,size_t)\n    Property(std::string const,std::string const,Gto::DataType,size_t)\n    Property(std::string const,Gto::DataType,size_t,size_t)\n    Property(std::string const,Gto::DataType,size_t)\n    Property(std::string const,std::string const,PyObject *)\n    Property(std::string const,PyObject *)\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_Property___repr__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; char *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Property___repr__",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property___repr__" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); { try { result = (char *)Property___repr__(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_FromCharPtr((const char *)result);
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Property___cmp__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; PyObject *arg2 = (PyObject *) 0 ; long result; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:Property___cmp__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property___cmp__" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); arg2 = obj1; { try { result = (long)Property___cmp__(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_long(static_cast< long >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Property___len__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; long result; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Property___len__",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property___len__" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); { try { result = (long)Property___len__(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_long(static_cast< long >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Property___call__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Property___call__",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property___call__" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); { try { result = (PyObject *)Property___call__(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = result; return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Property___getitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; PyObject *arg2 = (PyObject *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Property___getitem__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property___getitem__" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); arg2 = obj1; { try { result = (PyObject *)Property___getitem__(arg1,arg2); }
        catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = result; return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Property___setitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; PyObject *arg2 = (PyObject *) 0 ; PyObject *arg3 = (PyObject *) 0 ; void *argp1 = 0 ;
  int res1 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:Property___setitem__",&obj0,&obj1,&obj2)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property___setitem__" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); arg2 = obj1; arg3 = obj2; { try { Property___setitem__(arg1,arg2,arg3); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Property___contains__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; PyObject *arg2 = (PyObject *) 0 ; bool result; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Property___contains__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Property___contains__" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); arg2 = obj1; { try { result = (bool)Property___contains__(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_bool(static_cast< bool >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_delete_Property(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Property *arg1 = (Property *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:delete_Property",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Property, SWIG_POINTER_DISOWN |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "delete_Property" "', argument " "1"" of type '" "Property *""'");  }
   arg1 = reinterpret_cast< Property * >(argp1); { try { delete arg1; }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *Property_swigregister(PyObject *SWIGUNUSEDPARM(self), PyObject *args) {
  PyObject *obj;
  if (!PyArg_ParseTuple(args,(char*)"O|swigregister", &obj)) return NULL;
  SWIG_TypeNewClientData(SWIGTYPE_p_Property, SWIG_NewClientData(obj));
  return SWIG_Py_Void();
}

SWIGINTERN PyObject *_wrap_new_Component__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::string *arg1 = 0 ; std::string *arg2 = 0 ; int arg3 ; Component *result = 0 ; int res1 = SWIG_OLDOBJ ;
  int res2 = SWIG_OLDOBJ ; int val3 ; int ecode3 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:new_Component",&obj0,&obj1,&obj2)) SWIG_fail; { std::string *ptr = (std::string *)0;
    res1 = SWIG_AsPtr_std_string(obj0, &ptr); if (!SWIG_IsOK(res1)) {
      SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "new_Component" "', argument " "1"" of type '" "std::string const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "new_Component" "', argument " "1"" of type '" "std::string const &""'"); }
     arg1 = ptr; }  { std::string *ptr = (std::string *)0; res2 = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res2)) {
      SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "new_Component" "', argument " "2"" of type '" "std::string const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "new_Component" "', argument " "2"" of type '" "std::string const &""'"); }
     arg2 = ptr; }  ecode3 = SWIG_AsVal_int(obj2, &val3); if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "new_Component" "', argument " "3"" of type '" "int""'"); }  
  arg3 = static_cast< int >(val3); { try {
      result = (Component *)new Component((std::string const &)*arg1,(std::string const &)*arg2,arg3); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_Component, SWIG_POINTER_NEW |  0 );
  if (SWIG_IsNewObj(res1)) delete arg1; if (SWIG_IsNewObj(res2)) delete arg2; return resultobj; fail:
  if (SWIG_IsNewObj(res1)) delete arg1; if (SWIG_IsNewObj(res2)) delete arg2; return NULL; }
SWIGINTERN PyObject *_wrap_new_Component__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::string *arg1 = 0 ; int arg2 ; Component *result = 0 ; int res1 = SWIG_OLDOBJ ; int val2 ; int ecode2 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:new_Component",&obj0,&obj1)) SWIG_fail; {
    std::string *ptr = (std::string *)0; res1 = SWIG_AsPtr_std_string(obj0, &ptr); if (!SWIG_IsOK(res1)) {
      SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "new_Component" "', argument " "1"" of type '" "std::string const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "new_Component" "', argument " "1"" of type '" "std::string const &""'"); }
     arg1 = ptr; }  ecode2 = SWIG_AsVal_int(obj1, &val2); if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "new_Component" "', argument " "2"" of type '" "int""'"); }  
  arg2 = static_cast< int >(val2); { try { result = (Component *)new Component((std::string const &)*arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_Component, SWIG_POINTER_NEW |  0 );
  if (SWIG_IsNewObj(res1)) delete arg1; return resultobj; fail: if (SWIG_IsNewObj(res1)) delete arg1; return NULL; }
SWIGINTERN PyObject *_wrap_Component_name_set(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; std::string *arg2 = 0 ; void *argp1 = 0 ; int res1 = 0 ; int res2 = SWIG_OLDOBJ ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Component_name_set",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component_name_set" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); { std::string *ptr = (std::string *)0;
    res2 = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res2)) {
      SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "Component_name_set" "', argument " "2"" of type '" "std::string const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Component_name_set" "', argument " "2"" of type '" "std::string const &""'"); }
     arg2 = ptr; }  if (arg1) (arg1)->name = *arg2; resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res2)) delete arg2;
  return resultobj; fail: if (SWIG_IsNewObj(res2)) delete arg2; return NULL; }
SWIGINTERN PyObject *_wrap_Component_name_get(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; std::string *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Component_name_get",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component_name_get" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); { std::string const &_result_ref =  ((arg1)->name);
    result = (std::string *) &_result_ref; }  resultobj = SWIG_From_std_string(static_cast< std::string >(*result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component_interp_set(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; std::string *arg2 = 0 ; void *argp1 = 0 ; int res1 = 0 ; int res2 = SWIG_OLDOBJ ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Component_interp_set",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component_interp_set" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); { std::string *ptr = (std::string *)0;
    res2 = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res2)) {
      SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "Component_interp_set" "', argument " "2"" of type '" "std::string const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Component_interp_set" "', argument " "2"" of type '" "std::string const &""'"); }
     arg2 = ptr; }  if (arg1) (arg1)->interp = *arg2; resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res2)) delete arg2;
  return resultobj; fail: if (SWIG_IsNewObj(res2)) delete arg2; return NULL; }
SWIGINTERN PyObject *_wrap_Component_interp_get(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; std::string *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Component_interp_get",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component_interp_get" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); { std::string const &_result_ref =  ((arg1)->interp);
    result = (std::string *) &_result_ref; }  resultobj = SWIG_From_std_string(static_cast< std::string >(*result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component_flags_set(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; unsigned short arg2 ; void *argp1 = 0 ; int res1 = 0 ; unsigned short val2 ;
  int ecode2 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Component_flags_set",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component_flags_set" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); ecode2 = SWIG_AsVal_unsigned_SS_short(obj1, &val2); if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "Component_flags_set" "', argument " "2"" of type '" "unsigned short""'"); }
    arg2 = static_cast< unsigned short >(val2); if (arg1) (arg1)->flags = arg2; resultobj = SWIG_Py_Void(); return resultobj;
  fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component_flags_get(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; unsigned short result; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Component_flags_get",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component_flags_get" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); result = (unsigned short) ((arg1)->flags);
  resultobj = SWIG_From_unsigned_SS_short(static_cast< unsigned short >(result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_new_Component__SWIG_2(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::string arg1 ; std::string arg2 ; Component *result = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:new_Component",&obj0,&obj1)) SWIG_fail; { std::string *ptr = (std::string *)0;
    int res = SWIG_AsPtr_std_string(obj0, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_Component" "', argument " "1"" of type '" "std::string const""'");  }
     arg1 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  { std::string *ptr = (std::string *)0;
    int res = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_Component" "', argument " "2"" of type '" "std::string const""'");  }
     arg2 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  { try { result = (Component *)new_Component__SWIG_2(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_Component, SWIG_POINTER_NEW |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_new_Component__SWIG_3(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::string arg1 ; Component *result = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:new_Component",&obj0)) SWIG_fail; { std::string *ptr = (std::string *)0;
    int res = SWIG_AsPtr_std_string(obj0, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_Component" "', argument " "1"" of type '" "std::string const""'");  }
     arg1 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  { try { result = (Component *)new_Component__SWIG_3(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_Component, SWIG_POINTER_NEW |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_new_Component(PyObject *self, PyObject *args) { int argc; PyObject *argv[4]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 3); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 1) { int _v;
    int res = SWIG_AsPtr_std_string(argv[0], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
      return _wrap_new_Component__SWIG_3(self, args);}  }  if (argc == 2) { int _v;
    int res = SWIG_AsPtr_std_string(argv[0], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) { {
        int res = SWIG_AsVal_int(argv[1], NULL); _v = SWIG_CheckState(res); }  if (_v) {
        return _wrap_new_Component__SWIG_1(self, args);}  }  }  if (argc == 2) { int _v;
    int res = SWIG_AsPtr_std_string(argv[0], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
      int res = SWIG_AsPtr_std_string(argv[1], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
        return _wrap_new_Component__SWIG_2(self, args);}  }  }  if (argc == 3) { int _v;
    int res = SWIG_AsPtr_std_string(argv[0], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
      int res = SWIG_AsPtr_std_string(argv[1], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) { {
          int res = SWIG_AsVal_int(argv[2], NULL); _v = SWIG_CheckState(res); }  if (_v) {
          return _wrap_new_Component__SWIG_0(self, args);}  }  }  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'new_Component'.\n  Possible C/C++ prototypes are:\n    Component(std::string const &,std::string const &,int)\n    Component(std::string const &,int)\n    Component(std::string const,std::string const)\n    Component(std::string const)\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_Component___repr__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; char *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Component___repr__",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component___repr__" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); { try { result = (char *)Component___repr__(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_FromCharPtr((const char *)result);
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component___len__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; long result; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Component___len__",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component___len__" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); { try { result = (long)Component___len__(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_long(static_cast< long >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component___cmp__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; PyObject *arg2 = (PyObject *) 0 ; long result; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:Component___cmp__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component___cmp__" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); arg2 = obj1; { try { result = (long)Component___cmp__(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_long(static_cast< long >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component___getitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; PyObject *arg2 = (PyObject *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Component___getitem__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component___getitem__" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); arg2 = obj1; { try { result = (PyObject *)Component___getitem__(arg1,arg2); }
        catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = result; return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component___setitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; PyObject *arg2 = (PyObject *) 0 ; PyObject *arg3 = (PyObject *) 0 ; void *argp1 = 0 ;
  int res1 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:Component___setitem__",&obj0,&obj1,&obj2)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component___setitem__" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); arg2 = obj1; arg3 = obj2; { try { Component___setitem__(arg1,arg2,arg3); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component___delitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; PyObject *arg2 = (PyObject *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:Component___delitem__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component___delitem__" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); arg2 = obj1; { try { Component___delitem__(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component___contains__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; PyObject *arg2 = (PyObject *) 0 ; bool result; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Component___contains__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component___contains__" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); arg2 = obj1; { try { result = (bool)Component___contains__(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_bool(static_cast< bool >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component_keys(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Component_keys",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component_keys" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); { try { result = (PyObject *)Component_keys(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = result; return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component_values(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; Properties result; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Component_values",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component_values" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); { try { result = Component_values(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj((new Properties(static_cast< const Properties& >(result))), SWIGTYPE_p_Properties, SWIG_POINTER_OWN |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component_items(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Component_items",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component_items" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); { try { result = (PyObject *)Component_items(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = result; return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component_append(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; PyObject *arg2 = (PyObject *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:Component_append",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component_append" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); arg2 = obj1; { try { Component_append(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Component_extend(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; PyObject *arg2 = (PyObject *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:Component_extend",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Component_extend" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); arg2 = obj1; { try { Component_extend(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_delete_Component(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Component *arg1 = (Component *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:delete_Component",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Component, SWIG_POINTER_DISOWN |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "delete_Component" "', argument " "1"" of type '" "Component *""'");  }
   arg1 = reinterpret_cast< Component * >(argp1); { try { delete arg1; }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *Component_swigregister(PyObject *SWIGUNUSEDPARM(self), PyObject *args) {
  PyObject *obj;
  if (!PyArg_ParseTuple(args,(char*)"O|swigregister", &obj)) return NULL;
  SWIG_TypeNewClientData(SWIGTYPE_p_Component, SWIG_NewClientData(obj));
  return SWIG_Py_Void();
}

SWIGINTERN PyObject *_wrap_new_Object(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::string *arg1 = 0 ; std::string *arg2 = 0 ; unsigned int arg3 ; Object *result = 0 ; int res1 = SWIG_OLDOBJ ;
  int res2 = SWIG_OLDOBJ ; unsigned int val3 ; int ecode3 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  PyObject * obj2 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OOO:new_Object",&obj0,&obj1,&obj2)) SWIG_fail; {
    std::string *ptr = (std::string *)0; res1 = SWIG_AsPtr_std_string(obj0, &ptr); if (!SWIG_IsOK(res1)) {
      SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "new_Object" "', argument " "1"" of type '" "std::string const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "new_Object" "', argument " "1"" of type '" "std::string const &""'"); }
     arg1 = ptr; }  { std::string *ptr = (std::string *)0; res2 = SWIG_AsPtr_std_string(obj1, &ptr); if (!SWIG_IsOK(res2)) {
      SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "new_Object" "', argument " "2"" of type '" "std::string const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "new_Object" "', argument " "2"" of type '" "std::string const &""'"); }
     arg2 = ptr; }  ecode3 = SWIG_AsVal_unsigned_SS_int(obj2, &val3); if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "new_Object" "', argument " "3"" of type '" "unsigned int""'"); }  
  arg3 = static_cast< unsigned int >(val3); { try {
      result = (Object *)new Object((std::string const &)*arg1,(std::string const &)*arg2,arg3); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_Object, SWIG_POINTER_NEW |  0 );
  if (SWIG_IsNewObj(res1)) delete arg1; if (SWIG_IsNewObj(res2)) delete arg2; return resultobj; fail:
  if (SWIG_IsNewObj(res1)) delete arg1; if (SWIG_IsNewObj(res2)) delete arg2; return NULL; }
SWIGINTERN PyObject *_wrap_delete_Object(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:delete_Object",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, SWIG_POINTER_DISOWN |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "delete_Object" "', argument " "1"" of type '" "Object *""'");  } 
  arg1 = reinterpret_cast< Object * >(argp1); { try { delete arg1; }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object_name_set(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; std::string *arg2 = 0 ; void *argp1 = 0 ; int res1 = 0 ; int res2 = SWIG_OLDOBJ ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:Object_name_set",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object_name_set" "', argument " "1"" of type '" "Object *""'");  } 
  arg1 = reinterpret_cast< Object * >(argp1); { std::string *ptr = (std::string *)0; res2 = SWIG_AsPtr_std_string(obj1, &ptr);
    if (!SWIG_IsOK(res2)) {
      SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "Object_name_set" "', argument " "2"" of type '" "std::string const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Object_name_set" "', argument " "2"" of type '" "std::string const &""'"); }
     arg2 = ptr; }  if (arg1) (arg1)->name = *arg2; resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res2)) delete arg2;
  return resultobj; fail: if (SWIG_IsNewObj(res2)) delete arg2; return NULL; }
SWIGINTERN PyObject *_wrap_Object_name_get(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; std::string *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Object_name_get",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object_name_get" "', argument " "1"" of type '" "Object *""'");  } 
  arg1 = reinterpret_cast< Object * >(argp1); { std::string const &_result_ref =  ((arg1)->name);
    result = (std::string *) &_result_ref; }  resultobj = SWIG_From_std_string(static_cast< std::string >(*result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object_protocol_set(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; std::string *arg2 = 0 ; void *argp1 = 0 ; int res1 = 0 ; int res2 = SWIG_OLDOBJ ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Object_protocol_set",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object_protocol_set" "', argument " "1"" of type '" "Object *""'");  }
   arg1 = reinterpret_cast< Object * >(argp1); { std::string *ptr = (std::string *)0; res2 = SWIG_AsPtr_std_string(obj1, &ptr);
    if (!SWIG_IsOK(res2)) {
      SWIG_exception_fail(SWIG_ArgError(res2), "in method '" "Object_protocol_set" "', argument " "2"" of type '" "std::string const &""'"); }
     if (!ptr) {
      SWIG_exception_fail(SWIG_ValueError, "invalid null reference " "in method '" "Object_protocol_set" "', argument " "2"" of type '" "std::string const &""'"); }
     arg2 = ptr; }  if (arg1) (arg1)->protocol = *arg2; resultobj = SWIG_Py_Void(); if (SWIG_IsNewObj(res2)) delete arg2;
  return resultobj; fail: if (SWIG_IsNewObj(res2)) delete arg2; return NULL; }
SWIGINTERN PyObject *_wrap_Object_protocol_get(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; std::string *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Object_protocol_get",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object_protocol_get" "', argument " "1"" of type '" "Object *""'");  }
   arg1 = reinterpret_cast< Object * >(argp1); { std::string const &_result_ref =  ((arg1)->protocol);
    result = (std::string *) &_result_ref; }  resultobj = SWIG_From_std_string(static_cast< std::string >(*result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object_protocolVersion_set(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; unsigned int arg2 ; void *argp1 = 0 ; int res1 = 0 ; unsigned int val2 ; int ecode2 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Object_protocolVersion_set",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object_protocolVersion_set" "', argument " "1"" of type '" "Object *""'");  }
   arg1 = reinterpret_cast< Object * >(argp1); ecode2 = SWIG_AsVal_unsigned_SS_int(obj1, &val2); if (!SWIG_IsOK(ecode2)) {
    SWIG_exception_fail(SWIG_ArgError(ecode2), "in method '" "Object_protocolVersion_set" "', argument " "2"" of type '" "unsigned int""'"); }
    arg2 = static_cast< unsigned int >(val2); if (arg1) (arg1)->protocolVersion = arg2; resultobj = SWIG_Py_Void();
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object_protocolVersion_get(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; unsigned int result; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Object_protocolVersion_get",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object_protocolVersion_get" "', argument " "1"" of type '" "Object *""'");  }
   arg1 = reinterpret_cast< Object * >(argp1); result = (unsigned int) ((arg1)->protocolVersion);
  resultobj = SWIG_From_unsigned_SS_int(static_cast< unsigned int >(result)); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object___repr__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; char *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Object___repr__",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object___repr__" "', argument " "1"" of type '" "Object *""'");  } 
  arg1 = reinterpret_cast< Object * >(argp1); { try { result = (char *)Object___repr__(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_FromCharPtr((const char *)result);
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object___len__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; long result; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Object___len__",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object___len__" "', argument " "1"" of type '" "Object *""'");  } 
  arg1 = reinterpret_cast< Object * >(argp1); { try { result = (long)Object___len__(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_long(static_cast< long >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object___cmp__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; PyObject *arg2 = (PyObject *) 0 ; long result; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:Object___cmp__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object___cmp__" "', argument " "1"" of type '" "Object *""'");  } 
  arg1 = reinterpret_cast< Object * >(argp1); arg2 = obj1; { try { result = (long)Object___cmp__(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_long(static_cast< long >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object___getitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; PyObject *arg2 = (PyObject *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Object___getitem__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object___getitem__" "', argument " "1"" of type '" "Object *""'");  }
   arg1 = reinterpret_cast< Object * >(argp1); arg2 = obj1; { try { result = (PyObject *)Object___getitem__(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = result; return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object___setitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; PyObject *arg2 = (PyObject *) 0 ; PyObject *arg3 = (PyObject *) 0 ; void *argp1 = 0 ;
  int res1 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:Object___setitem__",&obj0,&obj1,&obj2)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object___setitem__" "', argument " "1"" of type '" "Object *""'");  }
   arg1 = reinterpret_cast< Object * >(argp1); arg2 = obj1; arg3 = obj2; { try { Object___setitem__(arg1,arg2,arg3); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object___delitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; PyObject *arg2 = (PyObject *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:Object___delitem__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object___delitem__" "', argument " "1"" of type '" "Object *""'");  }
   arg1 = reinterpret_cast< Object * >(argp1); arg2 = obj1; { try { Object___delitem__(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object___contains__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; PyObject *arg2 = (PyObject *) 0 ; bool result; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:Object___contains__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object___contains__" "', argument " "1"" of type '" "Object *""'");  }
   arg1 = reinterpret_cast< Object * >(argp1); arg2 = obj1; { try { result = (bool)Object___contains__(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_bool(static_cast< bool >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object_keys(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Object_keys",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object_keys" "', argument " "1"" of type '" "Object *""'");  } 
  arg1 = reinterpret_cast< Object * >(argp1); { try { result = (PyObject *)Object_keys(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = result; return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object_values(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; Components result; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:Object_values",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object_values" "', argument " "1"" of type '" "Object *""'");  } 
  arg1 = reinterpret_cast< Object * >(argp1); { try { result = Object_values(arg1); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj((new Components(static_cast< const Components& >(result))), SWIGTYPE_p_Components, SWIG_POINTER_OWN |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object_append(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; PyObject *arg2 = (PyObject *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:Object_append",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object_append" "', argument " "1"" of type '" "Object *""'");  } 
  arg1 = reinterpret_cast< Object * >(argp1); arg2 = obj1; { try { Object_append(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_Object_extend(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  Object *arg1 = (Object *) 0 ; PyObject *arg2 = (PyObject *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:Object_extend",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_Object, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "Object_extend" "', argument " "1"" of type '" "Object *""'");  } 
  arg1 = reinterpret_cast< Object * >(argp1); arg2 = obj1; { try { Object_extend(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *Object_swigregister(PyObject *SWIGUNUSEDPARM(self), PyObject *args) {
  PyObject *obj;
  if (!PyArg_ParseTuple(args,(char*)"O|swigregister", &obj)) return NULL;
  SWIG_TypeNewClientData(SWIGTYPE_p_Object, SWIG_NewClientData(obj));
  return SWIG_Py_Void();
}

SWIGINTERN PyObject *_wrap_new_GtoDB__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *result = 0 ; if (!PyArg_ParseTuple(args,(char *)":new_GtoDB")) SWIG_fail; { try { result = (GtoDB *)new GtoDB(); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_GtoDB, SWIG_POINTER_NEW |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_new_GtoDB__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  std::string arg1 ; GtoDB *result = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:new_GtoDB",&obj0)) SWIG_fail; { std::string *ptr = (std::string *)0;
    int res = SWIG_AsPtr_std_string(obj0, &ptr); if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "new_GtoDB" "', argument " "1"" of type '" "std::string""'");  }
     arg1 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  { try { result = (GtoDB *)new GtoDB(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj(SWIG_as_voidptr(result), SWIGTYPE_p_GtoDB, SWIG_POINTER_NEW |  0 ); return resultobj; fail:
  return NULL; }
SWIGINTERN PyObject *_wrap_new_GtoDB(PyObject *self, PyObject *args) { int argc; PyObject *argv[2]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 1); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 0) { return _wrap_new_GtoDB__SWIG_0(self, args);}  if (argc == 1) {
    int _v; int res = SWIG_AsPtr_std_string(argv[0], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
      return _wrap_new_GtoDB__SWIG_1(self, args);}  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'new_GtoDB'.\n  Possible C/C++ prototypes are:\n    GtoDB()\n    GtoDB(std::string)\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB_read(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; std::string arg2 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:GtoDB_read",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB_read" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); { std::string *ptr = (std::string *)0; int res = SWIG_AsPtr_std_string(obj1, &ptr);
    if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "GtoDB_read" "', argument " "2"" of type '" "std::string""'");  }
     arg2 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  { try { (arg1)->read(arg2); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB_write__SWIG_0(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; std::string arg2 ; GtoDB::FileType arg3 ; void *argp1 = 0 ; int res1 = 0 ; int val3 ;
  int ecode3 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:GtoDB_write",&obj0,&obj1,&obj2)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB_write" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); { std::string *ptr = (std::string *)0; int res = SWIG_AsPtr_std_string(obj1, &ptr);
    if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "GtoDB_write" "', argument " "2"" of type '" "std::string""'");  }
     arg2 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  ecode3 = SWIG_AsVal_int(obj2, &val3); if (!SWIG_IsOK(ecode3)) {
    SWIG_exception_fail(SWIG_ArgError(ecode3), "in method '" "GtoDB_write" "', argument " "3"" of type '" "GtoDB::FileType""'"); }
    arg3 = static_cast< GtoDB::FileType >(val3); { try { (arg1)->write(arg2,arg3); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB_write__SWIG_1(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; std::string arg2 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:GtoDB_write",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB_write" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); { std::string *ptr = (std::string *)0; int res = SWIG_AsPtr_std_string(obj1, &ptr);
    if (!SWIG_IsOK(res) || !ptr) {
      SWIG_exception_fail(SWIG_ArgError((ptr ? res : SWIG_TypeError)), "in method '" "GtoDB_write" "', argument " "2"" of type '" "std::string""'");  }
     arg2 = *ptr; if (SWIG_IsNewObj(res)) delete ptr; }  { try { (arg1)->write(arg2); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB_write(PyObject *self, PyObject *args) { int argc; PyObject *argv[4]; int ii;
  if (!PyTuple_Check(args)) SWIG_fail; argc = PyObject_Length(args); for (ii = 0; (ii < argc) && (ii < 3); ii++) {
    argv[ii] = PyTuple_GET_ITEM(args,ii); }  if (argc == 2) { int _v; void *vptr = 0;
    int res = SWIG_ConvertPtr(argv[0], &vptr, SWIGTYPE_p_GtoDB, 0); _v = SWIG_CheckState(res); if (_v) {
      int res = SWIG_AsPtr_std_string(argv[1], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) {
        return _wrap_GtoDB_write__SWIG_1(self, args);}  }  }  if (argc == 3) { int _v; void *vptr = 0;
    int res = SWIG_ConvertPtr(argv[0], &vptr, SWIGTYPE_p_GtoDB, 0); _v = SWIG_CheckState(res); if (_v) {
      int res = SWIG_AsPtr_std_string(argv[1], (std::string**)(0)); _v = SWIG_CheckState(res); if (_v) { {
          int res = SWIG_AsVal_int(argv[2], NULL); _v = SWIG_CheckState(res); }  if (_v) {
          return _wrap_GtoDB_write__SWIG_0(self, args);}  }  }  }  fail:
  SWIG_SetErrorMsg(PyExc_NotImplementedError,"Wrong number of arguments for overloaded function 'GtoDB_write'.\n  Possible C/C++ prototypes are:\n    write(std::string,GtoDB::FileType)\n    write(std::string)\n");
  return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB___len__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; long result; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:GtoDB___len__",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB___len__" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); { try { result = (long)GtoDB___len__(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_long(static_cast< long >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB___getitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; PyObject *arg2 = (PyObject *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:GtoDB___getitem__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB___getitem__" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); arg2 = obj1; { try { result = (PyObject *)GtoDB___getitem__(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = result; return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB___setitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; PyObject *arg2 = (PyObject *) 0 ; PyObject *arg3 = (PyObject *) 0 ; void *argp1 = 0 ;
  int res1 = 0 ; PyObject * obj0 = 0 ; PyObject * obj1 = 0 ; PyObject * obj2 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OOO:GtoDB___setitem__",&obj0,&obj1,&obj2)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB___setitem__" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); arg2 = obj1; arg3 = obj2; { try { GtoDB___setitem__(arg1,arg2,arg3); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB___delitem__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; PyObject *arg2 = (PyObject *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:GtoDB___delitem__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB___delitem__" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); arg2 = obj1; { try { GtoDB___delitem__(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB___contains__(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; PyObject *arg2 = (PyObject *) 0 ; bool result; void *argp1 = 0 ; int res1 = 0 ;
  PyObject * obj0 = 0 ; PyObject * obj1 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"OO:GtoDB___contains__",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB___contains__" "', argument " "1"" of type '" "GtoDB *""'");  }
   arg1 = reinterpret_cast< GtoDB * >(argp1); arg2 = obj1; { try { result = (bool)GtoDB___contains__(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_From_bool(static_cast< bool >(result));
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB_keys(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:GtoDB_keys",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB_keys" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); { try { result = (PyObject *)GtoDB_keys(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = result; return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB_values(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; Objects result; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:GtoDB_values",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB_values" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); { try { result = GtoDB_values(arg1); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   } 
  resultobj = SWIG_NewPointerObj((new Objects(static_cast< const Objects& >(result))), SWIGTYPE_p_Objects, SWIG_POINTER_OWN |  0 );
  return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB_items(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; PyObject *result = 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:GtoDB_items",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB_items" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); { try { result = (PyObject *)GtoDB_items(arg1); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = result; return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB_append(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; PyObject *arg2 = (PyObject *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:GtoDB_append",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB_append" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); arg2 = obj1; { try { GtoDB_append(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB_extend(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; PyObject *arg2 = (PyObject *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  PyObject * obj1 = 0 ; if (!PyArg_ParseTuple(args,(char *)"OO:GtoDB_extend",&obj0,&obj1)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB_extend" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); arg2 = obj1; { try { GtoDB_extend(arg1,arg2); }    
    catch (std::invalid_argument& e) { SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_GtoDB_clear(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:GtoDB_clear",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, 0 |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "GtoDB_clear" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); { try { GtoDB_clear(arg1); }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *_wrap_delete_GtoDB(PyObject *SWIGUNUSEDPARM(self), PyObject *args) { PyObject *resultobj = 0;
  GtoDB *arg1 = (GtoDB *) 0 ; void *argp1 = 0 ; int res1 = 0 ; PyObject * obj0 = 0 ;
  if (!PyArg_ParseTuple(args,(char *)"O:delete_GtoDB",&obj0)) SWIG_fail;
  res1 = SWIG_ConvertPtr(obj0, &argp1,SWIGTYPE_p_GtoDB, SWIG_POINTER_DISOWN |  0 ); if (!SWIG_IsOK(res1)) {
    SWIG_exception_fail(SWIG_ArgError(res1), "in method '" "delete_GtoDB" "', argument " "1"" of type '" "GtoDB *""'");  } 
  arg1 = reinterpret_cast< GtoDB * >(argp1); { try { delete arg1; }     catch (std::invalid_argument& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::domain_error& e) {
      SWIG_exception_fail(SWIG_ValueError, e.what() ); }  catch (std::overflow_error& e) {
      SWIG_exception_fail(SWIG_OverflowError, e.what() ); }  catch (std::out_of_range& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::length_error& e) {
      SWIG_exception_fail(SWIG_IndexError, e.what() ); }  catch (std::runtime_error& e) {
      SWIG_exception_fail(SWIG_RuntimeError, e.what() ); }  catch (std::exception& e) {
      SWIG_exception_fail(SWIG_SystemError, e.what() ); }   }  resultobj = SWIG_Py_Void(); return resultobj; fail: return NULL; }
SWIGINTERN PyObject *GtoDB_swigregister(PyObject *SWIGUNUSEDPARM(self), PyObject *args) {
  PyObject *obj;
  if (!PyArg_ParseTuple(args,(char*)"O|swigregister", &obj)) return NULL;
  SWIG_TypeNewClientData(SWIGTYPE_p_GtoDB, SWIG_NewClientData(obj));
  return SWIG_Py_Void();
}

static PyMethodDef SwigMethods[] = {
	 { (char *)"delete_PySwigIterator", _wrap_delete_PySwigIterator, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator_value", _wrap_PySwigIterator_value, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator_incr", _wrap_PySwigIterator_incr, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator_decr", _wrap_PySwigIterator_decr, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator_distance", _wrap_PySwigIterator_distance, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator_equal", _wrap_PySwigIterator_equal, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator_copy", _wrap_PySwigIterator_copy, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator_next", _wrap_PySwigIterator_next, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator_previous", _wrap_PySwigIterator_previous, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator_advance", _wrap_PySwigIterator_advance, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator___eq__", _wrap_PySwigIterator___eq__, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator___ne__", _wrap_PySwigIterator___ne__, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator___iadd__", _wrap_PySwigIterator___iadd__, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator___isub__", _wrap_PySwigIterator___isub__, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator___add__", _wrap_PySwigIterator___add__, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator___sub__", _wrap_PySwigIterator___sub__, METH_VARARGS, NULL},
	 { (char *)"PySwigIterator_swigregister", PySwigIterator_swigregister, METH_VARARGS, NULL},
	 { (char *)"Strings_iterator", _wrap_Strings_iterator, METH_VARARGS, NULL},
	 { (char *)"Strings___nonzero__", _wrap_Strings___nonzero__, METH_VARARGS, NULL},
	 { (char *)"Strings___len__", _wrap_Strings___len__, METH_VARARGS, NULL},
	 { (char *)"Strings_pop", _wrap_Strings_pop, METH_VARARGS, NULL},
	 { (char *)"Strings___getslice__", _wrap_Strings___getslice__, METH_VARARGS, NULL},
	 { (char *)"Strings___setslice__", _wrap_Strings___setslice__, METH_VARARGS, NULL},
	 { (char *)"Strings___delslice__", _wrap_Strings___delslice__, METH_VARARGS, NULL},
	 { (char *)"Strings___delitem__", _wrap_Strings___delitem__, METH_VARARGS, NULL},
	 { (char *)"Strings___getitem__", _wrap_Strings___getitem__, METH_VARARGS, NULL},
	 { (char *)"Strings___setitem__", _wrap_Strings___setitem__, METH_VARARGS, NULL},
	 { (char *)"Strings_append", _wrap_Strings_append, METH_VARARGS, NULL},
	 { (char *)"Strings_empty", _wrap_Strings_empty, METH_VARARGS, NULL},
	 { (char *)"Strings_size", _wrap_Strings_size, METH_VARARGS, NULL},
	 { (char *)"Strings_clear", _wrap_Strings_clear, METH_VARARGS, NULL},
	 { (char *)"Strings_swap", _wrap_Strings_swap, METH_VARARGS, NULL},
	 { (char *)"Strings_get_allocator", _wrap_Strings_get_allocator, METH_VARARGS, NULL},
	 { (char *)"Strings_begin", _wrap_Strings_begin, METH_VARARGS, NULL},
	 { (char *)"Strings_end", _wrap_Strings_end, METH_VARARGS, NULL},
	 { (char *)"Strings_rbegin", _wrap_Strings_rbegin, METH_VARARGS, NULL},
	 { (char *)"Strings_rend", _wrap_Strings_rend, METH_VARARGS, NULL},
	 { (char *)"Strings_pop_back", _wrap_Strings_pop_back, METH_VARARGS, NULL},
	 { (char *)"Strings_erase", _wrap_Strings_erase, METH_VARARGS, NULL},
	 { (char *)"new_Strings", _wrap_new_Strings, METH_VARARGS, NULL},
	 { (char *)"Strings_push_back", _wrap_Strings_push_back, METH_VARARGS, NULL},
	 { (char *)"Strings_front", _wrap_Strings_front, METH_VARARGS, NULL},
	 { (char *)"Strings_back", _wrap_Strings_back, METH_VARARGS, NULL},
	 { (char *)"Strings_assign", _wrap_Strings_assign, METH_VARARGS, NULL},
	 { (char *)"Strings_resize", _wrap_Strings_resize, METH_VARARGS, NULL},
	 { (char *)"Strings_insert", _wrap_Strings_insert, METH_VARARGS, NULL},
	 { (char *)"Strings_reserve", _wrap_Strings_reserve, METH_VARARGS, NULL},
	 { (char *)"Strings_capacity", _wrap_Strings_capacity, METH_VARARGS, NULL},
	 { (char *)"delete_Strings", _wrap_delete_Strings, METH_VARARGS, NULL},
	 { (char *)"Strings_swigregister", Strings_swigregister, METH_VARARGS, NULL},
	 { (char *)"Property_name_set", _wrap_Property_name_set, METH_VARARGS, NULL},
	 { (char *)"Property_name_get", _wrap_Property_name_get, METH_VARARGS, NULL},
	 { (char *)"Property_interp_set", _wrap_Property_interp_set, METH_VARARGS, NULL},
	 { (char *)"Property_interp_get", _wrap_Property_interp_get, METH_VARARGS, NULL},
	 { (char *)"Property_size_get", _wrap_Property_size_get, METH_VARARGS, NULL},
	 { (char *)"Property_width_get", _wrap_Property_width_get, METH_VARARGS, NULL},
	 { (char *)"Property_type_get", _wrap_Property_type_get, METH_VARARGS, NULL},
	 { (char *)"new_Property", _wrap_new_Property, METH_VARARGS, NULL},
	 { (char *)"Property___repr__", _wrap_Property___repr__, METH_VARARGS, NULL},
	 { (char *)"Property___cmp__", _wrap_Property___cmp__, METH_VARARGS, NULL},
	 { (char *)"Property___len__", _wrap_Property___len__, METH_VARARGS, NULL},
	 { (char *)"Property___call__", _wrap_Property___call__, METH_VARARGS, NULL},
	 { (char *)"Property___getitem__", _wrap_Property___getitem__, METH_VARARGS, NULL},
	 { (char *)"Property___setitem__", _wrap_Property___setitem__, METH_VARARGS, NULL},
	 { (char *)"Property___contains__", _wrap_Property___contains__, METH_VARARGS, NULL},
	 { (char *)"delete_Property", _wrap_delete_Property, METH_VARARGS, NULL},
	 { (char *)"Property_swigregister", Property_swigregister, METH_VARARGS, NULL},
	 { (char *)"Component_name_set", _wrap_Component_name_set, METH_VARARGS, NULL},
	 { (char *)"Component_name_get", _wrap_Component_name_get, METH_VARARGS, NULL},
	 { (char *)"Component_interp_set", _wrap_Component_interp_set, METH_VARARGS, NULL},
	 { (char *)"Component_interp_get", _wrap_Component_interp_get, METH_VARARGS, NULL},
	 { (char *)"Component_flags_set", _wrap_Component_flags_set, METH_VARARGS, NULL},
	 { (char *)"Component_flags_get", _wrap_Component_flags_get, METH_VARARGS, NULL},
	 { (char *)"new_Component", _wrap_new_Component, METH_VARARGS, NULL},
	 { (char *)"Component___repr__", _wrap_Component___repr__, METH_VARARGS, NULL},
	 { (char *)"Component___len__", _wrap_Component___len__, METH_VARARGS, NULL},
	 { (char *)"Component___cmp__", _wrap_Component___cmp__, METH_VARARGS, NULL},
	 { (char *)"Component___getitem__", _wrap_Component___getitem__, METH_VARARGS, NULL},
	 { (char *)"Component___setitem__", _wrap_Component___setitem__, METH_VARARGS, NULL},
	 { (char *)"Component___delitem__", _wrap_Component___delitem__, METH_VARARGS, NULL},
	 { (char *)"Component___contains__", _wrap_Component___contains__, METH_VARARGS, NULL},
	 { (char *)"Component_keys", _wrap_Component_keys, METH_VARARGS, NULL},
	 { (char *)"Component_values", _wrap_Component_values, METH_VARARGS, NULL},
	 { (char *)"Component_items", _wrap_Component_items, METH_VARARGS, NULL},
	 { (char *)"Component_append", _wrap_Component_append, METH_VARARGS, NULL},
	 { (char *)"Component_extend", _wrap_Component_extend, METH_VARARGS, NULL},
	 { (char *)"delete_Component", _wrap_delete_Component, METH_VARARGS, NULL},
	 { (char *)"Component_swigregister", Component_swigregister, METH_VARARGS, NULL},
	 { (char *)"new_Object", _wrap_new_Object, METH_VARARGS, NULL},
	 { (char *)"delete_Object", _wrap_delete_Object, METH_VARARGS, NULL},
	 { (char *)"Object_name_set", _wrap_Object_name_set, METH_VARARGS, NULL},
	 { (char *)"Object_name_get", _wrap_Object_name_get, METH_VARARGS, NULL},
	 { (char *)"Object_protocol_set", _wrap_Object_protocol_set, METH_VARARGS, NULL},
	 { (char *)"Object_protocol_get", _wrap_Object_protocol_get, METH_VARARGS, NULL},
	 { (char *)"Object_protocolVersion_set", _wrap_Object_protocolVersion_set, METH_VARARGS, NULL},
	 { (char *)"Object_protocolVersion_get", _wrap_Object_protocolVersion_get, METH_VARARGS, NULL},
	 { (char *)"Object___repr__", _wrap_Object___repr__, METH_VARARGS, NULL},
	 { (char *)"Object___len__", _wrap_Object___len__, METH_VARARGS, NULL},
	 { (char *)"Object___cmp__", _wrap_Object___cmp__, METH_VARARGS, NULL},
	 { (char *)"Object___getitem__", _wrap_Object___getitem__, METH_VARARGS, NULL},
	 { (char *)"Object___setitem__", _wrap_Object___setitem__, METH_VARARGS, NULL},
	 { (char *)"Object___delitem__", _wrap_Object___delitem__, METH_VARARGS, NULL},
	 { (char *)"Object___contains__", _wrap_Object___contains__, METH_VARARGS, NULL},
	 { (char *)"Object_keys", _wrap_Object_keys, METH_VARARGS, NULL},
	 { (char *)"Object_values", _wrap_Object_values, METH_VARARGS, NULL},
	 { (char *)"Object_append", _wrap_Object_append, METH_VARARGS, NULL},
	 { (char *)"Object_extend", _wrap_Object_extend, METH_VARARGS, NULL},
	 { (char *)"Object_swigregister", Object_swigregister, METH_VARARGS, NULL},
	 { (char *)"new_GtoDB", _wrap_new_GtoDB, METH_VARARGS, NULL},
	 { (char *)"GtoDB_read", _wrap_GtoDB_read, METH_VARARGS, NULL},
	 { (char *)"GtoDB_write", _wrap_GtoDB_write, METH_VARARGS, NULL},
	 { (char *)"GtoDB___len__", _wrap_GtoDB___len__, METH_VARARGS, NULL},
	 { (char *)"GtoDB___getitem__", _wrap_GtoDB___getitem__, METH_VARARGS, NULL},
	 { (char *)"GtoDB___setitem__", _wrap_GtoDB___setitem__, METH_VARARGS, NULL},
	 { (char *)"GtoDB___delitem__", _wrap_GtoDB___delitem__, METH_VARARGS, NULL},
	 { (char *)"GtoDB___contains__", _wrap_GtoDB___contains__, METH_VARARGS, NULL},
	 { (char *)"GtoDB_keys", _wrap_GtoDB_keys, METH_VARARGS, NULL},
	 { (char *)"GtoDB_values", _wrap_GtoDB_values, METH_VARARGS, NULL},
	 { (char *)"GtoDB_items", _wrap_GtoDB_items, METH_VARARGS, NULL},
	 { (char *)"GtoDB_append", _wrap_GtoDB_append, METH_VARARGS, NULL},
	 { (char *)"GtoDB_extend", _wrap_GtoDB_extend, METH_VARARGS, NULL},
	 { (char *)"GtoDB_clear", _wrap_GtoDB_clear, METH_VARARGS, NULL},
	 { (char *)"delete_GtoDB", _wrap_delete_GtoDB, METH_VARARGS, NULL},
	 { (char *)"GtoDB_swigregister", GtoDB_swigregister, METH_VARARGS, NULL},
	 { NULL, NULL, 0, NULL }
};


/* -------- TYPE CONVERSION AND EQUIVALENCE RULES (BEGIN) -------- */

static swig_type_info _swigt__p_Component = {"_p_Component", "Component *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_Components = {"_p_Components", "Components *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_GtoDB = {"_p_GtoDB", "GtoDB *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_Object = {"_p_Object", "Object *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_Objects = {"_p_Objects", "Objects *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_Properties = {"_p_Properties", "Properties *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_Property = {"_p_Property", "Property *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_allocator_type = {"_p_allocator_type", "allocator_type *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_char = {"_p_char", "char *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_difference_type = {"_p_difference_type", "difference_type *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_double = {"_p_double", "double *|Gto::float64 *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_float = {"_p_float", "float *|Gto::float32 *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_int = {"_p_int", "int *|Gto::int32 *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_p_PyObject = {"_p_p_PyObject", "PyObject **", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_size_type = {"_p_size_type", "size_type *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_std__invalid_argument = {"_p_std__invalid_argument", "std::invalid_argument *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_std__vectorTstd__string_std__allocatorTstd__string_t_t = {"_p_std__vectorTstd__string_std__allocatorTstd__string_t_t", "std::vector<std::string > *|std::vector<std::string,std::allocator<std::string > > *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_std__vectorTstd__string_std__allocatorTstd__string_t_t__allocator_type = {"_p_std__vectorTstd__string_std__allocatorTstd__string_t_t__allocator_type", "std::allocator<std::string > *|std::vector<std::string >::allocator_type *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_swig__PySwigIterator = {"_p_swig__PySwigIterator", "swig::PySwigIterator *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_unsigned_char = {"_p_unsigned_char", "unsigned char *|Gto::uint8 *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_unsigned_int = {"_p_unsigned_int", "unsigned int *|Gto::uint32 *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_unsigned_short = {"_p_unsigned_short", "unsigned short *|Gto::uint16 *", 0, 0, (void*)0, 0};
static swig_type_info _swigt__p_value_type = {"_p_value_type", "value_type *", 0, 0, (void*)0, 0};

static swig_type_info *swig_type_initial[] = {
  &_swigt__p_Component,
  &_swigt__p_Components,
  &_swigt__p_GtoDB,
  &_swigt__p_Object,
  &_swigt__p_Objects,
  &_swigt__p_Properties,
  &_swigt__p_Property,
  &_swigt__p_allocator_type,
  &_swigt__p_char,
  &_swigt__p_difference_type,
  &_swigt__p_double,
  &_swigt__p_float,
  &_swigt__p_int,
  &_swigt__p_p_PyObject,
  &_swigt__p_size_type,
  &_swigt__p_std__invalid_argument,
  &_swigt__p_std__vectorTstd__string_std__allocatorTstd__string_t_t,
  &_swigt__p_std__vectorTstd__string_std__allocatorTstd__string_t_t__allocator_type,
  &_swigt__p_swig__PySwigIterator,
  &_swigt__p_unsigned_char,
  &_swigt__p_unsigned_int,
  &_swigt__p_unsigned_short,
  &_swigt__p_value_type,
};

static swig_cast_info _swigc__p_Component[] = {  {&_swigt__p_Component, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_Components[] = {  {&_swigt__p_Components, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_GtoDB[] = {  {&_swigt__p_GtoDB, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_Object[] = {  {&_swigt__p_Object, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_Objects[] = {  {&_swigt__p_Objects, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_Properties[] = {  {&_swigt__p_Properties, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_Property[] = {  {&_swigt__p_Property, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_allocator_type[] = {  {&_swigt__p_allocator_type, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_char[] = {  {&_swigt__p_char, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_difference_type[] = {  {&_swigt__p_difference_type, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_double[] = {  {&_swigt__p_double, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_float[] = {  {&_swigt__p_float, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_int[] = {  {&_swigt__p_int, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_p_PyObject[] = {  {&_swigt__p_p_PyObject, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_size_type[] = {  {&_swigt__p_size_type, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_std__invalid_argument[] = {  {&_swigt__p_std__invalid_argument, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_std__vectorTstd__string_std__allocatorTstd__string_t_t[] = {  {&_swigt__p_std__vectorTstd__string_std__allocatorTstd__string_t_t, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_std__vectorTstd__string_std__allocatorTstd__string_t_t__allocator_type[] = {  {&_swigt__p_std__vectorTstd__string_std__allocatorTstd__string_t_t__allocator_type, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_swig__PySwigIterator[] = {  {&_swigt__p_swig__PySwigIterator, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_unsigned_char[] = {  {&_swigt__p_unsigned_char, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_unsigned_int[] = {  {&_swigt__p_unsigned_int, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_unsigned_short[] = {  {&_swigt__p_unsigned_short, 0, 0, 0},{0, 0, 0, 0}};
static swig_cast_info _swigc__p_value_type[] = {  {&_swigt__p_value_type, 0, 0, 0},{0, 0, 0, 0}};

static swig_cast_info *swig_cast_initial[] = {
  _swigc__p_Component,
  _swigc__p_Components,
  _swigc__p_GtoDB,
  _swigc__p_Object,
  _swigc__p_Objects,
  _swigc__p_Properties,
  _swigc__p_Property,
  _swigc__p_allocator_type,
  _swigc__p_char,
  _swigc__p_difference_type,
  _swigc__p_double,
  _swigc__p_float,
  _swigc__p_int,
  _swigc__p_p_PyObject,
  _swigc__p_size_type,
  _swigc__p_std__invalid_argument,
  _swigc__p_std__vectorTstd__string_std__allocatorTstd__string_t_t,
  _swigc__p_std__vectorTstd__string_std__allocatorTstd__string_t_t__allocator_type,
  _swigc__p_swig__PySwigIterator,
  _swigc__p_unsigned_char,
  _swigc__p_unsigned_int,
  _swigc__p_unsigned_short,
  _swigc__p_value_type,
};


/* -------- TYPE CONVERSION AND EQUIVALENCE RULES (END) -------- */

static swig_const_info swig_const_table[] = {
{0, 0, 0, 0.0, 0, 0}};

#ifdef __cplusplus
}
#endif
/* -----------------------------------------------------------------------------
 * Type initialization:
 * This problem is tough by the requirement that no dynamic 
 * memory is used. Also, since swig_type_info structures store pointers to 
 * swig_cast_info structures and swig_cast_info structures store pointers back
 * to swig_type_info structures, we need some lookup code at initialization. 
 * The idea is that swig generates all the structures that are needed. 
 * The runtime then collects these partially filled structures. 
 * The SWIG_InitializeModule function takes these initial arrays out of 
 * swig_module, and does all the lookup, filling in the swig_module.types
 * array with the correct data and linking the correct swig_cast_info
 * structures together.
 *
 * The generated swig_type_info structures are assigned staticly to an initial 
 * array. We just loop through that array, and handle each type individually.
 * First we lookup if this type has been already loaded, and if so, use the
 * loaded structure instead of the generated one. Then we have to fill in the
 * cast linked list. The cast data is initially stored in something like a
 * two-dimensional array. Each row corresponds to a type (there are the same
 * number of rows as there are in the swig_type_initial array). Each entry in
 * a column is one of the swig_cast_info structures for that type.
 * The cast_initial array is actually an array of arrays, because each row has
 * a variable number of columns. So to actually build the cast linked list,
 * we find the array of casts associated with the type, and loop through it 
 * adding the casts to the list. The one last trick we need to do is making
 * sure the type pointer in the swig_cast_info struct is correct.
 *
 * First off, we lookup the cast->type name to see if it is already loaded. 
 * There are three cases to handle:
 *  1) If the cast->type has already been loaded AND the type we are adding
 *     casting info to has not been loaded (it is in this module), THEN we
 *     replace the cast->type pointer with the type pointer that has already
 *     been loaded.
 *  2) If BOTH types (the one we are adding casting info to, and the 
 *     cast->type) are loaded, THEN the cast info has already been loaded by
 *     the previous module so we just ignore it.
 *  3) Finally, if cast->type has not already been loaded, then we add that
 *     swig_cast_info to the linked list (because the cast->type) pointer will
 *     be correct.
 * ----------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#if 0
} /* c-mode */
#endif
#endif

#if 0
#define SWIGRUNTIME_DEBUG
#endif


SWIGRUNTIME void
SWIG_InitializeModule(void *clientdata) {
  size_t i;
  swig_module_info *module_head, *iter;
  int found;
  
  clientdata = clientdata;
  
  /* check to see if the circular list has been setup, if not, set it up */
  if (swig_module.next==0) {
    /* Initialize the swig_module */
    swig_module.type_initial = swig_type_initial;
    swig_module.cast_initial = swig_cast_initial;
    swig_module.next = &swig_module;
  }
  
  /* Try and load any already created modules */
  module_head = SWIG_GetModule(clientdata);
  if (!module_head) {
    /* This is the first module loaded for this interpreter */
    /* so set the swig module into the interpreter */
    SWIG_SetModule(clientdata, &swig_module);
    module_head = &swig_module;
  } else {
    /* the interpreter has loaded a SWIG module, but has it loaded this one? */
    found=0;
    iter=module_head;
    do {
      if (iter==&swig_module) {
        found=1;
        break;
      }
      iter=iter->next;
    } while (iter!= module_head);
    
    /* if the is found in the list, then all is done and we may leave */
    if (found) return;
    /* otherwise we must add out module into the list */
    swig_module.next = module_head->next;
    module_head->next = &swig_module;
  }
  
  /* Now work on filling in swig_module.types */
#ifdef SWIGRUNTIME_DEBUG
  printf("SWIG_InitializeModule: size %d\n", swig_module.size);
#endif
  for (i = 0; i < swig_module.size; ++i) {
    swig_type_info *type = 0;
    swig_type_info *ret;
    swig_cast_info *cast;
    
#ifdef SWIGRUNTIME_DEBUG
    printf("SWIG_InitializeModule: type %d %s\n", i, swig_module.type_initial[i]->name);
#endif
    
    /* if there is another module already loaded */
    if (swig_module.next != &swig_module) {
      type = SWIG_MangledTypeQueryModule(swig_module.next, &swig_module, swig_module.type_initial[i]->name);
    }
    if (type) {
      /* Overwrite clientdata field */
#ifdef SWIGRUNTIME_DEBUG
      printf("SWIG_InitializeModule: found type %s\n", type->name);
#endif
      if (swig_module.type_initial[i]->clientdata) {
        type->clientdata = swig_module.type_initial[i]->clientdata;
#ifdef SWIGRUNTIME_DEBUG
        printf("SWIG_InitializeModule: found and overwrite type %s \n", type->name);
#endif
      }
    } else {
      type = swig_module.type_initial[i];
    }
    
    /* Insert casting types */
    cast = swig_module.cast_initial[i];
    while (cast->type) {
      /* Don't need to add information already in the list */
      ret = 0;
#ifdef SWIGRUNTIME_DEBUG
      printf("SWIG_InitializeModule: look cast %s\n", cast->type->name);
#endif
      if (swig_module.next != &swig_module) {
        ret = SWIG_MangledTypeQueryModule(swig_module.next, &swig_module, cast->type->name);
#ifdef SWIGRUNTIME_DEBUG
        if (ret) printf("SWIG_InitializeModule: found cast %s\n", ret->name);
#endif
      }
      if (ret) {
        if (type == swig_module.type_initial[i]) {
#ifdef SWIGRUNTIME_DEBUG
          printf("SWIG_InitializeModule: skip old type %s\n", ret->name);
#endif
          cast->type = ret;
          ret = 0;
        } else {
          /* Check for casting already in the list */
          swig_cast_info *ocast = SWIG_TypeCheck(ret->name, type);
#ifdef SWIGRUNTIME_DEBUG
          if (ocast) printf("SWIG_InitializeModule: skip old cast %s\n", ret->name);
#endif
          if (!ocast) ret = 0;
        }
      }
      
      if (!ret) {
#ifdef SWIGRUNTIME_DEBUG
        printf("SWIG_InitializeModule: adding cast %s\n", cast->type->name);
#endif
        if (type->cast) {
          type->cast->prev = cast;
          cast->next = type->cast;
        }
        type->cast = cast;
      }
      cast++;
    }
    /* Set entry in modules->types array equal to the type */
    swig_module.types[i] = type;
  }
  swig_module.types[i] = 0;
  
#ifdef SWIGRUNTIME_DEBUG
  printf("**** SWIG_InitializeModule: Cast List ******\n");
  for (i = 0; i < swig_module.size; ++i) {
    int j = 0;
    swig_cast_info *cast = swig_module.cast_initial[i];
    printf("SWIG_InitializeModule: type %d %s\n", i, swig_module.type_initial[i]->name);
    while (cast->type) {
      printf("SWIG_InitializeModule: cast type %s\n", cast->type->name);
      cast++;
      ++j;
    }
    printf("---- Total casts: %d\n",j);
  }
  printf("**** SWIG_InitializeModule: Cast List ******\n");
#endif
}

/* This function will propagate the clientdata field of type to
* any new swig_type_info structures that have been added into the list
* of equivalent types.  It is like calling
* SWIG_TypeClientData(type, clientdata) a second time.
*/
SWIGRUNTIME void
SWIG_PropagateClientData(void) {
  size_t i;
  swig_cast_info *equiv;
  static int init_run = 0;
  
  if (init_run) return;
  init_run = 1;
  
  for (i = 0; i < swig_module.size; i++) {
    if (swig_module.types[i]->clientdata) {
      equiv = swig_module.types[i]->cast;
      while (equiv) {
        if (!equiv->converter) {
          if (equiv->type && !equiv->type->clientdata)
          SWIG_TypeClientData(equiv->type, swig_module.types[i]->clientdata);
        }
        equiv = equiv->next;
      }
    }
  }
}

#ifdef __cplusplus
#if 0
{
  /* c-mode */
#endif
}
#endif



#ifdef __cplusplus
extern "C" {
#endif
  
  /* Python-specific SWIG API */
#define SWIG_newvarlink()                             SWIG_Python_newvarlink()
#define SWIG_addvarlink(p, name, get_attr, set_attr)  SWIG_Python_addvarlink(p, name, get_attr, set_attr)
#define SWIG_InstallConstants(d, constants)           SWIG_Python_InstallConstants(d, constants)
  
  /* -----------------------------------------------------------------------------
   * global variable support code.
   * ----------------------------------------------------------------------------- */
  
  typedef struct swig_globalvar {
    char       *name;                  /* Name of global variable */
    PyObject *(*get_attr)(void);       /* Return the current value */
    int       (*set_attr)(PyObject *); /* Set the value */
    struct swig_globalvar *next;
  } swig_globalvar;
  
  typedef struct swig_varlinkobject {
    PyObject_HEAD
    swig_globalvar *vars;
  } swig_varlinkobject;
  
  SWIGINTERN PyObject *
  swig_varlink_repr(swig_varlinkobject *SWIGUNUSEDPARM(v)) {
    return PyString_FromString("<Swig global variables>");
  }
  
  SWIGINTERN PyObject *
  swig_varlink_str(swig_varlinkobject *v) {
    PyObject *str = PyString_FromString("(");
    swig_globalvar  *var;
    for (var = v->vars; var; var=var->next) {
      PyString_ConcatAndDel(&str,PyString_FromString(var->name));
      if (var->next) PyString_ConcatAndDel(&str,PyString_FromString(", "));
    }
    PyString_ConcatAndDel(&str,PyString_FromString(")"));
    return str;
  }
  
  SWIGINTERN int
  swig_varlink_print(swig_varlinkobject *v, FILE *fp, int SWIGUNUSEDPARM(flags)) {
    PyObject *str = swig_varlink_str(v);
    fprintf(fp,"Swig global variables ");
    fprintf(fp,"%s\n", PyString_AsString(str));
    Py_DECREF(str);
    return 0;
  }
  
  SWIGINTERN void
  swig_varlink_dealloc(swig_varlinkobject *v) {
    swig_globalvar *var = v->vars;
    while (var) {
      swig_globalvar *n = var->next;
      free(var->name);
      free(var);
      var = n;
    }
  }
  
  SWIGINTERN PyObject *
  swig_varlink_getattr(swig_varlinkobject *v, char *n) {
    PyObject *res = NULL;
    swig_globalvar *var = v->vars;
    while (var) {
      if (strcmp(var->name,n) == 0) {
        res = (*var->get_attr)();
        break;
      }
      var = var->next;
    }
    if (res == NULL && !PyErr_Occurred()) {
      PyErr_SetString(PyExc_NameError,"Unknown C global variable");
    }
    return res;
  }
  
  SWIGINTERN int
  swig_varlink_setattr(swig_varlinkobject *v, char *n, PyObject *p) {
    int res = 1;
    swig_globalvar *var = v->vars;
    while (var) {
      if (strcmp(var->name,n) == 0) {
        res = (*var->set_attr)(p);
        break;
      }
      var = var->next;
    }
    if (res == 1 && !PyErr_Occurred()) {
      PyErr_SetString(PyExc_NameError,"Unknown C global variable");
    }
    return res;
  }
  
  SWIGINTERN PyTypeObject*
  swig_varlink_type(void) {
    static char varlink__doc__[] = "Swig var link object";
    static PyTypeObject varlink_type;
    static int type_init = 0;  
    if (!type_init) {
      const PyTypeObject tmp
      = {
        PyObject_HEAD_INIT(NULL)
        0,                                  /* Number of items in variable part (ob_size) */
        (char *)"swigvarlink",              /* Type name (tp_name) */
        sizeof(swig_varlinkobject),         /* Basic size (tp_basicsize) */
        0,                                  /* Itemsize (tp_itemsize) */
        (destructor) swig_varlink_dealloc,   /* Deallocator (tp_dealloc) */ 
        (printfunc) swig_varlink_print,     /* Print (tp_print) */
        (getattrfunc) swig_varlink_getattr, /* get attr (tp_getattr) */
        (setattrfunc) swig_varlink_setattr, /* Set attr (tp_setattr) */
        0,                                  /* tp_compare */
        (reprfunc) swig_varlink_repr,       /* tp_repr */
        0,                                  /* tp_as_number */
        0,                                  /* tp_as_sequence */
        0,                                  /* tp_as_mapping */
        0,                                  /* tp_hash */
        0,                                  /* tp_call */
        (reprfunc)swig_varlink_str,        /* tp_str */
        0,                                  /* tp_getattro */
        0,                                  /* tp_setattro */
        0,                                  /* tp_as_buffer */
        0,                                  /* tp_flags */
        varlink__doc__,                     /* tp_doc */
        0,                                  /* tp_traverse */
        0,                                  /* tp_clear */
        0,                                  /* tp_richcompare */
        0,                                  /* tp_weaklistoffset */
#if PY_VERSION_HEX >= 0x02020000
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* tp_iter -> tp_weaklist */
#endif
#if PY_VERSION_HEX >= 0x02030000
        0,                                  /* tp_del */
#endif
#ifdef COUNT_ALLOCS
        0,0,0,0                             /* tp_alloc -> tp_next */
#endif
      };
      varlink_type = tmp;
      varlink_type.ob_type = &PyType_Type;
      type_init = 1;
    }
    return &varlink_type;
  }
  
  /* Create a variable linking object for use later */
  SWIGINTERN PyObject *
  SWIG_Python_newvarlink(void) {
    swig_varlinkobject *result = PyObject_NEW(swig_varlinkobject, swig_varlink_type());
    if (result) {
      result->vars = 0;
    }
    return ((PyObject*) result);
  }
  
  SWIGINTERN void 
  SWIG_Python_addvarlink(PyObject *p, char *name, PyObject *(*get_attr)(void), int (*set_attr)(PyObject *p)) {
    swig_varlinkobject *v = (swig_varlinkobject *) p;
    swig_globalvar *gv = (swig_globalvar *) malloc(sizeof(swig_globalvar));
    if (gv) {
      size_t size = strlen(name)+1;
      gv->name = (char *)malloc(size);
      if (gv->name) {
        strncpy(gv->name,name,size);
        gv->get_attr = get_attr;
        gv->set_attr = set_attr;
        gv->next = v->vars;
      }
    }
    v->vars = gv;
  }
  
  SWIGINTERN PyObject *
  SWIG_globals(void) {
    static PyObject *_SWIG_globals = 0; 
    if (!_SWIG_globals) _SWIG_globals = SWIG_newvarlink();  
    return _SWIG_globals;
  }
  
  /* -----------------------------------------------------------------------------
   * constants/methods manipulation
   * ----------------------------------------------------------------------------- */
  
  /* Install Constants */
  SWIGINTERN void
  SWIG_Python_InstallConstants(PyObject *d, swig_const_info constants[]) {
    PyObject *obj = 0;
    size_t i;
    for (i = 0; constants[i].type; ++i) {
      switch(constants[i].type) {
      case SWIG_PY_POINTER:
        obj = SWIG_NewPointerObj(constants[i].pvalue, *(constants[i]).ptype,0);
        break;
      case SWIG_PY_BINARY:
        obj = SWIG_NewPackedObj(constants[i].pvalue, constants[i].lvalue, *(constants[i].ptype));
        break;
      default:
        obj = 0;
        break;
      }
      if (obj) {
        PyDict_SetItemString(d, constants[i].name, obj);
        Py_DECREF(obj);
      }
    }
  }
  
  /* -----------------------------------------------------------------------------*/
  /* Fix SwigMethods to carry the callback ptrs when needed */
  /* -----------------------------------------------------------------------------*/
  
  SWIGINTERN void
  SWIG_Python_FixMethods(PyMethodDef *methods,
    swig_const_info *const_table,
    swig_type_info **types,
    swig_type_info **types_initial) {
    size_t i;
    for (i = 0; methods[i].ml_name; ++i) {
      const char *c = methods[i].ml_doc;
      if (c && (c = strstr(c, "swig_ptr: "))) {
        int j;
        swig_const_info *ci = 0;
        const char *name = c + 10;
        for (j = 0; const_table[j].type; ++j) {
          if (strncmp(const_table[j].name, name, 
              strlen(const_table[j].name)) == 0) {
            ci = &(const_table[j]);
            break;
          }
        }
        if (ci) {
          size_t shift = (ci->ptype) - types;
          swig_type_info *ty = types_initial[shift];
          size_t ldoc = (c - methods[i].ml_doc);
          size_t lptr = strlen(ty->name)+2*sizeof(void*)+2;
          char *ndoc = (char*)malloc(ldoc + lptr + 10);
          if (ndoc) {
            char *buff = ndoc;
            void *ptr = (ci->type == SWIG_PY_POINTER) ? ci->pvalue : 0;
            if (ptr) {
              strncpy(buff, methods[i].ml_doc, ldoc);
              buff += ldoc;
              strncpy(buff, "swig_ptr: ", 10);
              buff += 10;
              SWIG_PackVoidPtr(buff, ptr, ty->name, lptr);
              methods[i].ml_doc = ndoc;
            }
          }
        }
      }
    }
  } 
  
#ifdef __cplusplus
}
#endif

/* -----------------------------------------------------------------------------*
 *  Partial Init method
 * -----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"
#endif
SWIGEXPORT void SWIG_init(void) {
  PyObject *m, *d;
  
  /* Fix SwigMethods to carry the callback ptrs when needed */
  SWIG_Python_FixMethods(SwigMethods, swig_const_table, swig_types, swig_type_initial);
  
  m = Py_InitModule((char *) SWIG_name, SwigMethods);
  d = PyModule_GetDict(m);
  
  SWIG_InitializeModule(0);
  SWIG_InstallConstants(d,swig_const_table);
  
  
  SWIG_Python_SetConstant(d, "GTO_MAGIC",SWIG_From_int(static_cast< int >(0x29f)));
  SWIG_Python_SetConstant(d, "GTO_MAGICl",SWIG_From_int(static_cast< int >(0x9f020000)));
  SWIG_Python_SetConstant(d, "GTO_MAGIC_TEXT",SWIG_From_int(static_cast< int >(0x47544f61)));
  SWIG_Python_SetConstant(d, "GTO_MAGIC_TEXTl",SWIG_From_int(static_cast< int >(0x614f5447)));
  SWIG_Python_SetConstant(d, "GTO_VERSION",SWIG_From_int(static_cast< int >(3)));
  SWIG_Python_SetConstant(d, "Transposed",SWIG_From_int(static_cast< int >(Gto::Transposed)));
  SWIG_Python_SetConstant(d, "Matrix",SWIG_From_int(static_cast< int >(Gto::Matrix)));
  SWIG_Python_SetConstant(d, "Int",SWIG_From_int(static_cast< int >(Gto::Int)));
  SWIG_Python_SetConstant(d, "Float",SWIG_From_int(static_cast< int >(Gto::Float)));
  SWIG_Python_SetConstant(d, "Double",SWIG_From_int(static_cast< int >(Gto::Double)));
  SWIG_Python_SetConstant(d, "Half",SWIG_From_int(static_cast< int >(Gto::Half)));
  SWIG_Python_SetConstant(d, "String",SWIG_From_int(static_cast< int >(Gto::String)));
  SWIG_Python_SetConstant(d, "Boolean",SWIG_From_int(static_cast< int >(Gto::Boolean)));
  SWIG_Python_SetConstant(d, "Short",SWIG_From_int(static_cast< int >(Gto::Short)));
  SWIG_Python_SetConstant(d, "Byte",SWIG_From_int(static_cast< int >(Gto::Byte)));
  SWIG_Python_SetConstant(d, "NumberOfDataTypes",SWIG_From_int(static_cast< int >(Gto::NumberOfDataTypes)));
  SWIG_Python_SetConstant(d, "ErrorType",SWIG_From_int(static_cast< int >(Gto::ErrorType)));
  SWIG_Python_SetConstant(d, "GtoDB_BinaryGTO",SWIG_From_int(static_cast< int >(GtoDB::BinaryGTO)));
  SWIG_Python_SetConstant(d, "GtoDB_CompressedGTO",SWIG_From_int(static_cast< int >(GtoDB::CompressedGTO)));
  SWIG_Python_SetConstant(d, "GtoDB_TextGTO",SWIG_From_int(static_cast< int >(GtoDB::TextGTO)));
}

