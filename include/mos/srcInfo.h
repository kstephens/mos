
#ifndef __rcs_id__
#ifndef __rcs_id_mos_srcInfo_h__
#define __rcs_id_mos_srcInfo_h__
static const char __rcs_id_mos_srcInfo_h[] = "$Id: srcInfo.h,v 1.2 1999-02-19 09:26:32 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

/*
*/

typedef struct mos_sourceInfo {
  mos_object_HDR;
  const char *_file;
  int _line;
} mos_sourceInfo;

#define mos_sourceInfo_DECLARE(X,F,L) \
  static const mos_sourceInfo X##_s = { mos_object_HDR_INIT_STATIC(sourceInfo), __FILE__ , __LINE__ }; \
  static const mos_value X = mos_MAKE_REF(&X##_s)
