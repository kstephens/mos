/*
**
*/

%{

#ifndef __rcs_id__
#ifndef __rcs_id_old_parse__y__
#define __rcs_id_old_parse__y__
static const char __rcs_id_old_parse__y[] = "$Id: parse_.y,v 1.2 1999-02-19 09:26:36 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

%{

#include "mos/mos.h"
#include "mos/expr.h"

#define exprGroup(X) X
#define nil mos_vector_make(0,0)
#define undef mos_undef
#define append(L,X) mos_send((L), mos_s(append_), (X))
#define cons(X,L) mos_send((L), mos_s(insert_), (X))
#define selExpr(S,L) mos_vector_make_(2, (S), (L))
#define vector1(X) mos_vector_make_(1, (X))
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
  SEL = mos_exprConstant(SEL);
  CNTX = mos_exprSend_V(CNTX,RCVR,SEL,ARGS);
  
  mos_printf(mos_FILE(err), "parse: got %MW\n", mos_s(printExprOn_), CNTX);

  return CNTX;
}

static int _mos_parseFromStream(mos_value STREAM);

#define yyparse() static int _mos_parseFromStream(mos_value STREAM)

#define YYDEBUG
#define YYSTYPE mos_value

%}

%start start

%token ERROR NAME KEYWORD CAPWORD OPERATOR STRING CONSTANT
%%

start :
    expr     { yyval = $1; return(0); }
  | expr ';' { yyval = $1; return(0); }
  | ';' expr { $$ = $2; }
  ;
  
expr :
    '(' expr ')'
    	{ $$ = exprGroup($2); }
  | msg
  ;
  

msg :
    keyword_expr
  ;

keyword_expr : 
    keyword_expr resend sel_expr_list
    	{ $$ = exprMsg($2, $1, sel($2), args($2)); }
  | resend sel_expr_list
    	{ $$ = exprMsg($1, undef, sel($1), args($1)); }
  | op_expr
  ;

sel_expr_list :
    name
	{ $$ = selExpr($1,nil); }
  | keyword keyword_expr
	{ $$ = selExpr($1,vector1($2)); }
  | sel_expr_list capword keyword_expr
	{ $$ = $1; append(sel($1),$2); append(args($1),$3); }
  ;

op_expr :
  | op_expr resend operator op_expr
	{ $$ = exprMsg($2, $1, $3, vector1($4)); }
  | resend operator op_expr
	{ $$ = exprMsg($1, undef, $2, vector1($3)); }
  | name_expr
  ;

name_expr :
    name
	{
	  $$ = exprMsg(undef, undef, $1, undef);
	}
  | constant
  | name_expr name
  	{
	  $$ = exprMsg(undef, $1, $2, undef);
	}
  ;

resend :
	{ $$ = undef; }
  | '@' expr
  	{ $$ = $2; }
  ;
  
    
constant :
    CONSTANT { $$ = mos_exprConstant($1); }
  | STRING { $$ = mos_exprConstant($1); }
  | '@' STRING { $$ = mos_exprConstant(mos_send($2, mos_s(asString))); }
  | object
  ;
  
object :
  '@' '{' slots '}'
  	{ $$ = mos_exprObject($2); }
  ;

slots : 
        { $$ = nil; }
  | slots slot
  	{ $$ = append($1, $2); }
  ;

slot :
    name '=' expr ';'
    	{ $$ = mos_exprReadonlySlot($1, $3); }
  | name ';'
    	{ $$ = mos_exprSlot($1, undef); }
  | keyword expr ';'
  	{ $$ = mos_exprSlot($1, $2);}
  | keyword name capword_name_list block
  	{ $$ = mos_exprMethodSlot(append($1, sel($3)), mos_exprMethod(cons($2, args($3)), $3)); }
  ;

capword_name_list :

	{ $$ = selExpr(mos_string_make(0,0),nil); }
  | capword name
	{ $$ = selExpr($1,vector1($2)); }
  | capword_name_list capword name
	{ $$ = $1; append(sel($1),$2); append(args($1),$3); }
  ;

block :
    '{' locals stmts '}'
	{ $$ = mos_exprBlock($2, $3); }
  ;

locals :
     
     	{ $$ = nil; }
   | '{' local_list '}'
   	{ $$ = $1; }
   ;

local_list :
     	{ $$ = nil; }
   | local_list local
   	{ $$ = append($1, $2); }
   ;

local :
    name '=' expr ';'
    	{ $$ = mos_exprReadonlySlot($1, $3); }
  | name ';'
    	{ $$ = mos_exprSlot($1, undef); }
  | keyword expr ';'
  	{ $$ = mos_exprSlot($1, $2);}
  ;
  
stmts :
     	{ $$ = nil; }
   | stmts stmt
   	{ $$ = append($1, $2); }
   ;
   
stmt:
     expr ';'
   	{ $$ = $1; }
   ;

