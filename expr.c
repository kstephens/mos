
#ifndef __rcs_id__
#ifndef __rcs_id_mos_expr_c__
#define __rcs_id_mos_expr_c__
static const char __rcs_id_mos_expr_c[] = "$Id: expr.c,v 1.4 1999-12-04 01:47:39 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"

mos_ANNOT("Module: expr")
mos_ANNOT("Doc: The expression objects.  Expression objects are parsed by the parse object and compiled by the compiler object.")

/************************************************************************/

mos_ANNOT("Doc: Represents a constant value expression.")

mos_ANNOT("Category: Printing")
mos_METHOD(exprConstant,printExprOn_)
{
  mos_return(mos_send(mos_send(mos_RCVR, mos_s(value)), mos_s(printOn_), mos_ARGV[0]));
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(exprConstant)
mos_OBJECT_M(exprConstant,printExprOn_)
mos_OBJECT_M(exprConstant,compile_)
mos_OBJECT_M(constant,isConstant)
mos_OBJECT_A(value,0)
mos_OBJECT_SLOTS(exprConstant)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(expr,exprConstant,mos_object,basicMeta)

mos_ANNOT_END

/************************************************************************/

mos_ANNOT("Doc: Represent a <CODE>(<expr>)</CODE> expression.")
 
mos_ANNOT("Category: Print")
mos_METHOD(exprGroup,printExprOn_)
{
  mos_printf(mos_ARGV[0], "(%MP)", mos_s(printExprOn_), mos_send(mos_RCVR, mos_s(value)));
}
mos_METHOD_END
mos_ANNOT_END

mos_METHOD(exprGroup,compile_)
{
  mos_send(mos_send(mos_RCVR, mos_s(value)), mos_s(compile_), mos_ARGV[0]);
}
mos_METHOD_END

mos_OBJECT(exprGroup)
mos_OBJECT_M(exprGroup,printExprOn_)
mos_OBJECT_M(exprGroup,compile_)
mos_OBJECT_A(value,0)
mos_OBJECT_SLOTS(exprGroup)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(expr,exprGroup,mos_object,basicMeta)

mos_ANNOT_END

/************************************************************************/

mos_ANNOT("Doc: Represents a memorized expression.  Memoized expressions are only evaluated once.")

mos_ANNOT("Category: Print")
mos_METHOD(exprMemo,printExprOn_)
{
  mos_printf(mos_ARGV[0], "`%MP", mos_s(printExprOn_), mos_send(mos_RCVR, mos_s(value)));
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(exprMemo)
mos_OBJECT_M(exprMemo,printExprOn_)
mos_OBJECT_M(exprMemo,compile_)
mos_OBJECT_M(constant,isConstant)
mos_OBJECT_A(value,0)
mos_OBJECT_SLOTS(exprMemo)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(expr,exprMemo,mos_object,basicMeta)

mos_ANNOT_END

/************************************************************************/

mos_ANNOT("Doc: Represents a message send expression.")

mos_ANNOT("Category: Print")
mos_METHOD(exprSend,printExprOn_)
{
  mos_value
  	context = mos_send(mos_RCVR, mos_s(context)),
  	selector = mos_send(mos_send(mos_RCVR, mos_s(selector)), mos_s(value)),
  	selectorSplit = mos_send(selector, mos_s(split)),
  	receiver = mos_send(mos_RCVR, mos_s(receiver)),
  	arguments = mos_send(mos_RCVR, mos_s(arguments));

  mos_printf(mos_ARGV[0], "(");
  if ( mos_NE(receiver, mos_undef) ) {
    mos_printf(mos_ARGV[0], "%MP ", mos_s(printExprOn_), receiver);
  }
  if ( mos_NE(context,receiver) && mos_NE(context,mos_undef) ) {
    mos_printf(mos_ARGV[0], ". %MP ", mos_s(printExprOn_), context);
  }
  mos_printf(mos_ARGV[0], "%-W", mos_vector_V(selectorSplit)[0]);
  if ( mos_vector_L(arguments) ) {
    size_t i;
    
    mos_printf(mos_ARGV[0], " %MP", mos_s(printExprOn_), mos_vector_V(arguments)[0]);
    for ( i = 1; i < mos_vector_L(arguments); i ++ ) {
      mos_printf(mos_ARGV[0], " %-W %MP", mos_vector_V(selectorSplit)[i], mos_s(printExprOn_), mos_vector_V(arguments)[i]);
    }
  }
  mos_printf(mos_ARGV[0], ")");
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(exprSend)
mos_OBJECT_M(exprSend,printExprOn_)
mos_OBJECT_M(exprSend,compile_)
mos_OBJECT_A(context,0)
mos_OBJECT_A(receiver,1)
mos_OBJECT_A(selector,2)
mos_OBJECT_A(arguments,3)
mos_OBJECT_A(value,4)
mos_OBJECT_SLOTS(exprSend)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(expr,exprSend,mos_object,basicMeta)

mos_ANNOT_END

/************************************************************************/

mos_ANNOT("Doc: Represents an object creation expression.")

mos_ANNOT("Category: Print")
mos_METHOD(exprObject,printExprOn_)
{
  mos_value v;
  
  mos_ARGV[0] = mos_printf(mos_ARGV[0], "%<{");

  v = mos_send(mos_RCVR, mos_s(slots));
  mos_vector_LOOP(v, ep)
    mos_printf(mos_ARGV[0], "\n%MP", mos_s(printExprOn_), *ep);
  mos_vector_LOOP_END
  
  v = mos_send(mos_RCVR, mos_s(body));
  if ( mos_vector_L(v) ) {
    mos_printf(mos_ARGV[0], "%<{");
    mos_vector_LOOP(v, ep)
      mos_printf(mos_ARGV[0], "\n%MP;", mos_s(printExprOn_), *ep);
    mos_vector_LOOP_END
    mos_printf(mos_ARGV[0], "%>\n}");
  }
  mos_printf(mos_ARGV[0], "%>\n}");
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(exprObject)
mos_OBJECT_M(exprObject,printExprOn_)
mos_OBJECT_M(exprObject,compile_)
mos_OBJECT_A(slots,0)
mos_OBJECT_A(body,1)
mos_OBJECT_A(annot,2)
mos_OBJECT_SLOTS(exprObject)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(expr,exprObject,mos_object,basicMeta)
mos_ANNOT_END

/************************************************************************/

mos_ANNOT("Doc: Represents a value slot in an object.")

mos_ANNOT("Category: Print")
mos_METHOD(exprSlot,printExprOn_)
{
  mos_value value = mos_send(mos_RCVR, mos_s(value));
  if ( mos_EQ(value,mos_undef) ) {
    mos_printf(mos_ARGV[0], "%W;",
    	mos_send(mos_send(mos_RCVR, mos_s(selector)), mos_s(asString)));
  } else {
    mos_printf(mos_ARGV[0], "%[%W -> %MP%];",
    	mos_send(mos_send(mos_RCVR, mos_s(selector)), mos_s(asString)),
     	mos_s(printExprOn_), value);
  }
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(exprSlot)
mos_OBJECT_M(exprSlot,printExprOn_)
mos_OBJECT_M(exprSlot,compile_WithConstructor_)
mos_OBJECT_M(exprSlot,compile_)
mos_OBJECT_A(selector,0)
mos_OBJECT_A(value,1)
mos_OBJECT_A(annot,2)
mos_OBJECT_SLOTS(exprSlot)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(expr,exprSlot,mos_object,basicMeta)
mos_ANNOT_END

/************************************************************************/

mos_ANNOT("Doc: Represents a read-only slot.")

mos_ANNOT("Category: Print")
mos_METHOD(exprReadOnlySlot,printExprOn_)
{
  mos_value value = mos_send(mos_RCVR, mos_s(value));
  mos_printf(mos_ARGV[0], "%<%W = %MP%>;", mos_send(mos_send(mos_RCVR, mos_s(selector)), mos_s(asString)), mos_s(printExprOn_), value);
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(exprReadOnlySlot)
mos_OBJECT_M(exprReadOnlySlot,printExprOn_)
mos_OBJECT_M(exprReadOnlySlot,compile_WithConstructor_)
mos_OBJECT_M(exprReadOnlySlot,compile_)
mos_OBJECT_A(selector,0)
mos_OBJECT_A(value,1)
mos_OBJECT_A(annot,2)
mos_OBJECT_SLOTS(exprReadOnlySlot)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(expr,exprReadOnlySlot,mos_object,basicMeta)
mos_ANNOT_END

/************************************************************************/

mos_ANNOT("Doc: Represents a method slot.")

mos_ANNOT("Category: Print")
mos_METHOD(exprMethodSlot,printExprOn_)
{
  mos_value value = mos_send(mos_RCVR, mos_s(value));
  mos_value selector = mos_send(mos_RCVR, mos_s(selector));
  mos_value selectorSplit = mos_send(selector, mos_s(split));
  mos_value arguments = mos_send(value, mos_s(arguments));

  /* print the selector and argument pairs */
  mos_printf(mos_ARGV[0], "%-W", mos_vector_V(selectorSplit)[0]);
  if ( mos_vector_L(arguments) ) {
    size_t i;
    
    mos_printf(mos_ARGV[0], " %-W", mos_vector_V(arguments)[0]);
    for ( i = 1; i < mos_vector_L(arguments); i ++ ) {
      mos_printf(mos_ARGV[0], " %-W %-W", mos_vector_V(selectorSplit)[i], mos_vector_V(arguments)[i]);
    }
  }
  mos_send(value, mos_s(printExprOn_), mos_ARGV[0]);
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(exprMethodSlot)
mos_OBJECT_M(exprMethodSlot,printExprOn_)
mos_OBJECT_M(exprMethodSlot,compile_WithConstructor_)
mos_OBJECT_A(selector,0)
mos_OBJECT_A(value,1)
mos_OBJECT_A(annot,2)
mos_OBJECT_SLOTS(exprMethodSlot)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(expr,exprMethodSlot,mos_object,basicMeta)
mos_ANNOT_END

/************************************************************************/

mos_ANNOT("Doc: Represents a method object.")

mos_ANNOT("Category: Print")
mos_METHOD(exprMethod,printExprOn_)
{
  mos_value x;
  int locals = 0;

  mos_ARGV[0] = mos_printf(mos_ARGV[0], "{%<");
  
  x = mos_send(mos_RCVR, mos_s(locals));
  mos_vector_LOOP(x,ep);
  {
    if ( ! locals ) {
      mos_printf(mos_ARGV[0], "@ ");
      locals = 1;
    }
    mos_printf(mos_ARGV[0], "%MP ", mos_s(printExprOn_), *ep);
  }
  mos_vector_LOOP_END
  if ( locals ) {
    mos_printf(mos_ARGV[0], "@ ");
  }

  x = mos_send(mos_RCVR, mos_s(body));
  mos_vector_LOOP(x,ep)
    mos_printf(mos_ARGV[0], "\n%MP; ", mos_s(printExprOn_), *ep);
  mos_vector_LOOP_END
  
  mos_printf(mos_ARGV[0], "%>\n}");
  
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(exprMethod)
mos_OBJECT_M(exprMethod,printExprOn_)
mos_OBJECT_M(exprMethod,compile_)
mos_OBJECT_A(arguments,0)
mos_OBJECT_A(locals,1)
mos_OBJECT_A(body,2)
mos_ANNOT("Doc: This method has a block in its lexical scope that will do a non-local return.")
mos_OBJECT_A(hasBlockReturn,3)
mos_ANNOT_END
mos_ANNOT("Doc: This method is the @\"value*\" method for a block.")
mos_OBJECT_A(asBlock,4)
mos_ANNOT_END
mos_OBJECT_SLOTS(exprMethod)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_false)
mos_OBJECT_S(mos_false)
mos_OBJECT_END(expr,exprMethod,mos_object,basicMeta)
mos_ANNOT_END

/************************************************************************/

mos_ANNOT("Doc: Represents a block expression.")

mos_ANNOT("Category: Print")
mos_ANNOT("Doc: Print the block expression as an expression.")
mos_METHOD(exprBlock,printExprOn_)
{
  mos_value x;
  
  mos_ARGV[0] = mos_printf(mos_ARGV[0], "%<[");
  
  x = mos_send(mos_RCVR, mos_s(arguments));
  if ( mos_vector_L(x) ) {
    mos_printf(mos_ARGV[0], "( ");
    mos_vector_LOOP(x,ep)
      mos_printf(mos_ARGV[0], "%W ", mos_send(*ep, mos_s(asString)));
    mos_vector_LOOP_END
    mos_printf(mos_ARGV[0], ") ");
  }
  
  x = mos_send(mos_RCVR, mos_s(body));
  mos_vector_LOOP(x,ep)
    mos_printf(mos_ARGV[0], "\n%MP;", mos_s(printExprOn_), *ep);
  mos_vector_LOOP_END
  
  mos_printf(mos_ARGV[0], "%>\n]");
  
}
mos_METHOD_END
mos_ANNOT_END
mos_ANNOT_END

mos_OBJECT(exprBlock)
mos_OBJECT_M(exprBlock,printExprOn_)
mos_OBJECT_M(exprBlock,compile_)
mos_OBJECT_A(arguments,0)
mos_OBJECT_A(locals,1)
mos_OBJECT_A(body,2)
mos_OBJECT_SLOTS(exprBlock)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(expr,exprBlock,mos_object,basicMeta)
mos_ANNOT_END

mos_ANNOT_END
mos_ANNOT_END

/************************************************************************
**
** C interface
**
*/
#include "mos/expr.h"

#define mos_expr(X) mos_send(mos_o(X), mos_s(clone))
#define mos_SET(X,S,V) mos_send(X, mos_s(S##_), V)

mos_value mos_exprConstant(mos_value X)
{
  if ( mos_NE(mos_send(X, mos_s(isConstant)), mos_false) ) {
    return X;
  }
  return mos_SET(mos_expr(exprConstant), value, (X));
}

mos_value mos_exprMemo(mos_value X)
{
  if ( mos_DESC(X) == mos_DESC(mos_o(exprMemo)) ) {
    return X;
  }
  if ( mos_NE(mos_send(X, mos_s(isConstant)), mos_false) ) {
    return X;
  }
  return mos_SET(mos_expr(exprMemo), value, (X));
}

mos_value mos_exprGroup(mos_value X)
{
  return mos_SET(mos_expr(exprGroup), value, (X));
}

mos_value mos_exprSelf()
{
  static mos_value e;
  
  if ( ! mos_INITIALIZED(e) ) {
    e = mos_exprSend(mos_undef, mos_s(self), 0);
  }
  
  return e;
}

mos_value mos_exprReturnSelf()
{
  static mos_value e;
  
  if ( ! mos_INITIALIZED(e) ) {
    e = mos_exprSend(mos_undef, mos_s(__RTN__), 1, mos_exprSelf());
  }
  
  return e;
}

mos_value mos_exprMsg()
{
  static mos_value e;
  
  /* (_msg) */
  if ( ! mos_INITIALIZED(e) ) {
    e = mos_exprSend(mos_undef, mos_s(_msg), 0);
  }
  
  return e;
}

mos_value mos_exprMethodContext()
{
  static mos_value e;
  
  /* (_rcvr _methodContext) */
  if ( ! mos_INITIALIZED(e) ) {
    e = mos_exprSend(mos_undef, mos_s(_msg), 0);
    e = mos_exprSend(e, mos_s(_rcvr), 0);
    e = mos_exprSend(e, mos_s(_methodContext), 0);
  }
  
  return e;
}

mos_value mos_exprVectorClone(void)
{
  static mos_value e;
  
  if ( ! mos_INITIALIZED(e) ) {
    e =    
  	mos_exprSend(
	mos_exprSend(
	mos_exprSend(
	mos_exprSend(
	mos_exprSend(mos_undef,
		mos_s(_), 0),
		mos_s(system), 0),
		mos_s(protos), 0),
		mos_s(vector), 0),
		mos_s(clone), 0);
  }
  
  return e;
}

mos_value mos_exprVector(mos_value v)
{
  mos_value e;

  e = mos_exprVectorClone();   
  
  mos_vector_LOOP(v,ep);
  {
    e = mos_exprSend(e, mos_s(append_), 1, mos_exprConstant(*ep));
  }
  mos_vector_LOOP_END;
  
  return e;
}

mos_value mos_exprVectorFill(mos_value length)
{
  mos_value e;

  e = mos_exprVectorClone();   
  
  e = mos_exprSend(e, mos_s(length_), 1, mos_exprConstant(length));
  
  return e;
}

mos_value mos_exprMap()
{
  static mos_value e;
  
  if ( ! mos_INITIALIZED(e) ) {
    e =    
  	mos_exprSend(
	mos_exprSend(
	mos_exprSend(
	mos_exprSend(
	mos_exprSend(mos_undef,
		mos_s(_), 0),
		mos_s(system), 0),
		mos_s(protos), 0),
		mos_s(map), 0),
		mos_s(clone), 0);
  }
  
  return e;
}

mos_value mos_exprSend_V(mos_value cntx, mos_value rcvr, mos_value sel, mos_value args)
{
  mos_value e = mos_expr(exprSend);

  mos_SET(e, context, cntx);
  mos_SET(e, receiver, rcvr);
  mos_SET(e, selector, sel);
  mos_SET(e, arguments, args);
  
  return e;
}

mos_value mos_exprSendV(mos_value rcvr, mos_value sel, mos_value args)
{
  return mos_exprSend_V(mos_undef, rcvr, sel, args);
}

mos_value mos_exprSend(mos_value rcvr, mos_value sel, int nargs, ...)
{
  mos_value e;
  va_list vap;
  va_start(vap, nargs);
  
  e = mos_exprSendV(rcvr, sel, mos_vector_makev(nargs, &vap));
  
  va_end(vap);
  
  return e;
}

mos_value mos_exprSend_(mos_value cntx, mos_value rcvr, mos_value sel, int nargs, ...)
{
  mos_value e;
  va_list vap;
  va_start(vap, nargs);
  
  e = mos_exprSend_V(cntx, rcvr, sel, mos_vector_makev(nargs, &vap));
  
  va_end(vap);
  
  return e;
}

mos_value mos_exprObject(mos_value slots, mos_value body)
{
  mos_value e = mos_expr(exprObject);

  mos_SET(e, slots, slots);
  mos_SET(e, body, body);
  
  return e;
}
mos_value mos_exprSlot(mos_value sel, mos_value val)
{
  mos_value e = mos_expr(exprSlot);

  sel = mos_send(sel, mos_s(asSelector));
  mos_SET(e, selector, sel);
  mos_SET(e, value, val);
  
  return e;
}
mos_value mos_exprReadOnlySlot(mos_value sel, mos_value val)
{
  mos_value e = mos_expr(exprReadOnlySlot);

  sel = mos_send(sel, mos_s(asSelector));
  mos_SET(e, selector, sel);
  mos_SET(e, value, val);
  
  return e;
}

mos_value mos_exprMethodSlot(mos_value sel, mos_value val)
{
  mos_value e = mos_expr(exprMethodSlot);

  sel = mos_send(sel, mos_s(asSelector));
  mos_SET(e, selector, sel);
  mos_SET(e, value, val);
  
  return e;
}

mos_value mos_exprMethod(mos_value args, mos_value locals, mos_value body)
{
  mos_value e = mos_expr(exprMethod);

  mos_vector_LOOP(args,ep)
    *ep = mos_send(*ep, mos_s(asSelector));
  mos_vector_LOOP_END
  mos_SET(e, arguments, args);
  
  mos_SET(e, locals, locals);
  
  mos_SET(e, body, body);
  
  /*
  mos_printf(mos_FILE(err), "\nexprMethod: %MP\n", mos_s(printExprOn_), e);
  */

  return e;
}

mos_value mos_exprBlock(mos_value args, mos_value locals, mos_value body)
{
  mos_value e = mos_expr(exprBlock);

  mos_vector_LOOP(args,ep)
    *ep = mos_send(*ep, mos_s(asSelector));
  mos_vector_LOOP_END
  mos_SET(e, arguments, args);
  
  mos_SET(e, locals, locals);
  
  mos_SET(e, body, body);

  /*
  mos_printf(mos_FILE(err), "\nexprBlock: %MP\n", mos_s(printExprOn_), e);
  */

  return e;
}

#undef mos_expr
#undef mos_SET
