#ifndef __rcs_id__
#ifndef __rcs_id_mos_namedobj_c__
#define __rcs_id_mos_namedobj_c__
static const char __rcs_id_mos_namedobj_c[] = "$Id: namedobj.c,v 1.4 2000-03-21 07:13:44 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

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
#define mos_DEF_METHOD(O,S) { 0, mos_m(O,S), (int) "@M " #O " " #S },
#include "mos/meth.def"
#define mos_DEF_OBJECT(S,O,T,M) \
{ 0, mos_o(O), (int) "@O " #S " " #O }, \
{ 0, mos_od(O), (int) "@OD " #S " " #O }, \
{ 0, mos_om(O), (int) "@OM " #S " " #O }, \
{ 0, mos_o(O), (int) "@O " #O }, \
{ 0, mos_od(O), (int) "@OD " #O }, \
{ 0, mos_om(O), (int) "@OM " #O },
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
  
  for ( ms = map._m_slots; ms->_hits; ms ++ ) {
    char *s = (void*) mos_strdup((char*) ms->_hits);
    ms->_hits = 0;
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
