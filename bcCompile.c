#ifndef __rcs_id__
#ifndef __rcs_id_mos_bcCompile_c__
#define __rcs_id_mos_bcCompile_c__
static const char __rcs_id_mos_bcCompile_c[] = "$Id: bcCompile.c,v 1.4 2000-05-10 03:59:18 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include "mos/expr.h"

#ifndef compilerDebug
static int compilerDebug = 0;
#endif

/************************************************************************/

mos_ANNOT("Doc: The compiler interface.")

mos_ANNOT("Doc: Returns a method for the execution of the expression.")
mos_METHOD(compiler,compileExpr_)
{
  mos_value e = mos_ARGV[0];
  mos_value cc;
  mos_value m;
  
  cc = mos_send(mos_RCVR, mos_s(compilerContext));
  cc = mos_send(cc, mos_s(newContext));
  
  mos_send(cc, mos_s(compileExpr_), e);
  
  m = mos_send(cc, mos_s(method));
  
#if 0
  {
    mos_value dc;

    mos_printf(mos_FILE(err), "\n===============================\n");

    mos_printf(mos_FILE(err), "compileExpr: ");
    mos_send(e, mos_s(printExprOn_), mos_FILE(err));

    mos_printf(mos_FILE(err), "\ndecompiled: ");
    dc = mos_send(m, mos_s(decompile));
    mos_send(dc, mos_s(printExprOn_), mos_FILE(err));

    mos_printf(mos_FILE(err), "\n===============================\n");
  }
#endif

  mos_return(m);
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(compiler)
mos_OBJECT_M(compiler,compileExpr_)
mos_OBJECT_SLOTS(compiler)
mos_OBJECT_END(system,compiler,mos_object,basicMeta)

mos_ANNOT_END

/************************************************************************/

#define CONSTANTS mos_send(CC, mos_s(constants))
#define CODESTREAM mos_send(CC, mos_s(bytecodes))
#define MEMOCODES mos_send(CC, mos_s(memocodes))

#define CC mos_RCVR

mos_ANNOT("Doc: The prototype compiler context.  This object controls the compilation of expressions.")

mos_METHOD(compilerContext,newConstants)
{
  mos_send(mos_RCVR, mos_s(constants_),
  	mos_vector_make_(2,
 		/* [0] is the lobby */
		mos_undef,
  		/* [1] is always mos_undef */
		mos_undef));
}
mos_METHOD_END

mos_METHOD(compilerContext,newBytecodes)
{
  mos_send(mos_RCVR, mos_s(bytecodes_), mos_string_make(0, 0));
}
mos_METHOD_END

mos_METHOD(compilerContext,newMemocodes)
{
  mos_send(mos_RCVR, mos_s(memocodes_), mos_vector_make(0, 0));
}
mos_METHOD_END

mos_METHOD(compilerContext,newContext)
{
  mos_value cc;
  
  cc = mos_send(mos_RCVR, mos_s(clone));
  mos_send(cc, mos_s(prevContext_), mos_RCVR);
  mos_send(cc, mos_s(inMethod_), cc);
  mos_send(cc, mos_s(asBlock_), mos_false);
  mos_send(cc, mos_s(hasBlockReturn_), mos_false);
  mos_send(cc, mos_s(hasBlockReturn_), mos_true);
  mos_send(cc, mos_s(newConstants));
  mos_send(cc, mos_s(newBytecodes));
  mos_send(cc, mos_s(newMemocodes));
  
  mos_return(cc);
}
mos_METHOD_END

mos_METHOD(compilerContext,newConstant_)
{
  mos_value cs = CONSTANTS;
  /* Add the constant value to the constant table */
  mos_value clength = mos_send(cs, mos_s(length));
  mos_send(cs, mos_s(append_), mos_ARGV[0]);
  
  if ( compilerDebug ) {
    mos_printf(mos_FILE(err), "lit: %P at %P\n", mos_ARGV[0], clength);
  }
  
  mos_return(clength);
}
mos_METHOD_END

mos_METHOD(compilerContext,addConstant_)
{
  mos_value cs = CONSTANTS;
  int index;
  
  /* Search the table for an already existing constant */
  index = 0;
  mos_vector_LOOP(cs, ep);
  {
    /* Skip lobby slot at index 0 */
    if ( index && (mos_EQ(*ep, mos_ARGV[0]) || mos_EQUAL(*ep, mos_ARGV[0])) ) {
      mos_return(mos_integer_make(index));
    }
    index ++;
  }
  mos_vector_LOOP_END;

  /* Add the constant value to the constant table */
  mos_return(mos_send(mos_RCVR, mos_s(newConstant_), mos_ARGV[0]));
}
mos_METHOD_END

mos_METHOD(compilerContext,newMemocode_)
{
  mos_value cs = MEMOCODES;
  /* Add the constant value to the constant table */
  mos_value clength = mos_send(cs, mos_s(length));
  mos_send(cs, mos_s(append_), mos_ARGV[0]);

  if ( compilerDebug ) {
    mos_printf(mos_FILE(err), "memo: %P at %P\n", mos_ARGV[0], clength);
  }
  
  mos_return(clength);
}
mos_METHOD_END

mos_METHOD(compilerContext,emit_)
{
  mos_value bc;
  mos_value cs = CODESTREAM;
  
  /* Look up the opcode for the opcode name */
  bc = mos_send(mos_send(mos_RCVR, mos_s(bytecodeByName)), mos_s(get_), mos_ARGV[0]);
  
  mos_send(cs, mos_s(append_), mos_send(bc, mos_s(asChar)));

  if ( compilerDebug ) {
    mos_printf(mos_FILE(err), "emit: %3ld: %W(%W)\n", (long) mos_string_L(cs) - 1, mos_ARGV[0], bc);
  }
}
mos_METHOD_END

mos_METHOD(compilerContext,emit_Arg_)
{
  unsigned long arg;
  mos_value codestream = CODESTREAM;
  
  mos_send(mos_RCVR, mos_s(emit_), mos_ARGV[0]);
  arg = mos_INT(mos_send(mos_ARGV[1], mos_s(asInteger)));
  
  if ( compilerDebug ) {
    mos_printf(mos_FILE(err), "emit: Arg: %W\n", mos_ARGV[1]);
  }
  
  mos_send(codestream, mos_s(append_), mos_char_make(arg & 0xff));
  mos_send(codestream, mos_s(append_), mos_char_make((arg >> 8) & 0xff));
  mos_send(codestream, mos_s(append_), mos_char_make((arg >> 16) & 0xff));
  mos_send(codestream, mos_s(append_), mos_char_make((arg >> 24) & 0xff));
}
mos_METHOD_END

mos_METHOD(compilerContext,complete)
{
  mos_send(mos_RCVR, mos_s(emit_), mos_s(rtn));
}
mos_METHOD_END

mos_METHOD(compilerContext,compileExpr_)
{
  mos_send(mos_ARGV[0], mos_s(compile_), mos_RCVR);
  
  mos_send(mos_RCVR, mos_s(complete));
}
mos_METHOD_END

mos_METHOD(compilerContext,compileExprs_AndReturn_WithArguments_WithLocals_)
{
  int needs_pop = 0;

  /*
  ** Remember the arguments and locals.
  */
  mos_send(mos_RCVR, mos_s(arguments_), mos_ARGV[2]);
  mos_send(mos_RCVR, mos_s(locals_), mos_ARGV[3]);
  
  /*
  ** Save stack space for locals
  ** and generate initializations.
  */
  if ( mos_vector_L(mos_ARGV[3]) ) {
    mos_send(mos_RCVR, mos_s(emit_Arg_), mos_s(locs_), mos_send(mos_ARGV[3], mos_s(length)));
    
    mos_vector_LOOP(mos_ARGV[3], ep);
    {
      mos_send(*ep, mos_s(compile_), mos_RCVR);
      /* discard unused result */
      mos_send(mos_RCVR, mos_s(emit_), mos_s(pop));
    }
    mos_vector_LOOP_END;
  }
  
  /*
  ** Compile body stmts.
  */ 
  mos_vector_LOOP(mos_ARGV[0], ep);
  {
    if ( needs_pop ) {
      /* discard unused result */
      mos_send(mos_RCVR, mos_s(emit_), mos_s(pop));
    }
    needs_pop = 1;
    mos_send(*ep, mos_s(compile_), mos_RCVR);
  }
  mos_vector_LOOP_END;

  /*
  ** Compile the return expression
  */
  if ( mos_NE(mos_ARGV[1], mos_undef) ) {
    if ( needs_pop ) {
      mos_send(mos_RCVR, mos_s(emit_), mos_s(pop));
    }
    mos_send(mos_ARGV[1], mos_s(compile_), mos_RCVR);
  } else {
    /* We didn't compile a return */
    /* If we didn't create a last value, make it "self" */
    if ( ! needs_pop ) {
      mos_send(mos_exprSelf(), mos_s(compile_), mos_RCVR);
    }
  }
  
  /*
  ** Finish the context.
  */
  mos_send(mos_RCVR, mos_s(complete));
}
mos_METHOD_END

mos_METHOD(compilerContext,method)
{
  mos_value m;
  
  m = mos_send(mos_RCVR, mos_s(methodProto));
  m = mos_send(m, mos_s(clone));
  mos_send(m, mos_s(bytecodes_), CODESTREAM);
  mos_send(m, mos_s(constants_), CONSTANTS);
  mos_send(m, mos_s(memocodes_), MEMOCODES);
  mos_send(m, mos_s(memovals_), mos_send(mos_vector_make(0,0), mos_s(length_), mos_send(MEMOCODES, mos_s(length))));
  mos_send(m, mos_s(arguments_), mos_send(mos_RCVR, mos_s(arguments)));
  mos_send(m, mos_s(locals_), mos_send(mos_RCVR, mos_s(locals)));
  mos_send(m, mos_s(hasBlockReturn_), mos_send(mos_RCVR, mos_s(hasBlockReturn)));
  
  mos_return(m);
}
mos_METHOD_END


mos_METHOD(compilerContext,compileLiteral_)
{
  /* Emit a LIT that indexes into the methods's constant vector */
  mos_send(mos_RCVR, mos_s(emit_Arg_), mos_s(lit_), mos_send(mos_RCVR, mos_s(addConstant_), mos_ARGV[0]));
}
mos_METHOD_END

mos_METHOD(compilerContext,compileMemo_)
{
  mos_value bcprev, bcvec, bcpos;
  mos_value value = mos_ARGV[0];
  
  if ( mos_EQ(value,mos_undef) ) {
    mos_send(mos_RCVR, mos_s(compileLiteral_), value);
  } else {
    /* Compile the expression into a different bytecode vector */
    
    /* Remember old bcs */
    bcprev = mos_send(mos_RCVR, mos_s(bytecodes));
    
    /* Create new bcs */
    mos_send(mos_RCVR, mos_s(newBytecodes));
    
    /* Compile into new bcs */
    mos_send(value, mos_s(compile_), mos_RCVR);
    mos_send(mos_RCVR, mos_s(complete));
    
    /* Get new bcs */
    bcvec = mos_send(mos_RCVR, mos_s(bytecodes));
    
    /* Restore old bcs */
    mos_send(mos_RCVR, mos_s(bytecodes_), bcprev);
    
    /* Emit a MEMO that indexes into the memo vector */
    bcpos = mos_send(mos_RCVR, mos_s(newMemocode_), bcvec);
    mos_send(mos_RCVR, mos_s(emit_Arg_), mos_s(memo_), bcpos);
  }
}
mos_METHOD_END

#undef CC


mos_OBJECT(compilerContext)
mos_OBJECT_M(compilerContext,newConstants)
mos_OBJECT_M(compilerContext,newBytecodes)
mos_OBJECT_M(compilerContext,newMemocodes)
mos_OBJECT_M(compilerContext,newContext)
mos_OBJECT_M(compilerContext,newConstant_)
mos_OBJECT_M(compilerContext,addConstant_)
mos_OBJECT_M(compilerContext,newMemocode_)
mos_OBJECT_M(compilerContext,emit_)
mos_OBJECT_M(compilerContext,emit_Arg_)
mos_OBJECT_M(compilerContext,complete)
mos_OBJECT_M(compilerContext,compileExpr_)
mos_OBJECT_M(compilerContext,compileExprs_AndReturn_WithArguments_WithLocals_)
mos_OBJECT_M(compilerContext,method)
mos_OBJECT_M(compilerContext,compileLiteral_)
mos_OBJECT_M(compilerContext,compileMemo_)
mos_ANNOT("The map from bytecode name to bytecode value.")
mos_OBJECT_A(bytecodeByName,0)
mos_ANNOT_END
mos_ANNOT("The method prototype.")
mos_OBJECT_A(methodProto,1)
mos_ANNOT_END
mos_ANNOT("The vector of argument identifiers.")
mos_OBJECT_A(arguments,2)
mos_ANNOT_END
mos_ANNOT("The vector of local identifiers.")
mos_OBJECT_A(locals,3)
mos_ANNOT_END
mos_ANNOT("The vector of constant values.")
mos_OBJECT_A(constants,4)
mos_ANNOT_END
mos_ANNOT("The string containing the current bytecodes.")
mos_OBJECT_A(bytecodes,5)
mos_ANNOT_END
mos_ANNOT("The inclosing context.")
mos_OBJECT_A(prevContext,6)
mos_ANNOT_END
mos_ANNOT("Is this context compiling a block method?")
mos_OBJECT_A(asBlock,7)
mos_ANNOT_END
mos_ANNOT("The context of the method we are compiling this method into")
mos_OBJECT_A(inMethod,8)
mos_ANNOT_END
mos_ANNOT("This context has a block that returns")
mos_OBJECT_A(hasBlockReturn,9)
mos_ANNOT_END
mos_ANNOT("A vector of the current memoed expression bytecodes.")
mos_OBJECT_A(memocodes,10)
mos_ANNOT_END
mos_OBJECT_SLOTS(compilerContext)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_o(bcMethod))
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_false)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_false)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(compiler,compilerContext,mos_object,basicMeta)

