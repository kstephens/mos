
typedef struct mos_accessor {
  mos_method_HDR;
  unsigned int _sloti;
} mos_accessor;

typedef struct mos_constantMethod {
  mos_method_HDR;
  mos_value _value;
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
  mos_OBJECT_M(accessor,slotIndex)
