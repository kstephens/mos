#include "mos/mos.h"

#define TRACE_ON mos_send(mos_o(debugger), mos_s(sendTrace_), mos_MAKE_INT(1));
#define TRACE_OFF mos_send(mos_o(debugger), mos_s(sendTrace_), mos_MAKE_INT(0));
#define RESET_TRACE mos_send(mos_o(debugger), mos_s(resetOutput));
#define DUMP_TRACE mos_send(mos_o(debugger), mos_s(flushOutput));

int test1(int argc, char **argv)
{
  mos_value v, w, x, y, z;
  
  mos_init(&argc, &argv);
  TRACE_OFF
  
  v = mos_send(mos_s(foo), mos_s(name));
  mos_printf(mos_FILE(out), "%P\n", v);
  mos_printf(mos_FILE(out), "%P\n", mos_send(v, mos_s(length)));
  
  v = mos_send(mos_s(foo), mos_s(name));
  w = mos_send(v, mos_s(get_), mos_MAKE_INT(2));
  mos_printf(mos_FILE(out), "%P\n", w);
  DUMP_TRACE
  
  w = mos_MAKE_REAL(2);
  mos_printf(mos_FILE(out), "%P\n", w);
  DUMP_TRACE

  w = mos_send(mos_s(name), mos_s(eq_), mos_selector_make("name"));
  mos_printf(mos_FILE(out), "%P\n", w);
  DUMP_TRACE

  w = mos_send(mos_s(name), mos_s(_meta));
  mos_printf(mos_FILE(out), "%P\n", w);
  DUMP_TRACE

  v = mos_send(mos_send(w, mos_s(_meta)), mos_s(methodMapForObject_), w);
  mos_printf(mos_FILE(out), "%P\n", v);
  DUMP_TRACE

  w = mos_send(mos_o(os), mos_s(argv));
  mos_printf(mos_FILE(out), "%P\n", w);
  DUMP_TRACE

  w = mos_send(mos_send(mos_o(vector), mos_s(_meta)), mos_s(methodMapForObject_), mos_o(vector));
  mos_printf(mos_FILE(out), "%P\n", w);
  DUMP_TRACE

  w = mos_send(mos_s(accessor), mos_s(split));
  mos_printf(mos_FILE(out), "%P\n", w);
  DUMP_TRACE

  w = mos_send(mos_s(addMethod_ForSelector_ToObject_), mos_s(split));
  mos_printf(mos_FILE(out), "%P\n", w);
  DUMP_TRACE

  w = mos_send(mos_s(addMethod_ForSelector_ToObject_), mos_s(name));
  mos_printf(mos_FILE(out), "%P\n", w);
  DUMP_TRACE

  v = mos_send(w, mos_s(sliceFrom_Length_), mos_MAKE_INT(4), mos_MAKE_INT(6));
  mos_printf(mos_FILE(out), "%P\n", v);
  DUMP_TRACE

  mos_printf(mos_FILE(out), "%MP\n", mos_s(printSlotsOn_), mos_o(message));
  DUMP_TRACE

  v = mos_send(mos_s(foo), mos_s(name));
  w = mos_send(v, mos_s(get_), mos_MAKE_INT(-2));
  mos_printf(mos_FILE(out), "%P\n", w);
  DUMP_TRACE
  
  mos_send(mos_o(debugger), mos_s(debugger));
  DUMP_TRACE

  return 0;
}