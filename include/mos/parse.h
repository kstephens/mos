#ifndef YYSTYPE

#ifndef __rcs_id__
#ifndef __rcs_id_mos_parse_h__
#define __rcs_id_mos_parse_h__
static const char __rcs_id_mos_parse_h[] = "$Id: parse.h,v 1.2 1999-02-19 09:26:31 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define YYSTYPE int
#endif
#define	ERROR	258
#define	NAME	259
#define	KEYWORD	260
#define	CAPWORD	261
#define	OPERATOR	262
#define	STRING	263
#define	CONSTANT	264
#define	EXPR	265


extern YYSTYPE yylval;
