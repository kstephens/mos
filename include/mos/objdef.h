#ifndef _mos_objdef_h
#define _mos_objdef_h

/************************************************************
** Object definition
*/

#ifndef mos_OBJECT

#define mos_OBJECT(O) \
mos_map_BEGIN(,_mos_om_##O)

#define mos_OBJECT_M(O,S) mos_map_ENTRY(mos_s(S), mos_m(O,S))
#define mos_OBJECT_METHOD(M,S) mos_map_ENTRY(mos_s(S), M)
#define mos_OBJECT_G(S,I) mos_map_ENTRY(mos_s(S), mos_GETTER(I))
#define mos_OBJECT_A(S,I) mos_OBJECT_G(S,I) mos_map_ENTRY(mos_s(S##_), mos_SETTER(I))

#define mos_OBJECT_SLOTS(O) \
mos_map_slots_END; \
mos_value _mos_os_##O[] = {

#define mos_OBJECT_S(X) X,

#define mos_OBJECT_END(S,O,T,M) \
  0 \
}; \
mos_objectDesc _mos_od_##O = { \
  mos_object_HDR_INIT_STATIC(objectDesc), \
  mos_o(M), \
  mos_map_INIT(_mos_om_##O), \
  mos_ARRAY_LEN(_mos_os_##O) - 1, \
  sizeof(T), \
  0 \
}; \
T _mos_o_##O = { &_mos_od_##O, _mos_os_##O }; 

#endif

#endif


