#ifndef _mos_send_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_send_h__
#define __rcs_id_mos_send_h__
static const char __rcs_id_mos_send_h[] = "$Id: send.h,v 1.2 1999-02-19 09:26:31 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_send_h

extern mos_value *_mos_sp, *_mos_sp_top;

void _mos_basicMeta_lookup(mos_message *msg);
void _mos_lookup(mos_message *msg);

mos_value _mos_methodMeta_apply(mos_message *msg);
mos_value _mos_apply(mos_message *msg);

extern int _mos_send_trace;

mos_value _mos_send(mos_message *msg);
mos_value _mos_sendv_(mos_value _prevMsg, mos_value srcInfo, mos_value rcvr, mos_value sel, size_t nargs, mos_value *argv);
mos_value _mos_sendv__(mos_value _prevMsg, mos_value srcInfo, mos_value cntx,  mos_value rcvr, mos_value sel, size_t nargs, mos_value *argv);

mos_value mos_send(mos_value _rcvr, mos_value _sel, ...);
mos_value mos_send_(mos_value _cntx, mos_value _rcvr, mos_value _sel, ...);

#if 0
#define mos_send(_RCVR,_SEL,_ARGS...) \
({ \
  mos_sourceInfo_DECLARE(__mos_msg_si, __FILE__, __LINE__); \
  mos_value __mos_msg_argv[] = { _ARGS }; \
  _mos_sendv_(mos_MSG, __mos_msg_si, _RCVR, _SEL, mos_ARRAY_LEN(__mos_msg_argv), __mos_msg_argv); \
})

#define mos_send_(_CNTX,_RCVR,_SEL,_ARGS...) \
({ \
  mos_sourceInfo_DECLARE(__mos_msg_si, __FILE__, __LINE__); \
  mos_value __mos_msg_argv[] = { _ARGS }; \
  _mos_sendv__(mos_MSG, __mos_msg_si, _CNTX, _RCVR, _SEL, mos_ARRAY_LEN(__mos_msg_argv), __mos_msg_argv); \
})

#endif

#endif

