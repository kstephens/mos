#ifndef __rcs_id__
#ifndef __rcs_id_mos_bcMethod_c__
#define __rcs_id_mos_bcMethod_c__
static const char __rcs_id_mos_bcMethod_c[] = "$Id: bcMethod.c,v 1.5 2001-09-15 21:59:10 stephens Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include "mos/basicMeta.h"
#include "mos/bc.h"
#include "mos/block.h"


/******************************************************************/
/* Debugging support */


#ifndef _mos_bcExecDebug
int _mos_bcExecDebug = 0;
#endif


#ifndef _mos_bcSendDebug
int _mos_bcSendDebug = 0;
#endif


char _mos_bcExecCodeDebug[] = {
#define mos_BC(NAME,NARGS) 0,
#include "mos/bc.def"
  0
};


/******************************************************************/
/* Enumerate bcMethod slot indexes */


enum {
BYTECODES = 0,
CONSTANTS,
HASBLOCKRTN,
ARGUMENTS,
LOCALS,
MEMOCODES,
MEMOVALS,
};


/******************************************************************/
/* The system value stack */


/* NOT THREAD SAFE */
#ifndef vstack_LEN
#define vstack_LEN 1024
#endif
static mos_value _mos_vstack[vstack_LEN];

mos_value *_mos_sp = _mos_vstack + (sizeof(_mos_vstack)/sizeof(_mos_vstack[0])-1);
mos_value *_mos_sp_top = _mos_vstack;


/******************************************************************/


#define sp _mos_sp


