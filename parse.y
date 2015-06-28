%parse-param {mos_parse_cntx* cntx}
%lex-param {mos_parse_cntx *cntx}
%define api.pure full
%{
/*
**
*/

#include "mos/mos.h"
#include "mos/expr.h"
#include "mos/lex.h"
#include "parse.h"
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strcat */

#define nil mos_vector_make(0,0)
#define undef mos_undef
#define append(L,X) mos_send((L), mos_s(append_), (X))
#define cons(X,L) mos_send((L), mos_s(insert_), (X))
#define selExpr(S,L) mos_vector_make_(2, (S), (L))
#define vector1(X) mos_vector_make_(1, (X))
#define vector2(X,Y) mos_vector_make_(2, (X), (Y))
#define sel(X) mos_vector_V(X)[0]
#define args(X) mos_vector_V(X)[1]

#define PARSE_DEBUG _mos_yydebug

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

#define YYDEBUG 1

#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYERROR_VERBOSE 1

static int _mos_yyerror(YYLTYPE *_yylloc, mos_parse_cntx *cntx, const char *msg)
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
	{ cntx->expr = mos_eos; return(0); }
  | expr ';'
	{ cntx->expr = $1; return(0); }
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


/******************************************************************/


mos_ANNOT("Module: parser")
mos_ANNOT("Doc: Parse MOS language expressions.")


/******************************************************************/


  mos_ANNOT("Category: Parse")


mos_METHOD(parser,parseExprFrom_)
{
  extern int yydebug;
  mos_parse_cntx cntx;
  int yydebug_save;

  /* Initialize context. */
  cntx.stream = mos_ARGV[0];
  cntx.expr = mos_undef;
  cntx.error = mos_undef;

  /* Save yydebug. */
  yydebug_save = yydebug;

  /* Set yydebug. */
  yydebug = mos_NE(mos_send(mos_RCVR, mos_s(parseDebug)), mos_false);
  // yydebug = 1;

  /* Parse top-level expression. */
  if ( _mos_yyparse(&cntx) ) {
    /* Cannot parse. */
    cntx.expr = mos_undef;
  }

  /* Restore yydebug. */
  yydebug = yydebug_save;

  /* Return the expression. */
  mos_return(cntx.expr);
}
mos_METHOD_END

  mos_ANNOT_END


/******************************************************************/


mos_OBJECT(parser)
mos_OBJECT_M(parser,parseExprFrom_)
mos_ANNOT("Doc: Controls parse debug output.")
mos_OBJECT_A(parseDebug,0)
mos_ANNOT_END
mos_OBJECT_SLOTS(parser)
mos_OBJECT_S(mos_false)
mos_OBJECT_END(compiler,parser,mos_object,basicMeta)


/******************************************************************/


mos_ANNOT_END
mos_ANNOT_END


/******************************************************************/

