#ifndef _mos_weak_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_weak_h__
#define __rcs_id_mos_weak_h__
static const char __rcs_id_mos_weak_h[] = "$Id: weak.h,v 1.2 1999-02-19 09:26:32 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_weak_h

#include "mos/mos.h"

unsigned long mos_weakPtrForObject(mos_value x);
mos_value mos_objectForWeakPtr(unsigned long x);

#endif
