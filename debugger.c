#include "mos/mos.h"

#define OUT mos_send(mos_RCVR, mos_s(output))

mos_ANNOT("Module: debugger")
mos_ANNOT("Doc: Is the system debugger.")

  mos_ANNOT("Category: Backtrace")

mos_ANNOT("Doc: Prints a stack backtrace.")
mos_METHOD(debugger,backtrace)
{
  mos_value x = mos_MSG;
  x = mos_send(x, mos_s(previousMessage));
  mos_send(x, mos_s(backtraceOn_Using_), OUT, mos_undef);
}
mos_METHOD_END
mos_ANNOT_END

  mos_ANNOT_END

  mos_ANNOT("Category: Send Trace")

mos_ANNOT("Doc: Returns the current send trace level.")
mos_METHOD(debugger,sendTrace)
{
  mos_return(mos_integer_make(_mos_send_trace));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Sets the send trace level.  '0' disables send tracing.")
mos_METHOD(debugger,sendTrace_)
{
  _mos_send_trace = mos_INT(mos_send(mos_ARGV[0], mos_s(asInteger)));
  mos_return(mos_undef);
}
mos_METHOD_END
mos_ANNOT_END

  mos_ANNOT_END


  mos_ANNOT("Category: Output")

mos_ANNOT("Doc: Empties the debugger output buffer.")
mos_METHOD(debugger,resetOutput)
{
  mos_send(mos_RCVR, mos_s(output_), mos_send(mos_send(mos_string_make(0, 0), mos_s(deepen)), mos_s(asStructuredStream)));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Flushes the debugger output buffer to the debugger\'s outputStream.")
mos_METHOD(debugger,flushOutput)
{
  mos_send(mos_send(mos_RCVR, mos_s(outputStream)), mos_s(writeString_), mos_send(mos_send(mos_RCVR, mos_s(output)), mos_s(substream)));
  mos_send(mos_RCVR, mos_s(resetOutput));
}
mos_METHOD_END
mos_ANNOT_END

  mos_ANNOT_END


mos_OBJECT(debugger)
mos_OBJECT_M(debugger,backtrace)
mos_OBJECT_M(debugger,sendTrace)
mos_OBJECT_M(debugger,sendTrace_)
mos_OBJECT_M(debugger,resetOutput)
mos_OBJECT_M(debugger,flushOutput)
mos_ANNOT("Doc: The current debugger level")
mos_OBJECT_A(debugLevel,0)
mos_ANNOT_END
mos_ANNOT("Doc: The stream to read debugger commands from.")
mos_OBJECT_A(input,1)
mos_ANNOT_END
mos_ANNOT("Category: Output") 
mos_ANNOT("Doc: The debugger output buffer.") 
mos_OBJECT_A(output,2)
mos_ANNOT_END
mos_ANNOT("Doc: The stream to flush the debugger output buffer.")
mos_OBJECT_A(outputStream,3)
mos_ANNOT_END
mos_ANNOT_END
mos_OBJECT_SLOTS(debugger)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(system,debugger,mos_object,basicMeta)

mos_ANNOT_END
mos_ANNOT_END

mos_INIT(debugger,20)
{
  mos_send(mos_o(debugger), mos_s(debugLevel_), mos_integer_make(1));
  mos_send(mos_o(debugger), mos_s(input_), mos_FILE(in));
  mos_send(mos_o(debugger), mos_s(outputStream_), mos_FILE(err));
  mos_send(mos_o(debugger), mos_s(resetOutput));
  
  return 0;
}

