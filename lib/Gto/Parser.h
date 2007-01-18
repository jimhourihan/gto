/* A Bison parser, made by GNU Bison 2.0.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     STRINGCONST = 258,
     INTCONST = 259,
     FLOATCONST = 260,
     GTOID = 261,
     AS = 262,
     INTTYPE = 263,
     FLOATTYPE = 264,
     DOUBLETYPE = 265,
     STRINGTYPE = 266,
     HALFTYPE = 267,
     BOOLTYPE = 268,
     SHORTTYPE = 269,
     BYTETYPE = 270,
     ELLIPSIS = 271
   };
#endif
#define STRINGCONST 258
#define INTCONST 259
#define FLOATCONST 260
#define GTOID 261
#define AS 262
#define INTTYPE 263
#define FLOATTYPE 264
#define DOUBLETYPE 265
#define STRINGTYPE 266
#define HALFTYPE 267
#define BOOLTYPE 268
#define SHORTTYPE 269
#define BYTETYPE 270
#define ELLIPSIS 271




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 43 "Parser.y"
typedef union YYSTYPE {
    int             _token;
    int             _int;
    double          _double;
    Gto::TypeSpec   _type;
    Gto::Number     _number;
} YYSTYPE;
/* Line 1318 of yacc.c.  */
#line 77 "Parser.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif





