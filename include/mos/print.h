#include <stdarg.h>

mos_value mos_vprintf_(mos_value stream, mos_value format, va_list *vap);
mos_value mos_vprintf(mos_value stream, const char *format, va_list *vap);
mos_value mos_printf(mos_value stream, const char *format, ...);