static __inline__
mos_value _mos_bcMethod_run(mos_value mos_MSG, mos_value BCS, mos_value *fp)
{
  const unsigned char *pc;
  mos_value *constants = mos_vector_V(mos_SLOTS(mos_METH)[CONSTANTS]);
  
  _mos_bcExecCodeDebug[mos_bc_rtn] ++;
  _mos_bcExecCodeDebug[mos_bc_rtnBlk] ++;
  _mos_bcExecCodeDebug[mos_bc_memo_] ++;
  _mos_bcExecCodeDebug[mos_bc_memoval_] ++;
  _mos_bcExecCodeDebug[mos_bc_rcvr] ++;

  /* _restart is the sub-primitive for looping. */
  restart:
  pc = mos_string_V(BCS);

#define TOP *sp
#define PUSH(X) *(-- sp) = (X)
#define POP() *(sp ++)
#define DUP() (sp[-1] = *sp, sp --)
#define PUSHN(N) (sp -= (N))
#define POPN(N) (sp += (N))
#define ARG() (pc += 4, ((unsigned long) pc[-4] | ((unsigned long) pc[-3] << 8) | ((unsigned long) pc[-2] << 16) | ((unsigned long) pc[-1] << 24)))

#define PRINTF(X) fprintf X

  again:
  if ( _mos_bcExecDebug > 1 || (_mos_bcExecDebug == 1 && _mos_bcExecCodeDebug[*pc]) ) {
    PRINTF((stderr, "exec_bc: %8p: %3d: %3ld: %s", (void*) mos_string_V(BCS), fp - sp, (long) ((char*) pc - mos_string_V(BCS)), mos_bc_NAME(*pc)));
    if ( mos_bc_NARGS(*pc) ) {
      const void *pcs = pc;
      int i = mos_bc_NARGS(*(pc ++));
      while ( i -- ) {
        PRINTF((stderr, " %lx", (unsigned long) ARG()));
      }
      pc = pcs;
    }
    PRINTF((stderr, "\n"));
  }

  switch ( (mos_bc) *(pc ++) ) {
  case mos_bc_nop:
    goto again;
    
    /* Restart the method. */
  case mos_bc_restart:
    sp = fp;
    goto restart;
    
    /* Object creation */
  case mos_bc_object: /* | <object> */
    PUSH(mos_clone(mos_o(empty)));
    goto again;

    /* Block creation */
  case mos_bc_block: /* | <block> */
    PUSH(mos_clone(mos_o(block)));
    goto again;

    /* Method creation */
  case mos_bc_method: /* | <bcMethod> */
    PUSH(mos_clone(mos_o(bcMethod)));
    goto again;

    /* stack motion */
  case mos_bc_dup: /* <X> | <X> <X> */
    DUP();
    goto again;
      
  case mos_bc_pop: /* <X> | */
    (void) POP();
    goto again;
      
    /* rtn */
  case mos_bc_rtn: /* <X> | */
    /* sp = fp; */
    return POP();
    
  case mos_bc_rtnBlk: /* <X> | */
    {
      /*
      ** Get the block's (mos_RCVR) methodContext (a message)
      ** Set the message's return value.
      ** Throw to the message's return catch. (see
      */
      mos_value rtnval = POP();
      mos_value blk = mos_RCVR;
      {
	mos_value mos_MSG = mos_SLOTS(blk)[mos_block___methodContext];
	mos_RTNVAL = rtnval;
	return mos_THROW(mos_RTNCATCH, rtnval);
      }
    }
      
    /* const */
  case mos_bc_const: /* | _constants */
    PUSH(mos_SLOTS(mos_METH)[CONSTANTS]);
    goto again;

    /* lobby */
  case mos_bc_lobby: /* | _ */
    PUSH(constants[0]);
    goto again;

    /*****************************************************************/
    
    /* msg */
  case mos_bc_msg: /* | msg */
    PUSH(mos_MSG);
    goto again;

    /* rcvr */
#define MSG mos_SLOTST(rtnval,mos_message_slots)

  case mos_bc_rcvrSet: /* msg value | value */
    {
      mos_value rtnval = POP();
      MSG->_receiver = TOP;
    }
    break;
  case mos_bc_rcvr: /* msg | msg->receiver */
    {
      mos_value rtnval = TOP;
      TOP = MSG->_receiver;
    }
    goto again;
    
    /* args */
  case mos_bc_arg_: /* msg | <arg(i)> */
    {
      mos_value rtnval = TOP;
      TOP = mos_vector_V(MSG->_arguments)[ARG()];
    }
    goto again;

  case mos_bc_argSet_: /* msg value | rcvr */
    {
      mos_value rtnval = POP();
      mos_vector_V(MSG->_arguments)[ARG()] = TOP;
      TOP = mos_RCVR;
    }
    goto again;
    
    /* Locals */
  case mos_bc_locs_:
    PUSHN(ARG());
    goto again;

  case mos_bc_loc_: /* | <loc:arg> */
    PUSH(fp[-ARG()]);
    goto again;
      
  case mos_bc_locSet_: /* value | rcvr */
    fp[-ARG()] = TOP;
    TOP = mos_RCVR;
    goto again;
    
#undef MSG
    
/*****************************************************************/

    /* Block manipulation */
  case mos_bc__lexicalContext: /* block | (block _lexicalContext) */
    TOP = mos_SLOTS(TOP)[mos_block___lexicalContext];
    goto again;
  case mos_bc__lexicalContext_: /* block cntx | block */
    {
      mos_value rtnval = POP();
      mos_SLOTS(rtnval)[mos_block___lexicalContext] = TOP;
      TOP = rtnval;
    }
    goto again;
    
  case mos_bc__methodContext: /* block | (block _methodContext) */
    TOP = mos_SLOTS(TOP)[mos_block___methodContext];
    goto again;
  case mos_bc__methodContext_: /* block cntx | block */
    {
      mos_value rtnval = POP(); 
      mos_SLOTS(rtnval)[mos_block___methodContext] = TOP;
      TOP = rtnval;
    }
    goto again;
    
    /* literals */
  case mos_bc_lit_: /* | <constant:ARG()> */
    PUSH(constants[ARG()]);
    goto again;
    
    /* memo */
  case mos_bc_memo_: /* | <memoval[ARG()]> */
    {
      unsigned int i = ARG();
#define _MEMOVALS mos_SLOTS(mos_METH)[MEMOVALS]
#define _MEMOCODES mos_SLOTS(mos_METH)[MEMOCODES]

#define memovals mos_vector_V(_MEMOVALS)
#define memocodes mos_vector_V(_MEMOCODES)
      
#if 0
      {
	mos_value str = mos_printf(mos_undef, "BCM: MEMOCODE[%d]: %P: MEMOVALS: %P\n", i, _MEMOCODES, _MEMOVALS);
	mos_printf(mos_FILE(err), "%W\n", str);
      }
#endif
      /* LOCK */
      PUSH(memovals[i] = _mos_bcMethod_run(mos_MSG, memocodes[i], fp));
      pc[-5] = mos_bc_memoval_; /* rewrite */
      /* UNLOCK */
    }
    goto again;

  case mos_bc_memoval_:
    PUSH(memovals[ARG()]);
    goto again;

#undef memovals
#undef memocodes
    
    /* send */
  case mos_bc_send_: /* send(nargs) <sel> <rcvr> <args> ... | <result> */
    {
      register int nargs = ARG();
      register mos_value sel, rcvr;
      sel = POP();
      rcvr = POP();

#define PRINT_MSG() if ( _mos_bcSendDebug ) { \
	  int i; \
          mos_value str; \
	  str = mos_printf(mos_undef, "BCM: SEND: %-P %-P", rcvr, sel); \
          for ( i = 0; i < nargs; i ++ ) { \
            mos_printf(str, " %-P", sp[i]); \
	  } \
	  mos_printf(mos_FILE(err), "%W\n", str); \
        }

      PRINT_MSG();

      rcvr = _mos_sendv_(mos_MSG, mos_undef, rcvr, sel, nargs, sp);
      POPN(nargs);
      PUSH(rcvr);
    }
    goto again;
    
  case mos_bc_sendDir_: /* send(nargs) <sel> <cntx> <rcvr> <args> ... | <result> */
    {
      register int nargs = ARG();
      register mos_value sel, cntx, rcvr;
      sel = POP();
      cntx = POP();
      rcvr = POP();

      PRINT_MSG();

      cntx = _mos_sendv__(mos_MSG, mos_undef, cntx, rcvr, sel, nargs, sp);

      POPN(nargs); 
      PUSH(cntx);
    }
    goto again;
    
  default:
    mos_error(mos_s(badBytecode), "0x%x", pc[-1]);
    break;
  }