mos_ANNOT_END

/************************************************************************/

mos_ANNOT("Doc: Contains the prototype expression objects for the compiler.")
mos_OBJECT(expr)
mos_OBJECT_SLOTS(expr)
mos_OBJECT_END(compiler,expr,mos_object,basicMeta)
mos_ANNOT_END

/************************************************************************/

#define CC mos_ARGV[0]
#define EXPR mos_RCVR

#define VAL mos_send(EXPR,mos_s(value))
#define SEL mos_send(EXPR,mos_s(selector))
#define ARGS mos_send(EXPR,mos_s(arguments))
#define LOCALS mos_send(EXPR,mos_s(locals))
#define BODY mos_send(EXPR,mos_s(body))
#define SLOTS mos_send(EXPR,mos_s(slots))
#define RCVR mos_send(EXPR,mos_s(receiver))
#define CNTX mos_send(EXPR,mos_s(context))
#define INDEX mos_send(EXPR,mos_s(index))

/************************************************************************/

mos_METHOD(exprConstant,compile_)
{
  mos_send(CC, mos_s(compileLiteral_), VAL);
}
mos_METHOD_END

/************************************************************************/

mos_METHOD(exprMemo,compile_)
{
  mos_send(CC, mos_s(compileMemo_), VAL);
}
mos_METHOD_END

