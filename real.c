#include "mos/mos.h"
#include "mos/constant.h"
#include <string.h> /* strchr(), strcat() */
#include <math.h>

mos_ANNOT("Module: real")
mos_ANNOT("Doc: A real number.")

mos_real *_mos_real[mos_MAX_REALS];

#define SELF mos_REFT(mos_MIMPL,mos_real)
#define X SELF->_x
#define Y mos_REFT(mos_ARGV[0],mos_real)->_x

mos_METHOD(real,asReal)
{
}
mos_METHOD_END

mos_ANNOT("Category: Print")

mos_METHOD(real,printOn_)
{
  char buf[32];
  sprintf(buf, "%.10g", (double) X);
  if ( ! (strchr(buf, '.') || strchr(buf, 'e')) ) {
    strcat(buf, ".0");
  }
  mos_return(mos_printf(mos_ARGV[0], "%s", buf));
}
mos_METHOD_END

mos_ANNOT_END

mos_ANNOT("Category: Coder")

mos_METHOD(real,encodeOn_)
{
  mos_send(mos_ARGV[0], mos_s(encodeAsReal_), mos_RCVR);
}
mos_METHOD_END

mos_ANNOT_END

mos_ANNOT("Category: Equal")

mos_METHOD(real,_equal_)
{
  mos_return(X == Y ? mos_true : mos_false); 
}
mos_METHOD_END

mos_ANNOT_END


mos_METHOD(real,asInteger)
{
  mos_return(mos_integer_make(X));
}
mos_METHOD_END

#define BOP(NAME,OP) \
mos_METHOD(real,NAME) \
{ \
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asReal)); \
  mos_return(mos_real_make(X OP Y)); \
} \
mos_METHOD_END

#define UOP(NAME,OP) \
mos_METHOD(real,NAME) \
{ \
  mos_return(mos_real_make(OP X)); \
} \
mos_METHOD_END

#define IBOP(NAME,OP)
#define IUOP(NAME,OP)
#define LBOP(NAME,OP) BOP(NAME,OP)
#define LUOP(NAME,OP) UOP(NAME,OP)
#define ROP(NAME,OP) \
mos_METHOD(real,NAME) \
{ \
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asReal)); \
  mos_return(mos_boolean_make(X OP Y)); \
} \
mos_METHOD_END

#include "mos/op.def"

#define UOP(NAME) \
mos_METHOD(real,NAME) \
{ \
  mos_return(mos_real_make(NAME(X))); \
} \
mos_METHOD_END
#define BOP(NAME) \
mos_METHOD(real,NAME##_) \
{ \
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asReal)); \
  mos_return(mos_real_make(NAME(X, Y))); \
} \
mos_METHOD_END

#define OPS \
UOP(acos) \
UOP(acosh) \
UOP(asin) \
UOP(asinh) \
UOP(atan) \
UOP(atanh) \
BOP(atan2) \
UOP(cos) \
UOP(cosh) \
UOP(exp) \
UOP(log) \
UOP(log10) \
BOP(pow) \
UOP(sin) \
UOP(sinh) \
UOP(sqrt) \
UOP(tan) \
UOP(tanh)

OPS

#undef UOP
#undef BOP

#undef SELF
#undef X
#undef Y

mos_OBJECT(real)
mos_constant_METHODS
mos_OBJECT_M(real,printOn_)
mos_OBJECT_M(real,encodeOn_)
mos_OBJECT_M(real,_equal_)
mos_OBJECT_M(real,asReal)
mos_OBJECT_M(real,asInteger)
#define BOP(NAME,OP) mos_OBJECT_M(real,NAME)
#define UOP(NAME,OP) BOP(NAME,OP)
#define IBOP(NAME,OP)
#define IUOP(NAME,OP)
#define LBOP(NAME,OP) BOP(NAME,OP)
#define LUOP(NAME,OP) BOP(NAME,OP)
#define ROP(NAME,OP) BOP(NAME,OP)
#include "mos/op.def"

#define UOP(NAME) mos_OBJECT_M(real,NAME)
#define BOP(NAME) mos_OBJECT_M(real,NAME##_)
OPS
#undef UOP
#undef BOP

mos_OBJECT_SLOTS(real)
mos_OBJECT_END(protos,real,mos_real,basicMeta)

mos_ANNOT_END
mos_ANNOT_END


__inline
mos_real * _mos_real_make(double _x)
{
    mos_real *x = mos_REFT(mos_clone(mos_o(real)), mos_real);
    x->_x = _x;
    return x;
}

#ifdef mos_real_make
#undef mos_real_make
#endif
mos_value mos_real_make(double x)
{
  return _mos_MAKE_REAL(x);
}
