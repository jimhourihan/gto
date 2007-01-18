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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse GTOparse
#define yylex   GTOlex
#define yyerror GTOerror
#define yylval  GTOlval
#define yychar  GTOchar
#define yydebug GTOdebug
#define yynerrs GTOnerrs


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




/* Copy the first part of user declarations.  */
#line 1 "Parser.y"

/***********************************************************************
// Copyright (c) 2002-2006 Tweak Films
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

#define YYPARSE_PARAM state
#define YYLEX_PARAM state
#ifdef yyerror
#undef yyerror
#endif
#define yyerror(MSG) GTOParseError(YYPARSE_PARAM, MSG)
#define IN_GRAMMAR
#define READER (reinterpret_cast<GTOFlexLexer*>(state)->readerObject())

#include "FlexLexer.h"
#include <iostream>
#include "Reader.h"
#include "Utilities.h"
#include <stdarg.h>

int  yylex(void*, void*);
void GTOParseError(void*, const char *,...);
void GTOParseWarning(void*, const char *,...);



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 43 "Parser.y"
typedef union YYSTYPE {
    int             _token;
    int             _int;
    double          _double;
    Gto::TypeSpec   _type;
    Gto::Number     _number;
} YYSTYPE;
/* Line 190 of yacc.c.  */
#line 165 "Parser.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 213 of yacc.c.  */
#line 177 "Parser.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

