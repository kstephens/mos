
#ifndef __rcs_id__
#ifndef __rcs_id_mos_init_c__
#define __rcs_id_mos_init_c__
static const char __rcs_id_mos_init_c[] = "$Id: init.c,v 1.2 1999-02-19 09:26:22 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include <stdio.h>
#include <stdlib.h> /* qsort() */

#define mos_DEF_INIT(N,P) extern mos_INIT_DECL(_mos_if_##N);
#include "init.def"

static struct mos_init_s {
  const char *name;
  mos_INIT_DECL((*func));
  int priority;
} _inits[] = {
#define mos_DEF_INIT(N,P) { #N, _mos_if_##N, P },
#include "init.def"
  { 0 }
};

static
int _init_cmp(const void *a, const void *b)
{
  return ((struct mos_init_s*) a)->priority - ((struct mos_init_s*) b)->priority;
}

extern void GC_init(int *argcp, char ***argvp);

mos_INIT(user,100)
{
  mos_send(mos_o(_), mos_s(_loadFile_), mos_string_make_("lib/mos/init.mos"));
  return(0);
}

static
void _mos_init_print_inits()
{
  struct mos_init_s *i;

  for ( i = _inits; i->name; i ++ ) {
    fprintf(stderr, "%20s priority (%d)\n", i->name, i->priority);
  }
}

int _mos_inited = 0;

int mos_init(int *argcp, char ***argvp, char ***envp, int (*mainp)(int argc, char **argv, char **env))
{
  struct mos_init_s *i;
  
  if ( _mos_inited ) {
    return 0;
  }
  
  GC_init(argcp, argvp);

  _mos_inited ++;

  /* Sort the initializers by priority */
  qsort(_inits, (sizeof(_inits)/sizeof(_inits[0])) - 1, sizeof(_inits[0]), _init_cmp);
  
  /*
  _mos_init_print_inits();
  */

  fprintf(stderr, "mos: INIT: ");
  fflush(stderr);

  for ( i = _inits; i->name; i ++ ) {
    /* Make sure there are no conflicting init priorities */
    if ( i != _inits ) {
      if ( i->priority == i[-1].priority ) {
	fprintf(stderr, "\nmos: INIT: %s priority (%d) == %s priority\n", i->name, i->priority, i[-1].name);
	_mos_init_print_inits();
	abort();
      }
    }

    fprintf(stderr, "%s ", i->name);
    fflush(stderr);
    
    if ( (*i->func)(argcp, argvp) ) {
      fprintf(stderr, "FAILED!\n");
      return -1;
    }
  }
  
  fprintf(stderr, "DONE.\n");

  _mos_inited ++;

  return mainp(*argcp, *argvp, *envp);
}
