#ifndef __rcs_id__
#ifndef __rcs_id_mos_map_c__
#define __rcs_id_mos_map_c__
static const char __rcs_id_mos_map_c[] = "$Id: map.c,v 1.4 1999-12-26 20:05:56 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include "mos/memcpy.h"
#include "mos/mapfuncs.h"

/********************************************************************/

#define SELF mos_REFT(mos_MIMPL,mos_map)
#define M SELF
#define V M->_m_slots
#define L M->_m_nslots

#define I mos_INT(mos_ARGV[0])

mos_ANNOT("Module: map")
mos_ANNOT("Doc: The map object.  Map objects can map one object to another.")

  mos_ANNOT("Category: Coerce")

mos_METHOD(map,asMap)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END

  mos_ANNOT_END

  mos_ANNOT("Category: Clone")

mos_METHOD(map,clone)
{
  mos_map *m;
  mos_map_slot *v;

  m = mos_REFT(mos_clone(mos_MIMPL), mos_map);
  v = mos_malloc(sizeof(m->_m_slots[0]) * (m->_m_nslots + 1));
  mos_memcpy(v, V, sizeof(V[0]) * (L + 1));
  m->_m_slots = v;
  
  mos_return(mos_MAKE_REF(m));
}
mos_METHOD_END

  mos_ANNOT_END

/*******************************************************************/

  mos_ANNOT("Category: Print")

mos_METHOD(map,printOn_)
{
  mos_value ss;

  mos_ARGV[0] = mos_printf(mos_ARGV[0], "");
  
  ss = mos_printf(mos_ARGV[0], "%[(@M");
  
  {
  mos_map_LOOP(M,ms)
    mos_printf(ss, ",\n%-P -> %-P", ms->_key, ms->_value);
  mos_map_LOOP_END
  }
  
  mos_printf(ss, "%]\n)");
  mos_return(mos_ARGV[0]);
}
mos_METHOD_END

mos_METHOD(map,printShallowOn_)
{
  mos_return(mos_send(mos_RCVR, mos_s(printIdOn_), mos_ARGV[0]));
}
mos_METHOD_END

  mos_ANNOT_END

/*******************************************************************/

  mos_ANNOT("Category: Encode")

mos_METHOD(map,encodeOtherOn_)
{
  mos_map *o = SELF;

  mos_send(mos_ARGV[0], mos_s(encodeRawInteger_), mos_integer_make(o->_m_nslots));
  {
    mos_map_LOOP(o, ms);
    {
      mos_send(mos_ARGV[0], mos_s(encodeAsReference_), ms->_key);
      mos_send(mos_ARGV[0], mos_s(encodeAsReference_), ms->_value);
    }
    mos_map_LOOP_END;
  }
}
mos_METHOD_END

mos_METHOD(map,decodeOtherOn_)
{
  mos_map *o = SELF;
  size_t m_nslots;
  mos_map_slot *m_slots;
  static const mos_map_slot _zero;

  m_nslots = mos_INT(mos_send(mos_ARGV[0], mos_s(decodeRawInteger)));

  m_slots = mos_malloc(sizeof(m_slots[0]) * (m_nslots + 1));
  {
    size_t i;

    for ( i = 0; i < m_nslots; i ++ ) {
      m_slots[i] = _zero;
      m_slots[i]._key = mos_send(mos_ARGV[0], mos_s(decode));
      m_slots[i]._value = mos_send(mos_ARGV[0], mos_s(decode));
    }
    m_slots[i] = _zero;
  }

  o->_m_nslots = m_nslots;
  o->_m_slots = m_slots;
}
mos_METHOD_END

  mos_ANNOT_END

/*******************************************************************/

  mos_ANNOT("Category: Length")


mos_METHOD(map,length)
{
  mos_return(mos_integer_make(L));
}
mos_METHOD_END

  mos_ANNOT_END

/*******************************************************************/

  mos_ANNOT("Category: Equal")