# ifndef YYFREE
#  define YYFREE free
# endif
# ifndef YYMALLOC
#  define YYMALLOC malloc
# endif

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   95

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  26
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  30
/* YYNRULES -- Number of rules. */
#define YYNRULES  58
/* YYNRULES -- Number of states. */
#define YYNSTATES  92

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   271

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      17,    18,     2,     2,     2,    25,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    21,     2,
       2,    22,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    23,     2,    24,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    19,     2,    20,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     4,     8,     9,    16,    18,    21,    22,
      28,    29,    37,    38,    49,    51,    54,    55,    58,    59,
      66,    68,    71,    72,    79,    80,    89,    91,    96,   104,
     106,   108,   110,   112,   114,   116,   118,   120,   121,   123,
     126,   128,   131,   133,   137,   139,   141,   143,   145,   147,
     150,   152,   154,   157,   159,   161,   163,   166,   168
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      27,     0,    -1,    -1,     6,    28,    30,    -1,    -1,     6,
      17,     4,    18,    29,    30,    -1,    31,    -1,    30,    31,
      -1,    -1,     3,    19,    32,    35,    20,    -1,    -1,     3,
      21,     3,    19,    33,    35,    20,    -1,    -1,     3,    21,
       3,    17,     4,    18,    19,    34,    35,    20,    -1,    37,
      -1,    35,    37,    -1,    -1,     7,     3,    -1,    -1,     3,
      36,    19,    38,    39,    20,    -1,    40,    -1,    39,    40,
      -1,    -1,    43,     3,    36,    22,    41,    49,    -1,    -1,
      43,     3,    36,    22,    42,    23,    45,    24,    -1,    44,
      -1,    44,    23,     4,    24,    -1,    44,    23,     4,    24,
      23,     4,    24,    -1,     9,    -1,     8,    -1,    11,    -1,
      14,    -1,    15,    -1,    12,    -1,    13,    -1,    10,    -1,
      -1,    46,    -1,    46,    16,    -1,    47,    -1,    46,    47,
      -1,    49,    -1,    23,    48,    24,    -1,    50,    -1,    52,
      -1,    51,    -1,    53,    -1,    51,    -1,    50,    51,    -1,
       3,    -1,    53,    -1,    52,    53,    -1,    54,    -1,    55,
      -1,     5,    -1,    25,     5,    -1,     4,    -1,    25,     4,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,    90,    90,    89,    99,    98,   109,   110,   115,   114,
     121,   120,   127,   126,   134,   135,   140,   144,   152,   151,
     159,   160,   165,   164,   192,   191,   229,   236,   243,   252,
     253,   254,   255,   256,   257,   258,   259,   263,   264,   265,
     269,   270,   274,   275,   287,   288,   292,   293,   297,   298,
     302,   321,   322,   326,   385,   446,   447,   451,   452
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "STRINGCONST", "INTCONST", "FLOATCONST",
  "GTOID", "AS", "INTTYPE", "FLOATTYPE", "DOUBLETYPE", "STRINGTYPE",
  "HALFTYPE", "BOOLTYPE", "SHORTTYPE", "BYTETYPE", "ELLIPSIS", "'('",
  "')'", "'{'", "'}'", "':'", "'='", "'['", "']'", "'-'", "$accept",
  "file", "@1", "@2", "object_list", "object", "@3", "@4", "@5",
  "component_list", "interp_string_opt", "component", "@6",
  "property_list", "property", "@7", "@8", "type", "basic_type",
  "complex_element_list", "element_list", "element", "atomic_value_list",
  "atomic_value", "string_value_list", "string_value",
  "numeric_value_list", "numeric_value", "float_num", "int_num", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,    40,    41,   123,
     125,    58,    61,    91,    93,    45
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    26,    28,    27,    29,    27,    30,    30,    32,    31,
      33,    31,    34,    31,    35,    35,    36,    36,    38,    37,
      39,    39,    41,    40,    42,    40,    43,    43,    43,    44,
      44,    44,    44,    44,    44,    44,    44,    45,    45,    45,
      46,    46,    47,    47,    48,    48,    49,    49,    50,    50,
      51,    52,    52,    53,    53,    54,    54,    55,    55
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     3,     0,     6,     1,     2,     0,     5,
       0,     7,     0,    10,     1,     2,     0,     2,     0,     6,
       1,     2,     0,     6,     0,     8,     1,     4,     7,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     2,
       1,     2,     1,     3,     1,     1,     1,     1,     1,     2,
       1,     1,     2,     1,     1,     1,     2,     1,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     2,     0,     0,     0,     1,     0,     0,     3,     6,
       4,     8,     0,     7,     0,     0,     0,     5,    16,     0,
      14,     0,    10,     0,     0,     9,    15,     0,     0,    17,
      18,     0,     0,     0,    12,    11,    30,    29,    36,    31,
      34,    35,    32,    33,     0,    20,     0,    26,     0,    19,
      21,    16,     0,     0,     0,     0,    13,    22,    27,     0,
       0,     0,    50,    57,    55,     0,    23,    46,    47,    53,
      54,    37,     0,    58,    56,     0,     0,    38,    40,    42,
      28,     0,    44,    48,    45,    51,    25,    39,    41,    43,
      49,    52
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     2,     4,    14,     8,     9,    15,    28,    48,    19,
      24,    20,    33,    44,    45,    59,    60,    46,    47,    76,
      77,    78,    81,    79,    82,    67,    84,    68,    69,    70
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -61
static const yysigned_char yypact[] =
{
      11,     3,    50,    47,    49,   -61,    35,     9,    49,   -61,
     -61,   -61,    61,   -61,    49,    62,     8,    49,    59,    13,
     -61,    63,   -61,    65,    51,   -61,   -61,    53,    62,   -61,
     -61,    54,    15,    48,   -61,   -61,   -61,   -61,   -61,   -61,
     -61,   -61,   -61,   -61,    29,   -61,    66,    52,    62,   -61,
     -61,    59,    68,    16,    55,    56,   -61,    58,    60,     4,
      64,    70,   -61,   -61,   -61,    43,   -61,   -61,   -61,   -61,
     -61,     1,    67,   -61,   -61,     4,    69,    -2,   -61,   -61,
     -61,    71,    73,   -61,     6,   -61,   -61,   -61,   -61,   -61,
     -61,   -61
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -61,   -61,   -61,   -61,    72,    37,   -61,   -61,   -61,   -16,
      27,   -19,   -61,   -61,    38,   -61,   -61,   -61,   -61,   -61,
     -61,     2,   -61,    25,   -61,   -60,   -61,   -29,   -61,   -61
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -25
static const yysigned_char yytable[] =
{
      26,    62,    63,    64,    62,    63,    64,    62,    63,    64,
      63,    64,    32,    26,    87,    83,    18,     1,    18,    18,
       3,    75,    90,    65,    75,    21,    65,    22,    11,    65,
      12,    65,    53,    25,    26,    35,    56,    36,    37,    38,
      39,    40,    41,    42,    43,    13,    85,    73,    74,    49,
       5,     6,     7,    10,    13,    91,    36,    37,    38,    39,
      40,    41,    42,    43,    16,    18,    23,    27,    29,    51,
      30,    31,    55,    34,    72,    52,    62,    57,    54,    88,
      58,   -24,    50,    61,    66,     0,    17,    71,     0,     0,
       0,    80,     0,    86,     0,    89
};

static const yysigned_char yycheck[] =
{
      19,     3,     4,     5,     3,     4,     5,     3,     4,     5,
       4,     5,    28,    32,    16,    75,     3,     6,     3,     3,
      17,    23,    82,    25,    23,    17,    25,    19,    19,    25,
      21,    25,    48,    20,    53,    20,    20,     8,     9,    10,
      11,    12,    13,    14,    15,     8,    75,     4,     5,    20,
       0,     4,     3,    18,    17,    84,     8,     9,    10,    11,
      12,    13,    14,    15,     3,     3,     7,     4,     3,     3,
      19,    18,     4,    19,     4,    23,     3,    22,    51,    77,
      24,    23,    44,    23,    59,    -1,    14,    23,    -1,    -1,
      -1,    24,    -1,    24,    -1,    24
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     6,    27,    17,    28,     0,     4,     3,    30,    31,
      18,    19,    21,    31,    29,    32,     3,    30,     3,    35,
      37,    17,    19,     7,    36,    20,    37,     4,    33,     3,
      19,    18,    35,    38,    19,    20,     8,     9,    10,    11,
      12,    13,    14,    15,    39,    40,    43,    44,    34,    20,
      40,     3,    23,    35,    36,     4,    20,    22,    24,    41,
      42,    23,     3,     4,     5,    25,    49,    51,    53,    54,
      55,    23,     4,     4,     5,    23,    45,    46,    47,    49,
      24,    48,    50,    51,    52,    53,    24,    16,    47,    24,
      51,    53
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  register short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;


  yyvsp[0] = yylval;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 90 "Parser.y"
    {
        READER->beginHeader(GTO_VERSION);
    ;}
    break;

  case 3:
#line 94 "Parser.y"
    { 
        READER->endFile(); 
    ;}
    break;

  case 4:
#line 99 "Parser.y"
    {
        READER->beginHeader((yyvsp[-1]._int));
    ;}
    break;

  case 5:
#line 103 "Parser.y"
    { 
        READER->endFile(); 
    ;}
    break;

  case 8:
#line 115 "Parser.y"
    {
        READER->beginObject((yyvsp[-1]._int), READER->internString("object"));
    ;}
    break;

  case 10:
#line 121 "Parser.y"
    {
        READER->beginObject((yyvsp[-3]._int), (yyvsp[-1]._int));
    ;}
    break;

  case 12:
#line 127 "Parser.y"
    {
        READER->beginObject((yyvsp[-6]._int), (yyvsp[-4]._int), (yyvsp[-2]._int));
    ;}
    break;

  case 16:
#line 140 "Parser.y"
    {
        (yyval._int) = READER->internString("");
    ;}
    break;

  case 17:
#line 145 "Parser.y"
    {
        (yyval._int) = (yyvsp[0]._int);
    ;}
    break;

  case 18:
#line 152 "Parser.y"
    {
        READER->beginComponent((yyvsp[-2]._int), (yyvsp[-1]._int));
    ;}
    break;

  case 22:
#line 165 "Parser.y"
    {
        READER->beginProperty((yyvsp[-2]._int), (yyvsp[-1]._int), (yyvsp[-3]._type).width, (yyvsp[-3]._type).size, (yyvsp[-3]._type).type);
    ;}
    break;

  case 23:
#line 169 "Parser.y"
    {
        if (READER->currentType().width != 1)
        {
            GTOParseError(state, "expected data width of %d, found 1",
                          READER->currentType().width);
            YYERROR;
        }
        else if ((yyvsp[-5]._type).size != 0 && 
                 READER->numAtomicValuesInBuffer() != (yyvsp[-5]._type).size)
        {
            GTOParseError(state, 
                          "property size mismatch, found %d, expect %d",
                          READER->numAtomicValuesInBuffer(),
                          (yyvsp[-5]._type).size);
            YYERROR;
        }
        else
        {
            READER->endProperty();
        }
    ;}
    break;

  case 24:
#line 192 "Parser.y"
    {
        READER->beginProperty((yyvsp[-2]._int), (yyvsp[-1]._int), (yyvsp[-3]._type).width, (yyvsp[-3]._type).size, (yyvsp[-3]._type).type);
    ;}
    break;

  case 25:
#line 196 "Parser.y"
    {
        size_t nelements = READER->numElementsInBuffer();

        if ((yyvsp[-7]._type).size != 0 && nelements != (yyvsp[-7]._type).size)
        {
            if ((yyvsp[-1]._token) == ELLIPSIS)
            {
                READER->fillToSize((yyvsp[-7]._type).size);
                READER->endProperty();
            }
            else
            {
                GTOParseError(state, 
                              "property size mismatch, found %d, expect %d",
                              nelements,
                              (yyvsp[-7]._type).size);
                YYERROR;
            }
        }
        else if ((yyvsp[-7]._type).size == 0 && (yyvsp[-1]._token) == ELLIPSIS)
        {
            GTOParseError(state,
              "use of ... requires fixed property size but none was provided");
            YYERROR;
        }
        else
        {
            READER->endProperty();
        }
    ;}
    break;

  case 26:
#line 230 "Parser.y"
    {
        (yyval._type).type  = (yyvsp[0]._type).type;
        (yyval._type).width = 1;
        (yyval._type).size  = 0;
    ;}
    break;

  case 27:
#line 237 "Parser.y"
    {
        (yyval._type).type  = (yyvsp[-3]._type).type;
        (yyval._type).width = (yyvsp[-1]._int);
        (yyval._type).size  = 0;
    ;}
    break;

  case 28:
#line 244 "Parser.y"
    {
        (yyval._type).type  = (yyvsp[-6]._type).type;
        (yyval._type).width = (yyvsp[-4]._int);
        (yyval._type).size  = (yyvsp[-1]._int);
    ;}
    break;

  case 29:
#line 252 "Parser.y"
    { (yyval._type).type = Gto::Float; ;}
    break;

  case 30:
#line 253 "Parser.y"
    { (yyval._type).type = Gto::Int; ;}
    break;

  case 31:
#line 254 "Parser.y"
    { (yyval._type).type = Gto::String; ;}
    break;

  case 32:
#line 255 "Parser.y"
    { (yyval._type).type = Gto::Short; ;}
    break;

  case 33:
#line 256 "Parser.y"
    { (yyval._type).type = Gto::Byte; ;}
    break;

  case 34:
#line 257 "Parser.y"
    { (yyval._type).type = Gto::Half; ;}
    break;

  case 35:
#line 258 "Parser.y"
    { (yyval._type).type = Gto::Boolean; ;}
    break;

  case 36:
#line 259 "Parser.y"
    { (yyval._type).type = Gto::Double; ;}
    break;

  case 37:
#line 263 "Parser.y"
    { (yyval._token) = 0; ;}
    break;

  case 38:
#line 264 "Parser.y"
    { (yyval._token) = 0; ;}
    break;

  case 39:
#line 265 "Parser.y"
    { (yyval._token) = ELLIPSIS; ;}
    break;

  case 43:
#line 276 "Parser.y"
    {
        if ((yyvsp[-1]._int) != READER->currentType().width)
        {
            GTOParseError(state, "expected data width of %d, found %d",
                          READER->currentType().width, (yyvsp[-1]._int));
            YYERROR;
        }
    ;}
    break;

  case 46:
#line 292 "Parser.y"
    { ;}
    break;

  case 47:
#line 293 "Parser.y"
    { ;}
    break;

  case 48:
#line 297 "Parser.y"
    { (yyval._int) = 1; ;}
    break;

  case 49:
#line 298 "Parser.y"
    { (yyval._int) = (yyvsp[-1]._int) + 1; ;}
    break;

  case 50:
#line 303 "Parser.y"
    {
        if (READER->currentType().type != Gto::String)
        {
            GTOParseError(state, 
                          "expected a numeric value, found string \"%s\"",
                          READER->stringFromId((yyvsp[0]._int)).c_str());
            YYERROR;
        }
        else
        {
            READER->addToPropertyBuffer((yyvsp[0]._int));
        }

        (yyval._int) = (yyvsp[0]._int);
    ;}
    break;

  case 51:
#line 321 "Parser.y"
    { (yyval._int) = 1; ;}
    break;

  case 52:
#line 322 "Parser.y"
    { (yyval._int) = (yyvsp[-1]._int) + 1; ;}
    break;

  case 53:
#line 327 "Parser.y"
    {
        Gto::DataType t = Gto::DataType(READER->properties().back().type);

        switch (t)
        {
          case Gto::Int:
              if ((yyvsp[0]._double) != int((yyvsp[0]._double)))
              {
                  GTOParseWarning(state, "floating point value truncated "
                                  "to match integer property type (%f => %d)",
                                  (yyvsp[0]._double), int((yyvsp[0]._double)));
              }
              
              (yyval._number).type = Gto::Int;
              (yyval._number)._int = int((yyvsp[0]._double));
              READER->addToPropertyBuffer(int((yyvsp[0]._double)));
              break;
              
          case Gto::Float:
              (yyval._number).type = Gto::Float;
              (yyval._number)._double = (yyvsp[0]._double);
              READER->addToPropertyBuffer(float((yyvsp[0]._double)));
              break;

          case Gto::Short:
              if ((yyvsp[0]._double) != short((yyvsp[0]._double)))
              {
                  GTOParseWarning(state, "floating point value truncated "
                                  "to match short property type (%f => %d)",
                                  (yyvsp[0]._double), int(short((yyvsp[0]._double))));
              }
              
              (yyval._number).type = Gto::Short;
              (yyval._number)._int = short((yyvsp[0]._double));
              READER->addToPropertyBuffer(short((yyvsp[0]._double)));
              break;

          case Gto::Byte:
              if ((yyvsp[0]._double) != (unsigned char)((yyvsp[0]._double)))
              {
                  GTOParseWarning(state, "floating point value truncated "
                                  "to match byte property type (%f => %d)",
                                  (yyvsp[0]._double), int((unsigned char)((yyvsp[0]._double))));
              }
              
              (yyval._number).type = Gto::Byte;
              (yyval._number)._int = (unsigned char)((yyvsp[0]._double));
              READER->addToPropertyBuffer((unsigned char)((yyvsp[0]._double)));
              break;

          case Gto::String:
              GTOParseError(state, "string expected; got a floating "
                            "point number (%f) instead", (yyvsp[0]._double));
              YYERROR;
              break;
        }
    ;}
    break;

  case 54:
#line 386 "Parser.y"
    {
        Gto::DataType t = Gto::DataType(READER->properties().back().type);

        switch (t)
        {
          case Gto::Int:
              (yyval._number).type = Gto::Int;
              (yyval._number)._int = (yyvsp[0]._int);
              READER->addToPropertyBuffer(int((yyvsp[0]._int)));
              break;
              
          case Gto::Float:
              if ((yyvsp[0]._int) != float((yyvsp[0]._int)))
              {
                  GTOParseWarning(state, "integer cannot be represented "
                                  "as floating point (%d => %f)",
                                  (yyvsp[0]._int), float((yyvsp[0]._int)));
              }
              
              (yyval._number).type = Gto::Float;
              (yyval._number)._double = (yyvsp[0]._int);
              READER->addToPropertyBuffer(float((yyvsp[0]._int)));
              break;

          case Gto::Short:
              if ((yyvsp[0]._int) != short((yyvsp[0]._int)))
              {
                  GTOParseWarning(state, "integer cannot be represented "
                                  "as short (%d => %d)",
                                  (yyvsp[0]._int), short((yyvsp[0]._int)));
              }
              
              (yyval._number).type = Gto::Short;
              (yyval._number)._int = short((yyvsp[0]._int));
              READER->addToPropertyBuffer(short((yyvsp[0]._int)));
              break;

          case Gto::Byte:
              if ((yyvsp[0]._int) != (unsigned char)((yyvsp[0]._int)))
              {
                  GTOParseWarning(state, "integer cannot be represented "
                                  "as byte (%d => %d)",
                                  (yyvsp[0]._int), (unsigned char)((yyvsp[0]._int)));
              }
              
              (yyval._number).type = Gto::Byte;
              (yyval._number)._int = (unsigned char)((yyvsp[0]._int));
              READER->addToPropertyBuffer((unsigned char)((yyvsp[0]._int)));
              break;

          case Gto::String:
              GTOParseError(state, "string expected; got an integer "
                            "(%d) instead", (yyvsp[0]._int));
              YYERROR;
              break;
        }
    ;}
    break;

  case 55:
#line 446 "Parser.y"
    { (yyval._double) = (yyvsp[0]._double); ;}
    break;

  case 56:
#line 447 "Parser.y"
    { (yyval._double) = -(yyvsp[0]._double); ;}
    break;

  case 57:
#line 451 "Parser.y"
    { (yyval._int) = (yyvsp[0]._int); ;}
    break;

  case 58:
#line 452 "Parser.y"
    { (yyval._int) = -(yyvsp[0]._int); ;}
    break;


    }

/* Line 1037 of yacc.c.  */
#line 1597 "Parser.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  const char* yyprefix;
	  char *yymsg;
	  int yyx;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 0;

	  yyprefix = ", expecting ";
	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		yysize += yystrlen (yyprefix) + yystrlen (yytname [yyx]);
		yycount += 1;
		if (yycount == 5)
		  {
		    yysize = 0;
		    break;
		  }
	      }
	  yysize += (sizeof ("syntax error, unexpected ")
		     + yystrlen (yytname[yytype]));
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yyprefix = ", expecting ";
		  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			yyp = yystpcpy (yyp, yyprefix);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yyprefix = " or ";
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* If at end of input, pop the error token,
	     then the rest of the stack, then return failure.  */
	  if (yychar == YYEOF)
	     for (;;)
	       {

		 YYPOPSTACK;
		 if (yyssp == yyss)
		   YYABORT;
		 yydestruct ("Error: popping",
                             yystos[*yyssp], yyvsp);
	       }
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

