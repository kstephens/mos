#ifndef _mos_map_h
#define _mos_map_h

#include "mos/value.h" /* mos_value */
#include "mos/objhdr.h" /* mos_object_HDR */
#include <stdarg.h> /* va_list */

/* A map table entry */
typedef struct mos_map_slot {
  mos_value _key;
  mos_value _value;
  unsigned int _hits; /* Used to record the number of hits for dynamic reordering */
} mos_map_slot;

/* Notice a mos_map is also an mos_object */
struct mos_map {
  mos_object_HDR;
  mos_map_slot *_m_slots;
  unsigned int _m_nslots;
};

/* Macros to create a static map structure */

#define mos_map_BEGIN(DECL,MAP) \
DECL mos_map_slot MAP##_m_slots[] = {

#define mos_map_ENTRY(KEY,VALUE) \
  { (KEY), (VALUE) },

#define mos_map_slots_END \
  { mos_UNINITIALIZED } \
}

#define mos_map_INIT(MAP) { mos_object_HDR_INIT_STATIC(map), MAP##_m_slots, sizeof(MAP##_m_slots)/sizeof(MAP##_m_slots[0]) - 1 }

#define mos_map_END(DECL,MAP) \
mos_map_slots_END \
DECL mos_map MAP = mos_map_INIT(MAP);


/* Macros to iterate through a map structure */

#define mos_map_LOOP(map,ms) \
  register mos_map_slot *ms; \
  for ( ms = (map)->_m_slots; mos_INITIALIZED(ms->_key); ms ++ ) {

#define mos_map_LOOP_END }

/* Map object creation */
mos_value mos_map_make(const mos_map *_v);
#define mos_map_make(X) mos_MAKE_REF(X)

/* Create from a map object */
mos_value mos_map_make1(const mos_map *_v);

/* Create a new map object */
mos_value mos_map_makev(size_t _l, va_list *_vap);
mos_value mos_map_make_(size_t _l, ...);

#endif