/************************************************************************/

mos_METHOD(exprSend,compile_)
{
  mos_value args = ARGS;
  mos_value rcvr = RCVR;
  mos_value sel = SEL;
  mos_value sel_ = mos_send(sel, mos_s(value));
  mos_value cntx = CNTX;
  mos_value op;
  int inBlock = mos_NE(mos_send(CC, mos_s(asBlock)), mos_false);
  
  /* Compile the args in reverse order */
  mos_vector_LOOP_REVERSE(args,ep);
  {
    mos_send(*ep, mos_s(compile_), CC);
  }
  mos_vector_LOOP_END;
  
  if ( mos_EQ(rcvr, mos_undef) ) {
    
    /*
    ** Handle special root messages.
    */

    /* _msg -> (current message) */
    if ( mos_EQ(sel_,mos_s(_msg)) ) {
      mos_send(CC, mos_s(emit_), mos_s(msg));
      mos_return(mos_RCVR);
    } else
    
    /* (_) -> lobby */
    if ( mos_EQ(sel_,mos_s(_)) ) {
      mos_send(CC, mos_s(emit_), mos_s(lobby));
      mos_return(mos_RCVR);
    } else
    
    /* (_:<expr>)) -> rtn */
    if ( mos_EQ(sel_,mos_s(__RTN__)) ) {
      /*
      ** If we are returning from a block, make sure we create
      ** a return catch for the method's message.
      */
      if ( inBlock ) {
        /*
	** Mark the inclosing method's context that
	** it generates a block with a return,
        ** so the compiler can generate code to create a block return catch in
	** the method's preamble.
	*/
	mos_send(mos_send(CC, mos_s(inMethod)), mos_s(hasBlockReturn_), mos_true);
	mos_send(CC, mos_s(emit_), mos_s(rtnBlk));
      } else {
	mos_send(CC, mos_s(emit_), mos_s(rtn));
      }
      mos_return(mos_RCVR);
    } else
    
    /* (_constants) -> <constants-vector> */
    if ( mos_EQ(sel_,mos_s(_constants)) ) {
      mos_send(CC, mos_s(emit_), mos_s(const));
      mos_return(mos_RCVR);
    } else
    
    /* (_restart) -> <restart> */
    if ( mos_EQ(sel_,mos_s(_restart)) ) {
      mos_send(CC, mos_s(emit_), mos_s(restart));
      mos_return(mos_RCVR);
    } else

    /* (__object) -> new empty object */
    if ( mos_EQ(sel_,mos_s(__object)) ) {
      mos_send(CC, mos_s(emit_), mos_s(object));
      mos_return(mos_RCVR);
    } else

    /* (__block) -> new empty block */
    if ( mos_EQ(sel_,mos_s(__block)) ) {
      mos_send(CC, mos_s(emit_), mos_s(block));
      mos_return(mos_RCVR);
    } else

    /* (__method) -> new method */
    if ( mos_EQ(sel_,mos_s(__method)) ) {
      mos_send(CC, mos_s(emit_), mos_s(method));
      mos_return(mos_RCVR);
    } else

    /*
    ** Scoped messages.
    */
    /* (self) -> rcvr */
    if ( mos_EQ(sel_,mos_s(self)) ) {
      /*
      ** 'self' in a block context is really 'self' in the inclosing method's
      ** active message object.
      */
      mos_value msg = inBlock ? mos_exprMethodContext() : mos_exprMsg();
      mos_send(msg, mos_s(compile_), CC);
      mos_send(CC, mos_s(emit_), mos_s(rcvr));
      mos_return(mos_RCVR);
    } else
    
    /* (self:<expr>)) -> rcvrSet. */
    if ( mos_EQ(sel_,mos_s(self_)) ) {
      mos_value msg = inBlock ? mos_exprMethodContext() : mos_exprMsg();
      mos_send(msg, mos_s(compile_), CC);
      mos_send(CC, mos_s(emit_), mos_s(rcvrSet));
      mos_return(mos_RCVR);
    } else
    
    /* Might be a local or argument access message in the block context stack. */
    {
      mos_value v;
      int i;
      mos_value getter;
      int isGetter;
      mos_value cc;
      mos_value msgRoot;
      int keepGoingUp = 0;
      
      getter = mos_send(sel_, mos_s(getter));
      if ( mos_EQ(getter,mos_undef) ) {
	/* Must be a getter. */
	getter = sel_;
	isGetter = 1;
      } else {
	isGetter = 0;
      }
      
      /* Begin search from current context. */
      cc = CC;
      msgRoot = mos_exprMsg();
      
      do { 
	/* Look in locals */
	v = mos_send(cc, mos_s(locals));
	i = 1; /* Note: arg indexes from 1. */
	mos_vector_LOOP(v,ep)
	  /* Locals are expr*Slot objects. */
	  mos_value s = mos_send(*ep, mos_s(selector));
	  if ( mos_EQ(s, getter) ) {
	    mos_send(msgRoot, mos_s(compile_), CC);
	    mos_send(CC, mos_s(emit_Arg_),
		  (isGetter ? mos_s(loc_) : mos_s(locSet_)),
		  mos_integer_make(i));
	    mos_return(mos_RCVR);
	  }
	  i ++;
	mos_vector_LOOP_END
  
	/* Look in arguments. */
	v = mos_send(cc, mos_s(arguments));
	i = 0;
	mos_vector_LOOP(v,ep)
	  if ( mos_EQ(*ep, getter) ) {
	    mos_send(msgRoot, mos_s(compile_), CC);
	    mos_send(CC, mos_s(emit_Arg_),
		  (isGetter ? mos_s(arg_) : mos_s(argSet_)),
		  mos_integer_make(i));
	    mos_return(mos_RCVR);
	  }
	  i ++;
	mos_vector_LOOP_END
      
	/* Try previous frame. */
	if ( (keepGoingUp = mos_NE(mos_send(cc, mos_s(asBlock)), mos_false)) ) {
	  /* Go up for the cc. */
	  cc = mos_send(cc, mos_s(prevContext));
	  
	  /* Go up for the _lexicalContext. */
	  msgRoot = mos_exprSend(msgRoot, (mos_s(_lexicalContext)), 0);
	}
      } while ( keepGoingUp );
    }
    
    /* Must be a default send to self. */
    mos_send(mos_exprSelf(), mos_s(compile_), CC);
  } else {
    /* rcvr is not root and is specified; compile the rcvr. */
    mos_send(rcvr, mos_s(compile_), CC);
  }

  /*
  ** Handle special inline methods.
  */
  /* <_msg> _rcvr -> (current receiver) */
  if ( mos_EQ(sel_,mos_s(_rcvr)) ) {
    mos_send(CC, mos_s(emit_), mos_s(rcvr));
    mos_return(mos_RCVR);
  } else
  
  /* <block> _lexicalContext -> (a block's _lexicalContext) */
  if ( mos_EQ(sel_,mos_s(_lexicalContext)) ) {
    mos_send(CC, mos_s(emit_), mos_s(_lexicalContext));
    mos_return(mos_RCVR);
  } else
  if ( mos_EQ(sel_,mos_s(_lexicalContext_)) ) {
    mos_send(CC, mos_s(emit_), mos_s(_lexicalContext_));
    mos_return(mos_RCVR);
  } else

  /* <block> _methodContext -> (a block's _methodContext) */
  if ( mos_EQ(sel_,mos_s(_methodContext)) ) {
    mos_send(CC, mos_s(emit_), mos_s(_methodContext));
    mos_return(mos_RCVR);
  } else
  if ( mos_EQ(sel_,mos_s(_methodContext_)) ) {
    mos_send(CC, mos_s(emit_), mos_s(_methodContext_));
    mos_return(mos_RCVR);
  } else
  
  {
    /* The reciever is on the top of the stack */
     
    /*
    ** If the rcvr and the cntx are the same expression
    ** simply use the undirected send.
    */
    if ( mos_EQ(rcvr,cntx) || mos_EQ(cntx,mos_undef) ) {
      op = mos_s(send_);
    } else {
      /* Push the send context on the stack */
      mos_send(cntx, mos_s(compile_), CC); 
      op = mos_s(sendDir_);
    }
    
    /* Compile the selector */
    mos_send(sel, mos_s(compile_), CC);
    
    /* Compile the number of items to pop */
    
    /* Emit the SEND ins */
    mos_send(CC, mos_s(emit_Arg_), op, mos_send(args, mos_s(length)));
  }
}
mos_METHOD_END

