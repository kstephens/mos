#ifndef _mos_regexp_h
#define _mos_regexp_h

#include <sys/types.h>
#include "pcre.h"

typedef struct mos_regexp {
  mos_object_HDR;
  pcre *_rx;
  pcre_extra *_rxe;
} mos_regexp;

#endif
