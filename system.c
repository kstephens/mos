#include "mos/mos.h"
#include "mos/slots.h"

mos_ANNOT("Module: system")
mos_ANNOT("Doc: Defines the (_ system) object.")

mos_ANNOT("Category: Meta")
mos_ANNOT("Doc: Contains the system meta objects.")
mos_OBJECT(metas)
mos_OBJECT_SLOTS(metas)
mos_OBJECT_END(system,metas,mos_object,basicMeta)
mos_ANNOT_END
mos_ANNOT_END

mos_ANNOT("Category: Behaviors")
mos_ANNOT("Doc: Contains the system behavior delegates.")
mos_OBJECT(behaviors)
mos_OBJECT_SLOTS(behaviors)
mos_OBJECT_END(system,behaviors,mos_object,basicMeta)
mos_ANNOT_END
mos_ANNOT_END

mos_ANNOT("Category: Prototypes")
mos_ANNOT("Doc: Contains the system prototype objects from which new objects are created by cloning.")
mos_OBJECT(protos)
mos_OBJECT_SLOTS(protos)
mos_OBJECT_END(system,protos,mos_object,basicMeta)
mos_ANNOT_END
mos_ANNOT_END

mos_ANNOT("Category: Constant")
mos_ANNOT("Doc: Contains the one-of-a-kind system objects, like @T, @F, @U, and @E.  These objects are never cloned.")
mos_OBJECT(constants)
mos_OBJECT_SLOTS(constants)
mos_OBJECT_END(system,constants,mos_object,basicMeta)
mos_ANNOT_END
mos_ANNOT_END

mos_ANNOT("Category: Empty")
mos_ANNOT("Doc: Is the object clone to create new objects using it object creation syntax.")
mos_OBJECT(empty)
mos_OBJECT_SLOTS(empty)
mos_OBJECT_END(protos,empty,mos_object,basicMeta)
mos_ANNOT_END
mos_ANNOT_END

mos_ANNOT("Category: System")
mos_ANNOT("Doc: Contains all system objects.")
mos_OBJECT(system)
mos_OBJECT_SLOTS(system)
mos_OBJECT_END(_,system,mos_object,basicMeta)
mos_ANNOT_END
mos_ANNOT_END

mos_ANNOT("Category: Core")
mos_ANNOT("Doc: Contains all core system objects.")
mos_OBJECT(core)
mos_OBJECT_SLOTS(core)
mos_OBJECT_END(system,core,mos_object,basicMeta)
mos_ANNOT_END
mos_ANNOT_END

mos_ANNOT("Category: Lobby")
mos_ANNOT("Doc: Is the lobby (user namespace).  \
It always contains a reference to the <code>(_ system)</code> object and itself.  \
The lobby is where you create new slots.")
mos_ANNOT("Example:<BR>\
<code>\
     _ _addSlots: {\n\
       foo -> 5;\n\
       bar;\n\
       baz = 2;\n\
     };\n\
\n\
     _ _loadFile: \"src/delegate.mos\";\n\
</code>\
")
mos_OBJECT(_)
mos_OBJECT_SLOTS(_)
mos_OBJECT_END(_,_,mos_object,basicMeta)
mos_ANNOT_END
mos_ANNOT_END
mos_ANNOT_END

mos_ANNOT_END
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

      _mos_addSlot(mos_o(core), i->n, i->o);
    }
  }
  
  if ( debugSystem ) fprintf(stderr, "} ");
  
  return 0;
}

