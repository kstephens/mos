#define mos_METHOD_DECL(X) mos_value X (mos_value mos_MSG)

#define mos_method_HDR \
  mos_object_HDR; \
  mos_METHOD_DECL((*_func))

typedef struct mos_method {
  mos_method_HDR;
} mos_method;

#define mos_METHOD_PREAMBLE mos_value *mos_ARGV = _mos_ARGV; mos_ARGV = mos_ARGV;
#ifndef mos_METHOD
#define mos_METHOD(O,S) \
  mos_METHOD_DECL(_mos_mf_##O##_##S); \
  mos_method _mos_m_##O##_##S = { mos_object_HDR_INIT_STATIC(method), &_mos_mf_##O##_##S }; \
  mos_METHOD_DECL(_mos_mf_##O##_##S) { \
  mos_METHOD_PREAMBLE
#define mos_METHOD_END mos_return(mos_RCVR); }
#endif

#define mos_method_METHODS \
  mos_OBJECT_M(method,asMethod)

#define mos_methodMeta_METHODS \
  mos_basicMeta_METHODS \
  mos_OBJECT_M(methodMeta,applyMethod_)

#define mos_m(O,S) mos_MAKE_REF(&_mos_m_##O##_##S)
#define mos_DEF_METHOD(O,S) extern mos_method _mos_m_##O##_##S;
#include "mos/meth.def"


