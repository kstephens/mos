#ifndef _mos_init_h
#define _mos_init_h

#define mos_INIT_DECL(X) int X (int *argcp, char ***argvp)
#define mos_INIT(N,P) mos_INIT_DECL(_mos_if_##N)

extern int _mos_inited;

int mos_init(int *argcp, char ***argvp, char ***envp, int (*mainp)(int argc, char **argv, char **env));

#endif
