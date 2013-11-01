#ifndef _mos_regexp_h
#define _mos_regexp_h

#include <sys/types.h>
#include <regex.h>

typedef struct mos_regexp {
 mos_object_HDR;
 int _rx;
 regex_t _regexp;
} mos_regexp;

#endif
