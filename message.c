#ifndef __rcs_id__
#ifndef __rcs_id_mos_message_c__
#define __rcs_id_mos_message_c__
static const char __rcs_id_mos_message_c[] = "$Id: message.c,v 1.7 2001-09-18 02:48:08 stephens Exp $";
#endif
#endif /* __rcs_id__ */


#include "mos/mos.h"


mos_ANNOT("Module: message")
mos_ANNOT("Doc: The message object.\
Message objects are created by the system whenever a message is sent to an object. \
Users can dynamically create messages by cloning the message object. \
")


/*******************************************************************/


  mos_ANNOT("Category: Coerce")


mos_METHOD(message,asMessage)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Print")


mos_METHOD(message,printExprOn_)
{
  mos_value
  	context = mos_send(mos_RCVR, mos_s(context)),
  	selector = mos_send(mos_RCVR, mos_s(selector)),
  	selectorSplit = mos_send(selector, mos_s(split)),
  	receiver = mos_send(mos_RCVR, mos_s(receiver)),
  	arguments = mos_send(mos_RCVR, mos_s(arguments));
	
  mos_ARGV[0] = mos_printf(mos_ARGV[0], "%-P ", receiver);
  if ( mos_NE(context,receiver) ) {
    mos_printf(mos_ARGV[0], "@ %-P ", context);
  }
  mos_printf(mos_ARGV[0], "%-W", mos_vector_V(selectorSplit)[0]);
  if ( mos_vector_L(arguments) ) {
    size_t i;
    
    mos_printf(mos_ARGV[0], " %-P", mos_vector_V(arguments)[0]);
    for ( i = 1; i < mos_vector_L(arguments); i ++ ) {
      mos_printf(mos_ARGV[0], " %-W %-P", mos_vector_V(selectorSplit)[i], mos_vector_V(arguments)[i]);
    }
  }
 
  mos_return(mos_ARGV[0]); 
}
mos_METHOD_END


mos_METHOD(message,printOn_)
{
  mos_value
  	context = mos_send(mos_RCVR, mos_s(context)),
  	method = mos_send(mos_RCVR, mos_s(method)),
  	methodImplementer = mos_send(mos_RCVR, mos_s(methodImplementer)),
  	sourceInfo = mos_send(mos_RCVR, mos_s(sourceInfo)),
  	returnValue = mos_send(mos_RCVR, mos_s(returnValue))
	;

  mos_ARGV[0] = mos_printf(mos_ARGV[0], "%P ", sourceInfo);
  
  mos_send(mos_RCVR, mos_s(printExprOn_), mos_ARGV[0]);
  
  if ( mos_NE(returnValue,mos_undef) ) {
    mos_printf(mos_ARGV[0], " = %-P", returnValue);
  }

#if 0
  if ( mos_NE(method,mos_undef) ) {
    mos_printf(mos_ARGV[0], " method: %-P", method);
  }
  if ( mos_NE(methodImplementer,mos_undef) && mos_NE(methodImplementer,context) ) {
    mos_printf(mos_ARGV[0], " @ %-P", methodImplementer);
  }
#endif

  mos_return(mos_ARGV[0]);  
}
mos_METHOD_END


mos_ANNOT("Doc: Formats a message backtrace on a stream object using a formatting selector.")
mos_METHOD(message,backtraceOn_Using_)
{
  mos_value x = mos_RCVR;
  int l = 1;
  
  while ( mos_NE(x,mos_undef) ) {
    mos_ARGV[0] = mos_printf(mos_ARGV[0], "%:%3d %{%MO%}\n", l, mos_ARGV[1], x);
    x = mos_send(x, mos_s(previousMessage));
    l ++;
  }
  
}
mos_METHOD_END
mos_ANNOT_END


mos_METHOD(message,printShallowOn_)
{
  mos_return(mos_send(mos_RCVR, mos_s(printIdOn_), mos_ARGV[0]));
}
mos_METHOD_END


mos_ANNOT_END


/*******************************************************************/


mos_ANNOT("Doc: Send the message to its receiver.")
mos_METHOD(message,send)
{
  mos_return(_mos_send(mos_REFT(mos_RCVR,mos_message)));
}
mos_METHOD_END
mos_ANNOT_END


mos_ANNOT("Doc: Send the message to the specified receiver.  Modifies the message's receiver.")
mos_METHOD(message,forwardTo_)
{
  mos_send(mos_RCVR, mos_s(receiver_), mos_ARGV[0]);
  mos_return(_mos_send(mos_REFT(mos_RCVR,mos_message)));
}
mos_METHOD_END
mos_ANNOT_END


mos_OBJECT(message)
mos_OBJECT_M(message,asMessage)
mos_OBJECT_M(message,printOn_)
mos_OBJECT_M(message,printExprOn_)
mos_OBJECT_M(message,printShallowOn_)
mos_OBJECT_M(message,backtraceOn_Using_)
mos_OBJECT_M(message,send)
mos_OBJECT_M(message,forwardTo_)
mos_ANNOT("Doc: The message that spawned this message.")
mos_OBJECT_A(previousMessage,0)
mos_ANNOT_END
mos_ANNOT("Doc: The selector for the message.")
mos_OBJECT_A(selector,1)
mos_ANNOT_END
mos_ANNOT("Doc: The method lookup context, if @u, then the receiver is assumed to be the implementor of the method.")
mos_OBJECT_A(context,2)
mos_ANNOT_END
mos_ANNOT("Doc: The receiver of the method.")
mos_OBJECT_A(receiver,3)
mos_ANNOT_END
mos_ANNOT("Doc: The vector object containing the arguments.")
mos_OBJECT_A(arguments,4)
mos_ANNOT_END
mos_ANNOT("Doc: The vector object containing the arguments.")
mos_OBJECT_A(locals,5)
mos_ANNOT_END
mos_ANNOT("Doc: The sourceInfor object containing the source location of the generation of this message.")
mos_OBJECT_A(sourceInfo,6)
mos_ANNOT_END
mos_ANNOT("Doc: The method that implements the method for the reciever.")
mos_OBJECT_A(method,7)
mos_ANNOT_END
mos_ANNOT("Doc: The object that implemented the method for the receiver.  This object is usually the receiver, but may not be if some delegate object is implementing the method.")
mos_OBJECT_A(methodImplementer,8)
mos_ANNOT_END
mos_ANNOT("Doc: The return value of the method.")
mos_OBJECT_A(returnValue,9)
mos_ANNOT_END
mos_ANNOT("Doc: The catch object used by blocks to return from this message.")
mos_OBJECT_A(returnCatch,10)
mos_ANNOT_END
mos_OBJECT_SLOTS(message)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(protos,message,mos_message,basicMeta)


mos_INIT(message,10)
{
  mos_send(mos_o(message), mos_s(selector_), mos_s(_));
  mos_send(mos_o(message), mos_s(context_), mos_o(_));
  mos_send(mos_o(message), mos_s(receiver_), mos_o(_));
  mos_send(mos_o(message), mos_s(arguments_), mos_vector_make(0,0));
  mos_send(mos_o(message), mos_s(locals_), mos_vector_make(0,0));
  return 0;
}

mos_ANNOT_END
mos_ANNOT_END


/******************************************************************/


