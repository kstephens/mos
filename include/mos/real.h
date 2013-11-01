#ifndef _mos_real_h
#define _mos_real_h

typedef union mos_double_int_u {
  double _x;
  unsigned int _i[sizeof(double)/sizeof(int)];
} mos_double_int_u;

typedef struct mos_real {
  mos_object_HDR;
  double _x;
} mos_real;

#ifndef mos_MAX_REALS
#define mos_MAX_REALS 256
#endif
extern mos_real *_mos_real[mos_MAX_REALS];

#define mos_REAL(X) mos_REFT(X,mos_real)->_x
#define mos_REAL_HASH(X) ((unsigned)(((X)>>12)^((X)>>3)^(X)))
#define _mos_MAKE_REAL(X) ({ \
  mos_double_int_u __mos_x; \
  mos_real **__mos_i; \
  __mos_x._x = (X); \
  __mos_i = &_mos_real[mos_REAL_HASH(__mos_x._i[0] ^ __mos_x._i[1]) % mos_MAX_REALS]; \
  if ( ! (*__mos_i && (*__mos_i)->_x == __mos_x._x) ) { \
    *__mos_i = _mos_real_make(__mos_x._x); \
  } \
  mos_MAKE_REF(*__mos_i); \
})
mos_real * _mos_real_make(double x);


mos_value mos_real_make(double x);

#if 0
#define mos_real_make(X) _mos_MAKE_REAL(X)
#endif

#endif
