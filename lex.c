#ifndef __rcs_id__
#ifndef __rcs_id_mos_lex_c__
#define __rcs_id_mos_lex_c__
static const char __rcs_id_mos_lex_c[] = "$Id: lex.c,v 1.5 2000-03-21 07:08:17 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include "parse.h"
#include "mos/expr.h"
#include "mos/lex.h"
#include <string.h>
#include <ctype.h>
#include <math.h> /* pow() */


#ifndef PARSE_DEBUG
#define PARSE_DEBUG _mos_yydebug
#endif

#ifndef PARSE_READ_DEBUG
#define PARSE_READ_DEBUG 0
#endif


static int mos_getc(mos_value stream)
{
  mos_value x = mos_send(stream, mos_s(readChar));
  if ( PARSE_READ_DEBUG ) {
    mos_printf(mos_FILE(err), "lex: getc: got %P\n", x);
  }
  return mos_NE(x,mos_eos) ? mos_CHAR(x) : EOF;
}


static int mos_peekc(mos_value stream)
{
  mos_value x = mos_send(stream, mos_s(peekChar));
  if ( PARSE_READ_DEBUG ) {
    mos_printf(mos_FILE(err), "lex: peekc: got %P\n", x);
  }
  return mos_NE(x,mos_eos) ? mos_CHAR(x) : EOF;
}


static int digitInRadix(int c, int radix)
{
  static char digitMap[0x80];
  
  if ( ! digitMap[0] ) {
    int i;
    
    for ( i = 0; i < 0x80; i ++ ) {
      int x = i;
      if ( '0' <= i && i <= '9' ) {
	x -= '0';
      } else
      if ( 'a' <= c && c <= 'z' ) {
	x -= 'a' - 10;
      } else
      if ( 'A' <= c && c <= 'Z' ) {
	x -= 'A' - 10;
      } else {
	x = -1;
      }
      digitMap[i] = x;
    }
  }
  
  c = (c > 0 && c < 0x80) ? digitMap[c] : -1;
  if ( c >= radix )
    c = -1;
  
  return c;
}


