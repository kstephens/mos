
#ifndef __rcs_id__
#ifndef __rcs_id_mos_error_h__
#define __rcs_id_mos_error_h__
static const char __rcs_id_mos_error_h[] = "$Id: error.h,v 1.2 1999-02-19 09:26:29 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include <stdarg.h>

mos_value mos_errorV(mos_value err, const char *format, va_list *vap);
mos_value mos_error(mos_value error, const char *format, ...);

