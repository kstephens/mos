#include "mos/mos.h"

#define TRACE_ON mos_send(mos_o(debugger), mos_s(sendTrace_), mos_MAKE_INT(1));
#define TRACE_OFF mos_send(mos_o(debugger), mos_s(sendTrace_), mos_MAKE_INT(0));
#define RESET_TRACE mos_send(mos_o(debugger), mos_s(resetOutput));
#define DUMP_TRACE mos_send(mos_o(debugger), mos_s(flushOutput));

int test2(int argc, char **argv)
{
  mos_value v, w, x, y, z;
  
  mos_init(&argc, &argv);

#define TIME_LOOPS 100
#define TIME_BEGIN { int t[3], i; t[0] = time(0); for ( i = 0; i < TIME_LOOPS; i ++ ) {
#define TIME_END(X) } ; t[1] = time(0); t[2] = t[1] - t[0]; printf("%d %ss in %d sec; %g/sec\n", n * TIME_LOOPS, X, t[2], (double)n * TIME_LOOPS / (double)t[2]); }
  {
  int n;
  
  TIME_BEGIN
  for ( n = 0; n < 1000000; n ++ ) {
    mos_send(mos_o(protos), mos_s(empty));
  }
  TIME_END("msg send")
  }
    

  return 0;
}
