
#ifndef __rcs_id__
#ifndef __rcs_id_mos_annot_c__
#define __rcs_id_mos_annot_c__
static const char __rcs_id_mos_annot_c[] = "$Id: annot.c,v 1.3 1999-12-26 20:05:37 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include "mos/slots.h"
#include <string.h> /* strcpy, strcat */

#define MAP mos_REFT(mos_MIMPL,mos_objectDesc)->_annot

mos_ANNOT("Module: annot")
mos_ANNOT("Doc: Annotation.")

mos_ANNOT("Doc: Sets the annotation for the selector.")
mos_METHOD(objectDesc,__annot_For_)
{
  if ( ! mos_INITIALIZED(MAP) ) {
    MAP = mos_clone(mos_o(map));
  }
  mos_send(MAP, mos_s(set_Value_), mos_ARGV[1], mos_ARGV[0]);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Returns the annotation for the selector.")
mos_METHOD(objectDesc,__annotFor_)
{
  mos_value str = mos_undef;
  mos_value meth = mos_undef;

  /* Attempt to auto-load annotations */
  if ( ! mos_INITIALIZED(MAP) ) {
    mos_send(mos_o(annot), mos_s(_loadAnnotFileFor_), mos_RCVR);
  }

  /* Attempt to get the objectDesc annotation map */
  if ( mos_INITIALIZED(MAP) ) {
    str = mos_send(MAP, mos_s(get_Or_), mos_ARGV[0], mos_undef);
  }

  /* Try to get the object's method's annotation */
  if ( mos_EQ(str, mos_undef) ) {
    str = mos_send(mos_RCVR, mos_s(methods));
    meth = mos_send(str, mos_s(get_Or_), mos_ARGV[0], mos_undef);
  }

  if ( mos_NE(meth, mos_undef) ) {
    str = mos_send(meth, mos_s(_annot));
  }

  mos_return(str);
}
mos_METHOD_END
mos_ANNOT_END

#define OBL_ANNOT_SEL mos_o(selector)

mos_ANNOT("Doc: Returns the annotation.")
mos_METHOD(objectDesc,__annot)
{
  /* Attempt to auto-load annotations */
  if ( ! mos_INITIALIZED(MAP) ) {
    mos_send(mos_o(annot), mos_s(_loadAnnotFileFor_), mos_RCVR);
  }

  if ( mos_INITIALIZED(MAP) ) {
    mos_return(mos_send(mos_RCVR, mos_s(__annotFor_), OBL_ANNOT_SEL));
  } else {
    mos_return(mos_undef);
  }
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Sets the annotation.")
mos_METHOD(objectDesc,__annot_)
{
  mos_send(mos_RCVR, mos_s(__annot_For_), mos_ARGV[0], OBL_ANNOT_SEL);
}
mos_METHOD_END
mos_ANNOT_END

#undef OBL_ANNOT_SEL
#undef MAP


/**************************************************************************/

mos_ANNOT("Doc: Returns the object's annotation.")
mos_METHOD(defaultBehavior,_annot)
{
  mos_return(mos_send(mos_send(mos_RCVR, mos_s(_objectDesc)), mos_s(__annot)));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Returns the object's slot's annotation.")
mos_METHOD(defaultBehavior,_annotFor_)
{
  mos_return(mos_send(mos_send(mos_RCVR, mos_s(_objectDesc)), mos_s(__annotFor_), mos_ARGV[0]));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Sets the object's  annotation.")
mos_METHOD(defaultBehavior,_annot_)
{
  mos_send(mos_send(mos_RCVR, mos_s(_objectDesc)), mos_s(__annot_), mos_ARGV[0]); 
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Sets the object's slot's annotation.")
mos_METHOD(defaultBehavior,_annot_For_)
{
  mos_send(mos_send(mos_RCVR, mos_s(_objectDesc)), mos_s(__annot_For_), mos_ARGV[0], mos_ARGV[1]); 
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: This method handles auto-loading of annotation files for stati system objects.  The annotation source files and annot.def are created by annot.pl.  The contents of annot.def are compiled into annot.c to create the \"loadedMap\" map object.  The \"loadedMap\" object maps object descriptors to their annotation filenames.  This map is used to keep track of which annotation files are need to be loaded and which ones are already loaded.")
mos_METHOD(annot,_loadAnnotFileFor_)
{
  mos_value o = mos_ARGV[0];
  mos_value loadedMap = mos_send(mos_RCVR, mos_s(loadedMap));

  {
    mos_value loaded;

    /* Has is been loaded before? */
    loaded = mos_send(loadedMap, mos_s(get_Or_), o, mos_false);\

    if ( ! (mos_EQ(loaded, mos_false) || mos_EQ(loaded, mos_true)) ) {
    /* Attempt to load the file named in loaded */
      char fpath[1024];
      FILE *fp;

      /* Mark it as loaded (or attempted to be loaded) */
      mos_send(loadedMap, mos_s(set_Value_), o, mos_false);

      /* Attempt to find a annot file */
      strcpy(fpath, mos_string_V(mos_send(mos_o(defaults), mos_s(rootDir))));
      strcat(fpath, "/");
      strcat(fpath, mos_string_V(loaded));

      if ( (fp = fopen(fpath, "r")) ) {
	fclose(fp);

	mos_send(o, mos_s(_loadFile_), mos_string_make_(fpath));
	mos_send(loadedMap, mos_s(set_Value_), o, mos_true);
      } else {
	fprintf(stderr, "mos: warning: cannot load annotation file \"%s\"\n", fpath);
	fclose(fp);
      }
    }
  }
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(annot)
mos_OBJECT_M(annot,_loadAnnotFileFor_)
mos_ANNOT("Doc: A map of static object descriptors to annotation maps.")
mos_OBJECT_A(loadedMap,0)
mos_ANNOT_END
mos_OBJECT_SLOTS(annot)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(system,annot,mos_object,basicMeta)

mos_ANNOT_END
mos_ANNOT_END

mos_INIT(annotation,98)
{
  /* Keep an map of all static object descriptors to dynamically load annotations */
  {
    mos_value loadedMap;

    loadedMap = mos_send(mos_o(map), mos_s(clone));
    mos_send(mos_o(annot), mos_s(loadedMap_), loadedMap);
    
#define mos_DEF_ANNOT(O,F) \
    mos_send(loadedMap, mos_s(set_Value_), mos_od(O), mos_string_make_(F));
#include "annot.def"
  }

#define AM(O,M) _mos_addMethod(mos_o(O), mos_s(M), mos_m(O,M));

AM(objectDesc,__annot)
AM(objectDesc,__annot_)
AM(objectDesc,__annot_For_)
AM(objectDesc,__annotFor_)

AM(defaultBehavior,_annot)
AM(defaultBehavior,_annotFor_)
AM(defaultBehavior,_annot_)
AM(defaultBehavior,_annot_For_)

  return 0;
}

