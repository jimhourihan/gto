
/*  A Bison parser, made from Grammar.y
    by GNU Bison version 1.28  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse wfobjparse
#define yylex wfobjlex
#define yyerror wfobjerror
#define yylval wfobjlval
#define yychar wfobjchar
#define yydebug wfobjdebug
#define yynerrs wfobjnerrs
#define	VERTEX	257
#define	VNORMAL	258
#define	VTEXTURE	259
#define	FACE	260
#define	LINE	261
#define	POINT	262
#define	GROUP	263
#define	SGROUP	264
#define	OGROUP	265
#define	BEVEL	266
#define	CINTERP	267
#define	DINTERP	268
#define	USEMTL	269
#define	USEMAP	270
#define	MAPLIB	271
#define	MTLLIB	272
#define	LOD	273
#define	SHADOW_OBJ	274
#define	TRACE_OBJ	275
#define	NAME	276
#define	ENDLINE	277
#define	SCALAR	278
#define	INTEGER	279

#line 1 "Grammar.y"

/*
// Copyright (c) 2002 Tweak Films
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
*/

#include <vector>
#include <string>
#include "Reader.h"

#define yyLineNum wfobjLineNum
#define yyReader wfobjReader
#define YYDEBUG 1

extern int yylex();
extern int yyLineNum;

using namespace std;

struct Parser_State
{
    float			vector[4];
    int				vector_size;

    int				element_type;
    std::vector<int>		vertex_index;
    std::vector<int>		normal_index;
    std::vector<int>		texture_index;
    int				index_mask;
    int				index_error;

    std::vector<std::string>	groups;
    std::vector<std::string>	active_groups;
    std::string			active_object;
    int				smoothing_group;
    int				lod;

    int				bevel;
    int				cinterp;
    int				dinterp;
    std::string			mtl;
    std::string			map;
    std::string			mtllib;
    std::string			shadow_obj;
    std::string			trace_obj;

    int				num_vertices;
    int				num_texture_vertices;
    int				num_normals;
    int				num_param_vertices;
};

static Parser_State state;
WFObj::Reader *yyReader;

static void add_group(const char*);
static void smoothing_group(int);
static int  smoothing_group_off(const char*);
static void group_statement();
static void clear_groups();
static void clear_vertex();
static void add_to_vertex(float);
static int  vertex(int);
static int  element(int);
static void clear_element();
static int  index(int,int,int,int);
static void active_object(const char*);
static int  on_off_only(const char*);
void yyerror(const char*);


#line 90 "Grammar.y"
typedef union
{
    int		_token;
    float	_float;
    int		_int;
    int		_flags;
    const char*	_name;
} YYSTYPE;
#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		72
#define	YYFLAG		-32768
#define	YYNTBASE	27

#define YYTRANSLATE(x) ((unsigned)(x) <= 279 ? yytranslate[x] : 45)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,    26,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     3,     4,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
    17,    18,    19,    20,    21,    22,    23,    24,    25
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     7,    10,    13,    14,    18,    19,    23,
    24,    28,    29,    33,    34,    38,    39,    43,    44,    48,
    51,    54,    57,    60,    63,    66,    69,    72,    75,    78,
    81,    84,    86,    89,    91,    95,   100,   106,   108,   111,
   113,   115,   117,   118,   120,   121,   123,   125
};

static const short yyrhs[] = {    28,
     0,    27,    28,     0,    23,     0,    29,    23,     0,     1,
    23,     0,     0,     3,    30,    39,     0,     0,     4,    31,
    39,     0,     0,     5,    32,    39,     0,     0,     6,    33,
    37,     0,     0,     7,    34,    37,     0,     0,     8,    35,
    37,     0,     0,     9,    36,    44,     0,    10,    42,     0,
    10,    22,     0,    11,    22,     0,    12,    22,     0,    13,
    22,     0,    14,    22,     0,    15,    22,     0,    16,    22,
     0,    18,    22,     0,    20,    22,     0,    21,    22,     0,
    19,    25,     0,    38,     0,    37,    38,     0,    41,     0,
    41,    26,    41,     0,    41,    26,    26,    41,     0,    41,
    26,    41,    26,    41,     0,    40,     0,    39,    40,     0,
    24,     0,    25,     0,    25,     0,     0,    25,     0,     0,
    22,     0,    43,     0,    44,    22,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   139,   141,   144,   146,   147,   150,   153,   157,   158,   162,
   163,   167,   168,   172,   173,   177,   178,   182,   183,   187,
   192,   197,   202,   208,   214,   220,   226,   232,   238,   244,
   250,   257,   259,   262,   269,   275,   281,   288,   290,   293,
   295,   298,   302,   304,   307,   309,   312,   314
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","VERTEX",
"VNORMAL","VTEXTURE","FACE","LINE","POINT","GROUP","SGROUP","OGROUP","BEVEL",
"CINTERP","DINTERP","USEMTL","USEMAP","MAPLIB","MTLLIB","LOD","SHADOW_OBJ","TRACE_OBJ",
"NAME","ENDLINE","SCALAR","INTEGER","'/'","lines","line","statement","@1","@2",
"@3","@4","@5","@6","@7","element_list","element","vertex_list","scalar","integer",
"integer_opt","name_opt","name_list", NULL
};
#endif

