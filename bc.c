
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
