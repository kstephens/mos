#include "mos/mos.h"

#define TRACE_ON mos_send(mos_o(debugger), mos_s(sendTrace_), mos_integer_make(1));
#define TRACE_OFF mos_send(mos_o(debugger), mos_s(sendTrace_), mos_integer_make(0));
#define RESET_TRACE mos_send(mos_o(debugger), mos_s(resetOutput));
#define DUMP_TRACE mos_send(mos_o(debugger), mos_s(flushOutput));

static
int main_(int argc, char **argv, char **env)
{
  comptest(argc, argv);
  
  return 0;
}

int main(int argc, char **argv, char **env)
{
  return mos_init(&argc, &argv, &env, &main_);
}
