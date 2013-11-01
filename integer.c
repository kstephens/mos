#include "mos/mos.h"
#include "mos/constant.h"
#include "mos/weak.h"


mos_ANNOT("Module: integer")
mos_ANNOT("Doc: An integer value.")

mos_integer *_mos_integer[mos_MAX_INTEGERS];

#define SELF mos_REFT(mos_MIMPL,mos_integer)
#define X SELF->_x
#define Y mos_REFT(mos_ARGV[0],mos_integer)->_x

mos_METHOD(integer,asInteger)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END

mos_ANNOT("Category: Print")

mos_METHOD(integer,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "%I", X));
}
mos_METHOD_END

mos_ANNOT_END

mos_ANNOT("Category: Coder")

mos_METHOD(integer,encodeOn_)
{
  mos_send(mos_ARGV[0], mos_s(encodeAsInteger_), mos_RCVR);
}
mos_METHOD_END

mos_ANNOT_END

mos_ANNOT("Category: Equal")

mos_METHOD(integer,_equal_)
{
  mos_return(X == Y ? mos_true : mos_false); 
}
mos_METHOD_END

mos_ANNOT_END

mos_ANNOT("Category: Weak Pointer")

mos_METHOD(integer,weakPtrToObject)
{
  mos_return(mos_objectForWeakPtr(X));
}
mos_METHOD_END

mos_ANNOT_END

mos_ANNOT("Category: Coerce")

mos_METHOD(integer,asChar)
{
  mos_return(mos_char_make(X));
}
mos_METHOD_END

mos_METHOD(integer,asReal)
{
  mos_return(mos_real_make(X));
}
mos_METHOD_END

mos_METHOD(integer,asGetter)
{
  mos_return(mos_getter_method(X));
}
mos_METHOD_END

mos_METHOD(integer,asSetter)
{
  mos_return(mos_setter_method(X));
}
mos_METHOD_END

mos_ANNOT_END

#define BOP(NAME,OP) \
mos_METHOD(integer,NAME) { \
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger)); \
  mos_return(mos_integer_make(X OP Y)); \
} \
mos_METHOD_END

#define UOP(NAME,OP) \
mos_METHOD(integer,NAME) { \
  mos_return(mos_integer_make(OP X)); \
} \
mos_METHOD_END

#define IBOP(NAME,OP) BOP(NAME,OP)
#define IUOP(NAME,OP) UOP(NAME,OP)
#define LBOP(NAME,OP) BOP(NAME,OP)
#define LUOP(NAME,OP) UOP(NAME,OP)
#define ROP(NAME,OP) \
mos_METHOD(integer,NAME) { \
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger)); \
  mos_return(mos_boolean_make(X OP Y)); \
} \
mos_METHOD_END


mos_METHOD(integer,__DIV__) {
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  if ( Y == 0 ) {
    mos_return(mos_error(mos_s(divideByZero), ""));
  }
  mos_return(mos_integer_make(X / Y));
}
mos_METHOD_END

mos_METHOD(integer,__MOD__) {
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  if ( Y == 0 ) {
    mos_return(mos_error(mos_s(divideByZero), ""));
  }
  mos_return(mos_integer_make(X % Y));
}
mos_METHOD_END


#define _op_NO_DIV
#include "mos/op.def"

#undef SELF
#undef X
#undef Y

mos_OBJECT(integer)
mos_constant_METHODS
mos_OBJECT_M(integer,asInteger)
mos_OBJECT_M(integer,printOn_)
mos_OBJECT_M(integer,encodeOn_)
mos_OBJECT_M(integer,_equal_)
mos_OBJECT_M(integer,weakPtrToObject)
mos_OBJECT_M(integer,asReal)
mos_OBJECT_M(integer,asChar)
mos_OBJECT_M(integer,asGetter)
mos_OBJECT_M(integer,asSetter)
#define BOP(NAME,OP) mos_OBJECT_M(integer,NAME)
#define UOP(NAME,OP) BOP(NAME,OP)
#define IBOP(NAME,OP) BOP(NAME,OP)
#define IUOP(NAME,OP) BOP(NAME,OP)
#define LBOP(NAME,OP) BOP(NAME,OP)
#define LUOP(NAME,OP) BOP(NAME,OP)
#define ROP(NAME,OP) BOP(NAME,OP)
#include "mos/op.def"

mos_OBJECT_SLOTS(integer)
mos_OBJECT_END(protos,integer,mos_integer,basicMeta)

mos_ANNOT_END
mos_ANNOT_END

__inline
mos_integer * _mos_integer_make(mos_int _x)
{
    mos_integer *x = mos_REFT(mos_clone(mos_o(integer)), mos_integer);
    x->_x = _x;
    return x;
}

#ifdef mos_integer_make
#undef mos_integer_make
#endif
mos_value mos_integer_make(mos_int x)
{
  return _mos_MAKE_INT(x);
}
