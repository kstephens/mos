#ifndef __rcs_id__
#ifndef __rcs_id_mos_char_c__
#define __rcs_id_mos_char_c__
static const char __rcs_id_mos_char_c[] = "$Id: char.c,v 1.3 1999-12-26 20:05:42 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include "mos/constant.h"

#include <ctype.h>

mos_char _mos_chars[0x100];

#define SELF mos_REFT(mos_MIMPL,mos_char)
#define X mos_CHAR(mos_MIMPL)

mos_METHOD(char,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "'%W'", mos_send(mos_RCVR, mos_s(escapeC))));
}
mos_METHOD_END

mos_METHOD(char,asChar)
{
}
mos_METHOD_END

mos_METHOD(char,asInteger)
{
  mos_return(mos_integer_make(X));
}
mos_METHOD_END

mos_METHOD(char,asString)
{
}
mos_METHOD_END


#undef SELF
#undef X

mos_OBJECT(char)
mos_OBJECT_M(char,printOn_)
mos_OBJECT_M(char,asChar)
mos_OBJECT_M(char,asInteger)
mos_OBJECT_M(char,asString)
mos_OBJECT_M(string,escapeC)
mos_constant_METHODS
mos_OBJECT_SLOTS(char)
mos_OBJECT_END(protos,char,mos_char,basicMeta)

static char sbuf[0x200];

mos_INIT(char,2)
{
  unsigned int i;
  
  _mos_o_char._l = _mos_o_char._al = 1;
  _mos_o_char._v = "\0";
  
  for ( i = 0; i < 0x100; i ++ ) {
    sbuf[i * 2] = i;
    sbuf[i * 2 + 1] = '\0';
    
    _mos_chars[i] = _mos_o_char;
    _mos_chars[i]._l = _mos_chars[i]._al = 1;
    _mos_chars[i]._v = sbuf + i * 2;
    _mos_chars[i]._flags = mos_READONLY;
  }
  
  return 0;
}

#ifdef mos_char_make
#undef mos_char_make
#endif
mos_value mos_char_make(int x)
{
  return _mos_MAKE_CHAR(x);
}
