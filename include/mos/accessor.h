#ifndef _mos_accessor_h
#define _mos_accessor_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_accessor_h__
#define __rcs_id_mos_accessor_h__
static const char __rcs_id_mos_accessor_h[] = "$Id: accessor.h,v 1.3 2000-03-21 07:13:45 stephensk Exp $";
#endif
#endif /* __rcs_id__ */


typedef struct mos_accessor {
  mos_method_HDR;
  unsigned int _sloti;
} mos_accessor;

typedef struct mos_constantMethod {
  mos_method_HDR;
  /* mos_value _value; SEE slot(0) */
} mos_constantMethod;

mos_value mos_getter_method(unsigned int sloti);
mos_value mos_setter_method(unsigned int sloti);
mos_value mos_constant_method(mos_value v);

extern mos_accessor _mos_getter[], _mos_setter[];
#define mos_GETTER(I) mos_MAKE_REF(&_mos_getter[I])
#define mos_SETTER(I) mos_MAKE_REF(&_mos_setter[I])

#define mos_accessor_METHODS \
  mos_method_METHODS \
  mos_OBJECT_M(constant,clone) \
  mos_OBJECT_M(accessor,asAccessor) \
  mos_OBJECT_M(accessor,_equal_) \
  mos_OBJECT_M(accessor,slotIndex)

#endif
