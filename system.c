#ifndef __rcs_id__
#ifndef __rcs_id_mos_system_c__
#define __rcs_id_mos_system_c__
static const char __rcs_id_mos_system_c[] = "$Id: system.c,v 1.3 1999-12-26 20:06:04 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"

/**********************************************************************/

mos_ANNOT("Module: system")

/**********************************************************************/

mos_ANNOT("Doc: Contains the system meta objects.")
mos_OBJECT(metas)
mos_OBJECT_SLOTS(metas)
mos_OBJECT_END(system,metas,mos_object,basicMeta)
mos_ANNOT_END

/**********************************************************************/

mos_ANNOT("Doc: Contains the system behavior delegates.")
mos_OBJECT(behaviors)
mos_OBJECT_SLOTS(behaviors)
mos_OBJECT_END(system,behaviors,mos_object,basicMeta)
mos_ANNOT_END

/**********************************************************************/

mos_ANNOT("Doc: Contains the system prototype objects from which new objects are created by cloning.")
mos_OBJECT(protos)
mos_OBJECT_SLOTS(protos)
mos_OBJECT_END(system,protos,mos_object,basicMeta)
mos_ANNOT_END

/**********************************************************************/

mos_ANNOT("Doc: Contains the one-of-a-kind system objects, like @T, @F, @U, and @E.  These objects are never cloned.")
mos_OBJECT(constants)
mos_OBJECT_SLOTS(constants)
mos_OBJECT_END(system,constants,mos_object,basicMeta)
mos_ANNOT_END

/**********************************************************************/

mos_ANNOT("Doc: Is the object clone to create new objects using it object creation syntax.")
mos_OBJECT(empty)
mos_OBJECT_SLOTS(empty)
mos_OBJECT_END(protos,empty,mos_object,basicMeta)
mos_ANNOT_END

/**********************************************************************/

mos_ANNOT("Doc: Contains all system objects.")
mos_OBJECT(system)
mos_OBJECT_SLOTS(system)
mos_OBJECT_END(_,system,mos_object,basicMeta)
mos_ANNOT_END

/**********************************************************************/

mos_ANNOT("Doc: Is the lobby (user namespace).  It always contains a reference to the <code>(_ system)</code> object and itself;  This is where you create new slots.  Example:<BR>\
\
     _ _addSlots: {\
       foo -> 5;\
       bar;\
       baz = 2;\
     };\
\
     _ _loadFile: \"src/delegate.mos\";\
")
mos_OBJECT(_)
mos_OBJECT_SLOTS(_)
mos_OBJECT_END(_,_,mos_object,basicMeta)
mos_ANNOT_END

/**********************************************************************/

mos_ANNOT_END

static struct mos_init_s {
  const char *name;
  mos_value s;
  mos_value n;
  mos_value o;
} _inits[] = {
#define mos_DEF_OBJECT(S,O,T,M) { #S " " #O, mos_o(S), mos_s(O), mos_o(O) },
#include "mos/obj.def"
  { 0 }
};

#ifndef debugSystem
#define debugSystem 0
#endif

mos_INIT(system,50)
{
  if ( debugSystem ) fprintf(stderr, "{ ");
  {
    struct mos_init_s *i;

    for ( i = _inits; i->name; i ++ ) {
      if ( debugSystem ) {
      	fprintf(stderr, "(%s) ", i->name);
        fflush(stderr);
      } 
      _mos_addSlot(i->s, i->n, i->o);
    }
  }
  
  if ( debugSystem ) fprintf(stderr, "} ");
  
  return 0;
}

