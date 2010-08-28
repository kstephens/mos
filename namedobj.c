#include "mos/mos.h"
#include "mos/mapfuncs.h"

#include <string.h>

mos_ANNOT("Module: namedobj")
mos_ANNOT("Doc: \
All system (non-coder-able) objects have \"@\" as a named object prefix: \n\
  Methods have names of \"@M <obj> <sel>\" \n\
  Protos have names of \"@O <parent> <obj>\" \n\
  Proto descriptors have names of \"@OD <parent> <obj>\" \n\
  Proto method maps have names of \"@OM <parent> <obj>\" \n\
By forcing all static system objects to be named objects, we insure the coder object will properly decode properly into the local environment. \n\
")

static mos_map_slot map_slots[] = {
#define mos_DEF_METHOD(O,S)			\
  { (mos_value) "@M " #O " " #S, mos_m(O,S) },
#include "mos/meth.def"

#define mos_DEF_OBJECT(S,O,T,M)						\
  { (mos_value) "@O " #S " " #O, mos_o(O) },				\
  { (mos_value) "@OD " #S " " #O, mos_od(O) },				\
  { (mos_value) "@OM " #S " " #O, mos_om(O) },				\
  { (mos_value) "@O " #O, mos_o(O) },					\
  { (mos_value) "@OD " #O, mos_od(O) },					\
  { (mos_value) "@OM " #O, mos_om(O) },
#include "mos/obj.def"

{ 0 },
{ 0 },
{ 0 },
{ 0 },
{ 0 },
{ 0 },
{ 0 },
{ 0 }
};

static mos_map map = { 
  mos_object_HDR_INIT_STATIC(map),
  map_slots, 
  mos_ARRAY_LEN(map_slots) - 1
};

mos_INIT(namedobject,-1)
{
  mos_map_slot *ms;
  
  /* Rewrite keys as string objects. */
  for ( ms = map._m_slots; ms->_key; ms ++ ) {
    char *s = (void*) mos_strdup((char*) ms->_key);
    ms->_key = mos_string_make(s, strlen(s));
  }
  
  return 0;
}

mos_value mos_object_name(mos_value obj)
{
  mos_map_slot *ms;
  
  return (ms = _mos_map_findByValue(&map, obj)) ? ms->_key : mos_false;
}

mos_value mos_object_named(mos_value name)
{
  mos_map_slot *ms;
  
  return (ms = _mos_map_findEqual(&map, name)) ? ms->_value : mos_undef;
}

mos_value mos_name_object(mos_value name, mos_value obj)
{
  mos_map_slot *ms;

  if ( (ms = _mos_map_findEqual(&map, name)) ) {
    return(mos_error(mos_s(namedError), "name is already used"));
  }
  _mos_map_setOrAdd(&map, name, obj);
  return(name);
}

mos_value mos_name_object_(const char *n1, const char *n2, mos_value obj)
{
  char buf[256];
  sprintf(buf, "%s %s", n1, n2);
  return mos_name_object(mos_string_make_(mos_strdup(buf)), obj);
}

/**********************************************************************/

mos_METHOD(named,objectNamed_)
{
  mos_return(mos_object_named(mos_ARGV[0]));
}
mos_METHOD_END

mos_METHOD(named,objectName_)
{
  mos_return(mos_object_name(mos_ARGV[0]));
}
mos_METHOD_END

mos_METHOD(named,name_Object_)
{
  mos_return(mos_name_object(mos_ARGV[0], mos_ARGV[1]));
}
mos_METHOD_END

mos_METHOD(named,map)
{
  mos_return(mos_MAKE_REF(&map));
}
mos_METHOD_END

mos_OBJECT(named)
mos_OBJECT_M(named,objectNamed_)
mos_OBJECT_M(named,objectName_)
mos_OBJECT_M(named,name_Object_)
mos_OBJECT_M(named,map)
mos_OBJECT_SLOTS(named)
mos_OBJECT_END(system,named,mos_object,basicMeta);

mos_ANNOT_END
mos_ANNOT_END
