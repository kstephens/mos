
#ifndef __rcs_id__
#ifndef __rcs_id_old_mapobj_h__
#define __rcs_id_old_mapobj_h__
static const char __rcs_id_old_mapobj_h[] = "$Id: mapobj.h,v 1.2 1999-02-19 09:26:36 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/map.h"

typedef struct mos_mapobj {
  mos_object_HDR;
  mos_map _m, *_mp;
} mos_mapobj;

#define mos_mapobj_M(X) mos_REFT(X,mos_mapobj)->_mp
#define mos_mapobj_MS(X) mos_REFT(X,mos_mapobj)->_m