/************************************************************************/

mos_METHOD(exprObject,compile_)
{
  static mos_value cnstr;
  mos_value e;
  
  /*
  ** Create an object constructor expression
  ** (_ system protos empty clone) -> (__object)
  */
  if ( ! mos_INITIALIZED(cnstr) ) {
    cnstr = mos_exprSend(mos_undef, mos_s(__object), 0);
  }
  
  /* Apply the slot constructors to the object constructor */
  e = cnstr;
  mos_vector_LOOP(SLOTS,ep)
    e = mos_send(*ep, mos_s(compile_WithConstructor_), CC, e);
  mos_vector_LOOP_END
  
  /* Memoize the object construction value */
  e = mos_exprMemo(e);
  
  if ( compilerDebug ) {
    mos_printf(mos_FILE(err), "Compile: %MP -> %MP\n", mos_s(printExprOn_), mos_RCVR, mos_s(printExprOn_), e);
  }
  
  /* Compile the constructor expression */
  mos_send(e, mos_s(compile_), CC);
}
mos_METHOD_END

/************************************************************************/

mos_METHOD(exprSlot,compile_WithConstructor_)
{
  mos_value e;
  mos_value v = VAL;

  e = mos_exprSend(mos_ARGV[1], mos_s(_addSlot_Value_), 2, SEL, v);
  
  mos_return(e);
}
mos_METHOD_END