mos_METHOD(map,_equal_)
{
#define B mos_REFT(mos_ARGV[0],mos_map)

  if ( mos_EQ(mos_RCVR, mos_ARGV[0]) ) {
    mos_return(mos_true);
  }

  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asMap));

  /* Must have the same number of slots */
  if ( L != B->_m_nslots )
    return mos_false;

  /* Must have the keys and values that are eq */
  {
    mos_map_LOOP(SELF, ms);
    {
      mos_map_slot *ns = _mos_map_find(B, ms->_key);
      if ( ! (ns && mos_EQ(ms->_value, ns->_value)) ) {
	mos_return(mos_false);
      }
    }
    mos_map_LOOP_END;
  }

  mos_return(mos_true); 
#undef B
}
mos_METHOD_END

  mos_ANNOT_END

/*******************************************************************/

  mos_ANNOT("Category: Get")


mos_METHOD(map,get_Or_)
{
  mos_map_slot *ms = _mos_map_find(M, mos_ARGV[0]);
  mos_return(ms ? ms->_value : mos_ARGV[1]);
}
mos_METHOD_END

mos_METHOD(map,get_)
{
  mos_map_slot *ms = _mos_map_find(M, mos_ARGV[0]);
  if ( ! ms ) {
    mos_return(mos_error(mos_s(rangeError), "index: %P", mos_ARGV[0]));
  }
  mos_return(ms->_value);
}
mos_METHOD_END

mos_METHOD(map,has_)
{
  mos_map_slot *ms = _mos_map_find(M, mos_ARGV[0]);
  mos_return(ms ? mos_true : mos_false);
}
mos_METHOD_END


mos_METHOD(map,getKey_)
{
  mos_map_slot *ms = _mos_map_findByValue(M, mos_ARGV[0]);
  if ( ! ms ) {
    mos_return(mos_error(mos_s(rangeError), "index: %P", mos_ARGV[0]));
  }
  mos_return(ms->_key);
}
mos_METHOD_END

mos_METHOD(map,hasValue_)
{
  mos_map_slot *ms = _mos_map_findByValue(M, mos_ARGV[0]);
  mos_return(ms ? mos_true : mos_false);
}
mos_METHOD_END


  mos_ANNOT_END

/*******************************************************************/

  mos_ANNOT("Category: Keys")


mos_METHOD(map,keysInto_)
{
  {
  mos_map_LOOP(M,ms)
    mos_send(mos_ARGV[0], mos_s(append_), ms->_key);
  mos_map_LOOP_END
  }
  
  mos_return(mos_ARGV[0]);
}
mos_METHOD_END

mos_METHOD(map,keys)
{
  mos_return(mos_send(mos_RCVR, mos_s(keysInto_), mos_vector_make(0, 0)));
}
mos_METHOD_END


  mos_ANNOT_END

/*******************************************************************/

  mos_ANNOT("Category: Values")


mos_METHOD(map,valuesInto_)
{
  {
  mos_map_LOOP(M,ms)
    mos_send(mos_ARGV[0], mos_s(append_), ms->_value);
  mos_map_LOOP_END
  }
  
  mos_return(mos_ARGV[0]);
}
mos_METHOD_END

mos_METHOD(map,values)
{
  mos_return(mos_send(mos_RCVR, mos_s(valuesInto_), mos_vector_make(0, 0)));
}
mos_METHOD_END


  mos_ANNOT_END

/*******************************************************************/

  mos_ANNOT("Category: Set")

mos_METHOD(map,set_Value_)
{
  _mos_map_setOrAdd(M, mos_ARGV[0], mos_ARGV[1]);
}
mos_METHOD_END

mos_METHOD(map,remove_)
{
  mos_return(_mos_map_remove(M, mos_ARGV[0])  ? mos_RCVR : mos_false);
}
mos_METHOD_END

mos_METHOD(map,removeValue_)
{
  mos_return(_mos_map_removeByValue(M, mos_ARGV[0]) ? mos_RCVR : mos_false);
}
mos_METHOD_END


  mos_ANNOT_END

/*******************************************************************/

  mos_ANNOT("Category: Create")

mos_METHOD(map,__COM__)
{
  mos_value mm = mos_send(mos_o(protos), mos_s(mapMaker));
  mm = mos_send(mm, mos_s(clone));
  mos_send(mm, mos_s(map_), mos_RCVR);
  mos_send(mm, mos_s(key_), mos_ARGV[0]);
  mos_return(mm);
}
mos_METHOD_END

  mos_ANNOT_END

