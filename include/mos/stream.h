
#ifndef __rcs_id__
#ifndef __rcs_id_mos_stream_h__
#define __rcs_id_mos_stream_h__
static const char __rcs_id_mos_stream_h[] = "$Id: stream.h,v 1.2 1999-02-19 09:26:32 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include <stdio.h>

typedef struct mos_stream {
  mos_object_HDR;
  FILE *_FILEP;
} mos_stream;

extern mos_stream _mos_stream_stdin, _mos_stream_stdout, _mos_stream_stderr;
#define mos_FILE(X) mos_MAKE_REF(&_mos_stream_std##X)

mos_value mos_writeString(mos_value stream, const char *v, size_t l);

#define mos_eos mos_o(eos)