mos_ANNOT("Doc: Compile as local initializer")
mos_METHOD(exprSlot,compile_)
{
  mos_value v = VAL;

  mos_send(
  	mos_exprSend(mos_undef, 
		mos_send(SEL, mos_s(setter)),
		1, v),
	mos_s(compile_), CC);
}
mos_METHOD_END
mos_ANNOT_END

/************************************************************************/

mos_METHOD(exprReadOnlySlot,compile_WithConstructor_)
{
  mos_value e;
  
  e = mos_exprSend(mos_ARGV[1], mos_s(_addReadOnlySlot_Value_), 2, SEL, VAL);
  
  mos_return(e);
}
mos_METHOD_END

mos_ANNOT("Doc: Compile as local initializer.")
mos_METHOD(exprReadOnlySlot,compile_)
{
  mos_value v = VAL;

  mos_send(
  	mos_exprSend(mos_undef,
		mos_send(SEL, mos_s(setter)),
		1, mos_exprMemo(v)),
	mos_s(compile_), CC);
}
mos_METHOD_END
mos_ANNOT_END

/************************************************************************/

mos_METHOD(exprMethodSlot,compile_WithConstructor_)
{
  mos_value e;
  mos_value v = mos_send(VAL, mos_s(compile_), CC);
  
  e = mos_exprSend(mos_ARGV[1], mos_s(_addMethodSlot_Value_), 2, SEL, v);
  
  mos_return(e);
}
mos_METHOD_END

