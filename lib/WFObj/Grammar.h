typedef union
{
    int		_token;
    float	_float;
    int		_int;
    int		_flags;
    const char*	_name;
} YYSTYPE;
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


extern YYSTYPE wfobjlval;
