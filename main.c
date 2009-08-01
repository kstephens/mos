#ifndef __rcs_id__
#ifndef __rcs_id_mos_main_c__
#define __rcs_id_mos_main_c__
static const char __rcs_id_mos_main_c[] = "$Id: main.c,v 1.4 2009-08-01 10:47:31 stephens Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"

#define TRACE_ON mos_send(mos_o(debugger), mos_s(sendTrace_), mos_integer_make(1));
#define TRACE_OFF mos_send(mos_o(debugger), mos_s(sendTrace_), mos_integer_make(0));
#define RESET_TRACE mos_send(mos_o(debugger), mos_s(resetOutput));
#define DUMP_TRACE mos_send(mos_o(debugger), mos_s(flushOutput));

static
int main_(int argc, char **argv, char **env)
{
  fprintf(stdout, "type: \"_;\" to inspect the lobby.\n\n");
  comptest(argc, argv);
  
  return 0;
}

int main(int argc, char **argv, char **env)
{
  return mos_init(&argc, &argv, &env, &main_);
}
