#ifndef __rcs_id__
#ifndef __rcs_id_mos_selesc_c__
#define __rcs_id_mos_selesc_c__
static const char __rcs_id_mos_selesc_c[] = "$Id: selesc.c,v 1.1 2001-09-18 02:46:05 stephens Exp $";
#endif
#endif /* __rcs_id__ */

/*
** Escape C symbols into MOS selector names.
*/


#include "mos/mos.h"
#include "mos/constant.h"
#include <string.h>


extern const char *_mos_selector_escape(const char *x);


int main(int argc, char **argv)
{
  int i;

  if ( argc > 1 ) {
    for ( i = 1; i < argc; ++ i ) {
      fprintf(stdout, "%s\t%s\n", argv[i], _mos_selector_escape(argv[i]));
    }
  } else {
    char buf[256];

    while ( fgets(buf, sizeof(buf) - 1, stdin) ) {
      {
	char *r;
	while ( (r = strchr(buf, '\n')) || (r = strchr(buf, '\r')) ) {
	  *r = '\0';
	}
      }

      fprintf(stdout, "%s\t%s\n", buf, _mos_selector_escape(buf));
    }

  }


  return 0;
}

