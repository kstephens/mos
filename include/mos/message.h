
#ifndef __rcs_id__
#ifndef __rcs_id_mos_message_h__
#define __rcs_id_mos_message_h__
static const char __rcs_id_mos_message_h[] = "$Id: message.h,v 1.3 2001-08-13 09:39:19 stephens Exp $";
#endif
#endif /* __rcs_id__ */

/*
** A message object is created on the stack by mos_send().
** It contains all the information needed to lookup and apply a method.
** The arguments vector is also allocated on the stack.
** Stack-allocated objects must never be referenced
** beyond the scope of the allocation.
*/

typedef struct mos_message_slots {
  /* Filled in by mos_send() */
  mos_value _previousMessage;
    /* the current stack can be traced back through this slot  */
  mos_value _selector;
    /* The selector of the message */
  mos_value _context;
    /* The context of the lookup; == receiver for non-directed lookups */
  mos_value _receiver;
    /* The receiver of the message */
  mos_value _arguments;
    /* The argument vector of the message */
  mos_value _locals;
    /* The argument vector of the message */
  mos_value _sourceInfo;
    /* The object that represents the source code info. */
  
  /* Filled in by [message receiver _meta lookupMethod: message] */
  mos_value _method;
    /* The object to apply. */
  mos_value _methodImplementer;
    /* The object that actually had the method in its desc. */

  mos_value _returnValue;
    /* The value returned. */
  mos_value _returnCatch;
    /* The catch object used for "long" returns within block bodies. */
} mos_message_slots;

typedef mos_object mos_message;

extern mos_value mos_MSG;

#define _mos_MSG mos_MSG

#define mos_MSG_s mos_SLOTST(_mos_MSG, mos_message_slots)

#define _mos_SEL mos_MSG_s->_selector
#define mos_SEL _mos_SEL

#define _mos_CNTX mos_MSG_s->_context
#define mos_CNTX _mos_CNTX

#define _mos_RCVR mos_MSG_s->_receiver
#define mos_RCVR _mos_RCVR

#define _mos_ARGS mos_MSG_s->_arguments
#define mos_ARGS _mos_ARGS

#define _mos_LOCS mos_MSG_s->_locals
#define mos_LOCS _mos_LOCS

#define _mos_METH mos_MSG_s->_method
#define mos_METH _mos_METH

#define _mos_MIMPL mos_MSG_s->_methodImplementer
#define mos_MIMPL _mos_MIMPL

#define _mos_SRCINFO mos_MSG_s->_sourceInfo
#define mos_SRCINFO _mos_SRCINFO

#define _mos_RTNVAL mos_MSG_s->_returnValue
#define mos_RTNVAL _mos_RTNVAL

#define _mos_RTNCATCH mos_MSG_s->_returnCatch
#define mos_RTNCATCH _mos_RTNCATCH

#define _mos_ARGV mos_REFT(mos_ARGS,mos_vector)->_v

#define _mos_LOCV mos_REFT(mos_LOCS,mos_vector)->_v

/*
#define mos_ARGV _mos_ARGV
*/

#define mos_message_DECLARE(X,_SEL,_CNTX,_RCVR,_ARGS,_SRCINFO) \
  mos_message_slots X##_s = { mos_MSG, (_SEL), (_CNTX), (_RCVR), _ARGS, mos_undef, _SRCINFO, mos_undef, mos_undef, mos_undef, mos_undef }; \
  mos_message X = { &_mos_od_message, (mos_value*)&X##_s }