#undef CASE

#undef TOP
#undef PUSH
#undef POP
#undef POPN
#undef ARG

#undef PRINTF 
  
  return mos_undef;
}


mos_METHOD(bcMethod,__applyMethod_)
{
  /* If we need to create a return catch, do it here */
  if ( mos_NE(mos_SLOTS(mos_METH)[HASBLOCKRTN],mos_false) ) {
    register mos_value rtnval;

#if 0    
    /* Attempt to reuse a catch, if we are tail recursive */
    if ( mos_EQ(mos_RTNCATCH, mos_undef) ) {
      mos_RTNCATCH = mos_CATCH_NEW();
    }
#endif
    
    /* Begin the catch */
    mos_CATCH_BEGIN(mos_RTNCATCH)
      /* Run the code */
      rtnval = _mos_bcMethod_run(mos_MSG, mos_SLOTS(mos_METH)[BYTECODES], sp);
    mos_CATCH_CAUGHT
      rtnval = mos_RTNVAL;
    mos_CATCH_END
    
    mos_return(rtnval);
  } else {
    /* Run the code */
    mos_return(_mos_bcMethod_run(mos_MSG, mos_SLOTS(mos_METH)[BYTECODES], sp));
  }
}
mos_METHOD_END


#undef sp


mos_METHOD(bcMethod,lobby)
{
  mos_return(mos_send(mos_send(mos_RCVR, mos_s(constants)), mos_s(get_), mos_integer_make(0)));
}
mos_METHOD_END


