#ifndef _mos_mos_h
#define _mos_mos_h

#include <stddef.h> /* size_t */

#include "mos/value.h" /* mos_value */

#define mos_ARRAY_LEN(X) (sizeof(X)/sizeof(X[0]))

enum {
  mos_READONLY = 1
  /* This object cannot have any side-effecting methods applied */
};

/* Deep-equality testing */
extern int mos_EQUAL(mos_value x, mos_value y);

/* Memory allocation */
void *mos_malloc(size_t);
char *mos_strdup(const char*);

#define mos_return(X) return(X)

/* General mapping object */
#include "mos/map.h"

/* Object descriptor */
#include "mos/objdesc.h"

/* Object structure */
#include "mos/object.h"

/* Static object definition */
#include "mos/objdef.h"

/* Undefined value object */
#include "mos/undef.h"

/* Boolean object */
#include "mos/bool.h"

/* Numerical object */
#include "mos/integer.h"
#include "mos/real.h"

/* String/Character object */
#include "mos/str.h"
#include "mos/char.h"

/* Vector object */
#include "mos/vector.h"

/* Message selector object */
#include "mos/selector.h"

/* Source info object */
#include "mos/srcInfo.h"

/* Message object */
#include "mos/message.h"

/* Method object */
#include "mos/method.h"

/* Slot accessor object */
#include "mos/accessor.h"

/* Initialization */
#include "mos/init.h"

/* Message sending */
#include "mos/send.h"

/* Object cloning */
#include "mos/clone.h"

/* Character stream object */
#include "mos/stream.h"

/* Formatting */
#include "mos/print.h"

/* Error handling */
#include "mos/error.h"

/* Non-local exit */
#include "mos/catch.h"

/* Named object */
#include "mos/namedobj.h"

/* Annotations */
#include "mos/annot.h"

/* Annotations */
#include "mos/regexp.h"

/* Static object */
#define mos_o(X) mos_MAKE_REF(&_mos_o_##X)
/* Static object descriptor */
#define mos_od(X) mos_MAKE_REF(&_mos_od_##X)
/* Static object method map */
#define mos_om(X) mos_MAKE_REF(&_mos_od_##X._methods)

#define mos_DEF_OBJECT(S,O,T,M) extern T _mos_o_##O; extern mos_objectDesc _mos_od_##O; extern mos_value _mos_os_##O[];
#include "mos/obj.def"

#endif
