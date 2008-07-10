%{
/*
//	Copyright (c) 2002 Tweak Films
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
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "Grammar.h"
#include "Reader.h"

/* Redefine input and output so it can come from an iostream somewhere */
#define YY_INPUT(b,r,ms) (r = WFObjInput(b,ms))
//#define YY_ALWAYS_INTERACTIVE 1
#define YY_NO_UNPUT 1
#define yylval wfobjlval
#define yydebug wfobjdebug
#define yyparse wfobjparse
#define yyLineNum wfobjLineNum
#define yyReader wfobjReader
#define yyInputStream wfobjInputStream

#define YY_NO_UNISTD_H
extern int isatty (int );

using namespace std;

int			yyparse();
int			yyLineNum;
extern int		yydebug;
extern WFObj::Reader*   yyReader;
istream *		yyInputStream;

int			WFObjInput(char *buffer, int max_size);
void			WFObjParse(istream &i);
%}

%option always-interactive
%option prefix="wfobj"

WhiteSpace		[[:blank:]]+
FloatPart               [0-9]+\.?[0-9]*
FloatExp                [eE][-+]?[0-9]+
IntNum			-?[0-9]+
Name			[a-zA-Z0-9_.]+

%x continuation
%%

^{WhiteSpace}?f |
^{WhiteSpace}?fo	{ return FACE; }
^{WhiteSpace}?l		{ return LINE; }
^{WhiteSpace}?p		{ return POINT; }
^{WhiteSpace}?v		{ return VERTEX; }
^{WhiteSpace}?vn	{ return VNORMAL; }
^{WhiteSpace}?vt	{ return VTEXTURE; }
^{WhiteSpace}?g		{ return GROUP; }
^{WhiteSpace}?s		{ return SGROUP; }
^{WhiteSpace}?o		{ return OGROUP; }
^{WhiteSpace}?bevel	{ return BEVEL; }
^{WhiteSpace}?c_interp	{ return CINTERP; }
^{WhiteSpace}?d_interp	{ return DINTERP; }
^{WhiteSpace}?usemtl	{ return USEMTL; }
^{WhiteSpace}?usemap	{ return USEMAP; }
^{WhiteSpace}?mtllib	{ return MTLLIB; }
^{WhiteSpace}?maplib	{ return MAPLIB; }
^{WhiteSpace}?lod	{ return LOD; }
^{WhiteSpace}?shadow_obj { return SHADOW_OBJ; }
^{WhiteSpace}?trace_obj { return TRACE_OBJ; }
^{WhiteSpace}?\$.* { /* assumed comment -- appears in some viewpoint models*/ }
"{%%debug%%}"		{ yydebug=1; }

{WhiteSpace}    {}
-?{IntNum}		    { yylval._int   = strtol(yytext,0,0); return INTEGER; }
-?{FloatPart}{FloatExp}?    { yylval._float = atof(yytext);return SCALAR; }
-?{IntNum}{FloatExp}	    { yylval._float = atof(yytext);return SCALAR; }
{Name}			    { yylval._name = yytext; return NAME; }
"\\"			    { BEGIN(continuation); }
"\n"			    { return ENDLINE; }
#.*			    { /*comment*/ }
.			    { return *yytext; }

<continuation>{
.		{ BEGIN(0); /*scarf anything */ }
}

%%

int 
WFObjInput(char *buffer, int max_size)
{
    int count=0,sfail=0;

    while (max_size && !(sfail=yyInputStream->fail()))
    {
	int c = yyInputStream->get();
	if (c == EOF) { sfail=true; break; }
	buffer[count++] = c;
	max_size--;
    }

    if (sfail) buffer[count]=0;
    return count;
}

void 
WFObjParse(istream &i, WFObj::Reader *reader)
{
    yyInputStream   = &i;
    yyLineNum	    = 1;
    yyReader	    = reader;

    int result = yyparse();
}

int yywrap() { return 1; }
