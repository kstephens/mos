#ifndef _mos_memcpy_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_memcpy_h__
#define __rcs_id_mos_memcpy_h__
static const char __rcs_id_mos_memcpy_h[] = "$Id: memcpy.h,v 1.2 1999-02-19 09:26:29 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_memcpy_h

#define mos_memcpy(DST,SRC,SIZE) \
do { \
  int *__DST = (int*)(DST); \
  const int *__SRC = (int*)(SRC); \
  size_t _SIZE = (SIZE) / sizeof(*__DST); \
  while ( _SIZE -- ) { \
    *(__DST ++) = *(__SRC ++); \
  } \
} while(0)

#endif
