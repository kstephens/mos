
#ifndef __rcs_id__
#ifndef __rcs_id_mos_bc_c__
#define __rcs_id_mos_bc_c__
static const char __rcs_id_mos_bc_c[] = "$Id: bc.c,v 1.2 1999-02-19 09:26:19 stephensk Exp $";
#endif
#endif /* __rcs_id__ */


const char *_mos_bc_NAMES[] = {
#define mos_BC(NAME,ARGS) #NAME,
#include "mos/bc.def"
0
};

const short _mos_bc_NARGSS[] = {
#define mos_BC(NAME,ARGS) ARGS,
#include "mos/bc.def"
-1
};
