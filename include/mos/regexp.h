#ifndef _mos_regexp_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_regexp_h__
#define __rcs_id_mos_regexp_h__
static const char __rcs_id_mos_regexp_h[] = "$Id: regexp.h,v 1.2 1999-02-19 09:26:31 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_regexp_h

#ifndef NSUBEXP
#include <regexp.h>
#endif

typedef struct mos_regexp {
 mos_object_HDR;
 regexp *_regexp;
} mos_regexp;


#endif
