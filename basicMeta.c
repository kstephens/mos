#ifndef __rcs_id__
#ifndef __rcs_id_mos_basicMeta_c__
#define __rcs_id_mos_basicMeta_c__
static const char __rcs_id_mos_basicMeta_c[] = "$Id: basicMeta.c,v 1.3 1999-12-26 20:05:37 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include "mos/slots.h"
#include "mos/basicMeta.h"

mos_ANNOT("Module: basicMeta")
mos_ANNOT("Doc: Is the basic meta object.  All meta objects respond to @\"lookupMethod:\".  All meta objects implement the methods in the basic meta object.")

mos_METHOD(basicMeta,lookupMethod_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asMessage));
  _mos_basicMeta_lookup(mos_REFT(mos_ARGV[0],mos_message));
  mos_return(mos_SLOTST(mos_ARGV[0],mos_message_slots)->_method);
}
mos_METHOD_END

mos_METHOD(basicMeta,lookupMethodForSelector_InObject_)
{
  mos_vector_DECLARE(v, 0, 0);
  mos_message_DECLARE(msg, mos_ARGV[0], mos_ARGV[1], mos_ARGV[1], v, mos_undef);
  mos_return(mos_send(mos_META(mos_ARGV[1]), mos_s(lookupMethod_), mos_MAKE_REF(&msg)));
}
mos_METHOD_END

mos_METHOD(basicMeta,addMethod_ForSelector_ToObject_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asMethod));
  mos_ARGV[1] = mos_send(mos_ARGV[1], mos_s(asSelector));
  _mos_addMethod(mos_REF(mos_ARGV[2]), mos_ARGV[1], mos_ARGV[0]);
}
mos_METHOD_END

mos_METHOD(basicMeta,addReadOnlyValue_ForSelector_ToObject_)
{
  mos_ARGV[1] = mos_send(mos_ARGV[1], mos_s(asSelector));
  _mos_addReadOnlySlot(mos_REF(mos_ARGV[2]), mos_ARGV[1], mos_ARGV[0]);
}
mos_METHOD_END

mos_METHOD(basicMeta,addValue_ForSelector_ToObject_)
{
  mos_ARGV[1] = mos_send(mos_ARGV[1], mos_s(asSelector));
  _mos_addSlot(mos_REF(mos_ARGV[2]), mos_ARGV[1], mos_ARGV[0]);
}
mos_METHOD_END

mos_METHOD(basicMeta,removeMethodForSelector_FromObject_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asSelector));
  _mos_removeMethod(mos_REF(mos_ARGV[1]), mos_ARGV[0]);
}
mos_METHOD_END

mos_METHOD(basicMeta,methodMapForObject_)
{
  mos_return(mos_map_make(mos_METHODS(mos_ARGV[0])));
}
mos_METHOD_END

mos_METHOD(basicMeta,objectDescForObject_)
{
  mos_return(mos_MAKE_REF(mos_DESC(mos_ARGV[0])));
}
mos_METHOD_END

mos_METHOD(basicMeta,slotVectorForObject_)
{
  mos_return(mos_vector_make(mos_NSLOTS(mos_ARGV[0]), mos_SLOTS(mos_ARGV[0])));
}
mos_METHOD_END


mos_OBJECT(basicMeta)
mos_basicMeta_METHODS
mos_OBJECT_SLOTS(basicMeta)
mos_OBJECT_END(metas,basicMeta,mos_object,basicMeta)
mos_ANNOT_END
mos_ANNOT_END

/*
** Alternate interface
*/
mos_value mos_addSlot(mos_value x, mos_value sel, mos_value val)
{
  return mos_send(mos_send(x, mos_s(_meta)), mos_s(addValue_ForSelector_ToObject_), val, sel, x);
}
mos_value mos_addReadOnlySlot(mos_value x, mos_value sel, mos_value val)
{
  return mos_send(mos_send(x, mos_s(_meta)), mos_s(addReadOnlyValue_ForSelector_ToObject_), val, sel, x);
}
mos_value mos_addMethod(mos_value x, mos_value sel, mos_value val)
{
  return mos_send(mos_send(x, mos_s(_meta)), mos_s(addMethod_ForSelector_ToObject_), val, sel, x);
}
