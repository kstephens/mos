#include "mos/map.h"

typedef struct mos_mapobj {
  mos_object_HDR;
  mos_map _m, *_mp;
} mos_mapobj;

#define mos_mapobj_M(X) mos_REFT(X,mos_mapobj)->_mp
#define mos_mapobj_MS(X) mos_REFT(X,mos_mapobj)->_m

