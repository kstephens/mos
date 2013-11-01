#ifndef _mos_integer_h
#define _mos_integer_h

typedef struct mos_integer {
  mos_object_HDR;
  mos_int _x;
} mos_integer;

#ifndef mos_MAX_INTEGERS
#define mos_MAX_INTEGERS 256
#endif
extern mos_integer *_mos_integer[mos_MAX_INTEGERS];

#define mos_INT(X) mos_REFT(X,mos_integer)->_x
#define mos_INT_HASH(X) ((unsigned)(((X)>>12)^((X)>>3)^(X)))
#define _mos_MAKE_INT(X) ({ \
  mos_int __mos_x = (X); \
  mos_integer **__mos_i = &_mos_integer[mos_INT_HASH(__mos_x) % mos_MAX_INTEGERS]; \
  if ( ! (*__mos_i && (*__mos_i)->_x == __mos_x) ) { \
    *__mos_i = _mos_integer_make(__mos_x); \
  } \
  mos_MAKE_REF(*__mos_i); \
})
mos_integer * _mos_integer_make(mos_int x);

mos_value mos_integer_make(mos_int x);

#if 0
#define mos_integer_make(X) _mos_MAKE_INT(X)
#endif

#endif

