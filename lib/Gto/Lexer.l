%{
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
#include "Utilities.h"
#include "Parser.h"
#include "Reader.h"
#include <iostream>
using namespace std;
using namespace Gto;

extern int	GTOdebug;
#define LVAL reinterpret_cast<YYSTYPE*>(_yystype)

%}

%option always-interactive
%option noyywrap
%option c++

Identifier		([A-Za-z_][A-Za-z_0-9]*)|([0-9]+[a-zA-Z_][a-zA-Z_0-9]*)
NewLine 		\r?[\n]
WhiteSpace		[[:blank:]]+
NonWhiteSpace		[^[:blank:]]+
FloatNumA 		[[:digit:]]*\.[[:digit:]]+([Ee][+-]?[[:digit:]]+)?
FloatNumB 		[[:digit:]]+\.?([Ee][+-]?[[:digit:]]+)?
IntNum			[[:digit:]]+
HexNum			0x[[:xdigit:]]+
UnicodeEsc		\\u[[:xdigit:]][[:xdigit:]][[:xdigit:]][[:xdigit:]]+
CUnicodeEsc             \'\\u[[:xdigit:]][[:xdigit:]][[:xdigit:]][[:xdigit:]]+\'
OctalEsc		\\[0-3][0-7][0-7]

%x COMMENT
%x STRING

%%

{WhiteSpace}            { reader->m_charnum += yyleng; }
{NewLine}               { reader->m_linenum++; reader->m_charnum=0; }

"GTOa"			{ reader->m_charnum+=yyleng; return GTOID; }
"as"			{ reader->m_charnum+=yyleng; return AS; }
"float"			{ reader->m_charnum+=yyleng; return FLOATTYPE; }
"double"		{ reader->m_charnum+=yyleng; return DOUBLETYPE; }
"int"			{ reader->m_charnum+=yyleng; return INTTYPE; }
"bool"			{ reader->m_charnum+=yyleng; return BOOLTYPE; }
"half"                  { reader->m_charnum+=yyleng; return HALFTYPE; }
"byte"                  { reader->m_charnum+=yyleng; return BYTETYPE; }
"short"                 { reader->m_charnum+=yyleng; return SHORTTYPE; }
"string"                { reader->m_charnum+=yyleng; return STRINGTYPE; }
"..."                   { reader->m_charnum+=yyleng; return ELLIPSIS; }

#.*{NewLine}		{ reader->m_linenum++; reader->m_charnum=0; }
"\""			{ reader->m_charnum++; yyNewString(); BEGIN STRING; }

<STRING>{UnicodeEsc}	{ 
			  reader->m_charnum += yyleng; 
			  unsigned int u = strtol(yytext+2,(char**)0,16);
			  yyAddToString(u); 
			}

<STRING>{OctalEsc}	{ 
			  reader->m_charnum += yyleng; 
			  unsigned int u = strtol(yytext+1,(char**)0,8);
			  yyAddToString(u); 
			}

<STRING>{NewLine}       { reader->m_linenum++; reader->m_charnum=0; yyAddToString('\n'); }
<STRING>"\\b"		{ reader->m_charnum += yyleng; yyAddToString('\b'); }
<STRING>"\\t"		{ reader->m_charnum += yyleng; yyAddToString('\t'); }
<STRING>"\\n"		{ reader->m_charnum += yyleng; yyAddToString('\n'); }
<STRING>"\\f"		{ reader->m_charnum += yyleng; yyAddToString('\f'); }
<STRING>"\\r"		{ reader->m_charnum += yyleng; yyAddToString('\r'); }
<STRING>"\\\""		{ reader->m_charnum += yyleng; yyAddToString('"'); }
<STRING>"\'"		{ reader->m_charnum += yyleng; yyAddToString('\''); }
<STRING>"\\\\"		{ reader->m_charnum += yyleng; yyAddToString('\\'); }

<STRING>"\""		{ 
			  reader->m_charnum++; 
                          BEGIN 0;

                          LVAL->_int = yyReturnString();
                          return STRINGCONST; 
			}

<STRING>.		{ reader->m_charnum++; yyAddToString(*yytext);}

{IntNum} |
{HexNum}		{
			    LVAL->_int = strtol(yytext,0,0);
			    reader->m_charnum += yyleng;
			    return INTCONST;
			}

{FloatNumA} |
{FloatNumB}             {
			    LVAL->_double = atof(yytext);
			    reader->m_charnum += yyleng;
			    return FLOATCONST;
			}


\'\\[btnfr\\]\'         {
                            switch (yytext[2])
                            {
                              case 'b': LVAL->_int = int('\b'); break;
                              case 't': LVAL->_int = int('\t'); break;
                              case 'n': LVAL->_int = int('\n'); break;
                              case 'f': LVAL->_int = int('\f'); break;
                              case 'r': LVAL->_int = int('\r'); break;
                              default:
                                  LVAL->_int = yytext[2]; break;
                            }

                            reader->m_charnum += yyleng;
                            return INTCONST;
                        }

{CUnicodeEsc}           { 
                            reader->m_charnum += yyleng; 
                            unsigned int u = strtol(yytext+3,(char**)0,16);
                            LVAL->_int = u;
                            return INTCONST;
                        }

\'.\'                   { 
                            reader->m_charnum += yyleng; 
                            LVAL->_int = int(yytext[1]);
                            return INTCONST;
                        }

"{%%debug%%}"           {
			    GTOdebug=!GTOdebug;
                            if (GTOdebug) debug();
			    reader->m_charnum += yyleng;
			}

{Identifier}		{
			    reader->m_charnum += yyleng;
			    LVAL->_int = reader->internString(yytext);
                            return STRINGCONST;
			}

<<EOF>>			{
			    yyterminate();
			}

.			{
                            reader->m_charnum++;
			    return *yytext;
			}

%%

void 
yyFlexLexer::yyNewString()
{
#if COMPILER == GCC2_96
    stringBuffer = "";
#else
    stringBuffer.clear();
#endif
}

void 
yyFlexLexer::yyAddToString(char c)
{
    stringBuffer.push_back(c);
}

int
yyFlexLexer::yyReturnString()
{
    return reader->internString(stringBuffer);
}

void
yyFlexLexer::init(Reader* r)
{
    reader = r; 
    reader->m_linenum = 1;
    reader->m_charnum = 1;
}
