/*
**
*/

%{

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

%}

%start start

%token ERROR NAME KEYWORD CAPWORD OPERATOR STRING CONSTANT EXPR
%left KEYWORD
%left '['
%left OPERATOR
%left NAME
%%

start :
    /* NOTHING */
	{ *parsedExpr = mos_eos; return(0); }
  | expr ';'
  	{ *parsedExpr = $1; return(0); }
  | ';' start
  	{ $$ = $2; }
  ;
  
expr :
    msg
  ;
  

msg :
    keyword_expr
  ;

keyword_expr : 
    keyword_expr sel_expr_list
    	{ $$ = exprMsg(undef, $1, sel($2), args($2)); }
  | sel_expr_list
    	{ $$ = exprMsg(undef, undef, sel($1), args($1)); }
  | array_expr
  ;


sel_expr_list :
    KEYWORD keyword_expr
	{ $$ = selExpr($1,vector1($2)); }
  | sel_expr_list CAPWORD keyword_expr
	{ $$ = $1; append(sel($1), $2); append(args($1), $3); }
  ;

array_expr :
    array_expr '[' expr ']' 
	{ $$ = exprMsg(undef, $1, mos_s(get_), vector1($3)); }
  | array_expr '[' expr ']' ':' op_expr
	{ $$ = exprMsg(undef, $1, mos_s(set_Value_), vector2($3, $6)); }
  | op_expr
  ;


op_expr :
    op_expr OPERATOR op_expr
	{ $$ = exprMsg(undef, $1, $2, vector1($3)); }
  | OPERATOR op_expr
	{ $$ = exprMsg(undef, undef, $1, vector1($2)); }
  | name_expr
  ;

name_expr :
    NAME
	{
	  $$ = exprMsg(undef, undef, $1, undef);
	}
  | name_expr NAME
  	{
	  $$ = exprMsg(undef, $1, $2, undef);
	}
  | rootexpr
  ;

/*
resend :
	{ $$ = undef; }
  | '.' expr
  	{ $$ = $2; }
  ;
*/
    
rootexpr :
    CONSTANT { $$ = mos_exprConstant($1); }
  | STRING { $$ = mos_exprConstant($1); }
  | '`' expr { $$ = mos_exprMemo($2); }
  | EXPR
  | object
  | block
  | '(' expr ')' { $$ = mos_exprGroup($2); }
  ;
  
object :
    '{' slots object_body '}'
  	{ $$ = mos_exprObject($2, $3); }
  ;

object_body :
    /* NOTHING */
        { $$ = nil; }
  | '@' stmts
  	{ $$ = $2; }
  ;
  
slots : 
    /* NOTHING */
        { $$ = nil; }
  | regular_slot 
  	{ $$ = vector1($1); }
  | regular_slot ';' slots
  	{ $$ = cons($1, $3); }
  | method_slot slots
  	{ $$ = cons($1, $2); }
  | method_slot ';' slots
  	{ $$ = cons($1, $3); }
  ;

regular_slot :
    NAME OPERATOR expr 
    	{
	  if ( mos_EQ($2, mos_s(__ASN__)) ) {
	    $$ = mos_exprReadOnlySlot($1, $3);
	  } else
	  if ( mos_EQ($2, mos_s(__RAR__)) ) {
	    $$ = mos_exprSlot($1, $3);
	  } else {
	    mos_value n = mos_send($1, mos_s(asString));
	    mos_error(mos_s(parseError), "Slot definition should be \"%W -> ...\" or \"%W = ...\"", n, n);
	    return -1;
	  }
	}
  | NAME 
    	{ $$ = mos_exprSlot($1, undef); }
  ;

method_slot :
    sel_name_list '{' locals stmts '}'
  	{ $$ = mos_exprMethodSlot(sel($1), mos_exprMethod(args($1), $3, $4)); }
  ;

sel_name_list :
    NAME
    	{ $$ = selExpr($1, nil); }
  | OPERATOR NAME
    	{ $$ = selExpr($1, vector1($2)); }
  | KEYWORD NAME capword_name_list 
        { $$ = $3; cons($1, sel($3)); cons($2, args($3)); }
  ;
  
capword_name_list :
    /* NOTHING */
	{ $$ = selExpr(mos_string_make(0,0),nil); }
  | capword_name_list CAPWORD NAME
	{ $$ = $1; append(sel($1),$2); append(args($1),$3); }
  ;

locals :
    /* NOTHING */
     	{ $$ = nil; }
  | '@' local_list '@'
   	{ $$ = $2; }
  ;

local_list :
    /* NOTHING */
    	{ $$ = nil; }
  | local
   	{ $$ = vector1($1); }
  | local ';' local_list
   	{ $$ = cons($1, $3); }
  ;

local :
    regular_slot
        { $$ = $1; }
   ;
  
stmts :
    /* NOTHING */
     	{ $$ = nil; }
  | expr 
   	{ $$ = vector1($1); }
  | expr ';' stmts
   	{ $$ = cons($1, $3); }
  ;

block_arg_list :
   /* NOTHING */
   	{ $$ = nil; }
  | NAME block_arg_list
   	{ $$ = cons($1, $2); }
  ;

block_args :
    /* NOTHING */
     	{ $$ = nil; }
  | '(' block_arg_list ')'
   	{ $$ = $2; }

block :
    '[' block_args locals stmts ']'
	{ $$ = mos_exprBlock($2, $3, $4); }
  ;
%%



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


