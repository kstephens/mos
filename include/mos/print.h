
#ifndef __rcs_id__
#ifndef __rcs_id_mos_print_h__
#define __rcs_id_mos_print_h__
static const char __rcs_id_mos_print_h[] = "$Id: print.h,v 1.2 1999-02-19 09:26:31 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include <stdarg.h>

mos_value mos_vprintf_(mos_value stream, mos_value format, va_list *vap);
mos_value mos_vprintf(mos_value stream, const char *format, va_list *vap);
mos_value mos_printf(mos_value stream, const char *format, ...);

