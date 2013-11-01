#ifndef _mos_memcpy_h
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
