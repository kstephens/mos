#ifndef _mos_init_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_init_h__
#define __rcs_id_mos_init_h__
static const char __rcs_id_mos_init_h[] = "$Id: init.h,v 1.2 1999-02-19 09:26:29 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_init_h

#define mos_INIT_DECL(X) int X (int *argcp, char ***argvp)
#define mos_INIT(N,P) mos_INIT_DECL(_mos_if_##N)

extern int _mos_inited;

int mos_init(int *argcp, char ***argvp, char ***envp, int (*mainp)(int argc, char **argv, char **env));

#endif