/************************************************************************/

mos_METHOD(exprMethod,compile_)
{
  static mos_value cnstr;
  mos_value cc, e;
  mos_value _asBlock = mos_send(mos_RCVR, mos_s(asBlock));
  mos_value args, locals;
  int asBlock = mos_NE(_asBlock,mos_false);
  
  /*
  ** Create a new compiler context, but use the same constant vector.
  */
  cc = mos_send(CC, mos_s(newContext));
  mos_send(cc, mos_s(constants_), mos_send(CC, mos_s(constants)));
  
  /*
  ** If we are a block let our new context know.
  */
  mos_send(cc, mos_s(asBlock_), _asBlock);
  
  /*
  ** If we are a block, make our inMethod the inclosing methods block return target.
  ** Otherwise, make the new context the target for block returns.
 remember our previous method context.
  */
  if ( asBlock ) {
    mos_send(cc, mos_s(inMethod_), mos_send(CC, mos_s(inMethod)));
  }
  
  /*
  ** Compile the body of the method in the new context.
  ** Methods return the rcvr if a return stmt is not specified.
  ** Block methods return the last expression in the block's body.
  ** Block methods return from the lexical method if a return stmt is
  ** specified.
  */
  args = ARGS;
  locals = LOCALS;
  mos_send(cc, mos_s(compileExprs_AndReturn_WithArguments_WithLocals_),
  	BODY,
	(asBlock ?
		mos_undef :
		mos_exprReturnSelf()),
	args,
	locals);
   
  /*
  ** Create an object constructor expression
  ** (_ system protos methods bcMethod clone) -> (__method)
  */
  if ( ! mos_INITIALIZED(cnstr) ) {
    cnstr = mos_exprSend(mos_undef, mos_s(__method), 0);
  }
  e = cnstr;
  
  /*
  ** (((((_ system protos methods bcMethod clone) bytecodes: <bc>) constants: <constants>) arguments: <args>) hasBlockReturn: <hasBlockReturn>)
  */ 
  e = mos_exprSend(e, (mos_s(bytecodes_)), 1,
    mos_exprConstant(mos_send(cc, mos_s(bytecodes))));

  e = mos_exprSend(e, (mos_s(constants_)), 1,
    mos_exprSend(mos_undef, (mos_s(_constants)), 0));

  if ( mos_vector_L(args) ) {
    e = mos_exprSend(e, (mos_s(arguments_)), 1,
      mos_exprVector(args));
  }

  if ( mos_vector_L(locals) ) {
    e = mos_exprSend(e, (mos_s(locals_)), 1,
      mos_exprVector(locals));
  }

  /* ((<method> memocodes: <memocodes-vector>) memovals: ((vector clone) length: (<memocodes-vector> length))) */

  /* Attach memocodes to the method, and fill its memovals */
  args = mos_send(cc, mos_s(memocodes));
  if ( mos_vector_L(args) ) {
    e = mos_exprSend(e, mos_s(memocodes_), 1,
      mos_exprVector(args));

    e = mos_exprSend(e, mos_s(memovals_), 1,
      mos_exprVectorFill(mos_send(args, mos_s(length))));
  }
  
  /* Tell the method if it contains a return with a block */
  if ( mos_NE(mos_send(cc, mos_s(hasBlockReturn)), mos_false) ) {
    e = mos_exprSend(e, mos_s(hasBlockReturn_), 1, mos_true);
  }
    
  mos_return(e);
}
mos_METHOD_END


