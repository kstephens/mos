#ifndef _mos_selector_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_selector_h__
#define __rcs_id_mos_selector_h__
static const char __rcs_id_mos_selector_h[] = "$Id: selector.h,v 1.2 1999-02-19 09:26:31 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_selector_h

typedef struct mos_selector {
  mos_object_HDR;
  const char *_namestr;
  struct mos_selector *_next;
  mos_value _name;
  mos_value _getter;
  mos_value _setter;
  mos_value _split;
  int _nargs;
} mos_selector;

/* Create a new selector */
mos_value mos_selector_make_(const char *name, size_t l);

#define mos_selector_make(N) mos_selector_make_((N), 0)

/* Get the getter selector for a setter selector */
mos_value mos_getter_selector(mos_value x);

/* Get the setter selector for a getter selector */
mos_value mos_setter_selector(mos_value x);

/* Get a list of all interned selectors */
mos_value _mos_selector_list(void);

/* Declaration of all system selectors */
#define mos_DEF_s(X) extern mos_selector _mos_s_##X;
#include "mos/sel.def"
#ifndef mos_s
#define mos_s(X) mos_MAKE_REF(&_mos_s_##X)
#endif

/* A string containing all operator selector characters */
extern const char _mos_selector_op_charset[];

#endif
