%{
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
static void clear_all();
static void active_object(const char*);
static int  on_off_only(const char*);
void yyerror(const char*);

%}

%union
{
    int		_token;
    float	_float;
    int		_int;
    int		_flags;
    const char*	_name;
}

%token <_token>	    VERTEX
%token <_token>	    VNORMAL
%token <_token>	    VTEXTURE
%token <_token>	    FACE
%token <_token>	    LINE
%token <_token>	    POINT
%token <_token>	    GROUP
%token <_token>	    SGROUP
%token <_token>	    OGROUP
%token <_token>	    BEVEL
%token <_token>	    CINTERP
%token <_token>	    DINTERP
%token <_token>	    USEMTL
%token <_token>	    USEMAP
%token <_token>	    MAPLIB
%token <_token>	    MTLLIB
%token <_token>	    LOD
%token <_token>	    SHADOW_OBJ
%token <_token>	    TRACE_OBJ
%token <_name>      NAME
%token <_token>	    ENDLINE
%token <_float>	    SCALAR
%token <_int>	    INTEGER

%type <_float>	    scalar
%type <_int>	    integer
%type <_int>	    integer_opt
%type <_token>	    line
%type <_token>	    lines
%type <_token>	    statement
%type <_flags>	    element_list
%type <_flags>	    element
%type <_token>	    vertex_list
%type <_token>	    name_opt
%type <_token>	    name_list

%start lines

%%

lines: 
	line
	| lines line
;

line:
	ENDLINE		    { yyLineNum++; }
	| statement ENDLINE { yyLineNum++; }
	| error ENDLINE	    { yyLineNum++; }	/* resynch at next line */
;

statement:

	  VERTEX { clear_vertex(); } vertex_list   
	{ 
	    if (vertex(VERTEX)) YYERROR;
	}

	| VNORMAL { clear_vertex(); } vertex_list  
	{ 
	    if (vertex(VNORMAL)) YYERROR;
	}

	| VTEXTURE { clear_vertex(); } vertex_list 
	{ 
	    if (vertex(VTEXTURE)) YYERROR;
	}

	| FACE { clear_element(); } element_list   
	{ 
	    if (element(FACE)) YYERROR;
	}

	| LINE { clear_element(); } element_list   
	{ 
	    if (element(LINE)) YYERROR;
	}

	| POINT { clear_element(); } element_list  
	{ 
	    if (element(POINT)) YYERROR;
	}

	| GROUP { clear_groups(); } name_list	   
	{ 
	    group_statement(); 
	}

	| SGROUP integer_opt			   
	{ 
	    smoothing_group($2); 
	}

	| SGROUP NAME    			   
	{ 
	    if (smoothing_group_off($2)) YYERROR;
	}

	| OGROUP NAME
	{
	    active_object($2);
	}

	| BEVEL NAME
	{
	    if ((state.bevel = on_off_only($2)) == -1) YYERROR;
	    else yyReader->bevel(state.bevel);
	}

	| CINTERP NAME
	{
	    if ((state.cinterp = on_off_only($2)) == -1) YYERROR;
	    else yyReader->cinterp(state.cinterp);
	}

	| DINTERP NAME
	{
	    if ((state.dinterp = on_off_only($2)) == -1) YYERROR;
	    else yyReader->dinterp(state.dinterp);
	}

	| USEMTL NAME
	{
	    state.mtl = $2;
	    yyReader->usemtl(state.mtl);
	}

	| USEMAP NAME
	{
	    state.map = $2;
	    yyReader->usemap(state.map);
	}

	| MTLLIB NAME
	{
	    state.mtllib = $2;
	    yyReader->mtllib(state.mtllib);
	}

	| SHADOW_OBJ NAME
	{
	    state.shadow_obj = $2;
	    yyReader->shadow_obj(state.shadow_obj);
	}

	| TRACE_OBJ NAME
	{
	    state.trace_obj = $2;
	    yyReader->trace_obj(state.trace_obj);
	}

	| LOD INTEGER
	{
	    state.lod = $2;
	    yyReader->lod(state.lod);
	}
;

element_list:
	element
	| element_list element
;

element:
	integer				    
	{ 
	    $$=1; 
	    if (index(1,$1,0,0)) YYERROR;
	}

	| integer '/' integer		    
	{ 
	    $$=3; 
	    if (index(3,$1,$3,0)) YYERROR;
	}

	| integer '/' '/' integer	    
	{ 
	    $$=5;
	    if (index(5,$1,0,$4)) YYERROR;
	}

	| integer '/' integer '/' integer   
	{ 
	    $$=7; 
	    if (index(7,$1,$3,$5)) YYERROR;
	}
;

vertex_list:
	scalar			{ add_to_vertex($1); }
	| vertex_list scalar	{ add_to_vertex($2); }
;

scalar:
	SCALAR
	| INTEGER { $$ = float($1); }
;	

integer:
	INTEGER
;	

integer_opt:
	/* empty */ { $$ = -1; }
	| INTEGER
;

name_opt:
	/* empty */ { add_group(""); }
	| NAME	    { add_group($1); }
;

name_list:
	name_opt
	| name_list NAME { add_group($2); }
;

%%

void
clear_all()
{
    state.groups.clear();
    state.active_groups.clear();
    state.vertex_index.clear();
    state.normal_index.clear();
    state.texture_index.clear();

    state.active_object   = "";
    state.mtllib 	  = "";
    state.mtl 		  = "";
    state.map 		  = "";
    state.shadow_obj 	  = "";
    state.trace_obj 	  = "";
    state.smoothing_group = 0;
    state.cinterp 	  = 0;
    state.dinterp 	  = 0;
    state.bevel 	  = 0;
}

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

    bool found = false;

    for (int i=0; i<state.active_groups.size(); i++)
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

