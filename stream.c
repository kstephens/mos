
#ifndef __rcs_id__
#ifndef __rcs_id_mos_stream_c__
#define __rcs_id_mos_stream_c__
static const char __rcs_id_mos_stream_c[] = "$Id: stream.c,v 1.2 1999-02-19 09:26:25 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"


/******************************************************************************/

mos_ANNOT("Module: stream")

/******************************************************************************/

#include "mos/constant.h"


mos_ANNOT("Doc: The end-of-stream object")

mos_ANNOT("Category: Print")
mos_METHOD(eos,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "@E"));
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(eos)
mos_constant_METHODS
mos_OBJECT_M(eos,printOn_)
mos_OBJECT_SLOTS(eos)
mos_OBJECT_END(constants,eos,mos_object,basicMeta)

mos_ANNOT_END

/******************************************************************************/

mos_ANNOT("Doc: Is a stream object.  Stream objects can be read from and written to.")

#define F mos_REFT(mos_MIMPL,mos_stream)->_FILEP

mos_METHOD(stream,asStream)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END


mos_ANNOT("Category: Print")
mos_METHOD(stream,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "(@F name:%P mode:%P)", mos_send(mos_RCVR, mos_s(name)), mos_send(mos_RCVR, mos_s(mode))));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Internal")

