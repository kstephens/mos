
#ifndef __rcs_id__
#ifndef __rcs_id_mos_share_c__
#define __rcs_id_mos_share_c__
static const char __rcs_id_mos_share_c[] = "$Id: share.c,v 1.2 1999-02-19 09:26:25 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"

mos_INIT(share,-100)
{
#define mos_DEF_SHARED_DESC(X) mos_FLAGS(mos_o(X)) |= mos_SHARED_DESC;
#include "share.def"

  /* the empty object should never be shared */
  mos_FLAGS(mos_o(empty)) &= ~ mos_SHARED_DESC;
  return 0;
}

mos_INIT(unshare,99)
{
#define mos_DEF_SHARED_DESC(X) mos_FLAGS(mos_o(X)) &= ~mos_SHARED_DESC;
#include "share.def"
  
  /* the following object should always be shared */
#define S(X) mos_FLAGS(mos_o(X)) |= mos_SHARED_DESC

  S(integer);
  S(real);
  S(string);
  S(vector);

#undef S
  
  return 0;
}

