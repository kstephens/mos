#ifndef _mos_lex_h_
#define _mos_lex_h_

/********************************************************************/
/* Info */

/* File:          lex.h */
/* Purpose:       MOS lexical scanner */
/* Author:        Kurt A. Stephens <stephensk@acm.org> */
/* Copyright:     (C) 2000 by ION, Inc. */
/* Creation-Date: Mon Jan 31 22:02:00 2000 */
/* Version-Id:    $Header: /cvs/ioncvs/cvsroot/home/ion/src/mos/include/mos/lex.h,v 1.1 2001-08-13 09:29:28 stephens Exp $ */

/********************************************************************/
/* Interfaces */

#include "mos/mos.h"


/********************************************************************/
/* Types */

typedef struct mos_parse_cnxt {
  mos_value stream;
  mos_value expr;
  mos_value error;
} mos_parse_cnxt;


/********************************************************************/
/* Declarations */

extern int _mos_yydebug;
int _mos_yylex(mos_value *_yylval, void *cntx);


/********************************************************************/
/* EOF */

#endif