int _mos_yylex(mos_value *_yylval, void *cntx)
{
#define STREAM (((mos_parse_cnxt*) cntx)->stream)
#define yylval (*_yylval)
  int c;
  mos_value lexeme;

#define appendLexeme(X) mos_send(lexeme, mos_s(append_), mos_char_make(X))
#define isop(X) strchr(_mos_selector_op_charset, (X))

#if 1
#define GETC() mos_getc(STREAM)
#define PEEKC() mos_peekc(STREAM)
#else
#define GETC() getc(stdin)
#define PEEKC(X) ungetc(X, stdin)
#endif

#define return(X) do { if ( PARSE_DEBUG ) { char buf[12]; sprintf(buf, isprint(X) ? "'%c'" : "'\\x%02x'", (int) (X)); mos_printf(mos_FILE(err), "lex: got %s(%s): value %P\n", #X, buf, yylval); } return X; } while(0)

  whitespace:
  c = PEEKC();
  while ( isspace(c) && c != EOF ) {
    GETC();
    c = PEEKC();
  }
  if ( c == '#' ) {
    int annotation = 0;

    GETC();
    
    c = GETC();
    if ( c == '|' ) {
      c = GETC();
      if ( c == '{' ) {
	annotation = 1;
      } else if ( c == '}' ) {
	annotation = -1;
      }

      do {
	while ( c != '|' && c != EOF ) {
	  c = GETC();
	  if ( annotation == 1 ) {
	  }
	}
	c = GETC();
      } while ( c != '#' && c != EOF );
    } else {
      c = GETC();
      if ( c == '{' ) {
	annotation = 1;
      } else if ( c == '}' ) {
	annotation = -1;
      }

      while ( c != '\n' && c != EOF ) {
	c = GETC();
	if ( annotation == 1 ) {
	}
      } 
    }
    
    goto whitespace;
  }
  
  if ( (isalpha(c) && islower(c)) || (c == '_') ) {
    /* NAME || KEYWORD */
    lexeme = mos_string_make(0,0);
    do {
      appendLexeme(c);
      GETC(); c = PEEKC();
    } while ( (isalpha(c) || isdigit(c) || (c == '_')) && (c != ':') && c != EOF );
    if ( c == ':' ) {
      appendLexeme(c);
      GETC();
      yylval = lexeme;
      return(KEYWORD);
    } else {
      yylval = lexeme;
      return(NAME);
    }
  } else
  if ( (isalpha(c) && isupper(c)) ) {
    /* CAPWORD | RETURN | ERROR */
    lexeme = mos_string_make(0,0);
    do {
      appendLexeme(c);
      GETC(); c = PEEKC();
    } while ( (isalpha(c) || isdigit(c) || (c == '_')) && (c != ':') && c != EOF );
    if ( c == ':' ) {
      appendLexeme(c);
      GETC();
      yylval = lexeme;
      return(CAPWORD);
    } else {
      /* ERROR */
      mos_error(mos_s(parseError), "at token %P", lexeme);
      yylval = mos_undef;
      return(ERROR);
    }
  } else
  if ( c == '-' || c == '+' || isdigit(c) || c == '.' ) {
    /* NUMBER */
#ifndef DIGIT
#define DIGIT char
#endif
    DIGIT dbuf[96], *digits = dbuf;
    short radix = 10;
    short readRadix = radix;
    short neg = c == '-';
    int exp = 0;
    short isReal = 0;
    short digit;
    
    if ( c == '0' ) {
      radix = 8;
      GETC(); c = PEEKC();
      if ( c == 'x' || c == 'X' ) {
	GETC(); c = PEEKC();
	readRadix = radix = 16;
      } else
      if ( c == 'b' || c == 'B' ) {
	GETC(); c = PEEKC();
	readRadix = radix = 2;
      }
    } else
    if ( c == '-' || c == '+' || c == '.' ) { /* could be "->" or "-1234" */
      int nc;
      
      GETC();
      nc = PEEKC();
      if ( ! isdigit(nc) ) {
	if ( (c == '-' || c == '+') && nc == '.' ) {
	  c = nc;
	  goto doFractional;
	}
	if ( c == '.' ) {
	  goto doChar;
	} else {
	  lexeme = mos_string_make(0,0);
	  appendLexeme(c);
	  c = nc;
	  goto doOp;
	}
      }
      c = nc;
    }
    
    while ( (digit = digitInRadix(c, readRadix)) >= 0 ) {
      *(digits ++) = digit;
      GETC(); c = PEEKC();
    }
    
    if ( c == '.' ) {
      doFractional:

      readRadix = radix = 10;
      isReal = 1;
      GETC(); c = PEEKC();
      while ( (digit = digitInRadix(c, radix)) >= 0 ) {
	*(digits ++) = digit;
	exp --;
	GETC(); c = PEEKC();
      }
    }
      
    if ( radix == 10 ) {
      if ( c == 'e' || c == 'E' || c == 'f' || c == 'F' || c == 'g' || c == 'G' ) {
	int exp2 = 0;
	int exp2Neg = 0;
	
	isReal = 1;
	GETC(); c = PEEKC();
	
	if ( c == '-' || c == '+' ) {
	  exp2Neg = c == '-';
	  GETC(); c = PEEKC();
	}
	
	while ( (digit = digitInRadix(c, radix)) >= 0 ) {
	  exp2 *= radix;
	  exp2 += digit;
	  GETC(); c = PEEKC();
	}
	
	if ( exp2Neg ) exp2 = - exp2;
	exp += exp2;
      }
    }
    
    /* Let's do the number contruction here with the digits and exponent */
    
    if ( isReal ) { doReal: {
      double n = 0;
      double p = pow((double) radix, (double) exp);
      while ( digits > dbuf ) {
        n += p * *(-- digits);
	p *= radix;
      }
      if ( neg )
        n = - n;
      yylval = mos_real_make(n);
    } } else {
      long n = 0;
      DIGIT *d = dbuf;
      
      while ( d < digits ) {
        /* Check for overflow */
        if ( n > n * radix ) {
	  goto doReal;
	}
	n *= radix;
        n += *(d ++);
      }
      if ( neg )
        n = - n;
      yylval = mos_integer_make(n);
    }
    return(CONSTANT);
#undef DIGIT
  } else
  if ( c == '"' || c == '\'' ) {
    /* STRING || CHAR */
    int term = c;
    
    lexeme = mos_string_make(0,0);
    GETC();
    while ( (c = GETC()) != EOF && c != term ) {
      appendLexeme(c);
      if ( c == '\\' ) {
	c = GETC();
	appendLexeme(c);
      }
    }
    yylval = mos_send(lexeme, mos_s(unescapeC));
    mos_send(lexeme, mos_s(makeReadonly));
    if ( term == '\'' ) {
      yylval = mos_send(yylval, mos_s(asChar));
    }
    return(STRING);
  } else
  if ( isop(c) ) {
    /* OPERATOR */
    lexeme = mos_string_make(0,0);
    doOp:
    while ( c != EOF && isop(c) ) {
      GETC();
      appendLexeme(c);
      c = PEEKC();
    }
    yylval = mos_send(lexeme, mos_s(asSelector));
    return(OPERATOR);
  } else
  if ( c == '@' ) {
    GETC();
    c = PEEKC();

    switch ( c ) {
    case 'U': case 'u': {
	GETC();
	yylval = mos_undef;
	return(CONSTANT);
    }
    break;
    
    case 'E': case 'e': {
	GETC();
	yylval = mos_eos;
	return(CONSTANT);
    }
    break;
    
    case 'T': case 't': {
	GETC();
	yylval = mos_true;
	return(CONSTANT);
    }
    break;
    
    case 'F': case 'f': {
	GETC();
	yylval = mos_false;
	return(CONSTANT);
    }
    break;
    
    case 'V': case 'v': {
	GETC();
	yylval = mos_exprVector(mos_o(vector));
	return(EXPR);
    }
    break;
    
    case 'M': case 'm': {
	GETC();
	yylval = mos_exprMap();
	return(EXPR);
    }
    break;
    
    case 'N': case 'n': {
	GETC();
	/* Parse a named object identifier */
	_mos_yylex(&yylval, cntx);

	/* Get an object by name */
	yylval = mos_object_named(yylval);

	/* It's a constant */
	yylval = mos_exprConstant(yylval);
	return(EXPR);
    }
    break; 
    
    case 'G': case 'g': {
	GETC();
	/* Parse a getter object */
	_mos_yylex(&yylval, cntx);

	/* Get an object by name */
	yylval = mos_getter_method(mos_INT(yylval));

	/* It's a constant */
	yylval = mos_exprConstant(yylval);
	return(EXPR);
    }
    break; 
    
    case 'S': case 's': {
	GETC();
	/* Parse a getter object */
	_mos_yylex(&yylval, cntx);

	/* Get an object by name */
	yylval = mos_setter_method(mos_INT(yylval));

	/* It's a constant */
	yylval = mos_exprConstant(yylval);
	return(EXPR);
    }
    break; 
    
    case '#': {
	GETC();
	/* Parse a getter object */
	_mos_yylex(&yylval, cntx);

	/* Get an object by name */
	yylval = mos_send(yylval, mos_s(asObject));

	/* It's a constant */
	yylval = mos_exprConstant(yylval);
	return(EXPR);
    }
    break;
 
    case '"': {
	/* @"xxx" -> `("xxx" asSelector) */
	_mos_yylex(&yylval, cntx);
	yylval = mos_exprConstant(mos_send(yylval, mos_s(asSelector)));
	return(EXPR);
    }
    break;
    }
    
    yylval = mos_undef;
    return('@');
  } else {
    GETC();
    doChar:
    yylval = mos_undef;
    return(c);
  }
}
#undef STREAM
#undef return
#undef yylval

#undef appendLexeme
#undef isop
