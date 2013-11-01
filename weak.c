#include "mos/weak.h"
#if MOS_USE_GC
#include "gc.h"
#else
#define GC_ptr_to_weak(x) ((size_t)(x))
#define GC_weak_to_ptr(x) ((void*)(x))
#endif

mos_int mos_weakPtrForObject(mos_value x)
{
  return (mos_int) GC_ptr_to_weak(mos_REF(x));
}

mos_value mos_objectForWeakPtr(mos_int x)
{
  void *ptr = GC_weak_to_ptr(x);
  return ptr ? mos_MAKE_REF(ptr) : mos_undef;
}

