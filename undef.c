#include "mos/mos.h"
#include "mos/constant.h"

mos_ANNOT("Module: undef")

mos_ANNOT("Doc: The undefined value constant object.")

mos_ANNOT("Category: Equal")
mos_METHOD(undefinedValue,equal_)
{
  mos_return(mos_EQ(mos_RCVR, mos_ARGV[0]) ? mos_true : mos_false);
}
mos_METHOD_END

mos_ANNOT_END

mos_ANNOT("Doc: The method applied if no method is found.")

mos_METHOD(undefinedValue,undefinedMethod)
{
  mos_return(mos_error(mos_s(undefinedMethodError), ""));
}
mos_METHOD_END
mos_ANNOT_END


mos_ANNOT("Category: Print")
mos_METHOD(undefinedValue,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "@U"));
}
mos_METHOD_END

mos_ANNOT_END

mos_OBJECT(undefinedValue)
mos_constant_METHODS
mos_OBJECT_M(undefinedValue,equal_)
mos_OBJECT_M(undefinedValue,undefinedMethod)
mos_OBJECT_M(undefinedValue,printOn_)
mos_OBJECT_M(message,backtraceOn_Using_)
mos_OBJECT_SLOTS(undefinedValue)
mos_OBJECT_END(constants,undefinedValue,mos_object,basicMeta)

mos_ANNOT_END
mos_ANNOT_END


