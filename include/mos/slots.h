#ifndef _mos_slots_h
#define _mos_slots_h

void _mos_addMethod(mos_object *x, mos_value sel, mos_value meth);
void _mos_addReadOnlySlot(mos_object *x, mos_value sel, mos_value val);
int _mos_addSlot(mos_object *x, mos_value sel, mos_value val);
void _mos_addSlots(mos_value x, mos_value src, int ifAbsent);

#endif
