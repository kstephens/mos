
#ifndef __rcs_id__
#ifndef __rcs_id_mos_accessor_c__
#define __rcs_id_mos_accessor_c__
static const char __rcs_id_mos_accessor_c[] = "$Id: accessor.c,v 1.2 1999-02-19 09:26:19 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"

#ifndef mos_MAX_ACCESSORS
#define mos_MAX_ACCESSORS 256
#endif


#define SELF mos_REFT(mos_MIMPL,mos_accessor)

mos_METHOD(accessor,asAccessor)
{
}
mos_METHOD_END

mos_METHOD(accessor,slotIndex)
{
  mos_return(mos_integer_make(SELF->_sloti));
}
mos_METHOD_END

mos_METHOD(accessor,encodeOtherOn_)
{
  mos_send(mos_ARGV[0], mos_s(encodeRawInteger_), mos_integer_make(SELF->_sloti));
}
mos_METHOD_END

mos_METHOD(getter,asGetter)
{
}
mos_METHOD_END

mos_METHOD(getter,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "@G%d", SELF->_sloti));
}
mos_METHOD_END

mos_METHOD(setter,asSetter)
{
}
mos_METHOD_END

mos_METHOD(setter,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "@S%d", SELF->_sloti));
}
mos_METHOD_END


#define SLOTI mos_REFT(mos_METH,mos_accessor)->_sloti
#define SLOT mos_SLOTS(mos_MIMPL)[SLOTI]

mos_METHOD(getter,_get)
{
  mos_return(SLOT);
}
mos_METHOD_END

mos_METHOD(setter,_set_)
{
  SLOT = mos_ARGV[0];
}
mos_METHOD_END

mos_METHOD(getter,decodeOtherOn_)
{
  SELF->_sloti = mos_INT(mos_send(mos_ARGV[0], mos_s(decodeRawInteger)));
  SELF->_func = _mos_mf_getter__get;
  mos_return(mos_getter_method(SELF->_sloti));
}
mos_METHOD_END

mos_METHOD(setter,decodeOtherOn_)
{
  SELF->_sloti = mos_INT(mos_send(mos_ARGV[0], mos_s(decodeRawInteger)));
  SELF->_func = _mos_mf_setter__set_;
  mos_return(mos_setter_method(SELF->_sloti));
}
mos_METHOD_END


#undef SELF
#undef SLOTI
#undef SLOT

#define SELF mos_REFT(mos_MIMPL,mos_constantMethod)
#define SLOT(X)mos_SLOTS(X)[0]

mos_METHOD(constantMethod,_get)
{
  mos_return(SLOT(mos_METH));
}
mos_METHOD_END

mos_METHOD(constantMethod,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "@C(%O)", SLOT(mos_RCVR)));
}
mos_METHOD_END

mos_METHOD(constantMethod,printOnAsMethodBody_)
{
  mos_return(mos_printf(mos_ARGV[0], "= %-P;", SLOT(mos_MIMPL)));
}
mos_METHOD_END

mos_METHOD(constantMethod,decodeOtherOn_)
{
  SELF->_func = _mos_mf_constantMethod__get;
}
mos_METHOD_END

mos_value mos_constant_method(mos_value x)
{
  mos_value m = mos_clone(mos_o(constantMethod));
  SLOT(m) = x;
  return m;
}

#undef SELF
#undef SLOT

mos_OBJECT(getter)
mos_accessor_METHODS
mos_OBJECT_M(getter,asGetter)
mos_OBJECT_M(getter,printOn_)
mos_OBJECT_METHOD(mos_m(constant,printShallowOn_),printShallowOn_)
mos_OBJECT_M(accessor,encodeOtherOn_)
mos_OBJECT_M(getter,decodeOtherOn_)
mos_OBJECT_SLOTS(getter)
mos_OBJECT_END(protos,getter,mos_accessor,methodMeta)

mos_OBJECT(setter)
mos_accessor_METHODS
mos_OBJECT_M(setter,asSetter)
mos_OBJECT_M(setter,printOn_)
mos_OBJECT_METHOD(mos_m(constant,printShallowOn_),printShallowOn_)
mos_OBJECT_M(accessor,encodeOtherOn_)
mos_OBJECT_M(setter,decodeOtherOn_)
mos_OBJECT_SLOTS(setter)
mos_OBJECT_END(protos,setter,mos_accessor,methodMeta)

mos_OBJECT(constantMethod)
mos_accessor_METHODS
mos_OBJECT_M(constant,isConstant)
mos_OBJECT_M(getter,asGetter)
mos_OBJECT_M(constantMethod,printOn_)
mos_OBJECT_METHOD(mos_m(constant,printShallowOn_),printShallowOn_)
mos_OBJECT_M(constantMethod,printOnAsMethodBody_)
mos_OBJECT_M(constantMethod,decodeOtherOn_)
mos_OBJECT_A(value,0)
mos_OBJECT_SLOTS(constantMethod)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(protos,constantMethod,mos_constantMethod,methodMeta)

     /********************************************************************/

mos_accessor _mos_getter[mos_MAX_ACCESSORS];
mos_accessor _mos_setter[mos_MAX_ACCESSORS];

mos_INIT(accessor,1)
{
  int i;
  
  _mos_o_getter._func = _mos_mf_getter__get;
  _mos_o_setter._func = _mos_mf_setter__set_;
  _mos_o_constantMethod._func = _mos_mf_constantMethod__get;
  
  for ( i = 0; i < mos_MAX_ACCESSORS; i ++ ) {
    _mos_getter[i] = _mos_o_getter;
    _mos_getter[i]._sloti = i;

    _mos_setter[i] = _mos_o_setter;
    _mos_setter[i]._sloti = i;
  }
  
  return 0;
}

mos_value mos_getter_method(unsigned int sloti)
{
  if ( sloti >= mos_MAX_ACCESSORS ) {
    mos_accessor *x = mos_REFT(mos_clone(mos_o(getter)), mos_accessor);
    x->_sloti = sloti;
    return mos_MAKE_REF(x);
  }
  return mos_GETTER(sloti);
}

mos_value mos_setter_method(unsigned int sloti)
{
  if ( sloti >= mos_MAX_ACCESSORS ) {
    mos_accessor *x = mos_REFT(mos_clone(mos_o(setter)), mos_accessor);
    x->_sloti = sloti;
    return mos_MAKE_REF(x);
  }
  return mos_SETTER(sloti);
}

