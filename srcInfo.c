
#ifndef __rcs_id__
#ifndef __rcs_id_mos_srcInfo_c__
#define __rcs_id_mos_srcInfo_c__
static const char __rcs_id_mos_srcInfo_c[] = "$Id: srcInfo.c,v 1.2 1999-02-19 09:26:25 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"

mos_ANNOT("Module: srcInfo")
mos_ANNOT("Doc: A strInfo object contains a source file name and line number.  They are created by the compiler and are used in the debugger.")

#define SELF mos_REFT(mos_MIMPL,mos_sourceInfo)

mos_ANNOT("Category: Print")
mos_METHOD(sourceInfo,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "%s:%d", SELF->_file, SELF->_line));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Coder")
mos_METHOD(sourceInfo,encodeOtherOn_)
{
  mos_send(mos_ARGV[0], mos_s(encodeRawString_), mos_string_make_(SELF->_file));
  mos_send(mos_ARGV[0], mos_s(encodeRawInteger_), mos_integer_make(SELF->_line));
}
mos_METHOD_END

mos_METHOD(sourceInfo,decodeOtherOn_)
{
  SELF->_file = mos_string_V(mos_send(mos_ARGV[0], mos_s(decodeRawString)));
  SELF->_line = mos_INT(mos_send(mos_ARGV[0], mos_s(decodeRawInteger)));
}
mos_METHOD_END
mos_ANNOT_END

#undef SELF

mos_OBJECT(sourceInfo)
mos_OBJECT_M(sourceInfo,printOn_)
mos_OBJECT_M(sourceInfo,encodeOtherOn_)
mos_OBJECT_M(sourceInfo,decodeOtherOn_)
mos_OBJECT_SLOTS(sourceInfo)
mos_OBJECT_END(protos,sourceInfo,mos_sourceInfo,basicMeta)

mos_ANNOT_END
mos_ANNOT_END
