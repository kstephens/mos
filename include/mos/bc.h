#ifndef _mos_bc_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_bc_h__
#define __rcs_id_mos_bc_h__
static const char __rcs_id_mos_bc_h[] = "$Id: bc.h,v 1.2 1999-02-19 09:26:28 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_bc_h

typedef enum mos_bc {
#define mos_BC(NAME,ARGS) mos_bc_##NAME,
#include "mos/bc.def"
mos_bc_LAST
} mos_bc;

enum {
#define mos_BC(NAME,ARGS) mos_bc_##NAME##_nargs = ARGS,
#include "mos/bc.def"
mos_bc__nargs
};

extern const char *_mos_bc_NAMES[];
#define mos_bc_NAME(X) _mos_bc_NAMES[X]
extern short _mos_bc_NARGSS[];
#define mos_bc_NARGS(X) _mos_bc_NARGSS[X]

#endif
