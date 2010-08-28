#include "mos/mos.h"
#include "mos/basicMeta.h"


#ifndef mos_MAX_ACCESSORS
#define mos_MAX_ACCESSORS 256
#endif


#define SELF mos_REFT(mos_MIMPL,mos_accessor)


/******************************************************************/


mos_ANNOT("Module: accessor")
mos_ANNOT("Doc: The value slot accessor method behavior.")


/******************************************************************/


  mos_ANNOT("Category: Coerce")


mos_METHOD(accessor,asAccessor)
{
}
mos_METHOD_END


  mos_ANNOT_END


/******************************************************************/


  mos_ANNOT("Category: SlotIndex")
  mos_ANNOT("Doc: Returns the value slot index of the accessor method.")


mos_METHOD(accessor,slotIndex)
{
  mos_return(mos_integer_make(SELF->_sloti));
}
mos_METHOD_END


  mos_ANNOT_END
  mos_ANNOT_END


/******************************************************************/


  mos_ANNOT("Category: Coder")
  mos_ANNOT("Doc: Encodes the slotIndex.")


mos_METHOD(accessor,encodeOtherOn_)
{
  mos_send(mos_ARGV[0], mos_s(encodeRawInteger_), mos_integer_make(SELF->_sloti));
}
mos_METHOD_END


  mos_ANNOT_END
  mos_ANNOT_END


/******************************************************************/


  mos_ANNOT("Category: Equal")


mos_METHOD(accessor,_equal_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asAccessor));
  mos_return(SELF->_sloti == mos_REFT(mos_ARGV[0],mos_accessor)->_sloti ? mos_true : mos_false);
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


#define SLOTI mos_REFT(mos_METH,mos_accessor)->_sloti
#define SLOT mos_SLOTS(mos_MIMPL)[SLOTI]


mos_ANNOT("Module: getter")
mos_ANNOT("Doc: The value slot getter method.")


/*******************************************************************/


  mos_ANNOT("Category: Coerce")


mos_METHOD(getter,asGetter)
{
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Getter")


mos_METHOD(getter,_get)
{
  mos_return(SLOT);
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Coder")


mos_METHOD(getter,decodeOtherOn_)
{
  SELF->_sloti = mos_INT(mos_send(mos_ARGV[0], mos_s(decodeRawInteger)));
  SELF->_func = _mos_mf_getter__get;
  mos_return(mos_getter_method(SELF->_sloti));
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Print")


mos_METHOD(getter,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "@G%d", SELF->_sloti));
}
mos_METHOD_END


  mos_ANNOT_END


mos_OBJECT(getter)
mos_accessor_METHODS
mos_OBJECT_M(getter,asGetter)
mos_OBJECT_M(getter,printOn_)
mos_OBJECT_METHOD(mos_m(constant,printShallowOn_),printShallowOn_)
mos_OBJECT_M(accessor,encodeOtherOn_)
mos_OBJECT_M(getter,decodeOtherOn_)
mos_OBJECT_SLOTS(getter)
mos_OBJECT_END(protos,getter,mos_accessor,methodMeta)


mos_ANNOT_END
mos_ANNOT_END


/*******************************************************************/


mos_ANNOT("Module: getter")
mos_ANNOT("Doc: The value slot setter method.")


/*******************************************************************/


  mos_ANNOT("Category: Coerce")


mos_METHOD(setter,asSetter)
{
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Print")


mos_METHOD(setter,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "@S%d", SELF->_sloti));
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Getter")


mos_METHOD(setter,_set_)
{
  SLOT = mos_ARGV[0];
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Coder")


mos_METHOD(setter,decodeOtherOn_)
{
  SELF->_sloti = mos_INT(mos_send(mos_ARGV[0], mos_s(decodeRawInteger)));
  SELF->_func = _mos_mf_setter__set_;
  mos_return(mos_setter_method(SELF->_sloti));
}
mos_METHOD_END


  mos_ANNOT_END


mos_OBJECT(setter)
mos_accessor_METHODS
mos_OBJECT_M(setter,asSetter)
mos_OBJECT_M(setter,printOn_)
mos_OBJECT_METHOD(mos_m(constant,printShallowOn_),printShallowOn_)
mos_OBJECT_M(accessor,encodeOtherOn_)
mos_OBJECT_M(setter,decodeOtherOn_)
mos_OBJECT_SLOTS(setter)
mos_OBJECT_END(protos,setter,mos_accessor,methodMeta)


mos_ANNOT_END
mos_ANNOT_END


#undef SELF
#undef SLOTI
#undef SLOT


/*******************************************************************/


#define SELF mos_REFT(mos_MIMPL,mos_constantMethod)
#define SLOT(X)mos_SLOTS(X)[0]


mos_ANNOT("Module: constantMethod")
mos_ANNOT("Doc: A method object that always returns the same value.")


/*******************************************************************/


  mos_ANNOT("Category: Getter")


mos_METHOD(constantMethod,_get)
{
  mos_return(SLOT(mos_METH));
}
mos_METHOD_END


  mos_ANNOT_END


/******************************************************************/


  mos_ANNOT("Category: Equal")


mos_METHOD(constantMethod,_equal_)
{
  mos_return(mos_EQUAL_(mos_RCVR, SLOT(mos_RCVR)));
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Print")


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


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Coder")


mos_METHOD(constantMethod,decodeOtherOn_)
{
  SELF->_func = _mos_mf_constantMethod__get;
}
mos_METHOD_END


  mos_ANNOT_END


mos_value mos_constant_method(mos_value x)
{
  mos_value m = mos_clone(mos_o(constantMethod));
  SLOT(m) = x;
  return m;
}


#undef SELF
#undef SLOT


mos_OBJECT(constantMethod)
mos_OBJECT_M(constantMethod,_equal_)
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


mos_ANNOT_END
mos_ANNOT_END


/********************************************************************/


/* End of accessor. */


mos_ANNOT_END
mos_ANNOT_END


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

