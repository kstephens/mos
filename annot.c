#ifndef __rcs_id__
#ifndef __rcs_id_mos_annot_c__
#define __rcs_id_mos_annot_c__
static const char __rcs_id_mos_annot_c[] = "$Id: annot.c,v 1.5 2001-09-18 02:46:47 stephens Exp $";
#endif
#endif /* __rcs_id__ */


#include "mos/mos.h"
#include "mos/slots.h"
#include <string.h> /* strcpy, strcat */
#include <ctype.h>


static
mos_value mos_parse_annotation(mos_value x)
{
  const char *b = mos_string_V(x);
  const char *e = b + mos_string_L(x);
  mos_value parent = mos_o(annot), child = mos_undef;
  const char *sb = b;
  const char *se = b;
  int done = 0;

  do {
    if ( *se == '\x7f' || se == e ) {
      /* Got annotation element! */
      mos_value childrenMap, str;
      const char *sn = se + 1;

      if ( se == e )
	done = 1;

      /* Remove leading whitespace. */
      while ( sb < se ) {
	if ( ! isspace(*sb) )
	  break;
	++ sb;
      }

      /* Remove trailing whitespace. */
      while ( se > sb ) {
	if ( ! isspace(*se) )
	  break;
	-- se;
      }

      /* Ignore null elements. */
      if ( sb == se ) {
	sb = se = sn;
	continue;
      }

      /* Get parent's children map. */
      childrenMap = mos_send(parent, mos_s(childrenMap));

      /* Create an element string. */
      str = mos_string_make(sb, se - sb);
      
      // mos_printf(mos_FILE(err), "mos_parse_annotation(%P, %P)\n", x, str);
     
      /* Look up element string in parent */
      child = mos_send(childrenMap, mos_s(getEqual_Or_), str, mos_undef);

      /* Create and install a new child annotation, if needed. */
      if ( mos_EQ(child, mos_undef) ) {
	
        // fprintf(stderr, "parse_annot(%s)\n", sb);

	/* Create new annotation. */
	child = mos_send(parent, mos_s(clone));

	/* Attach child to parent. */
	mos_send(child, mos_s(parent_), parent);

	/* Name child. */
        mos_send(str, mos_s(deepen));
	mos_send(child, mos_s(name_), str);

	/* Attach parent to child. */
	mos_send(childrenMap, mos_s(setEqual_Value_), str, child);
      }

      /* Go to next parent. */
      parent = child;

      /* Next element. */
      sb = se = sn;
    } else {
      /* Scan for termination. */
      ++ se;
    }
  } while ( ! done );

  return child;
}


/******************************************************************/


#define MAP mos_REFT(mos_MIMPL,mos_objectDesc)->_annot



/******************************************************************/


mos_ANNOT("Module: annot")
mos_ANNOT("Doc: Annotations.\n\
Annotations are structured documentation strings attached to objects.   Annotations can be nested.  Nested annotations have parents and children.\n")


/******************************************************************/


  mos_ANNOT("Category: Coerce")


mos_METHOD(annot,asAnnotation)
{
  mos_return(mos_RCVR);
}
mos_METHOD_END


mos_METHOD(string,asAnnotation)
{
  mos_return(mos_parse_annotation(mos_RCVR));
}
mos_METHOD_END


  mos_ANNOT_END


/**************************************************************************/


mos_ANNOT("Category: Clone")


mos_METHOD(annot,clone)
{
  mos_value x = mos_clone(mos_RCVR);
  mos_send(x, mos_s(childrenMap_), mos_clone(mos_o(map)));
  mos_return(x);
}
mos_METHOD_END


mos_ANNOT_END


/**************************************************************************/


mos_ANNOT("Category: Print")


mos_METHOD(annot,printOn_)
{
  mos_value x = mos_RCVR;
  mos_value str = mos_string_make(0, 0);

  while ( mos_NE(x, mos_o(annot)) ) {
    mos_send(str, mos_s(insert_), mos_printf(mos_undef, ",\n %P", mos_send(x, mos_s(name))));
    x = mos_send(x, mos_s(parent));
  }

  mos_printf(mos_ARGV[0], "(@A%W\n)", str);
}
mos_METHOD_END


mos_METHOD(annot,printShallowOn_)
{
  mos_return(mos_send(mos_RCVR, mos_s(printOn_), mos_ARGV[0]));
}
mos_METHOD_END


mos_ANNOT_END


/**************************************************************************/


mos_ANNOT("Category: Children")


