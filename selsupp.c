#ifndef __rcs_id__
#ifndef __rcs_id_mos_selsupp_c__
#define __rcs_id_mos_selsupp_c__
static const char __rcs_id_mos_selsupp_c[] = "$Id: selsupp.c,v 1.1 2001-09-18 02:46:05 stephens Exp $";
#endif
#endif /* __rcs_id__ */


/*
** Selector support.
** Use by both selector.c, selesc.c.
*/


#include "mos/mos.h"
#include "mos/constant.h"
#include <string.h>


const char _mos_selector_op_charset[] = "~!$%^&*+-<>?/,|=";


static struct strmap {
  const char *search;
  const char *replace;
  int searchlen;
  int replacelen;
} maps[] = {
  { "__NEG__", "neg" },
  { "__BNOT__", "bnot" },
  { "__NOT__", "not" },
#define BOP(NAME,OP) { #NAME, #OP },
#define UOP(NAME,OP) BOP(NAME,OP)
#define IBOP(NAME,OP) BOP(NAME,OP)
#define IUOP(NAME,OP) BOP(NAME,OP)
#define LBOP(NAME,OP) BOP(NAME,OP)
#define LUOP(NAME,OP) BOP(NAME,OP)
#define ROP(NAME,OP) BOP(NAME,OP)
#include "mos/op.def"
  
  { "__ASN__", "=" },
  { "__RAR__", "->" },
  { "__LAR__", "<-" },
  { "__RTN__", "_:" },
  { "__DOL__", "$" },
  { "__COM__", "," },
  { "__QUE__", "?" },
  
  { "__", "_" },
  
  { 0, 0 }
};


static
const struct strmap *_mos_map_match(const char *s)
{
  struct strmap *m = maps;
  
  while ( m->search ) {
    const char *ss = s;
    const char *se = m->search;
    
    while ( *se ) {
      if ( *(se ++) != *(ss ++) ) {
	goto next_m;
      }
    }
    
    if ( ! m->searchlen ) {
      m->searchlen = strlen(m->search);
      m->replacelen = strlen(m->replace);
    }
    return m;
    
    next_m:
    m ++;
  }
  
  return 0;
}


const char *_mos_selector_escape(const char *x)
{
  static char buf[1024];
  char *t = buf;
  const char *s = x;
  const struct strmap *m;
  size_t x_len = strlen(x);

#define REPLACE(m) do { \
  const char *re = m->replace; \
  while ( *re ) { \
    *(t ++) = *(re ++); \
  } \
  s += m->searchlen; \
} while(0)
  
  if ( (m = _mos_map_match(s)) && m->searchlen == x_len ) {
    REPLACE(m);
  } else {
    /* Skip leading '_' */
    while ( *s == '_' ) {
      *(t ++) = *(s ++);
    }
  }
  
  while ( *s ) {
    if ( (m = _mos_map_match(s)) ) {
      REPLACE(m);
    } else
    if ( *s == '_' ) {
      *(t ++) = ':'; s ++;
    } else  {
      /* Put the char literally */
      *(t ++) = *(s ++);
    }
  }
  *t = '\0';

#undef REPLACE
  
  return buf;  
}