mos_METHOD(stream,_name_Mode_)
{
  mos_send(mos_RCVR, mos_s(name_), mos_send(mos_ARGV[0], mos_s(asConstant)));
  mos_send(mos_RCVR, mos_s(mode_), mos_send(mos_ARGV[1], mos_s(asConstant)));
  mos_send(mos_RCVR, mos_s(line_), mos_integer_make(0));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Open")

mos_METHOD(stream,open_Mode_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  mos_ARGV[1] = mos_send(mos_ARGV[1], mos_s(asString));
  if ( (F = fopen(mos_string_V(mos_ARGV[0]), mos_string_V(mos_ARGV[1]))) ) {
    mos_send(mos_RCVR, mos_s(_name_Mode_), mos_ARGV[0], mos_ARGV[1]);
  }
  mos_return(F ? mos_RCVR : mos_false);
}
mos_METHOD_END

mos_METHOD(stream,reopen_Mode_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  mos_ARGV[1] = mos_send(mos_ARGV[1], mos_s(asString));
  if ( (F = freopen(mos_string_V(mos_ARGV[0]), mos_string_V(mos_ARGV[1]), F)) ) {
    mos_send(mos_RCVR, mos_s(_name_Mode_), mos_ARGV[0], mos_ARGV[1]);
  }
  mos_return(F ? mos_RCVR : mos_false);
}
mos_METHOD_END

mos_METHOD(stream,fdopen_Mode_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  mos_ARGV[1] = mos_send(mos_ARGV[1], mos_s(asString));
  if ( (F = fdopen(mos_INT(mos_ARGV[0]), mos_string_V(mos_ARGV[1]))) ) {
    mos_send(mos_RCVR, mos_s(_name_Mode_), mos_ARGV[0], mos_ARGV[1]);
  }
  mos_return(F ? mos_RCVR : mos_false);
}
mos_METHOD_END

mos_METHOD(stream,close)
{
  if ( F ) {
    fclose(F);
    F = 0;
  }
}
mos_METHOD_END
 
  mos_ANNOT_END

  mos_ANNOT("Category: Write")

mos_METHOD(stream,writeString_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  if ( F ) 
    fwrite(mos_string_V(mos_ARGV[0]), sizeof(mos_string_V(mos_ARGV[0])[0]), mos_string_L(mos_ARGV[0]), F);
}
mos_METHOD_END

mos_METHOD(stream,flush)
{
  if ( F ) 
    fflush(F);
}
mos_METHOD_END

  mos_ANNOT_END

  mos_ANNOT("Category: Read")

mos_METHOD(stream,readString_)
{
  mos_value s;
  size_t bytes_read;
  
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  s = mos_send(mos_string_make(0, 0), mos_s(length_), mos_ARGV[0]);
  if ( F ) {
    bytes_read = fread(mos_string_V(s), sizeof(mos_string_V(s)[0]), mos_string_L(s), F);
    if ( bytes_read >= 0 ) {
      if ( bytes_read != mos_string_L(s) ) {
	mos_send(s, mos_s(length_), mos_integer_make(bytes_read));
      }
    } else {
      s = mos_eos;
    }
  }
  
  mos_return(s);
}
mos_METHOD_END

mos_METHOD(stream,readChar)
{
  if ( F ) {
    int c = getc(F);
    if ( c != EOF ) {
      if ( c == '\n' ) {
        mos_send(mos_RCVR, mos_s(line_), mos_send(mos_send(mos_RCVR, mos_s(line)), mos_s(__ADD__), mos_integer_make(1)));
      }
      mos_return(mos_char_make(c));
    }
  }

  mos_return(mos_eos);
}
mos_METHOD_END

mos_METHOD(stream,peekChar)
{
  int c;
  
  if ( (c = F ? getc(F) : EOF) != EOF ) {
    ungetc(c, F);
  }
  mos_return(c != EOF ? mos_char_make(c) : mos_eos);
}
mos_METHOD_END

#undef F
  mos_ANNOT_END


  mos_ANNOT("Category: Structured Stream")
mos_METHOD(stream,asStructuredStream)
{
  mos_return(mos_send(mos_o(structuredStream), mos_s(newWithStream_), mos_RCVR));
}
mos_METHOD_END
  mos_ANNOT_END


mos_OBJECT(stream)
mos_OBJECT_M(stream,asStream)
mos_OBJECT_M(stream,printOn_)
mos_OBJECT_M(stream,_name_Mode_)
mos_OBJECT_M(stream,open_Mode_)
mos_OBJECT_M(stream,reopen_Mode_)
mos_OBJECT_M(stream,fdopen_Mode_)
mos_OBJECT_M(stream,close)
mos_OBJECT_M(stream,writeString_)
mos_OBJECT_M(stream,flush)
mos_OBJECT_M(stream,readString_)
mos_OBJECT_M(stream,readChar)
mos_OBJECT_M(stream,peekChar)
mos_OBJECT_M(stream,asStructuredStream)
mos_ANNOT("Category: Open")
mos_OBJECT_A(name,0)
mos_OBJECT_A(mode,1)
mos_ANNOT_END
mos_ANNOT("Category: Internal")
mos_OBJECT_A(line,2)
mos_ANNOT_END
mos_OBJECT_SLOTS(stream)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(protos,stream,mos_stream,basicMeta)

mos_ANNOT_END

mos_ANNOT_END


mos_stream
_mos_stream_stdin = { mos_object_HDR_INIT_STATIC(stream) /* , stdin */ },
_mos_stream_stdout = { mos_object_HDR_INIT_STATIC(stream) /* , stdout */ },
_mos_stream_stderr = { mos_object_HDR_INIT_STATIC(stream) /* , stderr */ };

mos_INIT(stream0,5)
{
  _mos_stream_stdin._FILEP = stdin;
  _mos_stream_stdout._FILEP = stdout;
  _mos_stream_stderr._FILEP = stderr;

  return 0;
}

mos_INIT(stream,22)
{
#define INIT(in,N,M) \
  mos_send(mos_FILE(in), mos_s(_deepenSlots)); \
  mos_send(mos_FILE(in), mos_s(_name_Mode_), mos_string_make_(N), mos_string_make_(M))

  INIT(in, "*in*", "r");
  INIT(out, "*out*", "w");
  INIT(err, "*err*", "w");

#undef INIT
  
  return 0;
}

mos_value mos_writeString(mos_value stream, const char *v, size_t l)
{
  mos_string x = { mos_object_HDR_INIT_STATIC(string), (char*) v, l, l };
  return mos_send(stream, mos_s(writeString_), mos_MAKE_REF(&x));
}

