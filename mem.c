#ifndef __rcs_id__
#ifndef __rcs_id_mos_mem_c__
#define __rcs_id_mos_mem_c__
static const char __rcs_id_mos_mem_c[] = "$Id: mem.c,v 1.3 1999-12-26 20:05:56 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include <stdlib.h> /* malloc() */
#include <string.h> /* strcpy(), strlen() */
#ifdef malloc
#undef malloc
#endif

#ifndef MOS_USE_GC
#define MOS_USE_GC 0
#endif

#if MOS_USE_GC
#if 1
  #include "gc.h"
#else
  #include "gc/gc.h"
void GC_init(int *argcp, char ***argvp) { }
#endif

#else
void GC_init() { }
#define GC_malloc(X) malloc(X)
#define GC_gc_no 0
#define GC_gcollect() 0
#endif


static long bytesAllocated = 0;
static long blocksAllocated = 0;

void *mos_malloc(size_t size)
{
  return size ?
    (blocksAllocated ++, bytesAllocated += size, GC_malloc(size)) :
    0;
}

char *mos_strdup(const char *x)
{
  return strcpy(mos_malloc(strlen(x) + 1), x);
}

mos_ANNOT("Doc: The interface to the garbage collector.")

mos_ANNOT("Doc: Returns the total number of bytes allocated.")
mos_METHOD(gc,bytesAllocated)
{
  mos_return(mos_integer_make(bytesAllocated));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Returns the total number of blocks allocated.")
mos_METHOD(gc,blocksAllocated)
{
  mos_return(mos_integer_make(blocksAllocated));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Returns the total number of collections.")
mos_METHOD(gc,collections)
{
  mos_return(mos_integer_make(GC_gc_no));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Forces a collection.")
mos_METHOD(gc,collect)
{
  (void) GC_gcollect();
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(gc)
mos_OBJECT_M(gc,bytesAllocated)
mos_OBJECT_M(gc,blocksAllocated)
mos_OBJECT_M(gc,collections)
mos_OBJECT_M(gc,collect)
mos_OBJECT_SLOTS(gc)
mos_OBJECT_END(system,gc,mos_object,basicMeta)

mos_ANNOT_END

