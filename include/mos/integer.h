#ifndef _mos_integer_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_integer_h__
#define __rcs_id_mos_integer_h__
static const char __rcs_id_mos_integer_h[] = "$Id: integer.h,v 1.2 1999-02-19 09:26:29 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_integer_h

typedef struct mos_integer {
  mos_object_HDR;
  long _x;
} mos_integer;

#ifndef mos_MAX_INTEGERS
#define mos_MAX_INTEGERS 256
#endif
extern mos_integer *_mos_integer[mos_MAX_INTEGERS];

#define mos_INT(X) mos_REFT(X,mos_integer)->_x
#define mos_INT_HASH(X) ((unsigned)(((X)>>12)^((X)>>3)^(X)))
#define _mos_MAKE_INT(X) ({ \
  long __mos_x = (X); \
  mos_integer **__mos_i = &_mos_integer[mos_INT_HASH(__mos_x) % mos_MAX_INTEGERS]; \
  if ( ! (*__mos_i && (*__mos_i)->_x == __mos_x) ) { \
    *__mos_i = _mos_integer_make(__mos_x); \
  } \
  mos_MAKE_REF(*__mos_i); \
})
mos_integer * _mos_integer_make(long x);

mos_value mos_integer_make(long x);

#if 0
#define mos_integer_make(X) _mos_MAKE_INT(X)
#endif

#endif

