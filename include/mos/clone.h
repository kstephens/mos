#ifndef _mos_clone_h
#define _mos_clone_h

void _mos_deepenSlots(mos_object *x);
mos_object *_mos_cloneTall(mos_object *x);
mos_object *_mos_cloneFlat(mos_object *x);
mos_object *_mos_clone(mos_object *x);
mos_value mos_clone(mos_value x);
mos_value mos_create(mos_value desc);

#endif
