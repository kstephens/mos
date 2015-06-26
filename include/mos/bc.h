#ifndef _mos_bc_h
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
