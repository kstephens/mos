#ifndef _mos_regexp_h
#define _mos_regexp_h

#ifndef NSUBEXP
#include <regexp.h>
#endif

typedef struct mos_regexp {
 mos_object_HDR;
 regexp *_regexp;
} mos_regexp;


#endif
