
#ifndef __rcs_id__
#ifndef __rcs_id_mos_bcCompile_h__
#define __rcs_id_mos_bcCompile_h__
static const char __rcs_id_mos_bcCompile_h[] = "$Id: bcCompile.h,v 1.2 1999-02-19 09:26:28 stephensk Exp $";
#endif
#endif /* __rcs_id__ */


#define mos_expr(X) mos_send(mos_o(X), mos_s(clone))
#define mos_exprConstant(X) mos_send(mos_expr(X), mos_s(value_), (X))
