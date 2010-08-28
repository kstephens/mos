#include "mos/mos.h"
#include "mos/basicMeta.h"
#include <stdlib.h> /* abort() */


mos_ANNOT("Module: method")

mos_ANNOT("Doc: Contains all the system method object protos.")

mos_OBJECT(methods)
mos_OBJECT_SLOTS(methods)
mos_OBJECT_END(protos,methods,mos_object,basicMeta)
mos_ANNOT_END

mos_ANNOT("Doc: Is a method object.  Method objects have a meta object that respond to <CODE>@\"applyMethod:\"</CODE>.")

mos_ANNOT("Doc: Returns the method object.")
mos_METHOD(method,asMethod)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Returns the argument identifier by index.")
mos_METHOD(method,argumentAt_)
{
  char arg[8];
  sprintf(arg, "a%ld", (long) mos_INT(mos_ARGV[0]));
  mos_return(mos_selector_make(arg));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Aborts; because primitive methods should be encoded by name.")
mos_METHOD(method,encodeOtherOn_)
{
  abort();
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(method)
mos_method_METHODS
mos_OBJECT_M(method,encodeOtherOn_)
mos_OBJECT_M(method,argumentAt_)
mos_OBJECT_SLOTS(method)
mos_OBJECT_END(methods,method,mos_method,methodMeta)

mos_ANNOT_END

mos_ANNOT("Doc: A meta object for all system method objects.  It responds to <code>@\"applyMethod:\"</code> and implements all of the methods in <code>(_ system meta basicMeta)</code>.")

mos_ANNOT("Doc: Applies the message's method to the receiver.")
mos_METHOD(methodMeta,applyMethod_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asMessage));
  mos_return(_mos_methodMeta_apply(mos_REFT(mos_ARGV[0],mos_message)));
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(methodMeta)
mos_methodMeta_METHODS
mos_OBJECT_SLOTS(methodMeta)
mos_OBJECT_END(metas,methodMeta,mos_object,basicMeta)

mos_ANNOT_END

mos_ANNOT_END

