
#ifndef __rcs_id__
#ifndef __rcs_id_mos_sel2def_h__
#define __rcs_id_mos_sel2def_h__
static const char __rcs_id_mos_sel2def_h[] = "$Id: sel2def.h,v 1.2 1999-02-19 09:26:31 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define mos_DEF_METHOD(O,S) mos_s(O) mos_s(S)
#include "mos/meth.def"
#define mos_DEF_OBJECT(S,O,T,M) mos_s(S) mos_s(O) mos_s(M)
#include "mos/obj.def"
