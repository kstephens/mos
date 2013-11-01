#include "mos/mos.h"
#include "mos/basicMeta.h"

#define TRACE_OUTPUT mos_send(mos_o(debugger), mos_s(output))

mos_METHOD(traceMethod,beforeMethod_)
{
  mos_printf(TRACE_OUTPUT, "TRACE: APPLY: %P\n", mos_ARGV[0]);
}
mos_METHOD_END

mos_METHOD(traceMethod,afterMethod_)
{
  mos_printf(TRACE_OUTPUT, "TRACE: RETURN: %P\n", mos_ARGV[0]);
}
mos_METHOD_END

#undef TRACE_OUTPUT

mos_OBJECT(traceMethod)
mos_OBJECT_M(traceMethod,asMethod)
mos_OBJECT_M(traceMethod,beforeMethod_)
mos_OBJECT_M(traceMethod,afterMethod_)
mos_OBJECT_A(method,0)
mos_OBJECT_SLOTS(traceMethod)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(protos,traceMethod,mos_object,traceMethodMeta)

mos_METHOD(traceMethod,asMethod)
{
  mos_return(mos_RCVR);
}
mos_METHOD_END


mos_METHOD(traceMethodMeta,applyMethod_)
{
  mos_value temp, rtnval;
  
  /* Replace the message's method with our delegate */
  temp = mos_METH;
  mos_METH = mos_send(mos_METH, mos_s(method));
  
  /* Do something before the real method */
  mos_send(temp, mos_s(beforeMethod_), mos_MSG);
  
  /* Apply the real method */
  rtnval = _mos_apply(mos_REFT(mos_MSG,mos_message));
  
  /* Do something after the real method */
  mos_send(temp, mos_s(beforeMethod_), mos_MSG);

  mos_return(rtnval);
}
mos_METHOD_END


mos_OBJECT(traceMethodMeta)
mos_OBJECT_M(traceMethodMeta,applyMethod_)
mos_methodMeta_METHODS
mos_OBJECT_SLOTS(traceMethodMeta)
mos_OBJECT_END(metas,traceMethodMeta,mos_object,basicMeta)

/*
call [trace traceMethod:@"printOn:" InObject:(defaultBehavior)]
*/
mos_METHOD(trace,traceMethod_InObject_)
{
  mos_value meta, meth, tmeth;
  
  meta = mos_send(mos_ARGV[1], mos_s(_meta));
  meth = mos_send(meta, mos_s(lookupMethodForSelector_InObject_), mos_ARGV[0], mos_ARGV[1]);
  tmeth = mos_send(mos_RCVR, mos_s(methodProto));
  tmeth = mos_send(tmeth, mos_s(clone));
  mos_send(tmeth, mos_s(method_), meth);
  mos_send(meta, mos_s(addMethod_ForSelector_ToObject_), tmeth, mos_ARGV[0], mos_ARGV[1]);
  
  
}
mos_METHOD_END

mos_METHOD(trace,untraceMethod_InObject_)
{
  mos_value meta, meth, tmeth;
  
  meta = mos_send(mos_ARGV[1], mos_s(_meta));
  tmeth = mos_send(meta, mos_s(lookupMethodForSelector_InObject_), mos_ARGV[0], mos_ARGV[1]);
  meth = mos_send(tmeth, mos_s(method));
  mos_send(meta, mos_s(addMethod_ForSelector_ToObject_), meth, mos_ARGV[0], mos_ARGV[1]);
  
  
}
mos_METHOD_END

mos_OBJECT(trace)
mos_OBJECT_M(trace,traceMethod_InObject_)
mos_OBJECT_M(trace,untraceMethod_InObject_)
mos_OBJECT_A(methodProto,0)
mos_OBJECT_SLOTS(trace)
mos_OBJECT_S(mos_o(traceMethod))
mos_OBJECT_END(system,trace,mos_object,basicMeta)
