#ifndef _mos_slots_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_slots_h__
#define __rcs_id_mos_slots_h__
static const char __rcs_id_mos_slots_h[] = "$Id: slots.h,v 1.2 1999-02-19 09:26:32 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_slots_h

void _mos_addMethod(mos_object *x, mos_value sel, mos_value meth);
void _mos_addReadOnlySlot(mos_object *x, mos_value sel, mos_value val);
int _mos_addSlot(mos_object *x, mos_value sel, mos_value val);
void _mos_addSlots(mos_value x, mos_value src, int ifAbsent);

#endif
