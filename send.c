#ifndef __rcs_id__
#ifndef __rcs_id_mos_send_c__
#define __rcs_id_mos_send_c__
static const char __rcs_id_mos_send_c[] = "$Id: send.c,v 1.4 2001-09-18 02:48:08 stephens Exp $";
#endif
#endif /* __rcs_id__ */


#include "mos/mos.h"
#include "mos/mapfuncs.h"
#include <assert.h>

#ifndef mos_TRACE_ENABLED
#define mos_TRACE_ENABLED 1
#endif


#define mos_MSG mos_MAKE_REF(msg)


/****************************************************************************
**
** Lookup primitive
*/

#define FIND(X,S) _mos_map_find(mos_METHODS(X), S)

__inline__
void _mos_basicMeta_lookup(mos_message *msg)
{
  mos_map_slot *ms;
  
  if ( (ms = _mos_map_find(mos_METHODS(mos_CNTX), mos_SEL)) ) {
    mos_METH = ms->_value; 
    mos_MIMPL = mos_CNTX;
  }
}


/* Do not inline this, it will bloat the stack frame */
void _mos_otherMeta_lookup(mos_message *msg)
{
  mos_send(mos_META(mos_CNTX), mos_s(lookupMethod_), mos_MSG);
}

__inline__
void _mos_lookup(mos_message *msg)
{
  if ( mos_EQ(mos_META(mos_CNTX), mos_o(basicMeta)) /* || mos_EQ(mos_META(mos_CNTX), mos_o(methodMeta)) */ ) {
   _mos_basicMeta_lookup(msg);
  } else {
   _mos_otherMeta_lookup(msg);
  }
}


/****************************************************************************
**
** Apply primitive
*/

__inline__
mos_value _mos_methodMeta_apply(mos_message *msg)
{
  return mos_RTNVAL = (mos_REFT(mos_METH,mos_method)->_func)(mos_MSG);
}

/* Do not inline this, it will bloat the stack frame */
mos_value _mos_otherMeta_apply(mos_message *msg)
{
  return mos_RTNVAL = mos_send(mos_META(mos_METH), mos_s(applyMethod_), mos_MSG);
}

__inline__
mos_value _mos_apply(mos_message *msg)
{
  /* Handle methodMeta */
  if ( mos_EQ(mos_META(mos_METH), mos_o(methodMeta)) ) {
    return _mos_methodMeta_apply(msg);
  } else {
    return _mos_otherMeta_apply(msg);
  }
}

#undef mos_MSG


/****************************************************************************
**
** Send primitive
*/

/* Place holder for mos_MSG outside of method body */
mos_value mos_MSG = mos_undef; /* NOT_THREAD_SAFE */

int _mos_send_trace = 0;

#define TRACE_OUTPUT mos_send(mos_o(debugger), mos_s(output))

__inline__
mos_value _mos_send(mos_message *msg)
{
  mos_value rtnval;
#if mos_TRACE_ENABLED
  static int _mos_trace_lock = 0;
  int this_send_trace = _mos_send_trace;
  static unsigned long msgid;
  static unsigned long traceLevel;
  unsigned long this_traceLevel = traceLevel;
  unsigned long this_msgid;
#endif

  /* save and set global mos_MSG for message stack chaining */
  mos_value old_mos_MSG = mos_MSG;
  mos_MSG = mos_MAKE_REF(msg);

#undef mos_MSG
#define mos_MSG mos_MAKE_REF(msg)

#if mos_TRACE_ENABLED
  if ( ! _mos_trace_lock && this_traceLevel < this_send_trace ) {
    this_msgid = ++ msgid;
    _mos_trace_lock ++;
    traceLevel ++;
    mos_printf(TRACE_OUTPUT, "TRACE: %03ld %ld: LOOKUP: %P\n", traceLevel, this_msgid, mos_MSG);
    _mos_trace_lock --;
  }
#endif
  
  _mos_lookup(msg);
  
  /* If meta lookup fails, search the defaultBehavior object */
  /* Otherwise use the undef::undefinedMethod to raise an error */
  if ( mos_EQ(mos_METH,mos_undef) ) {
    mos_map_slot *ms;
    
    if ( (ms = FIND(mos_o(defaultBehavior), mos_SEL)) ) {
      mos_METH = ms->_value; 
      mos_MIMPL = mos_o(defaultBehavior);
    } else {
      mos_METH = mos_m(undefinedValue,undefinedMethod); 
      mos_MIMPL = mos_undef;
    }
  }

#if mos_TRACE_ENABLED
  if ( ! _mos_trace_lock && this_traceLevel < this_send_trace ) {
    _mos_trace_lock ++;
    mos_printf(TRACE_OUTPUT, "TRACE: %03ld %ld: APPLY: %-P\n", traceLevel, this_msgid, mos_METH);
    _mos_trace_lock --;
  }
#endif

  rtnval = _mos_apply(msg);
  
#if mos_TRACE_ENABLED
  if ( ! _mos_trace_lock && this_traceLevel < this_send_trace ) {
    _mos_trace_lock ++;
    mos_printf(TRACE_OUTPUT, "TRACE: %03ld %ld: RETURN: %-P\n", traceLevel, this_msgid, rtnval);
    traceLevel --;
    _mos_trace_lock --;
  }
#endif

#undef mos_MSG

  mos_MSG = old_mos_MSG;
  
  return rtnval;
}


