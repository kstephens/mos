#ifndef _mos_mapfuncs_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_mapfuncs_h__
#define __rcs_id_mos_mapfuncs_h__
static const char __rcs_id_mos_mapfuncs_h[] = "$Id: mapfuncs.h,v 1.3 2000-03-21 07:12:22 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_mapfuncs_h

#define mos_map_REORDER(map,ms) do { \
  ms->_hits ++; \
  while ( ms > (map)->_m_slots && ms->_hits > (ms - 1)->_hits ) { \
    mos_map_slot temp = *ms; \
    *ms = *(ms - 1); \
    *(ms - 1) = temp; \
    ms --; \
  } \
} while(0)

static __inline__
mos_map_slot * _mos_map_find(mos_map *map, mos_value key)
{
  mos_map_LOOP(map,ms)
    if ( mos_EQ(ms->_key, key) ) {
      mos_map_REORDER(map,ms);
      return ms;
    }
  mos_map_LOOP_END
  return 0;
}


static __inline__
mos_map_slot * _mos_map_findEqual(mos_map *map, mos_value key)
{
  mos_map_LOOP(map,ms)
    if ( mos_EQUAL(ms->_key, key) ) {
      mos_map_REORDER(map,ms);
      return ms;
    }
  mos_map_LOOP_END
  return 0;
}

static __inline__
mos_map_slot * _mos_map_findByValue(mos_map *map, mos_value key)
{
  mos_map_LOOP(map,ms)
    if ( mos_EQ(ms->_value, key) ) {
      mos_map_REORDER(map,ms);
      return ms;
    }
  mos_map_LOOP_END
  return 0;
}

static __inline__
mos_map_slot * _mos_map_findByValueEqual(mos_map *map, mos_value key)
{
  mos_map_LOOP(map,ms)
    if ( mos_EQUAL(ms->_value, key) ) {
      mos_map_REORDER(map,ms);
      return ms;
    }
  mos_map_LOOP_END
  return 0;
}

#define _mos_map_ADD \
  /* Look for empty slot at end (created by _mos_map_remove) */\
  if ( ms - map->_m_slots >= map->_m_nslots ) { \
    register const mos_map_slot *oms; \
     \
    oms = map->_m_slots; \
    map->_m_slots = ms = mos_malloc(sizeof(map->_m_slots[0]) * ((++ map->_m_nslots) + 1)); \
    while ( mos_INITIALIZED(oms->_key) ) { \
      *(ms ++) = *(oms ++); \
    } \
  } \
     \
  ms->_key = key; \
  ms->_value = value; \
  ms->_hits = 0; \
   \
  (ms + 1)->_key = mos_UNINITIALIZED; \
  (ms + 1)->_value = mos_UNINITIALIZED; \
  (ms + 1)->_hits = 0

static __inline__
void _mos_map_setOrAdd(mos_map *map, mos_value key, mos_value value)
{
  /* First look for existing slot */
  mos_map_LOOP(map,ms)
    if ( mos_EQ(ms->_key, key) ) {
      ms->_value = value;
      mos_map_REORDER(map,ms);
      return;
    }
  mos_map_LOOP_END
  
  _mos_map_ADD;
}


static __inline__
void _mos_map_setOrAddEqual(mos_map *map, mos_value key, mos_value value)
{
  /* First look for existing slot */
  mos_map_LOOP(map,ms)
    if ( mos_EQUAL(ms->_key, key) ) {
      ms->_value = value;
      mos_map_REORDER(map,ms);
      return;
    }
  mos_map_LOOP_END
  
  _mos_map_ADD;
}


#undef _mos_map_ADD

static __inline__
int _mos_map_remove(mos_map *map, mos_value key)
{
  mos_map_LOOP(map,ms)
    if ( mos_EQ(ms->_key, key) ) {
      do {
	*ms = *(ms + 1);
	ms ++;
      } while ( mos_INITIALIZED(ms->_key) );

      return 1;
    }
  mos_map_LOOP_END
  
  return 0;
}

static __inline__
int _mos_map_removeEqual(mos_map *map, mos_value key)
{
  mos_map_LOOP(map,ms)
    if ( mos_EQUAL(ms->_key, key) ) {
      do {
	*ms = *(ms + 1);
	ms ++;
      } while ( mos_INITIALIZED(ms->_key) );

      return 1;
    }
  mos_map_LOOP_END
  
  return 0;
}

static __inline__
int _mos_map_removeByValue(mos_map *map, mos_value key)
{
  mos_map_LOOP(map,ms)
    if ( mos_EQ(ms->_value, key) ) {
      do {
	*ms = *(ms + 1);
	ms ++;
      } while ( mos_INITIALIZED(ms->_key) );

      return 1;
    }
  mos_map_LOOP_END
  
  return 0;
}


static __inline__
int _mos_map_removeByValueEqual(mos_map *map, mos_value key)
{
  mos_map_LOOP(map,ms)
    if ( mos_EQUAL(ms->_value, key) ) {
      do {
	*ms = *(ms + 1);
	ms ++;
      } while ( mos_INITIALIZED(ms->_key) );

      return 1;
    }
  mos_map_LOOP_END
  
  return 0;
}

#undef mos_map_REORDER

#endif
