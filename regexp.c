#include "mos/mos.h"
#include <stdlib.h> /* free() */
#include <string.h> /* strchr() */

#define SELF mos_REFT(mos_MIMPL, mos_regexp)
#define rx SELF->_rx
#define rxe SELF->_rxe

mos_ANNOT("Module: regexp")
mos_ANNOT("Doc: A regular expression matcher.")

mos_ANNOT("Category: Clone")
mos_ANNOT("Category: Internal")
mos_ANNOT("Doc: free the regexp struct when collected.")
mos_METHOD(regexp,_regexpFinalize)
{
  if ( rx ) {
    pcre_free(rx);
    rx = 0;
  }
  if ( rxe ) {
    pcre_free(rxe); // pcre_free_study(rxe); // 
    rxe = 0;
  }
}
mos_METHOD_END
mos_ANNOT_END

mos_METHOD(regexp,clone)
{
  mos_value x = mos_send(mos_MIMPL, mos_s(_clone));
  mos_send(x, mos_s(_finalize_), mos_s(_regexpFinalize));
  mos_return(x);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Return a new regexp object with the specfied regular expression string.")
mos_METHOD(regexp,new_)
{
  mos_value x = mos_send(mos_RCVR, mos_s(clone));
  mos_return(mos_send(x, mos_s(expr_), mos_ARGV[0]));
}
mos_METHOD_END
mos_ANNOT_END
mos_ANNOT_END

mos_ANNOT("Category: Expression")
mos_METHOD(regexp,expr)
{
  mos_return(mos_send(mos_RCVR, mos_s(_expr)));
}
mos_METHOD_END
mos_METHOD(regexp,expr_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asConstant));

  mos_send(mos_RCVR, mos_s(_regexpFinalize));

  mos_return(mos_send(mos_RCVR, mos_s(_expr_), mos_ARGV[0]));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Internal")
mos_METHOD(regexp,_compile)
{
  const char *rx_err = 0;
  int rx_erroffset;

  if ( ! rx ) {
    rx = pcre_compile(mos_string_V(mos_ARGV[0]), 0,
                      &rx_err, &rx_erroffset, 0);
    if ( rx ) {
      rxe = pcre_study(rx, 0, &rx_err);
    }
  }
}
mos_METHOD_END

mos_METHOD(regexp,_resultStrings_)
{
  mos_value rtnval; // , str;
  const char *s, *s_end;
  
  rtnval = mos_vector_make(0, 0);
  
  s = mos_string_V(mos_ARGV[0]);
  s_end = strchr(s, '\0');

#if 0
  /* Put the remainder string first */
  if ( rx && rx->endp[0] ) {
    int i = 0;

    /* Matched string */
    str = mos_string_make(rb->startp[i], rb->endp[i] - rb->startp[i]);
    mos_send(rtnval, mos_s(append_), str);
	
    /* Before match */
    str = mos_string_make(s, rb->startp[0] - s);
    mos_send(rtnval, mos_s(append_), str);

    /* After match */
    str = mos_string_make(rx->endp[0], s_end - rx->endp[0]);
    mos_send(rtnval, mos_s(append_), str);
    
    /* Put the matched string and subexpression matches */
    for ( i = 1; i < NSUBEXP && rx->startp[i] && rx->endp[i] ; i ++ ) {
      str = mos_string_make(rx->startp[i], rx->endp[i] - rx->startp[i]);
      mos_send(rtnval, mos_s(append_), str);
    }
  }
#endif

  mos_return(rtnval);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Match")
mos_ANNOT("Doc: Returns true if str matches the regexpr.")
mos_METHOD(regexp,matches_)
{
  int result;
  const char *s;
  int s_len;
  int ovector[30];

  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));

  s     = mos_string_V(mos_ARGV[0]);
  s_len = mos_string_L(mos_ARGV[0]);

  mos_send(mos_RCVR, mos_s(_compile));

  if ( s && (result = pcre_exec(rx, rxe,
                                s, s_len, 0,
                                0,
                                ovector, 30)) ) {
#if 0
    int i;

    fprintf(stderr, "regexp: result: %d\n", result);
    for ( i = 0; i < NSUBEXP && rx->startp[i] && rx->endp[i] ; i ++ ) {
      fprintf(stderr, "regexp: match: %d: %*s, %d\n", i, rx->endp[i] - rx->startp[i], rx->startp[i], rx->endp[i] - rx->startp[i]);
    }
#endif

    mos_return(mos_true);
  } else {
    mos_return(mos_false);
  }
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Returns a vector containing: the matched string, the string before the matched string, thestring after the matched string,  and any subexpression within (); if the string is matched.  If the string does not match, returns @f.")
mos_METHOD(regexp,matchesStrings_)
{
  int result;
  const char *s;
  int s_len;
  int ovector[30];
  mos_value rtnval = mos_false;

  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  s     = mos_string_V(mos_ARGV[0]);
  s_len = mos_string_L(mos_ARGV[0]);

  mos_send(mos_RCVR, mos_s(_compile));

  if ( s && (result = pcre_exec(rx, rxe,
                                s, s_len, 0,
                                0,
                                ovector, 30)) ) {
  }

  mos_return(rtnval);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT_END

#undef SELF

mos_OBJECT(regexp)
mos_OBJECT_M(regexp,_regexpFinalize)
mos_OBJECT_M(regexp,clone)
mos_OBJECT_M(regexp,new_)
mos_OBJECT_M(regexp,expr)
mos_OBJECT_M(regexp,expr_)
mos_OBJECT_M(regexp,_compile)
mos_OBJECT_M(regexp,_resultStrings_)
mos_OBJECT_M(regexp,matches_)
mos_OBJECT_M(regexp,matchesStrings_)
mos_ANNOT("Category: Internal")
mos_OBJECT_A(_expr,0)
mos_ANNOT_END
mos_OBJECT_A(matches,1)
mos_OBJECT_SLOTS(regexp)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(protos,regexp,mos_regexp,basicMeta)

mos_ANNOT_END
mos_ANNOT_END

