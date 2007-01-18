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

%}

%union
{
    int             _token;
    int             _int;
    double          _double;
    Gto::TypeSpec   _type;
    Gto::Number     _number;
}

//
//  Make it reentrant
//

%pure_parser

%token <_int>       STRINGCONST
%token <_int>       INTCONST
%token <_double>    FLOATCONST
%token <_int>       GTOID
%token <_token>     AS
%token <_token>     INTTYPE
%token <_token>     FLOATTYPE
%token <_token>     DOUBLETYPE
%token <_token>     STRINGTYPE
%token <_token>     HALFTYPE
%token <_token>     BOOLTYPE
%token <_token>     SHORTTYPE
%token <_token>     BYTETYPE
%token <_token>     ELLIPSIS

%type <_type>   type
%type <_type>   basic_type
%type <_int>    interp_string_opt
%type <_double> float_num
%type <_int>    int_num
%type <_number> numeric_value
%type <_int>    numeric_value_list;
%type <_int>    atomic_value_list;
%type <_int>    string_value_list;
%type <_int>    string_value;
%type <_token>  complex_element_list

%start file

%%

file:
    GTOID 
    {
        READER->beginHeader(GTO_VERSION);
    }
    object_list 
    { 
        READER->endFile(); 
    }

    | GTOID '(' INTCONST ')' 
    {
        READER->beginHeader($3);
    }
    object_list 
    { 
        READER->endFile(); 
    }
;

object_list:
    object
    | object_list object
;

object:
    STRINGCONST '{' 
    {
        READER->beginObject($1, READER->internString("object"));
    }
    component_list '}'

    | STRINGCONST ':' STRINGCONST '{' 
    {
        READER->beginObject($1, $3);
    }
    component_list '}'

    | STRINGCONST ':' STRINGCONST '(' INTCONST ')' '{' 
    {
        READER->beginObject($1, $3, $5);
    }
    component_list '}'
;

component_list:
    component
    | component_list component
;

interp_string_opt:
    /* empty */
    {
        $$ = READER->internString("");
    }

    | AS STRINGCONST
    {
        $$ = $2;
    }
;

component:
    STRINGCONST interp_string_opt '{' 
    {
        READER->beginComponent($1, $2);
    }
    property_list '}'
;

property_list:
    property
    | property_list property
;

property:
    type STRINGCONST interp_string_opt '=' 
    {
        READER->beginProperty($2, $3, $1.width, $1.size, $1.type);
    }
    atomic_value
    {
        if (READER->currentType().width != 1)
        {
            GTOParseError(state, "expected data width of %d, found 1",
                          READER->currentType().width);
            YYERROR;
        }
        else if ($1.size != 0 && 
                 READER->numAtomicValuesInBuffer() != $1.size)
        {
            GTOParseError(state, 
                          "property size mismatch, found %d, expect %d",
                          READER->numAtomicValuesInBuffer(),
                          $1.size);
            YYERROR;
        }
        else
        {
            READER->endProperty();
        }
    }

    | type STRINGCONST interp_string_opt '=' 
    {
        READER->beginProperty($2, $3, $1.width, $1.size, $1.type);
    }
    '[' complex_element_list ']'
    {
        size_t nelements = READER->numElementsInBuffer();

        if ($1.size != 0 && nelements != $1.size)
        {
            if ($7 == ELLIPSIS)
            {
                READER->fillToSize($1.size);
                READER->endProperty();
            }
            else
            {
                GTOParseError(state, 
                              "property size mismatch, found %d, expect %d",
                              nelements,
                              $1.size);
                YYERROR;
            }
        }
        else if ($1.size == 0 && $7 == ELLIPSIS)
        {
            GTOParseError(state,
              "use of ... requires fixed property size but none was provided");
            YYERROR;
        }
        else
        {
            READER->endProperty();
        }
    }
;

type:
    basic_type
    {
        $$.type  = $1.type;
        $$.width = 1;
        $$.size  = 0;
    }

    | basic_type '[' INTCONST ']'
    {
        $$.type  = $1.type;
        $$.width = $3;
        $$.size  = 0;
    }

    | basic_type '[' INTCONST ']' '[' INTCONST ']'
    {
        $$.type  = $1.type;
        $$.width = $3;
        $$.size  = $6;
    }
;

basic_type:
    FLOATTYPE       { $$.type = Gto::Float; }
    | INTTYPE       { $$.type = Gto::Int; }
    | STRINGTYPE    { $$.type = Gto::String; }
    | SHORTTYPE     { $$.type = Gto::Short; }
    | BYTETYPE      { $$.type = Gto::Byte; }
    | HALFTYPE      { $$.type = Gto::Half; }
    | BOOLTYPE      { $$.type = Gto::Boolean; }
    | DOUBLETYPE    { $$.type = Gto::Double; }
