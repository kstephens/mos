
/*  A Bison parser, made from parse.y
 by  GNU Bison version 1.25
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	ERROR	258
#define	NAME	259
#define	KEYWORD	260
#define	CAPWORD	261
#define	OPERATOR	262
#define	STRING	263
#define	CONSTANT	264
#define	EXPR	265

#line 5 "parse.y"


#include "mos/mos.h"
#include "mos/expr.h"
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcat */

#ifndef PARSE_DEBUG
extern int yydebug;
#define PARSE_DEBUG yydebug
#endif

#define nil mos_vector_make(0,0)
#define undef mos_undef
#define append(L,X) mos_send((L), mos_s(append_), (X))
#define cons(X,L) mos_send((L), mos_s(insert_), (X))
#define selExpr(S,L) mos_vector_make_(2, (S), (L))
#define vector1(X) mos_vector_make_(1, (X))
#define vector2(X,Y) mos_vector_make_(2, (X), (Y))
#define sel(X) mos_vector_V(X)[0]
#define args(X) mos_vector_V(X)[1]

static
mos_value exprMsg(mos_value CNTX, mos_value RCVR, mos_value SEL, mos_value ARGS)
{
  if ( mos_EQ(ARGS,mos_undef) ) {
    static mos_value x = mos_undef;

    if ( mos_EQ(x,mos_undef) ) {
      x = mos_vector_make(0,0);
    }

    ARGS = x;
  }

  SEL = mos_send(SEL, mos_s(asSelector));

  /* SEL = mos_exprConstant(SEL); */
  CNTX = mos_exprSend_V(CNTX, RCVR, SEL, ARGS);
  
  if ( PARSE_DEBUG ) {
    mos_printf(mos_FILE(err), "parse: got %MW\n", mos_s(printExprOn_), CNTX);
  }
  
  return CNTX;
}

#define YYSTYPE mos_value

#ifndef YYBISON
#error Must use bison
#endif

#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYERROR_VERBOSE 1
#define YYPURE 1

extern int _mos_yylex(mos_value *_yylval, mos_value STREAM);
#define yylex _mos_yylex
#define YYLEX_PARAM STREAM

#define _MOS_PARSE _mos_parseFromStream
#define _MOS_PARSE_PARAM_ARG STREAM, parsedExpr
#define _MOS_PARSE_PARAM_DECL mos_value STREAM; mos_value *parsedExpr;
_MOS_PARSE ();

#define yyparse _MOS_PARSE
extern mos_value yylval;
#define YYPARSE_PARAM_ARG _MOS_PARSE_PARAM_ARG
#define YYPARSE_PARAM_DECL _MOS_PARSE_PARAM_DECL


static int yyerror(const char *msg)
{
  mos_error(mos_s(parseError), msg);
  return 0;
}

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		89
#define	YYFLAG		-32768
#define	YYNTBASE	21

#define YYTRANSLATE(x) ((unsigned)(x) <= 265 ? yytranslate[x] : 44)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,    16,
    17,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    14,    12,     2,
     2,     2,     2,    20,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    11,     2,    13,     2,     2,    15,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    18,     2,    19,     2,     2,     2,     2,     2,
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
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     1,     4,     7,     9,    11,    14,    16,    18,    21,
    25,    30,    37,    39,    43,    46,    48,    50,    53,    55,
    57,    59,    62,    64,    66,    68,    72,    77,    78,    81,
    82,    84,    88,    91,    95,    99,   101,   107,   109,   112,
   116,   117,   121,   122,   126,   127,   129,   133,   135,   136,
   138,   142,   143,   146,   147,   151
};

static const short yyrhs[] = {    -1,
    22,    12,     0,    12,    21,     0,    23,     0,    24,     0,
    24,    25,     0,    25,     0,    26,     0,     5,    24,     0,
    25,     6,    24,     0,    26,    11,    22,    13,     0,    26,
    11,    22,    13,    14,    27,     0,    27,     0,    27,     7,
    27,     0,     7,    27,     0,    28,     0,     4,     0,    28,
     4,     0,    29,     0,     9,     0,     8,     0,    15,    22,
     0,    10,     0,    30,     0,    43,     0,    16,    22,    17,
     0,    18,    32,    31,    19,     0,     0,    20,    40,     0,
     0,    33,     0,    33,    12,    32,     0,    34,    32,     0,
    34,    12,    32,     0,     4,     7,    22,     0,     4,     0,
    35,    18,    37,    40,    19,     0,     4,     0,     7,     4,
     0,     5,     4,    36,     0,     0,    36,     6,     4,     0,
     0,    20,    38,    20,     0,     0,    39,     0,    39,    12,
    38,     0,    33,     0,     0,    22,     0,    22,    12,    40,
     0,     0,     4,    41,     0,     0,    16,    41,    17,     0,
    11,    42,    37,    40,    13,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    96,    99,   101,   105,   110,   114,   117,   119,   123,   126,
   130,   133,   135,   139,   142,   144,   147,   152,   156,   167,
   169,   170,   171,   172,   173,   174,   177,   182,   185,   189,
   192,   194,   196,   198,   202,   216,   220,   225,   228,   230,
   234,   237,   241,   244,   248,   251,   253,   257,   262,   265,
   267,   271,   274,   278,   281,   284
};
#endif


