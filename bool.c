#ifndef __rcs_id__
#ifndef __rcs_id_mos_bool_c__
#define __rcs_id_mos_bool_c__
static const char __rcs_id_mos_bool_c[] = "$Id: bool.c,v 1.5 2001-09-15 21:59:10 stephens Exp $";
#endif
#endif /* __rcs_id__ */


#include "mos/mos.h"
#include "mos/constant.h"


mos_ANNOT("Module: boolean")
mos_ANNOT("Doc: The boolean constants.")


  mos_ANNOT("Category: Coerce")


mos_METHOD(boolean,asBoolean)
{
}
mos_METHOD_END


mos_METHOD(true,asInteger)
{
  mos_return(mos_integer_make(! 0));
}
mos_METHOD_END


mos_METHOD(false,asInteger)
{
  mos_return(mos_integer_make(0));
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Print")


mos_METHOD(true,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "@T"));
}
mos_METHOD_END


mos_METHOD(false,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "@F"));
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Invert")
  mos_ANNOT("Doc: Return the inverse of the boolean value.")


mos_METHOD(true,not)
{
  mos_return(mos_false);
}
mos_METHOD_END


mos_METHOD(false,not)
{
  mos_return(mos_true);
}
mos_METHOD_END


  mos_ANNOT_END
  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Flow Control")


/*******************************************************************/


  mos_ANNOT("Doc: if receiver is true, send argument @\"value\".")


mos_METHOD(true,ifTrue_)
{
  mos_return(mos_send(mos_ARGV[0], mos_s(value)));
}
mos_METHOD_END


mos_METHOD(false,ifTrue_)
{
  mos_return(mos_undef);
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Doc: if receiver is false, send argument @\"value\".")


mos_METHOD(true,ifFalse_)
{
  mos_return(mos_undef);
}
mos_METHOD_END


mos_METHOD(false,ifFalse_)
{
  mos_return(mos_send(mos_ARGV[0], mos_s(value)));
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Doc: if receiver is true, send argument0 @\"value\"., otherwise send arg1 @\"value\".")


mos_METHOD(true,ifTrue_False_)
{
  mos_return(mos_send(mos_ARGV[0], mos_s(value)));
}
mos_METHOD_END


mos_METHOD(false,ifTrue_False_)
{
  mos_return(mos_send(mos_ARGV[1], mos_s(value)));
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Doc: if receiver is false, send argument0 @\"value\"., otherwise send arg1 @\"value\".")


mos_METHOD(true,ifFalse_True_)
{
  mos_return(mos_send(mos_ARGV[1], mos_s(value)));
}
mos_METHOD_END


mos_METHOD(false,ifFalse_True_)
{
  mos_return(mos_send(mos_ARGV[0], mos_s(value)));
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT_END /* End of "Category: Flow Control" */


/*******************************************************************/


  mos_ANNOT("Doc: Contains the true and false constants.")


mos_OBJECT(boolean)
mos_OBJECT_SLOTS(boolean)
mos_OBJECT_END(constants,boolean,mos_object,basicMeta)


/*******************************************************************/


mos_ANNOT("Doc: Is the true constant.")
mos_OBJECT(true)
mos_constant_METHODS
mos_OBJECT_METHOD(mos_m(boolean,asBoolean),asBoolean)
mos_OBJECT_M(true,printOn_)
mos_OBJECT_M(true,not)
mos_OBJECT_M(true,ifTrue_)
mos_OBJECT_M(true,ifFalse_)
mos_OBJECT_M(true,ifTrue_False_)
mos_OBJECT_M(true,ifFalse_True_)
mos_OBJECT_M(true,asInteger)
mos_OBJECT_SLOTS(true)
mos_OBJECT_END(boolean,true,mos_object,basicMeta)
mos_ANNOT_END


mos_ANNOT("Doc: Is the false constant.")
mos_OBJECT(false)
mos_constant_METHODS
mos_OBJECT_METHOD(mos_m(boolean,asBoolean),asBoolean)
mos_OBJECT_M(false,printOn_)
mos_OBJECT_M(false,not)
mos_OBJECT_M(false,ifTrue_)
mos_OBJECT_M(false,ifFalse_)
mos_OBJECT_M(false,ifTrue_False_)
mos_OBJECT_M(false,ifFalse_True_)
mos_OBJECT_SLOTS(false)
mos_OBJECT_END(boolean,false,mos_object,basicMeta)
mos_ANNOT_END


mos_ANNOT_END


mos_ANNOT_END
mos_ANNOT_END



