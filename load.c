#ifndef __rcs_id__
#ifndef __rcs_id_mos_load_c__
#define __rcs_id_mos_load_c__
static const char __rcs_id_mos_load_c[] = "$Id: load.c,v 1.3 1999-12-26 20:05:54 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"

static
mos_value doExpr(mos_value lobby, mos_value rcvr, mos_value expr)
{
  mos_value method, msg, result;

  /* Compile the expression into a method */
  method = mos_send(mos_o(compiler), mos_s(compileExpr_), expr);

  /* Set the method's lobby */
  mos_send(method, mos_s(lobby_), lobby);
  
  /* Create a message for the method */
  msg = mos_send(mos_o(message), mos_s(clone));
  mos_send(msg, mos_s(receiver_), rcvr);
  mos_send(msg, mos_s(context_), rcvr);
  mos_send(msg, mos_s(selector_), mos_s(_));
  mos_send(msg, mos_s(arguments_), mos_vector_make(0, 0));
  mos_send(msg, mos_s(method_), method);
  mos_send(msg, mos_s(methodImplementer), lobby);
  
  /* Apply the method to the message */
  result = mos_send(mos_send(method, mos_s(_meta)), mos_s(applyMethod_), msg);
  
  return result;
}

mos_METHOD(defaultBehavior,_loadInStream_ResultStream_PromptStream_Prompt_)
{
  mos_value result;
  mos_value instrm, outstrm, prstrm, prompt;
  
  instrm = mos_ARGV[0];
  outstrm = mos_ARGV[1];
  prstrm = mos_ARGV[2];
  prompt = mos_ARGV[3];
  
  result = mos_undef;
  
  while ( 1 ) {
    mos_value expr;
	
    mos_send(mos_o(debugger), mos_s(flushOutput));
    if ( mos_NE(outstrm, mos_undef) ) {
      mos_send(outstrm, mos_s(reset));
    }

    if ( mos_NE(instrm, mos_o(readline)) && mos_NE(prstrm, mos_undef) && mos_NE(prompt, mos_undef) ) {
      mos_printf(prstrm, "%W%F", prompt);
    }

    expr = mos_send(mos_o(parser), mos_s(parseExprFrom_), instrm);
    if ( mos_EQ(expr,mos_eos) ) {
      return result;
    }
	
    result = doExpr(mos_o(_), mos_RCVR, expr);
	
    /* print the result */
    if ( mos_NE(outstrm, mos_undef) ) {
      mos_printf(outstrm, "%{%P%}\n", result);
    }
  }

  return result;
}
mos_METHOD_END

mos_METHOD(defaultBehavior,_loadFile_)
{
  mos_value instrm, result;

  static mos_value r;
  if ( ! mos_INITIALIZED(r) )
    r = mos_string_make_("r");

  instrm = mos_send(mos_o(stream), mos_s(clone));
  instrm = mos_send(instrm, mos_s(open_Mode_), mos_ARGV[0], r);

  if ( mos_EQ(instrm, mos_false) ) {
    mos_error(mos_s(load), "cannot open %W for reading", mos_ARGV[0]);
  } else {
    result = mos_send(mos_RCVR, mos_s(_loadInStream_ResultStream_PromptStream_Prompt_), instrm, mos_undef, mos_undef, mos_undef);

    mos_send(instrm, mos_s(close));
  }

  mos_return(result);
}
mos_METHOD_END

mos_INIT(load,75)
{
  mos_addMethod(mos_o(defaultBehavior), mos_s(_loadFile_), mos_m(defaultBehavior,_loadFile_));
  mos_addMethod(mos_o(defaultBehavior), mos_s(_loadInStream_ResultStream_PromptStream_Prompt_), mos_m(defaultBehavior,_loadInStream_ResultStream_PromptStream_Prompt_));

  return 0;
}