static const short yyr1[] = {     0,
    27,    27,    28,    28,    28,    30,    29,    31,    29,    32,
    29,    33,    29,    34,    29,    35,    29,    36,    29,    29,
    29,    29,    29,    29,    29,    29,    29,    29,    29,    29,
    29,    37,    37,    38,    38,    38,    38,    39,    39,    40,
    40,    41,    42,    42,    43,    43,    44,    44
};

static const short yyr2[] = {     0,
     1,     2,     1,     2,     2,     0,     3,     0,     3,     0,
     3,     0,     3,     0,     3,     0,     3,     0,     3,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     1,     2,     1,     3,     4,     5,     1,     2,     1,
     1,     1,     0,     1,     0,     1,     1,     2
};

static const short yydefact[] = {     0,
     0,     6,     8,    10,    12,    14,    16,    18,    43,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     3,
     0,     1,     0,     5,     0,     0,     0,     0,     0,     0,
    45,    21,    44,    20,    22,    23,    24,    25,    26,    27,
    28,    31,    29,    30,     2,     4,    40,    41,     7,    38,
     9,    11,    42,    13,    32,    34,    15,    17,    46,    47,
    19,    39,    33,     0,    48,     0,    35,    36,     0,    37,
     0,     0
};

static const short yydefgoto[] = {    21,
    22,    23,    25,    26,    27,    28,    29,    30,    31,    54,
    55,    49,    50,    56,    34,    60,    61
};

static const short yypact[] = {    21,
    -6,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    30,   -20,
    16,    25,    42,    43,    44,    45,    46,    47,    48,-32768,
     0,-32768,    49,-32768,    32,    32,    32,    50,    50,    50,
    51,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    32,-32768,
    32,    32,-32768,    50,-32768,    52,    50,    50,-32768,-32768,
    54,-32768,-32768,    33,-32768,    50,    53,-32768,    50,-32768,
    68,-32768
};

static const short yypgoto[] = {-32768,
    56,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    31,
    -8,    36,     2,   -21,-32768,-32768,-32768
};


#define	YYLAST		79


static const short yytable[] = {    71,
     1,    35,     2,     3,     4,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    24,    16,    17,    18,
    19,     1,    20,     2,     3,     4,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,    36,    16,    17,
    18,    19,    67,    20,    68,    63,    37,    70,    63,    63,
    62,    32,    62,    62,    33,    47,    48,    53,    66,    57,
    58,    51,    52,    38,    39,    40,    41,    72,    43,    44,
    42,    46,    59,     0,    53,    65,    45,    64,    69
};

static const short yycheck[] = {     0,
     1,    22,     3,     4,     5,     6,     7,     8,     9,    10,
    11,    12,    13,    14,    15,    16,    23,    18,    19,    20,
    21,     1,    23,     3,     4,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    22,    18,    19,
    20,    21,    64,    23,    66,    54,    22,    69,    57,    58,
    49,    22,    51,    52,    25,    24,    25,    25,    26,    29,
    30,    26,    27,    22,    22,    22,    22,     0,    22,    22,
    25,    23,    22,    -1,    25,    22,    21,    26,    26
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/usr/share/bison.simple"
/* This file comes from bison-1.28.  */

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

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

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

#ifndef YYSTACK_USE_ALLOCA
#ifdef alloca
#define YYSTACK_USE_ALLOCA
#else /* alloca not defined */
#ifdef __GNUC__
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi) || (defined (__sun) && defined (__i386))
#define YYSTACK_USE_ALLOCA
#include <alloca.h>
#else /* not sparc */
/* We think this test detects Watcom and Microsoft C.  */
/* This used to test MSDOS, but that is a bad idea
   since that symbol is in the user namespace.  */