mos_ANNOT("Doc: Get the annotation's children.")
mos_METHOD(annot,children)
{
  mos_value x = mos_send(mos_RCVR, mos_s(childrenMap));
  x = mos_send(x, mos_s(values));
  x = mos_send(x, mos_s(foreachApplySend_), mos_s(name));
  mos_return(x);
}
mos_METHOD_END
mos_ANNOT_END


mos_ANNOT("Doc: Get the annotation's child by name.")
mos_METHOD(annot,child_)
{
  mos_value x = mos_send(mos_RCVR, mos_s(childrenMap));
  mos_return(mos_send(x, mos_s(getEqual_), mos_ARGV[0]));
}
mos_METHOD_END
mos_ANNOT_END


mos_ANNOT_END


/******************************************************************/


mos_ANNOT("Category: Annotation")


/******************************************************************/


mos_ANNOT("Doc: Sets the annotation for the selector.")
mos_METHOD(objectDesc,annotations)
{
  /* Attempt to auto-load annotations */
  if ( ! mos_INITIALIZED(MAP) ) {
    mos_send(mos_o(annot), mos_s(_loadAnnotFileFor_), mos_RCVR);
  }

  mos_return(mos_INITIALIZED(MAP) ? MAP : mos_undef);
}
mos_METHOD_END
mos_ANNOT_END


mos_ANNOT("Doc: Sets the annotation for the selector.")
mos_METHOD(objectDesc,__annot_For_)
{
  if ( ! mos_INITIALIZED(MAP) ) {
    MAP = mos_clone(mos_o(map));
  }
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asAnnotation));

  /* Force the key to be a selector. See sel.pl. */
  mos_ARGV[1] = mos_send(mos_ARGV[1], mos_s(asSelector));

  mos_send(MAP, mos_s(set_Value_), mos_ARGV[1], mos_ARGV[0]);
}
mos_METHOD_END
mos_ANNOT_END


