
#ifndef __rcs_id__
#ifndef __rcs_id_old_mapobj_c__
#define __rcs_id_old_mapobj_c__
static const char __rcs_id_old_mapobj_c[] = "$Id: mapobj.c,v 1.2 1999-02-19 09:26:35 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos.h"
#include "memcpy.h"
#include "mapfuncs.h"

#define SELF mos_REFT(mos_MIMPL,mos_mapobj)
#define M SELF->_mp
#define V M->_slots
#define L M->_nslots

#define I mos_INT(mos_ARGV[0])

mos_METHOD(map,asMap)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END

mos_METHOD(map,clone)
{
  void *v;
  
  mos_MIMPL = mos_clone(mos_MIMPL);
  v = mos_malloc(sizeof(V[0]) * (L + 1));
  mos_memcpy(v, V, sizeof(V[0]) * (L + 1));
  M = &(mos_REFT(mos_MIMPL,mos_mapobj)->_m);
  V = v;
  
  mos_return(mos_MIMPL);
}
mos_METHOD_END

mos_METHOD(map,printOn_)
{
  size_t i;
  mos_value ss;

  mos_ARGV[0] = mos_printf(mos_ARGV[0], "");
  
  ss = mos_send(mos_ARGV[0], mos_s(asStructuredStream));
  
  mos_printf(ss, "%[(@M");
  
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

mos_METHOD(map,length)
{
  mos_return(mos_MAKE_INT(L));
}
mos_METHOD_END

mos_METHOD(map,_equal_)
{
  if ( mos_NE(mos_RCVR, mos_ARGV[0]) ) {
    size_t i;

    mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asMap));

    if ( M != mos_REFT(mos_ARGV[0],mos_mapobj)->_mp ) {
      if ( L != mos_REFT(mos_ARGV[0],mos_mapobj)->_mp->_nslots )
	return mos_false;
  #if 0
      for ( i = 0; i < L; i ++ ) {
	if ( ! mos_EQUAL(V[i], mos_REFT(mos_ARGV[0],mos_mapobj)->_v[i]) )
	  mos_return(mos_false);
      }
  #endif
    }
  }
  mos_return(mos_true); 
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


mos_METHOD(map,__COM__)
{
  mos_value mm = mos_send(mos_RCVR, mos_s(mapMaker));
  mm = mos_send(mm, mos_s(clone));
  mos_send(mm, mos_s(map_), mos_RCVR);
  mos_send(mm, mos_s(key_), mos_ARGV[0]);
  mos_return(mm);
}
mos_METHOD_END

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
mos_OBJECT_M(map,length)
mos_OBJECT_M(map,_equal_)
mos_OBJECT_M(map,has_)
mos_OBJECT_M(map,get_)
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
mos_OBJECT_END(map,mapMaker,mos_map,basicMeta)

mos_INIT(map,0)
{
  mos_mapobj_MS(mos_o(map)) = mos_map_EMPTY;
  mos_mapobj_M(mos_o(map)) = &mos_mapobj_MS(mos_o(map));
  
  return 0;
}

mos_value mos_map_make(const mos_map *_v)
{
  mos_mapobj *x = mos_REFT(mos_clone(mos_o(map)),mos_mapobj);
  x->_mp = (mos_map*) _v;
  return mos_MAKE_REF(x);
}

mos_value mos_map_make1(const mos_map *_v)
{
  mos_mapobj *x = mos_REFT(mos_clone(mos_o(map)),mos_mapobj);
  x->_m = *_v;
  x->_mp = &x->_m;
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
  
  _m._slots = _v;
  _m._nslots = _l;
  
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

