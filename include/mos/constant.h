#ifndef _mos_constant_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_constant_h__
#define __rcs_id_mos_constant_h__
static const char __rcs_id_mos_constant_h[] = "$Id: constant.h,v 1.2 1999-02-19 09:26:28 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_constant_h

#define mos_constant_METHODS \
  mos_OBJECT_M(constant,clone) \
  mos_OBJECT_M(constant,compile_) \
  mos_OBJECT_M(constant,isConstant) \
  mos_OBJECT_M(constant,asConstant) \
  mos_OBJECT_M(constant,printShallowOn_) \
  mos_OBJECT_M(constant,printExprOn_)

#endif
