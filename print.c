
#ifndef __rcs_id__
#ifndef __rcs_id_mos_print_c__
#define __rcs_id_mos_print_c__
static const char __rcs_id_mos_print_c[] = "$Id: print.c,v 1.2 1999-02-19 09:26:24 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include <stdarg.h>
#include <string.h>

mos_value mos_vprintf_(mos_value stream, mos_value format, va_list *vap)
{
  const char *b, *e, *ef;
  
  if ( mos_EQ(stream,mos_undef) ) {
    stream = mos_string_make(0,0);
  }
  
  format = mos_send(format, mos_s(asString));
  
  e = mos_string_V(format);
  ef = e + mos_string_L(format);
  
  while ( e < ef ) {
    b = e;
    
    while ( e < ef && *e != '%' && *e != '~' ) {
      e ++;
    }
    
    if ( e != b ) {
      mos_writeString(stream, b, e - b);
    }
    
    if ( e < ef ) {
      char fbuf[32];
      char buf[32];
      int isLong = 0;
      int isMinus = 0;
      char *f;
      mos_value fmtMsg = mos_undef;
      
      f = fbuf;
      
      *(f ++) = '%';
      moreFormat:
      e ++;
      switch ( *e ) {
	case 'l':
	  isLong ++;
	  *(f ++) = *e;
	  goto moreFormat;
	case '-':
	  isMinus ++;
	  *(f ++) = *e;
	  goto moreFormat;
		
        case '#': case '+': case ' ': case '.':
	case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
	  *(f ++) = *e;
	  goto moreFormat;
	  
	case '*':
	  sprintf(f, "%d", va_arg(*vap, int));
	  f = strchr(f, '\0');
	  goto moreFormat;
	  
        case 'd':
	case 'c':
	case 'o':
	case 'u':
	case 'x':
	  *(f ++) = *e; *f = '\0';
	  if ( isLong ) {
	    sprintf(buf, fbuf, va_arg(*vap, long));
	  } else {
	    sprintf(buf, fbuf, va_arg(*vap, int));
	  }
	  doBuf:
	  mos_writeString(stream, buf, strlen(buf));
	  break;

        case 'f':
        case 'g': case 'G':
        case 'e': case 'E':
	  *(f ++) = *e; *f = '\0';
	  if ( isLong ) {
	    sprintf(buf, fbuf, va_arg(*vap, long double));
	  } else {
	    sprintf(buf, fbuf, va_arg(*vap, double));
	  }
          goto doBuf;
	  
	case 's':
	  *(f ++) = *e; *f = '\0';
	  f = va_arg(*vap, char*);
	  if ( ! f )
	    f = "(null)";
	  mos_writeString(stream, f, strlen(f));
	  break;
	case 'p':
	  *(f ++) = *e; *f = '\0';
	  sprintf(buf, fbuf, va_arg(*vap, void*));
	  goto doBuf;
	
	case 'S':
	case 'M':
	  fmtMsg = va_arg(*vap, mos_value);
	  goto moreFormat;
	  
	case 'O':
	case 'P':
	  mos_send(va_arg(*vap, mos_value), (mos_NE(fmtMsg,mos_undef) ? fmtMsg : (isMinus ? mos_s(printShallowOn_) : mos_s(printOn_))), stream);
	  break;

	case 'W':
	  mos_send(va_arg(*vap, mos_value), (mos_NE(fmtMsg,mos_undef) ? fmtMsg : (isMinus ? mos_s(writeShallowOn_) : mos_s(writeOn_))), stream);
	  break;

	case 'F':
	  mos_send(stream, mos_s(flush));
	  break;
	
	case '{':
	  mos_send(stream, mos_s(indentToCurrentPosition));
	  break;
	case '<':
	  mos_send(stream, mos_s(indentByDefault));
	  break;
	case '[':
	  mos_send(stream, mos_s(indentToCurrentPosition));
	  mos_send(stream, mos_s(indentByDefault));
	  break;
	  
	case '}': case '>':
	  mos_send(stream, mos_s(unindent));
	  break;
	case ']':
	  mos_send(stream, mos_s(unindent));
	  mos_send(stream, mos_s(unindent));
	  break;
	  
	default:
	  mos_writeString(stream, fbuf + 1, strlen(fbuf + 1));
	  break;
      }
      e ++;
    }
  }
  
  return stream;
}

mos_value mos_vprintf(mos_value stream, const char *format, va_list *vap)
{
  mos_string fmt = _mos_o_string;
  fmt._v = (char*) format;
  fmt._l = fmt._al = format ? strlen(format) : 0;
  
  return mos_vprintf_(stream, mos_MAKE_REF(&fmt), vap);
}


mos_value mos_printf(mos_value stream, const char *format, ...)
{
  mos_value x;
  va_list vap;
  va_start(vap,format);
  
  x = mos_vprintf(stream, format, &vap);
  
  va_end(vap);
  
  return x;
}
