
#ifndef __rcs_id__
#ifndef __rcs_id_mos_weak_c__
#define __rcs_id_mos_weak_c__
static const char __rcs_id_mos_weak_c[] = "$Id: weak.c,v 1.2 1999-02-19 09:26:27 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/weak.h"
#if MOS_USE_GC
#include "gc.h"
#else
#define GC_ptr_to_weak(x) ((unsigned long)(x))
#define GC_weak_to_ptr(x) ((void*)(x))
#endif

unsigned long mos_weakPtrForObject(mos_value x)
{
  return (unsigned long) GC_ptr_to_weak(mos_REF(x));
}

mos_value mos_objectForWeakPtr(unsigned long x)
{
  void *ptr = GC_weak_to_ptr(x);
  return ptr ? mos_MAKE_REF(ptr) : mos_undef;
}