/*
** C-function call interface, see send.h
*/
__inline
mos_value _mos_sendv_(mos_value mos_MSG, mos_value srcInfo, mos_value __mos_mr, mos_value sel, size_t nargs, mos_value *argv)
{
  mos_vector_DECLARE(__mos_msg_args, argv, nargs);
  mos_message_DECLARE(__mos_msg, sel, __mos_mr, __mos_mr , __mos_msg_args, srcInfo);
  return _mos_send(&__mos_msg);
}


__inline
mos_value _mos_sendv__(mos_value mos_MSG, mos_value srcInfo, mos_value _CNTX,  mos_value __mos_mr, mos_value sel, size_t nargs, mos_value *argv)
{
  mos_vector_DECLARE(__mos_msg_args, argv, nargs);
  mos_message_DECLARE(__mos_msg, sel, _CNTX, __mos_mr , __mos_msg_args, srcInfo);
  return _mos_send(&__mos_msg);
}


/***************************************************************************/


#if 1
#define GET_ARGV \
{ \
  mos_value *argv = (_mos_sp -= argc); \
  va_list vap; \
  \
  if ( _mos_sp < _mos_sp_top ) { \
    fprintf(stderr, "\nmos: stack overrun\n"); \
    abort(); \
  } \
  va_start(vap,sel); \
  { \
    int i; \
     \
    for ( i = 0; i < argc; i ++ ) { \
      argv[i] = va_arg(vap, mos_value); \
    } \
  }
#define END_ARGV \
  va_end(vap); \
  _mos_sp += argc; \
}
#else
#if 1
#define GET_ARGV \
{ \
  mos_value *argv = (mos_value*)((char*)(&sel)+sizeof(sel));
#define END_ARGV \
}
#else
#define GET_ARGV \
{ \
  mos_value *argv = alloca(sizeof(argv[0]) * argc); \
  va_list vap; \
  \
  va_start(vap,sel); \
  { \
    int i; \
     \
    for ( i = 0; i < argc; i ++ ) { \
      argv[i] = va_arg(vap, mos_value); \
    } \
  }
#define END_ARGV \
  va_end(vap); \
}
#endif
#endif


#ifdef mos_send
#undef mos_send
#endif

mos_value mos_send(mos_value rcvr, mos_value sel, ...)
{
#define argc (mos_REFT(sel,mos_selector)->_nargs)
  mos_value rtnval;

#if 0
  mos_value *sp = _mos_sp;
  
#if 1
  if ( _mos_inited < 2 ) {
    fprintf(stderr, "mos_send(%p, %s, %d, ...)\n", (void*) mos_REF(rcvr), mos_REFT(sel,mos_selector)->_namestr, argc);
  }
#endif
#endif

  GET_ARGV
  rtnval = _mos_sendv_(mos_MSG, mos_undef, rcvr, sel, argc, argv);
  END_ARGV

#if 0
  assert(sp == _mos_sp);
#endif

  return rtnval;
}


#ifdef mos_send_
#undef mos_send_
#endif

mos_value mos_send_(mos_value cntx, mos_value rcvr, mos_value sel, ...)
{
  mos_value rtnval;
  
  GET_ARGV
  rtnval = _mos_sendv__(mos_MSG, mos_undef, cntx, rcvr, sel, argc, argv);
  END_ARGV
  
  return rtnval;
}

