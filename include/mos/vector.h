#ifndef _mos_vector_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_vector_h__
#define __rcs_id_mos_vector_h__
static const char __rcs_id_mos_vector_h[] = "$Id: vector.h,v 1.2 1999-02-19 09:26:32 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_vector_h

#include <stdarg.h>

typedef struct mos_vector {
  mos_object_HDR;
  mos_value *_v;
  size_t _l;
  size_t _al; /* Allocated length */
  int _flags; /* mos_READONLY */
} mos_vector;

#define mos_vector_L(X) mos_REFT(X,mos_vector)->_l
#define mos_vector_V(X) mos_REFT(X,mos_vector)->_v
#define mos_vector_AL(X) mos_REFT(X,mos_vector)->_al
#define mos_vector_FLAGS(X) mos_REFT(X,mos_vector)->_flags

#define mos_vector_LOOP(V,EP) { \
  mos_value *EP = mos_vector_V(V); \
  size_t __L = mos_vector_L(V); \
  for ( ; __L -- ; EP ++ ) {

#define mos_vector_LOOP_REVERSE(V,EP) { \
  mos_value *EP = mos_vector_V(V) + mos_vector_L(V); \
  size_t __L = mos_vector_L(V); \
  for ( ; EP --, __L -- ; ) {

#define mos_vector_LOOP_END }}

mos_value mos_vector_make(size_t _l, const mos_value *_v);
mos_value mos_vector_makev(size_t _l, va_list *_vap);
mos_value mos_vector_make_(size_t _l, ...);

#define mos_vector_DECLARE(X,_V,_L) \
 mos_vector __mos_v_##X = { mos_object_HDR_INIT_(mos_o(vector)), _V, _L }; \
 mos_value X = mos_MAKE_REF(&__mos_v_##X)

#define mos_vector_DECLARE_(X,_V...) \
 mos_value __mos_vv_##X[] = { _V }; \
 mos_vector_DECLARE(X, __mos_vv_##X, mos_ARRAY_LEN(__mos_vv_##X))

#define mos_vector_MAKE(_ARGS...) ({ \
  mos_vector_DECLARE_(__mos_v, _ARGS); \
  __mos_v; \
)}

#endif