;

complex_element_list:
    /* empty */ { $$ = 0; }
    | element_list { $$ = 0; }
    | element_list ELLIPSIS { $$ = ELLIPSIS; }
;

element_list:
    element
    | element_list element
;

element:
    atomic_value
    | '[' atomic_value_list ']'
    {
        if ($2 != READER->currentType().width)
        {
            GTOParseError(state, "expected data width of %d, found %d",
                          READER->currentType().width, $2);
            YYERROR;
        }
    }
;

atomic_value_list:
    string_value_list 
    | numeric_value_list
;

atomic_value:
    string_value { }
    | numeric_value { }
;

string_value_list:
    string_value { $$ = 1; }
    | string_value_list string_value { $$ = $1 + 1; }
;

string_value:
    STRINGCONST
    {
        if (READER->currentType().type != Gto::String)
        {
            GTOParseError(state, 
                          "expected a numeric value, found string \"%s\"",
                          READER->stringFromId($1).c_str());
            YYERROR;
        }
        else
        {
            READER->addToPropertyBuffer($1);
        }

        $$ = $1;
    }
;

numeric_value_list:
    numeric_value { $$ = 1; }
    | numeric_value_list numeric_value { $$ = $1 + 1; }
;

numeric_value:
    float_num
    {
        Gto::DataType t = Gto::DataType(READER->properties().back().type);

        switch (t)
        {
          case Gto::Int:
              if ($1 != int($1))
              {
                  GTOParseWarning(state, "floating point value truncated "
                                  "to match integer property type (%f => %d)",
                                  $1, int($1));
              }
              
              $$.type = Gto::Int;
              $$._int = int($1);
              READER->addToPropertyBuffer(int($1));
              break;
              
          case Gto::Float:
              $$.type = Gto::Float;
              $$._double = $1;
              READER->addToPropertyBuffer(float($1));
              break;

          case Gto::Short:
              if ($1 != short($1))
              {
                  GTOParseWarning(state, "floating point value truncated "
                                  "to match short property type (%f => %d)",
                                  $1, int(short($1)));
              }
              
              $$.type = Gto::Short;
              $$._int = short($1);
              READER->addToPropertyBuffer(short($1));
              break;

          case Gto::Byte:
              if ($1 != (unsigned char)($1))
              {
                  GTOParseWarning(state, "floating point value truncated "
                                  "to match byte property type (%f => %d)",
                                  $1, int((unsigned char)($1)));
              }
              
              $$.type = Gto::Byte;
              $$._int = (unsigned char)($1);
              READER->addToPropertyBuffer((unsigned char)($1));
              break;

          case Gto::String:
              GTOParseError(state, "string expected; got a floating "
                            "point number (%f) instead", $1);
              YYERROR;
              break;
        }
    }

    | int_num
    {
        Gto::DataType t = Gto::DataType(READER->properties().back().type);

        switch (t)
        {
          case Gto::Int:
              $$.type = Gto::Int;
              $$._int = $1;
              READER->addToPropertyBuffer(int($1));
              break;
              
          case Gto::Float:
              if ($1 != float($1))
              {
                  GTOParseWarning(state, "integer cannot be represented "
                                  "as floating point (%d => %f)",
                                  $1, float($1));
              }
              
              $$.type = Gto::Float;
              $$._double = $1;
              READER->addToPropertyBuffer(float($1));
              break;

          case Gto::Short:
              if ($1 != short($1))
              {
                  GTOParseWarning(state, "integer cannot be represented "
                                  "as short (%d => %d)",
                                  $1, short($1));
              }
              
              $$.type = Gto::Short;
              $$._int = short($1);
              READER->addToPropertyBuffer(short($1));
              break;

          case Gto::Byte:
              if ($1 != (unsigned char)($1))
              {
                  GTOParseWarning(state, "integer cannot be represented "
                                  "as byte (%d => %d)",
                                  $1, (unsigned char)($1));
              }
              
              $$.type = Gto::Byte;
              $$._int = (unsigned char)($1);
              READER->addToPropertyBuffer((unsigned char)($1));
              break;

          case Gto::String:
              GTOParseError(state, "string expected; got an integer "
                            "(%d) instead", $1);
              YYERROR;
              break;
        }
    }
;

float_num:
    FLOATCONST { $$ = $1; }
    | '-' FLOATCONST { $$ = -$2; }
;

int_num:
    INTCONST { $$ = $1; }
    | '-' INTCONST { $$ = -$2; }
;


%%

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