#if YYDEBUG != 0 || defined (YYERROR_VERBOSE)

static const char * const yytname[] = {   "$","error","$undefined.","ERROR",
"NAME","KEYWORD","CAPWORD","OPERATOR","STRING","CONSTANT","EXPR","'['","';'",
"']'","':'","'`'","'('","')'","'{'","'}'","'@'","start","expr","msg","keyword_expr",
"sel_expr_list","array_expr","op_expr","name_expr","rootexpr","object","object_body",
"slots","regular_slot","method_slot","sel_name_list","capword_name_list","locals",
"local_list","local","stmts","block_arg_list","block_args","block", NULL
};
#endif

static const short yyr1[] = {     0,
    21,    21,    21,    22,    23,    24,    24,    24,    25,    25,
    26,    26,    26,    27,    27,    27,    28,    28,    28,    29,
    29,    29,    29,    29,    29,    29,    30,    31,    31,    32,
    32,    32,    32,    32,    33,    33,    34,    35,    35,    35,
    36,    36,    37,    37,    38,    38,    38,    39,    40,    40,
    40,    41,    41,    42,    42,    43
};

static const short yyr2[] = {     0,
     0,     2,     2,     1,     1,     2,     1,     1,     2,     3,
     4,     6,     1,     3,     2,     1,     1,     2,     1,     1,
     1,     2,     1,     1,     1,     3,     4,     0,     2,     0,
     1,     3,     2,     3,     3,     1,     5,     1,     2,     3,
     0,     3,     0,     3,     0,     1,     3,     1,     0,     1,
     3,     0,     2,     0,     3,     5
};

static const short yydefact[] = {     1,
    17,     0,     0,    21,    20,    23,    54,     1,     0,     0,
    30,     0,     4,     5,     7,     8,    13,    16,    19,    24,
    25,     9,    15,    52,    43,     3,    22,     0,    36,     0,
     0,    28,    31,    30,     0,     2,     6,     0,     0,     0,
    18,    52,     0,    45,    49,    26,     0,    41,    39,    49,
     0,    30,    30,    33,    43,    10,     0,    14,    53,    55,
    36,    48,     0,    46,    50,     0,    35,    40,    29,    27,
    32,    34,    49,    11,    44,    45,    49,    56,     0,     0,
     0,    47,    51,    42,    37,    12,     0,     0,     0
};

static const short yydefgoto[] = {    26,
    65,    13,    14,    15,    16,    17,    18,    19,    20,    51,
    32,    33,    34,    35,    68,    45,    63,    64,    66,    43,
    25,    21
};

static const short yypact[] = {    11,
-32768,    45,    58,-32768,-32768,-32768,    -9,    11,    45,    45,
    29,    -4,-32768,     8,    25,     3,    10,    35,-32768,-32768,
-32768,-32768,-32768,    38,    24,-32768,-32768,    26,    23,    41,
    43,    31,    46,    75,    39,-32768,    25,    45,    45,    58,
-32768,    38,    42,    60,    45,-32768,    45,-32768,-32768,    45,
    51,    29,    29,-32768,    24,     8,    59,-32768,-32768,-32768,
    64,-32768,    55,    65,    69,    70,-32768,    78,-32768,-32768,
-32768,-32768,    45,    71,-32768,    60,    45,-32768,    82,    72,
    58,-32768,-32768,-32768,-32768,    10,    88,    89,-32768
};

static const short yypgoto[] = {    90,
     1,-32768,     0,   -10,-32768,    -3,-32768,-32768,-32768,-32768,
   -28,   -41,-32768,-32768,-32768,    37,    17,-32768,   -45,    52,
-32768,-32768
};


#define	YYLAST		94


static const short yytable[] = {    23,
    12,    22,    62,    37,    69,    54,    24,    36,    12,    27,
    28,    37,     2,    39,     1,     2,    40,     3,     4,     5,
     6,     7,     8,    71,    72,     9,    10,    80,    11,    47,
    38,    83,    29,    30,    62,    31,    58,    56,    41,    57,
   -38,    42,    46,    44,    48,    37,    49,    67,     1,     2,
    50,     3,     4,     5,     6,     7,    55,    52,    60,     9,
    10,     1,    11,    61,     3,     4,     5,     6,     7,    70,
    47,    74,     9,    10,    75,    11,    76,    86,    29,    30,
    77,    31,    78,    79,    81,    84,    53,    88,    89,    87,
    85,    73,    82,    59
};

