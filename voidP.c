
#ifndef __rcs_id__
#ifndef __rcs_id_mos_voidP_c__
#define __rcs_id_mos_voidP_c__
static const char __rcs_id_mos_voidP_c[] = "$Id: voidP.c,v 1.2 1999-02-19 09:26:26 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"

mos_voidP *_mos_voidP[mos_MAX_VOIDPS];

#define SELF mos_REFT(mos_MIMPL,mos_voidP)
#define X SELF->_x
#define Y mos_REFT(mos_ARGV[0],mos_voidP)->_x

mos_METHOD(voidP,asVoidP)
{
}
mos_METHOD_END

mos_METHOD(voidP,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "(%p asVoidP)", (void*) X));
}
mos_METHOD_END

mos_METHOD(voidP,asInteger)
{
  mos_return(mos_MAKE_INT(X));
}
mos_METHOD_END


#undef SELF
#undef X
#undef Y

mos_OBJECT(voidP)
mos_OBJECT_M(voidP,asVoidP)
mos_OBJECT_M(constant,clone)
mos_OBJECT_M(voidP,printOn_)
mos_OBJECT_M(voidP,asInteger)
mos_OBJECT_SLOTS(voidP)
mos_OBJECT_END(protos,voidP,mos_voidP,basicMeta)

mos_voidP * _mos_voidP_make(void* _x)
{
    mos_voidP *x = mos_REFT(mos_clone(mos_o(voidP)), mos_voidP);
    x->_x = _x;
    return x;
}