#ifdef __GNUC__
  /* Pacify GCC when the user code never invokes YYERROR and the label
     yyerrorlab therefore never appears in user code.  */
  if (0)
     goto yyerrorlab;
#endif

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yydestruct ("Error: discarding lookahead",
              yytoken, &yylval);
  yychar = YYEMPTY;
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 456 "Parser.y"


int yylex(void* yylval, void* state)
{
    GTOFlexLexer* lexer = reinterpret_cast<GTOFlexLexer*>(state);
    lexer->setYYSTYPE(yylval);
    return lexer->yylex();
}

void
GTOParseError(void* state, const char *text, ...)
{
    char temp[256];
    GTOFlexLexer* lexer = reinterpret_cast<GTOFlexLexer*>(state);

    va_list ap;
    va_start(ap,text);
    vsprintf(temp,text,ap);
    va_end(ap);

    READER->parseError(temp);
}

void
GTOParseWarning(void* state, const char *text, ...)
{
    char temp[256];
    GTOFlexLexer* lexer = (GTOFlexLexer*)state;

    va_list ap;
    va_start(ap,text);
    vsprintf(temp,text,ap);
    va_end(ap);

    READER->parseWarning(temp);
}

int
GTOParse(Gto::Reader* reader)
{
    //
    //  Initialize state
    //

    yydebug = 0;
    GTOFlexLexer lexer(reader->in(), &std::cerr);
    lexer.init(reader);

    //
    //  Call the (bison) parser
    //

    return yyparse(&lexer) == 0;
}   


// Local Variables:
// mode: c++
// compile-command: "bison -v Parser.y"
// End:



