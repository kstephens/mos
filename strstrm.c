
#ifndef __rcs_id__
#ifndef __rcs_id_mos_strstrm_c__
#define __rcs_id_mos_strstrm_c__
static const char __rcs_id_mos_strstrm_c[] = "$Id: strstrm.c,v 1.2 1999-02-19 09:26:25 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"

mos_METHOD(structuredStream,asStructuredStream)
{
}
mos_METHOD_END

mos_METHOD(structuredStream,asStream)
{
}
mos_METHOD_END

mos_METHOD(structuredStream,asString)
{
  mos_return(mos_send(mos_send(mos_RCVR, mos_s(substream)), mos_s(asString)));
}
mos_METHOD_END

mos_METHOD(structuredStream,newWithStream_)
{
  mos_RCVR = mos_send(mos_RCVR, mos_s(clone));
  mos_send(mos_RCVR, mos_s(reset));
  mos_send(mos_RCVR, mos_s(substream_), mos_ARGV[0]);
}
mos_METHOD_END

mos_METHOD(structuredStream,reset)
{
  mos_send(mos_RCVR, mos_s(indents_), mos_vector_make_(1, mos_integer_make(0)));
  mos_send(mos_RCVR, mos_s(currentPosition_), mos_integer_make(0));
}
mos_METHOD_END

mos_METHOD(structuredStream,currentIndent)
{
  mos_return(mos_send(mos_send(mos_RCVR, mos_s(indents)), mos_s(top)));
}
mos_METHOD_END

mos_METHOD(structuredStream,indentTo_)
{
  mos_value is;
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  is = mos_send(mos_RCVR, mos_s(indents));
  /* mos_printf(mos_FILE(err), "\nindents: %P%F\n", is); */
  mos_send(is, mos_s(push_), mos_ARGV[0]);
}
mos_METHOD_END

mos_METHOD(structuredStream,indentToCurrentPosition)
{
  mos_return(mos_send(mos_RCVR, mos_s(indentTo_), mos_send(mos_RCVR, mos_s(currentPosition))));
}
mos_METHOD_END

mos_METHOD(structuredStream,indentBy_)
{
  mos_value ind = mos_send(mos_RCVR, mos_s(currentIndent));
  ind = mos_send(ind, mos_s(__ADD__), mos_ARGV[0]);
  mos_return(mos_send(mos_RCVR, mos_s(indentTo_), ind));
}
mos_METHOD_END

mos_METHOD(structuredStream,indentByDefault)
{
  mos_return(mos_send(mos_RCVR, mos_s(indentBy_), mos_send(mos_RCVR, mos_s(defaultIndent))));
}
mos_METHOD_END

mos_METHOD(structuredStream,unindent)
{
  mos_value is = mos_send(mos_RCVR, mos_s(indents));
  mos_value x = mos_send(is, mos_s(length));
  if ( mos_INT(x) < 2 ) {
    mos_return(mos_error(mos_s(rangeError), "already at base indent"));
  }
  mos_send(is, mos_s(pop));
}
mos_METHOD_END

mos_METHOD(structuredStream,writeString_)
{
  mos_value stream = mos_send(mos_RCVR, mos_s(substream));
  int indent = -1;
  const char *b, *e, *ef;
  size_t cp = mos_INT(mos_send(mos_RCVR, mos_s(currentPosition)));
  
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  e = mos_string_V(mos_ARGV[0]);
  ef = e + mos_string_L(mos_ARGV[0]);
  
  while ( e < ef ) {
    b = e;
    
    while ( e < ef && *e != '\n' && *e != '\r' ) {
      e ++;
    }
    
    if ( e != b ) {
      mos_writeString(stream, b, e - b);
      cp += e - b;
    }
    
    if ( e < ef ) {
      /* Okay goto new line */
      mos_writeString(stream, e, 1);
      
      /* Skip over newline */
      e ++;

      /* Compute the indent if needed */
      if ( indent < 0 ) {
	mos_value x = mos_send(mos_RCVR, mos_s(currentIndent));
	indent = mos_INT(x);
      }

      /* Do the indent */
      {
        static char istr[] = "                               ";
        int i;
	
	for ( i = indent; i >= sizeof(istr) - 1; i -= sizeof(istr) - 1 ) {
	  mos_writeString(stream, istr, sizeof(istr) - 1);
	}
	mos_writeString(stream, istr, i);
      }
      
      /* Reset the currentPosition */
      cp = indent;
    }
  }
  
  /* Save the currentPosition */
  mos_send(mos_RCVR, mos_s(currentPosition_), mos_integer_make(cp));
}
mos_METHOD_END

mos_METHOD(structuredStream,flush)
{
  mos_send(mos_send(mos_RCVR, mos_s(substream)), mos_s(flush));
}
mos_METHOD_END


mos_OBJECT(structuredStream)
mos_OBJECT_M(structuredStream,asStructuredStream)
mos_OBJECT_M(structuredStream,asStream)
mos_OBJECT_M(structuredStream,asString)
mos_OBJECT_M(structuredStream,newWithStream_)
mos_OBJECT_M(structuredStream,reset)
mos_OBJECT_M(structuredStream,currentIndent)
mos_OBJECT_M(structuredStream,indentTo_)
mos_OBJECT_M(structuredStream,indentToCurrentPosition)
mos_OBJECT_M(structuredStream,indentBy_)
mos_OBJECT_M(structuredStream,indentByDefault)
mos_OBJECT_M(structuredStream,unindent)
mos_OBJECT_M(structuredStream,writeString_)
mos_OBJECT_M(structuredStream,flush)
mos_OBJECT_A(substream,0)
mos_OBJECT_A(currentPosition,1)
mos_OBJECT_A(indents,2)
mos_OBJECT_A(defaultIndent,3)
mos_OBJECT_SLOTS(structuredStream)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(protos,structuredStream,mos_object,basicMeta)

mos_INIT(structuredStream,23)
{
  mos_send(mos_o(structuredStream), mos_s(reset));
  mos_send(mos_o(structuredStream), mos_s(defaultIndent_), mos_integer_make(2));
  
  return 0;
}

