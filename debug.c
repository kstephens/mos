
#ifndef __rcs_id__
#ifndef __rcs_id_mos_debug_c__
#define __rcs_id_mos_debug_c__
static const char __rcs_id_mos_debug_c[] = "$Id: debug.c,v 1.2 1999-02-19 09:26:21 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include <stdarg.h>

void mos_p(mos_value x)
{
  mos_printf(mos_FILE(err), "%P\n", x);
}

mos_value mos_FILE_in() { return mos_FILE(in); }
mos_value mos_FILE_out() { return mos_FILE(out); }
mos_value mos_FILE_err() { return mos_FILE(err); }

#ifdef mos_RCVR
#undef mos_RCVR
#endif
mos_value mos_RCVR(mos_value mos_MSG) { return _mos_RCVR; }

#ifdef mos_ARGV
#undef mos_ARGV
#endif
mos_value* mos_ARGV(mos_value mos_MSG) { return _mos_ARGV; }


#ifdef mos_s
#undef mos_s
#endif

mos_value mos_s(const char *s) { return mos_selector_make_(s, 0); }

