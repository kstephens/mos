#ifndef _mos_regexp_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_regexp_h__
#define __rcs_id_mos_regexp_h__
static const char __rcs_id_mos_regexp_h[] = "$Id: regexp.h,v 1.3 1999-12-26 20:03:22 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_regexp_h

#include <sys/types.h>
#include <regex.h>

typedef struct mos_regexp {
 mos_object_HDR;
 int _rx;
 regex_t _regexp;
} mos_regexp;


#endif
