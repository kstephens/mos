
#ifndef __rcs_id__
#ifndef __rcs_id_mos_error_c__
#define __rcs_id_mos_error_c__
static const char __rcs_id_mos_error_c[] = "$Id: error.c,v 1.2 1999-02-19 09:26:21 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"

mos_METHOD(error,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "@E(%P %P %P)",
  	mos_send(mos_RCVR, mos_s(type)),
	mos_send(mos_RCVR, mos_s(description)),
	mos_send(mos_RCVR, mos_s(inMessage))
	));
}
mos_METHOD_END


mos_METHOD(error,handleError_)
{
  mos_return(mos_send(mos_send(mos_RCVR, mos_s(errorHandler)), mos_s(handleError_), mos_RCVR));
}
mos_METHOD_END

mos_OBJECT(error)
mos_OBJECT_M(error,printOn_)
mos_OBJECT_M(error,handleError_)

mos_OBJECT_A(errorHandler,0)
mos_OBJECT_A(type,1)
mos_OBJECT_A(description,2)
mos_OBJECT_A(inMessage,3)
mos_OBJECT_SLOTS(error)
mos_OBJECT_S(mos_o(errorHandler))
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(protos,error,mos_object,basicMeta)

#define OUT mos_send(mos_o(debugger), mos_s(output))

/*
** The default errorCatcher
** Simply does an abort.
*/
mos_METHOD(errorCatcher,value_)
{
  static int error_lock;
  if ( ! error_lock ) {
    error_lock ++;
    mos_printf(mos_FILE(err), "\nmos: uncaughtError: %W %W: aborting!\n",
    	mos_send(mos_send(mos_ARGV[0], mos_s(type)), mos_s(asString)),
	mos_send(mos_ARGV[0], mos_s(description)));
    error_lock --;
  }
  abort();
  mos_return(mos_undef);
}
mos_METHOD_END

mos_OBJECT(errorCatcher)
mos_OBJECT_M(errorCatcher,value_)
mos_OBJECT_SLOTS(errorCatcher)
mos_OBJECT_END(protos,errorCatcher,mos_object,basicMeta)

/*
** The default errorHandler handleError: method.
** 1. Print the error.
** 2. Print a backtrace to the debugger (and flush).
*/
mos_METHOD(errorHandler,handleError_)
{
  static int error_lock;
  if ( ! error_lock ) {
    error_lock ++;

    mos_printf(OUT, "\nmos: error: %W %W in %P\n",
    	mos_send(mos_send(mos_ARGV[0], mos_s(type)), mos_s(asString)),
	mos_send(mos_ARGV[0], mos_s(description)),
	mos_send(mos_send(mos_ARGV[0], mos_s(inMessage)), mos_s(previousMessage)));

    mos_printf(OUT, "mos: backtrace:\n");
    mos_send(mos_send(mos_ARGV[0], mos_s(inMessage)), mos_s(backtraceOn_Using_), OUT, mos_s(printOn_));

    error_lock --;
  }
  mos_send(mos_o(debugger), mos_s(flushOutput));
  
  /* Forward it to the errorCatcher */
  mos_return(mos_send(mos_send(mos_RCVR, mos_s(errorCatcher)), mos_s(value_), mos_ARGV[0]));
}
mos_METHOD_END

#undef OUT

mos_METHOD(errorHandler,begin)
{
  /* Remember the previous error handler */
  mos_send(mos_RCVR, mos_s(previousHandler_), mos_send(mos_o(error), mos_s(errorHandler)));
  
  /* Set the current error handler to us */
  mos_send(mos_o(error), mos_s(errorHandler_), mos_RCVR);
}
mos_METHOD_END

mos_METHOD(errorHandler,end)
{
  /* Restore the previous error handler */
  mos_send(mos_o(error), mos_s(errorHandler_), mos_send(mos_RCVR, mos_s(previousHandler)));
}
mos_METHOD_END

mos_METHOD(errorHandler,do_OnError_)
{
  mos_value result;
  
  /* Remember the previous error handler */
  mos_send(mos_RCVR, mos_s(begin));
  result = mos_send(mos_ARGV[0], mos_s(value_), mos_RCVR);
  mos_send(mos_RCVR, mos_s(end));
  
  mos_return(result);
}
mos_METHOD_END

mos_OBJECT(errorHandler)
mos_OBJECT_M(errorHandler,begin)
mos_OBJECT_M(errorHandler,end)
mos_OBJECT_M(errorHandler,handleError_)
mos_OBJECT_A(previousHandler,0)
mos_OBJECT_A(errorCatcher,1)
mos_OBJECT_SLOTS(errorHandler)
mos_OBJECT_S(mos_o(errorHandler))
mos_OBJECT_S(mos_o(errorCatcher))
mos_OBJECT_END(protos,errorHandler,mos_object,basicMeta)

mos_value mos_errorV(mos_value e, const char *format, va_list *vap)
{
  static int error_lock;
  mos_value desc = mos_undef, err = mos_undef, msg = mos_MSG;
  
  if ( ! error_lock ) {
    error_lock ++;
    err = mos_send(mos_o(error), mos_s(clone));
    desc = mos_string_make(0,0);
    mos_send(err, mos_s(type_), e);
    mos_send(err, mos_s(description_), desc);
    mos_send(err, mos_s(inMessage_), msg);
    if ( format )
      mos_vprintf(desc, format, vap);
    error_lock --;
  }
  
  return mos_send(mos_send(mos_o(error), mos_s(errorHandler)), mos_s(handleError_), err);
}

mos_value mos_error(mos_value err, const char *format, ...)
{
  mos_value x;
  va_list vap;
  va_start(vap,format);
  x = mos_errorV(err, format, &vap);
  va_end(vap);
  return x;
}