mos_METHOD(bcMethod,lobby_)
{
  mos_return(mos_send(mos_send(mos_RCVR, mos_s(constants)), mos_s(set_Value_), mos_integer_make(0), mos_ARGV[0]));
}
mos_METHOD_END


mos_METHOD(bcMethod,argumentAt_)
{
  mos_value a = mos_send(mos_RCVR, mos_s(arguments));
  if ( mos_NE(a,mos_undef) ) {
    mos_return(mos_send(a, mos_s(get_), mos_ARGV[0]));
  } else {
    mos_return(mos_send(mos_o(method), mos_s(argumentAt_), mos_ARGV[0]));
  }
}
mos_METHOD_END


mos_METHOD(bcMethod,dumpToStream_)
{
#ifndef bcDumpVersion
#define bcDumpVersion 1.1
#endif

  mos_ARGV[0] = mos_printf(mos_ARGV[0], "{\n");
  mos_printf(mos_ARGV[0], "  version = %g;\n", (double) bcDumpVersion);
  mos_printf(mos_ARGV[0], "  arguments = %O;\n", mos_send(mos_RCVR, mos_s(arguments)));
  mos_printf(mos_ARGV[0], "  locals = %O;\n", mos_send(mos_RCVR, mos_s(locals)));
  mos_printf(mos_ARGV[0], "  bytecodes = %O;\n", mos_send(mos_RCVR, mos_s(bytecodes)));
  mos_printf(mos_ARGV[0], "  memocodes = %O;\n", mos_send(mos_RCVR, mos_s(memocodes)));

  /* make the lobby go away in the constants vector */
  {
    mos_value constants = mos_send(mos_RCVR, mos_s(constants));
    constants = mos_send(constants, mos_s(clone));
    mos_send(constants, mos_s(deepen));
    mos_send(constants, mos_s(set_Value_), mos_integer_make(0), mos_undef);

    mos_printf(mos_ARGV[0], "  constants = %O;\n", constants);
  }

  mos_printf(mos_ARGV[0], "  hasBlockReturn = %O;\n", mos_send(mos_RCVR, mos_s(hasBlockReturn)));

  mos_printf(mos_ARGV[0], "}\n");

  mos_return(mos_ARGV[0]);
} 
mos_METHOD_END


mos_METHOD(bcMethod,decodeOtherOn_)
{
  mos_REFT(mos_MIMPL,mos_method)->_func = _mos_mf_bcMethod___applyMethod_;
}
mos_METHOD_END


mos_OBJECT(bcMethod)
mos_OBJECT_M(bcMethod,argumentAt_)
mos_OBJECT_M(bcMethod,lobby)
mos_OBJECT_M(bcMethod,lobby_)
mos_OBJECT_M(bcMethod,dumpToStream_)
mos_OBJECT_M(bcMethod,decodeOtherOn_)
mos_OBJECT_M(bcMethod,decompile)
mos_OBJECT_A(bytecodes,BYTECODES)
mos_OBJECT_A(constants,CONSTANTS)
mos_OBJECT_A(hasBlockReturn,HASBLOCKRTN)
mos_OBJECT_A(arguments,ARGUMENTS)
mos_OBJECT_A(locals,LOCALS)
mos_OBJECT_A(memocodes,MEMOCODES)
mos_OBJECT_A(memovals,MEMOVALS)
mos_method_METHODS
mos_OBJECT_SLOTS(bcMethod)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_false)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(methods,bcMethod,mos_method,methodMeta)


mos_INIT(bcMethod,3)
{
  /* mos_SLOTS(mos_o(bcMethod))[ARGUMENTS] = mos_vector_make(0, 0); */
  mos_REFT(mos_o(bcMethod),mos_method)->_func = _mos_mf_bcMethod___applyMethod_;
  return 0;
}



