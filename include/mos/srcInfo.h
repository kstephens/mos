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