name : NAME ;
keyword : KEYWORD ;
capword : CAPWORD ;
operator : OPERATOR ;

%%

#include <string.h>
#include <ctype.h>

static int yyerror(const char *msg)
{
  mos_error(mos_s(parseError), msg);
  return 0;
}

static int mos_getc(mos_value stream)
{
  mos_value x = mos_send(stream, mos_s(readChar));
  return mos_NE(x,mos_eos) ? mos_CHAR(x) : EOF;
}
static int mos_peekc(mos_value stream)
{
  mos_value x = mos_send(stream, mos_s(peekChar));
  return mos_NE(x,mos_eos) ? mos_CHAR(x) : EOF;
}

static int yylex(mos_value stream)
{
  int c;
  mos_value lexeme;
  mos_value string;
  
#define appendLexeme(X) mos_send(lexeme, mos_s(append_), mos_char_make(X))
#define isop(X) strchr(_mos_selector_op_charset, (X))

#if 1
#define GETC() mos_getc(stream)
#define PEEKC() mos_peekc(stream)
#else
#define GETC() getc(stdin)
#define PEEKC(X) ungetc(X, stdin)
#endif

#define return(X) do { mos_printf(mos_FILE(err), "lex: got %s: value %W\n", #X, yylval); return X; } while(0)

  whitespace:
  c = PEEKC();
  while ( isspace(c) && c != EOF ) {
    GETC();
    c = PEEKC();
  }
  if ( c == '#' ) {
    c = GETC();
    while ( c != '\n' && c != EOF ) {
      c = GETC();
    }
    goto whitespace;
  }
  
  if ( (isalpha(c) && islower(c)) || (c == '_') ) {
    /* NAME || KEYWORD */
    lexeme = mos_string_make(0,0);
    do {
      c = GETC();
      appendLexeme(c);
      c = PEEKC();
    } while ( (isalpha(c) || (c == '_')) && (c != ':') );
    if ( c == ':' ) {
      c = GETC();
      appendLexeme(c);
      yylval = lexeme;
      return(KEYWORD);
    } else {
      yylval = lexeme;
      return(NAME);
    }
  } else
  if ( (isalpha(c) && isupper(c)) || (c == ':') ) {
    /* CAPWORD | RETURN | ERROR */
    lexeme = mos_string_make(0,0);
    do {
      c = GETC();
      appendLexeme(c);
      c = PEEKC();
    } while ( (isalpha(c) && islower(c)) || (c == '_') || (c != ':') );
    if ( c == ':' ) {
      c = GETC();
      appendLexeme(c);
      yylval = lexeme;
      return(CAPWORD);
    } else {
      /* ERROR */
      mos_error(mos_s(parseError), "at token %W", lexeme);
      yylval = mos_undef;
      return(ERROR);
    }
  } else
  if ( c == '-' || c == '+' || isdigit(c) ) {
    /* NUMBER */
    long n = 0;
    int neg = c == '-';
    
    if ( c == '-' || c == '+' ) {
      int nc;
      c = GETC();
      nc = PEEKC();
      if ( ! isdigit(nc) ) {
        lexeme = mos_string_make(0,0);
        appendLexeme(c);
	goto doOp;
      }
    }
    
    do {
      c = GETC();
      n *= 10;
      n += c - '0';
      c = PEEKC();
    } while ( isdigit(c) );
    
    yylval = mos_integer_make(neg ? - n : n);
    return(CONSTANT);
  } else
  if ( c == '"' ) {
    /* STRING */
    lexeme = mos_string_make(0,0);
    GETC();
    while ( (c = GETC()) && c != '"' ) {
      appendLexeme(c);
      if ( c == '\\' ) {
	c = GETC();
	appendLexeme(c);
      }
    }
    yylval = mos_send(lexeme, mos_s(unescapeC));
    return(STRING);
  } else
  if ( isop(c) ) {
    /* OPERATOR */
    lexeme = mos_string_make(0,0);
    doOp:
    do {
      c = GETC();
      appendLexeme(c);
      c = PEEKC();
    } while ( isop(c) );
    yylval = lexeme;
    return(OPERATOR);
  } else {
    c = GETC();
    yylval = mos_undef;
    return(c);
  }
}
#undef return

#undef appendLexeme
#undef isop

#define yylex() yylex(STREAM)


mos_METHOD(parser,parseExprFrom_)
{
  extern int yydebug;

  yydebug = 1;
  if ( _mos_parseFromStream(mos_ARGV[0]) ) {
    mos_return(mos_undef);
  } else {
    mos_return(yyval);
  }
}
mos_METHOD_END

mos_OBJECT(parser)
mos_OBJECT_M(parser,parseExprFrom_)
mos_OBJECT_SLOTS(parser)
mos_OBJECT_END(compiler,parser,mos_object,basicMeta)

