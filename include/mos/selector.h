#ifndef _mos_selector_h
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

/* Escape selector strings. */
const char *_mos_selector_escape(const char *x);

#endif
