
#ifndef __rcs_id__
#ifndef __rcs_id_mos_defaults_c__
#define __rcs_id_mos_defaults_c__
static const char __rcs_id_mos_defaults_c[] = "$Id: defaults.c,v 1.2 1999-02-19 09:26:21 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include "mos/slots.h" /* _mos_addSlot() */
#include <stdlib.h> /* getenv() */
#include <string.h> /* strtoi() */
#include <ctype.h> /* isdigit() */
#include "config.h"

mos_ANNOT("Module: defaults")
mos_ANNOT("Doc: Contains system defaults.")

mos_OBJECT(defaults)
mos_OBJECT_SLOTS(defaults)
mos_OBJECT_END(system,defaults,mos_object,basicMeta)

mos_ANNOT_END
mos_ANNOT_END

mos_INIT(defaults,11)
{
  static struct {
    const char *value;
    const char *env;
    const char *arg;
    const char *sel;
  } dm[] = {
#ifndef mos_VERSION
#define mos_VERSION "?.?"
#endif
    { mos_VERSION,   "", "", "systemVersion" },

#ifndef mos_ROOT_DIR
#define mos_ROOT_DIR "."
#endif
    { mos_ROOT_DIR,   "MOS_ROOT_DIR", "mos-root-dir", "rootDir" },

#ifndef mos_SRC_SUFFIX 
#define mos_SRC_SUFFIX ".mos"
#endif
    { mos_SRC_SUFFIX, "MOS_SRC_SUFFIX", "mos-src-suffix", "srcDuffix" },

#ifndef mos_ANNOT_DIR 
#define mos_ANNOT_DIR "lib/mos/annot"
#endif
    { mos_ANNOT_DIR, "MOS_ANNOT_DIR", "mos-annot-dir", "annotDir" },

#ifndef mos_PROGNAME
#define mos_PROGNAME "mos"
#endif
    { mos_PROGNAME,  "",              "0",            "programName" },

    { 0 }
  };
  int argi;
  int i;

  /* Get values from environment */
  for ( i = 0; dm[i].sel; i ++ ) {
    const char *value = getenv(dm[i].env);
    if ( value ) 
      dm[i].value = value;
  }

  /* Get values directly from arguments */
  for ( i = 0; dm[i].sel; i ++ ) {
    if ( isdigit(dm[i].arg[0]) ) {
      argi = atoi(dm[i].arg);
      if ( argi < (*argcp) ) {
	dm[i].value = (*argvp)[argi];
      }
    }
  }

  /* Get values from options */
  for ( argi = 1; i < (*argcp); argi ++ ) {
    const char *arg = (*argvp)[argi];

    if ( arg[0] == '-' ) {
      const char *name = arg, *name_end, *value;
      size_t name_len;

      /* skip /--?/ to get to the name */
      name ++;
      if ( name[0] == '-' ) name ++;

      /* "--?NAME=VALUE", "--?NAME VALUE", "--?NAME" "VALUE" */
      name_end = strchr(name, '=');
      if ( ! name_end ) {
	name_end = strchr(name, ' ');
      }

      /* Find the end of the name and the value */
      if ( name_end ) {
	value = name_end + 1;
      } else {
	/* value must be in next arg */
	name_end = strchr(name, '\0');
	if ( ++ argi < (*argcp) ) 
	  value = (*argvp)[argi];
      }

      name_len = name_end - name;

      /* scan the default map table for a match */
      for ( i = 0; dm[i].sel; i ++ ) {
	if ( dm[i].env[0] && strlen(dm[i].env) == name_len && strncmp(name, dm[i].env, name_len) == 0 ) {
	  dm[i].value = value; 
	  break;
	}
	if ( dm[i].arg[0] && (! isdigit(dm[i].arg[0])) && strlen(dm[i].arg) == name_len && strncmp(name, dm[i].arg, name_len) == 0 ) {
	  dm[i].value = value; 
	  break;
	}
      }
    }
  }

  /* Install values in defaults object */
  for ( i = 0; dm[i].sel; i ++ ) {
#if 0
    fprintf(stderr, "mos: default: %s %s %s = %s\n", dm[i].env, dm[i].arg, dm[i].sel, dm[i].value);
#endif

    _mos_addSlot(mos_o(defaults), mos_selector_make(dm[i].sel), mos_send(mos_string_make_(dm[i].value), mos_s(deepen)));
  }

  return 0;
}