static const short yycheck[] = {     3,
     0,     2,    44,    14,    50,    34,    16,    12,     8,     9,
    10,    22,     5,    11,     4,     5,     7,     7,     8,     9,
    10,    11,    12,    52,    53,    15,    16,    73,    18,     7,
     6,    77,     4,     5,    76,     7,    40,    38,     4,    39,
    18,     4,    17,    20,     4,    56,     4,    47,     4,     5,
    20,     7,     8,     9,    10,    11,    18,    12,    17,    15,
    16,     4,    18,     4,     7,     8,     9,    10,    11,    19,
     7,    13,    15,    16,    20,    18,    12,    81,     4,     5,
    12,     7,    13,     6,    14,     4,    12,     0,     0,     0,
    19,    55,    76,    42
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "/logopolis/monitor/noer/b20/cygwin-b20/share/bison.simple"

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
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
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

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
/* int yyparse (void); */
#endif

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
     int count;
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
__yy_memcpy (char *to, char *from, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 196 "/logopolis/monitor/noer/b20/cygwin-b20/share/bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#if !(defined(YYPARSE_PARAM_ARG)&&defined(YYPARSE_PARAM_DECL))
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
#endif
#endif /* not YYPARSE_PARAM */

int
yyparse (YYPARSE_PARAM_ARG)
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
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss, (char *)yyss1, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs, (char *)yyvs1, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls, (char *)yyls1, size * sizeof (*yylsp));
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

case 1:
#line 98 "parse.y"
{ *parsedExpr = mos_eos; return(0); ;
    break;}
case 2:
#line 100 "parse.y"
{ *parsedExpr = yyvsp[-1]; return(0); ;
    break;}
case 3:
#line 102 "parse.y"
{ yyval = yyvsp[0]; ;
    break;}
case 6:
#line 116 "parse.y"
{ yyval = exprMsg(undef, yyvsp[-1], sel(yyvsp[0]), args(yyvsp[0])); ;
    break;}
case 7:
#line 118 "parse.y"
{ yyval = exprMsg(undef, undef, sel(yyvsp[0]), args(yyvsp[0])); ;
    break;}
case 9:
#line 125 "parse.y"
{ yyval = selExpr(yyvsp[-1],vector1(yyvsp[0])); ;
    break;}
case 10:
#line 127 "parse.y"
{ yyval = yyvsp[-2]; append(sel(yyvsp[-2]), yyvsp[-1]); append(args(yyvsp[-2]), yyvsp[0]); ;
    break;}
case 11:
#line 132 "parse.y"
{ yyval = exprMsg(undef, yyvsp[-3], mos_s(get_), vector1(yyvsp[-1])); ;
    break;}
case 12:
#line 134 "parse.y"
{ yyval = exprMsg(undef, yyvsp[-5], mos_s(set_Value_), vector2(yyvsp[-3], yyvsp[0])); ;
    break;}
case 14:
#line 141 "parse.y"
{ yyval = exprMsg(undef, yyvsp[-2], yyvsp[-1], vector1(yyvsp[0])); ;
    break;}
case 15:
#line 143 "parse.y"
{ yyval = exprMsg(undef, undef, yyvsp[-1], vector1(yyvsp[0])); ;
    break;}
case 17:
#line 149 "parse.y"
{
	  yyval = exprMsg(undef, undef, yyvsp[0], undef);
	;
    break;}
case 18:
#line 153 "parse.y"
{
	  yyval = exprMsg(undef, yyvsp[-1], yyvsp[0], undef);
	;
    break;}
case 20:
#line 168 "parse.y"
{ yyval = mos_exprConstant(yyvsp[0]); ;
    break;}
case 21:
#line 169 "parse.y"
{ yyval = mos_exprConstant(yyvsp[0]); ;
    break;}
case 22:
#line 170 "parse.y"
{ yyval = mos_exprMemo(yyvsp[0]); ;
    break;}
case 26:
#line 174 "parse.y"
{ yyval = mos_exprGroup(yyvsp[-1]); ;
    break;}
case 27:
#line 179 "parse.y"
{ yyval = mos_exprObject(yyvsp[-2], yyvsp[-1]); ;
    break;}
case 28:
#line 184 "parse.y"
{ yyval = nil; ;
    break;}
case 29:
#line 186 "parse.y"
{ yyval = yyvsp[0]; ;
    break;}
case 30:
#line 191 "parse.y"
{ yyval = nil; ;
    break;}
case 31:
#line 193 "parse.y"
{ yyval = vector1(yyvsp[0]); ;
    break;}
case 32:
#line 195 "parse.y"
{ yyval = cons(yyvsp[-2], yyvsp[0]); ;
    break;}
case 33:
#line 197 "parse.y"
{ yyval = cons(yyvsp[-1], yyvsp[0]); ;
    break;}
case 34:
#line 199 "parse.y"
{ yyval = cons(yyvsp[-2], yyvsp[0]); ;
    break;}
case 35:
#line 204 "parse.y"
{
	  if ( mos_EQ(yyvsp[-1], mos_s(__ASN__)) ) {
	    yyval = mos_exprReadOnlySlot(yyvsp[-2], yyvsp[0]);
	  } else
	  if ( mos_EQ(yyvsp[-1], mos_s(__RAR__)) ) {
	    yyval = mos_exprSlot(yyvsp[-2], yyvsp[0]);
	  } else {
	    mos_value n = mos_send(yyvsp[-2], mos_s(asString));
	    mos_error(mos_s(parseError), "Slot definition should be \"%W -> ...\" or \"%W = ...\"", n, n);
	    return -1;
	  }
	;
    break;}
case 36:
#line 217 "parse.y"
{ yyval = mos_exprSlot(yyvsp[0], undef); ;
    break;}
case 37:
#line 222 "parse.y"
{ yyval = mos_exprMethodSlot(sel(yyvsp[-4]), mos_exprMethod(args(yyvsp[-4]), yyvsp[-2], yyvsp[-1])); ;
    break;}
case 38:
#line 227 "parse.y"
{ yyval = selExpr(yyvsp[0], nil); ;
    break;}
case 39:
#line 229 "parse.y"
{ yyval = selExpr(yyvsp[-1], vector1(yyvsp[0])); ;
    break;}
case 40:
#line 231 "parse.y"
{ yyval = yyvsp[0]; cons(yyvsp[-2], sel(yyvsp[0])); cons(yyvsp[-1], args(yyvsp[0])); ;
    break;}
case 41:
#line 236 "parse.y"
{ yyval = selExpr(mos_string_make(0,0),nil); ;
    break;}
case 42:
#line 238 "parse.y"
{ yyval = yyvsp[-2]; append(sel(yyvsp[-2]),yyvsp[-1]); append(args(yyvsp[-2]),yyvsp[0]); ;
    break;}
case 43:
#line 243 "parse.y"
{ yyval = nil; ;
    break;}
case 44:
#line 245 "parse.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 45:
#line 250 "parse.y"
{ yyval = nil; ;
    break;}
case 46:
#line 252 "parse.y"
{ yyval = vector1(yyvsp[0]); ;
    break;}
case 47:
#line 254 "parse.y"
{ yyval = cons(yyvsp[-2], yyvsp[0]); ;
    break;}
case 48:
#line 259 "parse.y"
{ yyval = yyvsp[0]; ;
    break;}
case 49:
#line 264 "parse.y"
{ yyval = nil; ;
    break;}
case 50:
#line 266 "parse.y"
{ yyval = vector1(yyvsp[0]); ;
    break;}
case 51:
#line 268 "parse.y"
{ yyval = cons(yyvsp[-2], yyvsp[0]); ;
    break;}
case 52:
#line 273 "parse.y"
{ yyval = nil; ;
    break;}
case 53:
#line 275 "parse.y"
{ yyval = cons(yyvsp[-1], yyvsp[0]); ;
    break;}
case 54:
#line 280 "parse.y"
{ yyval = nil; ;
    break;}
case 55:
#line 282 "parse.y"
{ yyval = yyvsp[-1]; ;
    break;}
case 56:
#line 286 "parse.y"
{ yyval = mos_exprBlock(yyvsp[-3], yyvsp[-2], yyvsp[-1]); ;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 498 "/logopolis/monitor/noer/b20/cygwin-b20/share/bison.simple"

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
}
#line 288 "parse.y"




mos_METHOD(parser,parseExprFrom_)
{
  extern int yydebug;
  mos_value expr = mos_undef;

  int yydebug_save = yydebug;
  
  yydebug = mos_NE(mos_send(mos_RCVR, mos_s(parseDebug)), mos_false);
  if ( _mos_parseFromStream(mos_ARGV[0], &expr) ) {
    expr = mos_undef;
  }
  yydebug = yydebug_save;
  mos_return(expr);
}
mos_METHOD_END

mos_OBJECT(parser)
mos_OBJECT_M(parser,parseExprFrom_)
mos_OBJECT_A(parseDebug,0)
mos_OBJECT_SLOTS(parser)
mos_OBJECT_S(mos_false)
mos_OBJECT_END(compiler,parser,mos_object,basicMeta)


