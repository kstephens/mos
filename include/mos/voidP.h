typedef struct mos_voidP {
  mos_object_HDR;
  void* _x;
} mos_voidP;

#ifndef mos_MAX_VOIDPS
#define mos_MAX_VOIDPS 64
#endif
extern mos_voidP *_mos_voidP[mos_MAX_VOIDPS];

#define mos_VOIDP(X) mos_REFT(X,mos_voidP)->_x
#define mos_INT_HASH(X) ((unsigned)(((X)>>12)^((X)>>3)^(X)))
#define mos_MAKE_INT(X) ({ \
  void* __mos_x = (X); \
  mos_voidP **__mos_i = &_mos_voidP[mos_INT_HASH(__mos_x) % mos_MAX_VOIDPS]; \
  if ( ! (*__mos_i && (*__mos_i)->_x == __mos_x) ) { \
    *__mos_i = _mos_voidP_make(__mos_x); \
  } \
  mos_MAKE_REF(*__mos_i); \
})
mos_voidP * _mos_voidP_make(long x);

#define mos_voidP_make(X) mos_MAKE_REF(_mos_voidP_make(X))