/************************************************************************/

mos_METHOD(exprBlock,compile_)
{
  mos_value e, valSel, meth;
  
  /*
  ** Create and compile a method constructor from the block body.
  ** Return the last expression by default;
  */
  meth = mos_exprMethod(ARGS, LOCALS, BODY);
  mos_send(meth, mos_s(asBlock_), mos_true);
  meth = mos_send(meth, mos_s(compile_), CC);

  /*
  ** Create an block constructor expression
  ** (_ system protos block clone) -> __block
  */
  {
    static mos_value cnstr;
  
    if ( ! mos_INITIALIZED(cnstr) ) {
      cnstr = mos_exprSend(mos_undef, mos_s(__block), 0);
    }
    e = cnstr;
  }
  
  
  /*
  ** Create a memoized block object containing the value method.
  ** <block memo> = '((_ system protos block clone) _addMethodSlot: <valueSelector> Value:<method>)
  */
  valSel = mos_send(mos_o(selector), mos_s(valueSelectorForNargs_), mos_send(ARGS, mos_s(length)));
  
  e = mos_exprSend(e, (mos_s(_addMethodSlot_Value_)), 2,
    valSel, meth);
  e = mos_exprMemo(e);
  
  /*
  ** Clone the memoized block object.
  ** <block instance> = (<block memo> clone)
  */
  e = mos_exprSend(e, (mos_s(clone)), 0);
  
  /*
  ** Attach the block object to the current msg 
  ** (<block instance> _lexicalContext: <msg>)
  ** This allows the block object access to the
  ** message that created it (and it's return catch).
  */
  e = mos_exprSend(e, (mos_s(_lexicalContext_)), 1, mos_exprMsg());
    
  /*
  ** If this block is within another block, use the parent block's _methodContext
  ** instead of the current message (which is the block's value* message!).
  */
  {
    mos_value mC;
    
    mC = mos_NE(mos_send(CC, mos_s(asBlock)), mos_false) ? 
      		/* (<block> _methodContext: _rcvr _methodContext) */
     		mos_exprMethodContext() :
      		/* (<block> _methodContext: _msg) */
		mos_exprMsg();

    e = mos_exprSend(e, (mos_s(_methodContext_)), 1, mC);
  }
  
  if ( compilerDebug ) {
    mos_printf(mos_FILE(err), "compile: block expr: %MP\n", mos_s(printExprOn_), e);
  }
  
  /*
  ** Compile the expression.
  */
  mos_send(e, mos_s(compile_), CC);
}
mos_METHOD_END

/************************************************************************/

#undef CC
#undef VAL
#undef SEL
#undef ARGS
#undef LOCALS
#undef SLOTS
#undef BODY
#undef RCVR
#undef CNTX
#undef INDEX

#include "mos/bc.h"

mos_INIT(bcCompile,21)
{
  /* Create a map from opcode name (selector) to opcode value (integer) */
  mos_value bcs;
  
  bcs = mos_send(mos_o(map), mos_s(clone));

#define mos_BC(NAME,NARGS) mos_send(bcs, mos_s(set_Value_), mos_s(NAME), mos_integer_make(mos_bc_##NAME));
#include "mos/bc.def"

  /* Store it in the compilerContext proto */
  mos_send(mos_o(compilerContext), mos_s(bytecodeByName_), bcs);

  /* Store empty argument and local lists */
  mos_send(mos_o(compilerContext), mos_s(arguments_), mos_vector_make(0, 0));
  mos_send(mos_o(compilerContext), mos_s(locals_), mos_vector_make(0, 0));
  
  return 0;
}

