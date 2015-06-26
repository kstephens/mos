#ifndef _mos_char_h
#define _mos_char_h

typedef mos_string mos_char;

extern mos_char _mos_chars[0x100];

#define mos_CHAR(X) (mos_REFT(X,mos_char)-_mos_chars)
#define _mos_MAKE_CHAR(X) mos_MAKE_REF(_mos_chars + ((unsigned)(X) & 0xff))

mos_value mos_char_make(int x);
#define mos_char_make(X) _mos_MAKE_CHAR(X)

#endif

