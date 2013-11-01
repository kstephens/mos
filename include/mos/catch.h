#ifndef _mos_catch_h
#define _mos_catch_h

#include "mos/mos.h"

#include <setjmp.h>

#define mos_catch_HDR \
  mos_object_HDR; \
  jmp_buf _jb; \
  int _jbvalid; \
  struct mos_catch *_prevCatch; \
  mos_value *_spSave;
  
typedef struct mos_catch {
  mos_catch_HDR;
} mos_catch;

#define mos_catch_tvalue 0
#define mos_catch_thrown 1

extern void _mos_CATCH_BEGIN(mos_value catch);
extern void _mos_CATCH_END(mos_value catch);

extern mos_value mos_THROW(mos_value catch, mos_value value);

#define mos_CATCH_NEW() mos_send(mos_o(catch), mos_s(clone))
#define mos_CATCH_BEGIN(C) { mos_value __catch = (C); if ( ! setjmp(mos_REFT(__catch,mos_catch)->_jb) ) { _mos_CATCH_BEGIN(__catch); {
#define mos_CATCH_CAUGHT }} else { _mos_CATCH_END(__catch); {
#define mos_CATCH_END } _mos_CATCH_END(__catch); }}

#endif

