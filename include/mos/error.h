#include <stdarg.h>

mos_value mos_errorV(mos_value err, const char *format, va_list *vap);
mos_value mos_error(mos_value error, const char *format, ...);