#if (defined (_MSDOS) || defined (_MSDOS_)) && !defined (__TURBOC__)
#if 0 /* No need for malloc.h, which pollutes the namespace;
	 instead, just don't use alloca.  */
#include <malloc.h>
#endif
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
/* I don't know what this was needed for, but it pollutes the namespace.
   So I turned it off.   rms, 2 May 1997.  */
/* #include <malloc.h>  */
 #pragma alloca
#define YYSTACK_USE_ALLOCA
#else /* not MSDOS, or __TURBOC__, or _AIX */
#if 0
#ifdef __hpux /* haible@ilog.fr says this works for HPUX 9.05 and up,
		 and on HPUX 10.  Eventually we can turn this on.  */
#define YYSTACK_USE_ALLOCA
#define alloca __builtin_alloca
#endif /* __hpux */
#endif
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc */
#endif /* not GNU C */
#endif /* alloca not defined */
#endif /* YYSTACK_USE_ALLOCA not defined */

#ifdef YYSTACK_USE_ALLOCA
#define YYSTACK_ALLOC alloca
#else
#define YYSTACK_ALLOC malloc
#endif

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	goto yyacceptlab
#define YYABORT 	goto yyabortlab
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Define __yy_memcpy.  Note that the size argument
   should be passed with type unsigned int, because that is what the non-GCC
   definitions require.  With GCC, __builtin_memcpy takes an arg
   of type size_t, but it can handle unsigned int.  */

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(TO,FROM,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (to, from, count)
     char *to;
     char *from;
     unsigned int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *to, char *from, unsigned int count)
{
  register char *t = to;
  register char *f = from;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 217 "/usr/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#ifdef __cplusplus
#define YYPARSE_PARAM_ARG void *YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#else /* not __cplusplus */
#define YYPARSE_PARAM_ARG YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#endif /* not __cplusplus */
#else /* not YYPARSE_PARAM */
#define YYPARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL
#endif /* not YYPARSE_PARAM */

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
#ifdef YYPARSE_PARAM
int yyparse (void *);
#else
int yyparse (void);
#endif
#endif

int
yyparse(YYPARSE_PARAM_ARG)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;
  int yyfree_stacks = 0;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  if (yyfree_stacks)
	    {
	      free (yyss);
	      free (yyvs);
#ifdef YYLSP_NEEDED
	      free (yyls);
#endif
	    }
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
#ifndef YYSTACK_USE_ALLOCA
      yyfree_stacks = 1;
#endif
      yyss = (short *) YYSTACK_ALLOC (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1,
		   size * (unsigned int) sizeof (*yyssp));
      yyvs = (YYSTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1,
		   size * (unsigned int) sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) YYSTACK_ALLOC (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1,
		   size * (unsigned int) sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 3:
#line 145 "Grammar.y"
{ yyLineNum++; ;
    break;}
case 4:
#line 146 "Grammar.y"
{ yyLineNum++; ;
    break;}
case 5:
#line 147 "Grammar.y"
{ yyLineNum++; ;
    break;}
case 6:
#line 152 "Grammar.y"
{ clear_vertex(); ;
    break;}
case 7:
#line 153 "Grammar.y"
{ 
	    if (vertex(VERTEX)) YYERROR;
	;
    break;}
case 8:
#line 157 "Grammar.y"
{ clear_vertex(); ;
    break;}
case 9:
#line 158 "Grammar.y"
{ 
	    if (vertex(VNORMAL)) YYERROR;
	;
    break;}
case 10:
#line 162 "Grammar.y"
{ clear_vertex(); ;
    break;}
case 11:
#line 163 "Grammar.y"
{ 
	    if (vertex(VTEXTURE)) YYERROR;
	;
    break;}
case 12:
#line 167 "Grammar.y"
{ clear_element(); ;
    break;}
case 13:
#line 168 "Grammar.y"
{ 
	    if (element(FACE)) YYERROR;
	;
    break;}
case 14:
#line 172 "Grammar.y"
{ clear_element(); ;
    break;}
case 15:
#line 173 "Grammar.y"
{ 
	    if (element(LINE)) YYERROR;
	;
    break;}
case 16:
#line 177 "Grammar.y"
{ clear_element(); ;
    break;}
case 17:
#line 178 "Grammar.y"
{ 
	    if (element(POINT)) YYERROR;
	;
    break;}
case 18:
#line 182 "Grammar.y"
{ clear_groups(); ;
    break;}
case 19:
#line 183 "Grammar.y"
{ 
	    group_statement(); 
	;
    break;}
case 20:
#line 188 "Grammar.y"
{ 
	    smoothing_group(yyvsp[0]._int); 
	;
    break;}
case 21:
#line 193 "Grammar.y"
{ 
	    if (smoothing_group_off(yyvsp[0]._name)) YYERROR;
	;
    break;}
case 22:
#line 198 "Grammar.y"
{
	    active_object(yyvsp[0]._name);
	;
    break;}
case 23:
#line 203 "Grammar.y"
{
	    if ((state.bevel = on_off_only(yyvsp[0]._name)) == -1) YYERROR;
	    else yyReader->bevel(state.bevel);
	;
    break;}
case 24:
#line 209 "Grammar.y"
{
	    if ((state.cinterp = on_off_only(yyvsp[0]._name)) == -1) YYERROR;
	    else yyReader->cinterp(state.cinterp);
	;
    break;}
case 25:
#line 215 "Grammar.y"
{
	    if ((state.dinterp = on_off_only(yyvsp[0]._name)) == -1) YYERROR;
	    else yyReader->dinterp(state.dinterp);
	;
    break;}
case 26:
#line 221 "Grammar.y"
{
	    state.mtl = yyvsp[0]._name;
	    yyReader->usemtl(state.mtl);
	;
    break;}
case 27:
#line 227 "Grammar.y"
{
	    state.map = yyvsp[0]._name;
	    yyReader->usemap(state.map);
	;
    break;}
case 28:
#line 233 "Grammar.y"
{
	    state.mtllib = yyvsp[0]._name;
	    yyReader->mtllib(state.mtllib);
	;
    break;}
case 29:
#line 239 "Grammar.y"
{
	    state.shadow_obj = yyvsp[0]._name;
	    yyReader->shadow_obj(state.shadow_obj);
	;
    break;}
case 30:
#line 245 "Grammar.y"
{
	    state.trace_obj = yyvsp[0]._name;
	    yyReader->trace_obj(state.trace_obj);
	;
    break;}
case 31:
#line 251 "Grammar.y"
{
	    state.lod = yyvsp[0]._int;
	    yyReader->lod(state.lod);
	;
    break;}
case 34:
#line 264 "Grammar.y"
{ 
	    yyval._flags=1; 
	    if (index(1,yyvsp[0]._int,0,0)) YYERROR;
	;
    break;}
case 35:
#line 270 "Grammar.y"
{ 
	    yyval._flags=3; 
	    if (index(3,yyvsp[-2]._int,yyvsp[0]._int,0)) YYERROR;
	;
    break;}
case 36:
#line 276 "Grammar.y"
{ 
	    yyval._flags=5;
	    if (index(5,yyvsp[-3]._int,0,yyvsp[0]._int)) YYERROR;
	;
    break;}
case 37:
#line 282 "Grammar.y"
{ 
	    yyval._flags=7; 
	    if (index(7,yyvsp[-4]._int,yyvsp[-2]._int,yyvsp[0]._int)) YYERROR;
	;
    break;}
case 38:
#line 289 "Grammar.y"
{ add_to_vertex(yyvsp[0]._float); ;
    break;}
case 39:
#line 290 "Grammar.y"
{ add_to_vertex(yyvsp[0]._float); ;
    break;}
case 41:
#line 295 "Grammar.y"
{ yyval._float = float(yyvsp[0]._int); ;
    break;}
case 43:
#line 303 "Grammar.y"
{ yyval._int = -1; ;
    break;}
case 45:
#line 308 "Grammar.y"
{ add_group(""); ;
    break;}
case 46:
#line 309 "Grammar.y"
{ add_group(yyvsp[0]._name); ;
    break;}
case 48:
#line 314 "Grammar.y"
{ add_group(yyvsp[0]._name); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 543 "/usr/share/bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;

 yyacceptlab:
  /* YYACCEPT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 0;

 yyabortlab:
  /* YYABORT comes here.  */
  if (yyfree_stacks)
    {
      free (yyss);
      free (yyvs);
#ifdef YYLSP_NEEDED
      free (yyls);
#endif
    }
  return 1;
}
#line 317 "Grammar.y"



void
group_statement()
{
    yyReader->activeGroups(state.active_groups);
}

void
smoothing_group(int i)
{
    if (state.smoothing_group != i)
    {
	state.smoothing_group = i;
	yyReader->smoothingGroup(i);
    }
}

int
smoothing_group_off(const char *n)
{
    if (!strcmp(n,"off"))
    {
	smoothing_group(0);
	return 0;
    }

    return 1;
}

void
clear_groups()
{
    state.active_groups.clear();
}

void
add_group(const char *name)
{
    bool exists = false;

    for (int i=0; i<state.groups.size(); i++)
    {
	if (state.groups[i] == name) 
	{
	    exists = true;
	    break;
	}
    }

    if (!exists)
    {
	string n(name);
	state.groups.push_back(n);
	yyReader->newGroup(n);
    }

    for (size_t i=0; i<state.active_groups.size(); i++)
    {
	if (state.active_groups[i] == name) return;
    }

    state.active_groups.push_back(name);
}

void 
clear_vertex()
{
    state.vector_size = 0;
}

void 
add_to_vertex(float f)
{
    state.vector[state.vector_size++] = f;
}

int
vertex(int type)
{
    state.element_type = type;

    switch (type)
    {
      case VERTEX:   
	  state.num_vertices++;	    
	  switch (state.vector_size)
	  {
	    case 4:
		yyReader->v(state.vector[0],state.vector[1],state.vector[2],
			    state.vector[3]);
		break;
	    case 3:
		yyReader->v(state.vector[0],state.vector[1],state.vector[2]);
		break;
	    default:
		yyerror("\"v\" must have 3 or 4 dimensions");
		return 1;
		break;
	  }
	  break;

      case VTEXTURE: 
	  state.num_texture_vertices++;  

	  switch (state.vector_size)
	  {
	    case 3:
		yyReader->vt(state.vector[0],state.vector[1],state.vector[2]);
		break;
	    case 2:
		yyReader->vt(state.vector[0],state.vector[1]);
		break;
	    case 1:
		yyReader->vt(state.vector[0]);
		break;
	    default:
		yyerror("\"vt\" must have 1, 2, or 3 dimensions");
		return 1;
	  }
	  break;

      case VNORMAL:  
	  state.num_normals++;	    

	  if (state.vector_size != 3) 
	  {
	      yyerror("\"vn\" must have 3 dimensions");
	      return 1;
	  }
	  yyReader->vn(state.vector[0],state.vector[1],state.vector[2]);
	  break;
    }

    return 0;
}

int 
element(int type)
{
    if (state.index_error) 
    {
	yyerror("inconsistent indexing");
	return 1;
    }

    state.element_type = type;

    switch (type)
    {
      case FACE:
	  yyReader->f(state.vertex_index,
		      state.normal_index,
		      state.texture_index);
	  break;
      case LINE:
	  yyReader->l(state.vertex_index,
		      state.normal_index,
		      state.texture_index);
	  break;
      case POINT:
	  yyReader->p(state.vertex_index,
		      state.normal_index,
		      state.texture_index);
	  break;
    }

    return 0;
}

void 
clear_element()
{
    state.vertex_index.clear();
    state.normal_index.clear();
    state.texture_index.clear();
    state.index_mask = 0;
    state.index_error = 0;
}

int 
index(int mask, int v, int vt, int vn)
{
    if ( v<0) v  += state.num_vertices;		else v--;
    if (vt<0) vt += state.num_texture_vertices; else vt--;
    if (vn<0) vn += state.num_normals;		else vn--;

    if (v >= state.num_vertices || v < 0)
    {
	state.index_error = 1;
	yyerror("vertex index is refering to non-existent vertex");
	return 1;
    }

    if ((mask & 0x2) && (vt >= state.num_texture_vertices || vt < 0))
    {
	state.index_error = 1;
	yyerror("texture vertex index is refering to non-existent "
		"texture vertex");
	return 1;
    }

    if ((mask & 0x4) && (vn >= state.num_normals || vn < 0))
    {
	state.index_error = 1;
	yyerror("normal index is refering to non-existent normal");
	return 1;
    }

    state.vertex_index.push_back(v);
    if (vt >= 0) state.texture_index.push_back(vt);
    if (vn >= 0) state.normal_index.push_back(vn);

    if (mask != state.index_mask)
    {
	if (state.index_mask != 0) state.index_error = 1;
	state.index_mask = mask;
    }

    return 0;
}

void
active_object(const char *oname)
{
    if (state.active_object != oname)
    {
	state.active_object = oname;
	yyReader->activeObject(state.active_object);
    }
}


int
on_off_only(const char *name)
{
    if (!strcmp(name,"on")) return 1;
    if (!strcmp(name,"off")) return 0;
    yyerror("\"on\" or \"off\" are the only valid options");
    return -1;
}

void yyerror(const char *errorString)
{
    printf("Error: line %d: %s\n",yyLineNum,errorString);
}    

