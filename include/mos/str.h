#ifndef _mos_str_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_str_h__
#define __rcs_id_mos_str_h__
static const char __rcs_id_mos_str_h[] = "$Id: str.h,v 1.2 1999-02-19 09:26:32 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_str_h

typedef struct mos_string {
  mos_object_HDR;
  char *_v;
  size_t _l;
  size_t _al; /* Allocated length */
  int _flags;
} mos_string;

#define mos_string_L(X) mos_REFT(X,mos_string)->_l
#define mos_string_V(X) mos_REFT(X,mos_string)->_v
#define mos_string_AL(X) mos_REFT(X,mos_string)->_al
#define mos_string_FLAGS(X) mos_REFT(X,mos_string)->_flags

mos_value mos_string_make(const char *_v, size_t _l);
mos_value mos_string_make_(const char *_v);

#define mos_string_makeReadonly(X) mos_string_FLAGS(X)|=mos_READONLY

#endif

