#ifndef _mos_object_h
#define _mos_object_h

#include "mos/objdesc.h"

#define mos_DESC(X) mos_REF(X)->_desc
#define mos_META(X) mos_DESC(X)->_meta
#define mos_METHODS(X) (&(mos_DESC(X)->_methods))
#define mos_FLAGS(X) mos_DESC(X)->_flags
#define mos_NSLOTS(X) mos_DESC(X)->_nslots
#define mos_SIZEOF(X) mos_DESC(X)->_sizeof
#define mos_SLOTS(X) mos_REF(X)->_slots
#define mos_SLOTST(X,T) ((T*)mos_SLOTS(X))
#define mos_AUXSLOTS(X) (void*)(((char*)mos_REF(X))+sizeof(mos_object))
#define mos_AUXSIZE(X) (mos_SIZEOF(X)-sizeof(mos_object))

struct mos_object {
  mos_object_HDR;
};

#endif