mos_ANNOT("Doc: Returns the annotation for the selector.")
mos_METHOD(objectDesc,__annotFor_)
{
  mos_value map = mos_send(mos_RCVR, mos_s(annotations));
  mos_value str = mos_undef;
  mos_value meth = mos_undef;

  /* Attempt to get the objectDesc annotation map */
  if ( mos_NE(map, mos_undef) ) {
    str = mos_send(map, mos_s(get_Or_), mos_ARGV[0], mos_undef);
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
  mos_value map = mos_send(mos_RCVR, mos_s(annotations));

  if ( mos_NE(map, mos_undef) ) {
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
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asAnnotation));
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


mos_ANNOT("Doc: Sets the object's annotation.")
mos_METHOD(defaultBehavior,_annot_)
{
  mos_send(mos_send(mos_RCVR, mos_s(_objectDesc)), mos_s(__annot_), mos_ARGV[0]); 
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


mos_ANNOT("Doc: Sets the object's slot's annotation.")
mos_METHOD(defaultBehavior,_annot_For_)
{
  mos_send(mos_send(mos_RCVR, mos_s(_objectDesc)), mos_s(__annot_For_), mos_ARGV[0], mos_ARGV[1]); 
}
mos_METHOD_END
mos_ANNOT_END


/**************************************************************************/


mos_ANNOT_END /* End of "Catagory: Annotation" */


/**************************************************************************/


mos_ANNOT("Category: Loading")


mos_ANNOT("Doc: Loads an annotation file.")
mos_METHOD(annot,_loadAnnotFile_)
{
  char fpath[1024];
  FILE *fp;

  mos_value loaded = mos_ARGV[0];

  /* Attempt to find a annot file */
  strcpy(fpath, mos_string_V(mos_send(mos_o(defaults), mos_s(rootDir))));
  strcat(fpath, "/");
  strncat(fpath, mos_string_V(loaded), mos_string_L(loaded));
  
  if ( (fp = fopen(fpath, "r")) ) {
    fclose(fp);
    
    fprintf(stderr, "mos: loading annotation file \"%s\"\n", fpath);

    mos_send(mos_o(core), mos_s(_loadFile_), mos_string_make_(fpath));

    mos_return(mos_true);
  } else {
    fprintf(stderr, "mos: warning: cannot load annotation file \"%s\"\n", fpath);
    fclose(fp);
    mos_return(mos_false);
  }
}
mos_METHOD_END
mos_ANNOT_END


mos_ANNOT("Doc: This method handles auto-loading of annotation files for stati system objects.  \
The annotation source files and annot.def are created by annot.pl.  \
The contents of annot.def are compiled into annot.c to create the \"loadedMap\" map object.  \
The \"loadedMap\" object maps object descriptors to their annotation filenames.  \
This map is used to keep track of which annotation files are need to be loaded and which ones are already loaded.")
mos_METHOD(annot,_loadAnnotFileFor_)
{
  mos_value o = mos_ARGV[0];
  mos_value loadedMap = mos_send(mos_RCVR, mos_s(loadedMap));
  mos_value loaded;

  /* Has is been loaded before? */
  loaded = mos_send(loadedMap, mos_s(get_Or_), o, mos_false);
							       
  if ( ! (mos_EQ(loaded, mos_false) || mos_EQ(loaded, mos_true)) ) {
    /* Attempt to load the file named in loaded */
    mos_value v = loaded;

    /* Mark it as loaded (or attempted to be loaded) */
    mos_send(loadedMap, mos_s(set_Value_), o, mos_false);
    
    loaded = mos_false;
    
    /* Load the file. */
    mos_vector_LOOP(v, vp);
    {
      loaded = mos_send(mos_RCVR, mos_s(_loadAnnotFile_), *vp);
      if ( mos_EQ(loaded, mos_false) ) {
	break;
      }
    }
    mos_vector_LOOP_END;
    
    /* Mark it as loaded (@t) or unloadable (@f). */
    mos_send(loadedMap, mos_s(set_Value_), o, loaded);
  }
}
mos_METHOD_END
mos_ANNOT_END


mos_ANNOT_END


/******************************************************************/


mos_OBJECT(annot)
mos_OBJECT_M(annot,clone)
mos_OBJECT_M(annot,asAnnotation)
mos_OBJECT_M(annot,printOn_)
mos_OBJECT_M(annot,printShallowOn_)
mos_OBJECT_M(annot,children)
mos_OBJECT_M(annot,child_)
mos_OBJECT_M(annot,_loadAnnotFileFor_)
mos_OBJECT_M(annot,_loadAnnotFile_)


mos_ANNOT("Category: Loading")
mos_ANNOT("Doc: A map of static object descriptors to annotation files.")
mos_OBJECT_A(loadedMap,0)
mos_ANNOT_END
mos_ANNOT_END


mos_ANNOT("Category: Tree")
mos_ANNOT("Doc: The name of this annotation.")
mos_OBJECT_A(name,1)
mos_ANNOT_END


mos_ANNOT("Doc: The parent of this annotation.")
mos_OBJECT_A(parent,2)
mos_ANNOT_END


mos_ANNOT("Doc: A map of child annotations by their name.")
mos_OBJECT_A(childrenMap,3)
mos_ANNOT_END
mos_ANNOT_END


mos_OBJECT_SLOTS(annot)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(system,annot,mos_object,basicMeta)


/******************************************************************/


mos_ANNOT_END
mos_ANNOT_END


/******************************************************************/


static mos_value mos_string_split(const char *b, const char *e, int c, mos_value v)
{
  // mos_printf(mos_FILE(err), "mos_string_split(%s)\n", b);

  while ( b != e ) {
    const char *sb = b;
    const char *se = sb;
    const char *nb = e;

    while ( se != e ) {
      if ( *se == c ) {
	nb = se + 1;
	break;
      }
      ++ se;
    }

    {
      mos_value str = mos_string_make(sb, se - sb);

      // mos_printf(mos_FILE(err), "mos_string_split: %W\n", str);

      mos_send(v, mos_s(append_), str);
    }
    
    b = nb;
  }

  // mos_printf(mos_FILE(err), "mos_string_split: v = %W\n", v);
 
  return v;
}


static void add_loadedMap(mos_value loadedMap, mos_value o, const char *f)
{
  mos_value v = mos_vector_make(0, 0);
  v =  mos_string_split(f, strchr(f, '\0'), '\t', v);
  mos_send(loadedMap, mos_s(set_Value_), o, v);
}


mos_INIT(annotation,98)
{
  /* Initialize root annotation. */
  mos_send(mos_o(annot), mos_s(name_), mos_string_make_(""));
  mos_send(mos_o(annot), mos_s(childrenMap_), mos_send(mos_o(map), mos_s(clone)));

  /* Keep an map of all static object descriptors to dynamically load annotations */
  {
    mos_value loadedMap;

    loadedMap = mos_send(mos_o(map), mos_s(clone));
    mos_send(mos_o(annot), mos_s(loadedMap_), loadedMap);
    
#define mos_DEF_ANNOT(O,F) \
   add_loadedMap(loadedMap, mos_od(O), F);

#include "annot.def"
  }

  return 0;
}

