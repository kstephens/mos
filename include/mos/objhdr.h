#ifndef _mos_objhdr_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_objhdr_h__
#define __rcs_id_mos_objhdr_h__
static const char __rcs_id_mos_objhdr_h[] = "$Id: objhdr.h,v 1.2 1999-02-19 09:26:30 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_objhdr_h

#include "mos/value.h"

#define mos_object_HDR \
  mos_objectDesc *_desc; \
  mos_value *_slots
 
#define mos_object_HDR_INIT \
  0, \
  0
  
#define mos_object_HDR_INIT_(X) \
  mos_DESC(X), \
  mos_SLOTS(X)

#define mos_object_HDR_INIT_STATIC(X) \
  &_mos_od_##X, \
  _mos_os_##X

#endif
