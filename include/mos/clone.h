#ifndef _mos_clone_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_clone_h__
#define __rcs_id_mos_clone_h__
static const char __rcs_id_mos_clone_h[] = "$Id: clone.h,v 1.2 1999-02-19 09:26:28 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_clone_h

void _mos_deepenSlots(mos_object *x);
mos_object *_mos_cloneTall(mos_object *x);
mos_object *_mos_cloneFlat(mos_object *x);
mos_object *_mos_clone(mos_object *x);
mos_value mos_clone(mos_value x);
mos_value mos_create(mos_value desc);

#endif
