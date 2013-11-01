#include <stdio.h>

typedef struct mos_stream {
  mos_object_HDR;
  FILE *_FILEP;
} mos_stream;

extern mos_stream _mos_stream_stdin, _mos_stream_stdout, _mos_stream_stderr;
#define mos_FILE(X) mos_MAKE_REF(&_mos_stream_std##X)

mos_value mos_writeString(mos_value stream, const char *v, size_t l);

#define mos_eos mos_o(eos)

