#include "mos/mos.h"

#define TRACE_ON mos_send(mos_o(debugger), mos_s(sendTrace_), mos_integer_make(1));
#define TRACE_OFF mos_send(mos_o(debugger), mos_s(sendTrace_), mos_integer_make(0));
#define RESET_TRACE mos_send(mos_o(debugger), mos_s(resetOutput));
#define DUMP_TRACE mos_send(mos_o(debugger), mos_s(flushOutput));

static
mos_value readExpr(mos_value prompt, mos_value stream)
{
  if ( mos_NE(stream, mos_o(readline)) ) {
    mos_printf(prompt, "mos> %F");
  }
  return mos_send(mos_o(parser), mos_s(parseExprFrom_), stream);
}

static
mos_value doExpr(mos_value lobby, mos_value rcvr, mos_value expr)
{
  mos_value method, msg, result;

  /* Compile the expression into a method */
  method = mos_send(mos_o(compiler), mos_s(compileExpr_), expr);

  /* Set the method's lobby */
  mos_send(method, mos_s(lobby_), lobby);
  
  /*
  mos_printf(mos_FILE(err), "Method: {\n%P\n%P\n}\n",
  	mos_send(method, mos_s(constants)),
	mos_send(method, mos_s(bytecodes)));
	*/
  
  /* Create a message for the method */
  msg = mos_send(mos_o(message), mos_s(clone));
  mos_send(msg, mos_s(receiver_), rcvr);
  mos_send(msg, mos_s(context_), rcvr);
  mos_send(msg, mos_s(selector_), mos_s(_));
  mos_send(msg, mos_s(arguments_), mos_vector_make(0, 0));
  mos_send(msg, mos_s(method_), method);
  mos_send(msg, mos_s(methodImplementer_), lobby);
  
  /* Apply the method to the message */
  result = mos_send(mos_send(method, mos_s(_meta)), mos_s(applyMethod_), msg);
  
  return result;
}


int comptest(int argc, char **argv)
{
  mos_value catch, errorHandler, result;
  mos_value instrm, outstrm;
  
  instrm = mos_send(mos_o(debugger), mos_s(input));
  outstrm = mos_send(mos_FILE(out), mos_s(asStructuredStream));
  
  result = mos_undef;
  
  TRACE_OFF
  
  errorHandler = mos_send(mos_o(errorHandler), mos_s(clone));
  catch = mos_CATCH_NEW();
  mos_send(errorHandler, mos_s(errorCatcher_), catch);
  
  while ( 1 ) {
    mos_CATCH_BEGIN(catch)
      /* Remember, the catch is only valid within the mos_CATCH_BEGIN/END blocks */
      mos_send(errorHandler, mos_s(begin));
      
      while ( 1 ) {
	mos_value expr;
	
	mos_send(mos_o(debugger), mos_s(flushOutput));
	mos_send(outstrm, mos_s(reset));
	
	expr = readExpr(mos_FILE(err), instrm);
	if ( mos_EQ(expr,mos_eos) ) {
          mos_send(errorHandler, mos_s(end));
	  return 0;
	}
	
	/*
        mos_printf(outstrm, "expr: %{%MP%} =>\n", mos_s(printExprOn_), expr);
	*/

	result = doExpr(mos_o(_), mos_o(_), expr);
	
	/* print the result */
	mos_printf(outstrm, "%{%P%}\n", result);
      }

    mos_CATCH_END
    
    mos_send(errorHandler, mos_s(end));
  }
  
  return 0;
}