#undef SELF
#undef M
#undef V
#undef L
#undef I

mos_OBJECT(map)
mos_OBJECT_M(map,asMap)
mos_OBJECT_M(map,clone)
mos_OBJECT_M(map,printOn_)
mos_OBJECT_M(map,printShallowOn_)
mos_OBJECT_M(map,encodeOtherOn_)
mos_OBJECT_M(map,decodeOtherOn_)
mos_OBJECT_M(map,length)
mos_OBJECT_M(map,_equal_)
mos_OBJECT_M(map,has_)
mos_OBJECT_M(map,get_)
mos_OBJECT_M(map,get_Or_)
mos_OBJECT_M(map,has_)
mos_OBJECT_M(map,getKey_)
mos_OBJECT_M(map,hasValue_)
mos_OBJECT_M(map,keysInto_)
mos_OBJECT_M(map,keys)
mos_OBJECT_M(map,valuesInto_)
mos_OBJECT_M(map,values)
mos_OBJECT_M(map,set_Value_)
mos_OBJECT_M(map,remove_)
mos_OBJECT_M(map,removeValue_)
mos_OBJECT_M(map,__COM__)
mos_OBJECT_SLOTS(map)
mos_OBJECT_END(protos,map,mos_map,basicMeta)

mos_ANNOT("Category: Create")

mos_METHOD(mapMaker,__RAR__)
{
  mos_value map = mos_send(mos_RCVR, mos_s(map));
  mos_value key = mos_send(mos_RCVR, mos_s(key));
  mos_return(mos_send(map, mos_s(set_Value_), key, mos_ARGV[0]));
}
mos_METHOD_END

mos_OBJECT(mapMaker)
mos_OBJECT_M(mapMaker,__RAR__)
mos_OBJECT_METHOD(mos_m(mapMaker,__RAR__),__COM__)
mos_OBJECT_A(map,0)
mos_OBJECT_A(key,1)
mos_OBJECT_SLOTS(mapMaker)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(protos,mapMaker,mos_map,basicMeta)

mos_ANNOT_END
mos_ANNOT_END
mos_ANNOT_END

     static mos_map_slot _mos_empty_map_slots[] = { { mos_UNINITIALIZED } };

mos_INIT(map,-20)
{
  mos_REFT(mos_o(map),mos_map)->_m_slots = _mos_empty_map_slots;
  return 0;
}

mos_ANNOT_END
mos_ANNOT_END

/***************************************************************************/
/* Map creation. */

#ifdef mos_map_make
#undef mos_map_make
#endif
mos_value mos_map_make(const mos_map *_v)
{
  return mos_MAKE_REF(_v);
}

mos_value mos_map_make1(const mos_map *_v)
{
  mos_map *x = mos_REFT(mos_clone(mos_o(map)),mos_map);
  x->_m_nslots = _v->_m_nslots;
  x->_m_slots = mos_malloc(sizeof(x->_m_slots[0]) * (_v->_m_nslots + 1));
  mos_memcpy(x->_m_slots, _v->_m_slots, sizeof(x->_m_slots[0]) * (_v->_m_nslots + 1));
  return mos_MAKE_REF(x);
}

mos_value mos_map_makev(size_t _l, va_list *_vap)
{
  mos_map_slot *_v = mos_malloc(sizeof(_v[0]) * (_l + 1));
  mos_map _m;
  size_t i;
  
  for ( i = 0; i < _l; i ++ ) {
    _v[i]._key = va_arg(*_vap,mos_value);
    _v[i]._value = va_arg(*_vap,mos_value);
    _v[i]._hits = 0;
  }
  _v[i]._key = mos_UNINITIALIZED;
  _v[i]._value = mos_UNINITIALIZED;
  _v[i]._hits = 0;
  
  _m._m_slots = _v;
  _m._m_nslots = _l;
  
  return mos_map_make1(&_m);
}

mos_value mos_map_make_(size_t _l, ...)
{
  mos_value v;
  va_list vap;
  
  va_start(vap,_l);
  v = mos_map_makev(_l, &vap);
  va_end(vap);
   
  return v;
}

