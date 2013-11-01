#ifndef _mos_value_h
#define _mos_value_h

#include <unistd.h>

struct mos_object;
typedef struct mos_object mos_object;

struct mos_objectDesc;
typedef struct mos_objectDesc mos_objectDesc;

struct mos_map;
typedef struct mos_map mos_map;

typedef ssize_t mos_int;
typedef size_t  mos_uint;

#if 1
typedef mos_object *mos_value;

#define mos_UNINITIALIZED ((mos_value)0) /* The null value */
#define mos_INITIALIZED(X) ((X)!=0)	 /* Is it not the null value? */
#define mos_MAKE_REF(X) ((mos_value)(X))
#define mos_REF(X) ((mos_object*)(X))
#define mos_EQ(X,Y) ((X)==(Y))

#else

typedef struct mos_value {
  mos_object *_t;
} mos_value;

extern const mos_value mos_UNINITIALIZED;
#define mos_INITIALIZED(X) ((X)._t!=0)
#define mos_MAKE_REF(X) ({mos_value _X; _X._t = (X); _X})
#define mos_REF(X) ((X)._t)
#define mos_EQ(X,Y) ((X)._t==(Y)._t)

#endif

#define mos_REFT(X,T) ((T*)mos_REF(X))
#define mos_NE(X,Y) (!mos_EQ(X,Y))

#endif
