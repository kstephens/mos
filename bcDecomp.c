#ifndef __rcs_id__
#ifndef __rcs_id_mos_bcDecomp_c__
#define __rcs_id_mos_bcDecomp_c__
static const char __rcs_id_mos_bcDecomp_c[] = "$Id: bcDecomp.c,v 1.3 1999-12-26 20:05:38 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

/*
** A bytecode decompiler.
** Takes a bcMethod object and creates a expr tree, suitable for printing or recompiling.
*/
#include "mos/mos.h"
#include "mos/bc.h"
#include "mos/expr.h"

#ifndef _stk_SIZE
#define _stk_SIZE 256
#endif

mos_METHOD(bcMethod,decompile)
{
  static mos_value stk[_stk_SIZE];
  static mos_value *sp = stk + (_stk_SIZE-1);

#define TOP *sp
#define PUSH(X) *(-- sp) = (X)
#define DUP() (sp[-1] = *sp, sp --)
#define POP() *(sp ++)
#define PUSHN(N) (sp -= (N))
#define POPN(N) (sp += (N))
#define ARG() (pc += 4, ((unsigned long) pc[-4] | ((unsigned long) pc[-3] << 8) | ((unsigned long) pc[-2] << 16) | ((unsigned long) pc[-1] << 24)))

  const unsigned char *pc;
  mos_value *fp;
  mos_value meth = mos_RCVR;
  mos_value bytecodes = mos_send(meth, mos_s(bytecodes));

  /* Our constants vector */
  mos_value *constants = mos_vector_V(mos_send(meth, mos_s(constants)));

  /* Our arguments ident vector */
  mos_value *args = mos_vector_V(mos_send(meth, mos_s(arguments)));

  /* Out locals ident vector */
  mos_value *locals = mos_vector_V(mos_send(meth, mos_s(locals)));

  /* An array to hold the method body */
  mos_value body = mos_vector_make(0, 0);
  mos_value sel;

  /* Save a frame pointer */
  fp = sp;
 restart:
  pc = mos_string_V(bytecodes);

 again:
  switch ( (mos_bc) *(pc ++) ) {
  case mos_bc_nop:
    goto again;
    
  case mos_bc_restart:
    PUSH(mos_exprSend(mos_undef, mos_s(_restart), 0));
    goto again;
    
  case mos_bc_object:
    PUSH(mos_exprObject(mos_vector_make(0,0), mos_vector_make(0,0)));
    goto again;
    
  case mos_bc_block:
    PUSH(mos_exprBlock(mos_vector_make(0,0), mos_vector_make(0,0), mos_vector_make(0,0)));
    goto again;
    
  case mos_bc_method:
    PUSH(mos_exprMethod(mos_vector_make(0,0), mos_vector_make(0,0), mos_vector_make(0,0)));
    goto again;
    
  case mos_bc_dup:
    DUP();
    goto again;
    
  case mos_bc_pop:
    /* pop only occurs at the end of stmt */
    mos_send(body, mos_s(append_), POP());
    goto again;
    
  case mos_bc_rtn:
  case mos_bc_rtnBlk:
    mos_send(body, mos_s(append_), mos_exprSend(mos_undef, mos_s(__RTN__), 1, POP()));
    break;
    
  case mos_bc_const:
    PUSH(mos_exprSend(mos_undef, mos_s(_constants), 0));
    goto again;
    
  case mos_bc_lobby:
    PUSH(mos_exprSend(mos_undef, mos_s(_), 0));
    goto again;
    
  case mos_bc_msg:
    PUSH(mos_exprSend(mos_undef, mos_s(_msg), 0));
    goto again;
    
  case mos_bc_rcvr: /* msg | msg->rcvr */
    POP();
    PUSH(mos_exprSend(mos_undef, mos_s(self), 0));
    goto again;
  case mos_bc_rcvrSet: /* msg value | value */
    sel = POP();
    TOP = mos_exprSend(mos_undef, mos_s(self_), 1, sel);
    goto again;
    
  case mos_bc_arg_: /* | args[I] */
    sel = args[ARG()];
    PUSH(mos_exprSend(mos_undef, sel, 0));
    goto again;

  case mos_bc_argSet_: /* x | rcvr */
    sel = args[ARG()];
    sel = mos_setter_selector(sel);
    TOP = mos_exprSend(mos_undef, sel, 1, TOP);
    goto again;
    
  case mos_bc_locs_: /* | ... */
    goto again;

  case mos_bc_loc_: /* | local[I] */
    sel = locals[ARG() - 1];
    PUSH(mos_exprSend(mos_undef, sel, 0));
    goto again;
    
  case mos_bc_locSet_: /* locSet:<i> value | */
    sel = locals[ARG() - 1];
    sel = mos_setter_selector(sel);
    TOP = mos_exprSend(mos_undef, sel, 1, TOP);
    goto again;

  case mos_bc_lit_: /* | constant[I] */
    PUSH(mos_exprConstant(constants[ARG()]));
    goto again;
    
  case mos_bc_memo_:
  case mos_bc_memoval_:
    /* IMPLIMENT */
    goto again;

  case mos_bc_send_: /* sel rcvr args ... | result */ 
    {
      int nargs = ARG();
      mos_value sel = POP();
      mos_value rcvr = POP();
      mos_value args = mos_vector_make(nargs, sp);
      rcvr = mos_exprSendV(rcvr, sel, args);
      POPN(nargs);
      PUSH(rcvr);
    }
    goto again;

  case mos_bc_sendDir_: /* sel cntx rcvr args ... | result */
    {
      int nargs = ARG();
      mos_value sel = POP();
      mos_value cntx = POP();
      mos_value rcvr = POP();
      mos_value args = mos_vector_make(nargs, sp);
      rcvr = mos_exprSend_V(cntx, rcvr, sel, args);
      POPN(nargs);
      PUSH(rcvr);
    }
    goto again;

  default:
    fprintf(stderr, "\n:bcDecomp.c: unknown bytecode %d\n", pc[-1]);
    break;
  }
  
  sp = fp;

  /* Create a exprMethod object */
  {
    mos_value args = mos_send(meth, mos_s(arguments));
    mos_value locals = mos_send(meth, mos_s(locals));
   
    mos_return(mos_exprMethod(args, locals, body));
  }
}
mos_METHOD_END

