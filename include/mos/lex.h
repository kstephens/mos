#ifndef _mos_lex_h_
#define _mos_lex_h_

#include "mos/mos.h"

typedef struct mos_parse_cnxt {
  mos_value stream;
  mos_value expr;
  mos_value error;
} mos_parse_cnxt;

extern int _mos_yydebug;
int _mos_yylex(mos_value *_yylval, void *cntx);

#endif
