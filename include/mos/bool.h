
#ifndef __rcs_id__
#ifndef __rcs_id_mos_bool_h__
#define __rcs_id_mos_bool_h__
static const char __rcs_id_mos_bool_h[] = "$Id: bool.h,v 1.2 1999-02-19 09:26:28 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define mos_true mos_o(true)
#define mos_false mos_o(false)

#define mos_boolean_make(X) ((X) ? mos_true : mos_false)
