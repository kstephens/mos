#ifndef __rcs_id__
#ifndef __rcs_id_mos_constant_c__
#define __rcs_id_mos_constant_c__
static const char __rcs_id_mos_constant_c[] = "$Id: constant.c,v 1.4 2000-03-21 07:13:44 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include "mos/constant.h"

mos_ANNOT("Module: constant")
mos_ANNOT("Doc: Constant object behavior.")

/******************************************************************/

mos_ANNOT("Category: Clone")
mos_ANNOT("Doc: Returns the reciever.")
mos_METHOD(constant,clone)
{
}
mos_METHOD_END
mos_ANNOT_END
mos_ANNOT_END

/******************************************************************/

mos_ANNOT("Category: Complile")
mos_ANNOT("Doc: Compile the reciever as a literal value (or constant).")
mos_METHOD(constant,compile_)
{
  mos_send(mos_ARGV[0], mos_s(compileLiteral_), mos_RCVR);
}
mos_METHOD_END
mos_ANNOT_END
mos_ANNOT_END

/******************************************************************/

mos_ANNOT("Category: Coerce")
mos_ANNOT("Doc: Returns the reciever.")
mos_METHOD(constant,asConstant)
{
}
mos_METHOD_END
mos_ANNOT_END
mos_ANNOT_END

/******************************************************************/

mos_ANNOT("Category: Constant")
mos_ANNOT("Doc: Returns @T.")
mos_METHOD(constant,isConstant)
{
  mos_return(mos_true);
}
mos_METHOD_END
mos_ANNOT_END
mos_ANNOT_END

/******************************************************************/

mos_ANNOT("Category: Print")

mos_ANNOT("Doc: Prints the constant.")
mos_METHOD(constant,printShallowOn_)
{
  mos_return(mos_send(mos_RCVR, mos_s(printOn_), mos_ARGV[0]));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Prints the constant as an expression.")
mos_METHOD(constant,printExprOn_)
{
  mos_return(mos_send(mos_RCVR, mos_s(printOn_), mos_ARGV[0]));
}
mos_METHOD_END
mos_ANNOT_END
mos_ANNOT_END

/******************************************************************/

mos_OBJECT(constant)
mos_constant_METHODS
mos_OBJECT_SLOTS(constant)
mos_OBJECT_END(behaviors,constant,mos_object,basicMeta)

mos_ANNOT_END
mos_ANNOT_END
