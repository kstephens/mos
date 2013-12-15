#include "mos/mos.h"
#include "mos/constant.h"
#include <string.h>
#include <ctype.h>
#include <alloca.h> /* OS X only? */

#define SELF mos_REFT(mos_MIMPL,mos_string)
#define V mos_string_V(mos_MIMPL)
#define L mos_string_L(mos_MIMPL)
#define AL mos_string_AL(mos_MIMPL)
#define FLAGS SELF->_flags
#define I mos_INT(mos_ARGV[0])
#define J mos_INT(mos_ARGV[1])

mos_ANNOT("Module: string")
mos_ANNOT("Doc: Character string object.  Strings are compact vectors of character objects.")

mos_ANNOT("Category: Coerce")
mos_METHOD(string,asString)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END
mos_METHOD(string,clone)
{
  mos_return(mos_send(mos_RCVR, mos_s(_clone)));
}
mos_METHOD_END
mos_METHOD(string,asConstant)
{
  if ( ! (FLAGS & mos_READONLY) ) {
    /* If we're not readonly, clone, deepen and make readonly */
    mos_value x = mos_send(mos_RCVR, mos_s(clone));
    x = mos_send(x, mos_s(deepen));
    x = mos_send(x, mos_s(makeReadonly));
    mos_return(x);
  }
}
mos_METHOD_END
mos_METHOD(string,asChar)
{
  if ( L ) {
    mos_return(mos_char_make(V[0]));
  } else {
    mos_return(mos_error(mos_s(rangeError), "length: 0"));
  }
}
mos_METHOD_END
mos_METHOD(string,asSelector)
{
  mos_return(mos_selector_make_(V, L));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Print")
mos_METHOD(string,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "\"%W\"", mos_send(mos_RCVR, mos_s(escapeC))));
}
mos_METHOD_END
mos_METHOD(string,writeOn_)
{
  mos_return(mos_send(mos_ARGV[0], mos_s(writeString_), mos_MIMPL));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Coder")
mos_METHOD(string,encodeOn_)
{
  mos_send(mos_ARGV[0], mos_s(encodeAsString_), mos_MIMPL);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Length")
mos_METHOD(string,length)
{
  mos_return(mos_integer_make(L));
}
mos_METHOD_END
mos_METHOD(string,allocatedLength)
{
  mos_return(mos_integer_make(AL));
}
mos_METHOD_END
mos_METHOD(string,length_)
{
  char *v;
 
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  if ( I < 0 ) {
    mos_return(mos_error(mos_s(rangeError), "index: %d", (int) I));
  }
  
  if ( AL < I ) {
    while ( AL < I ) {
      AL *= 2;
      AL ++;
    }
    v = mos_malloc(sizeof(V[0]) * (AL + 1));
    if ( V ) {
      memcpy(v, V, sizeof(V[0]) * (I < L ? I : L));
    }
  } else {
    v = V;
  }
  
  if ( I > L ) {
    memset(v + L, 0, I - L);
  }
  v[I] = 0;
  
  V = v;
  L = I;
  
}
mos_METHOD_END
mos_METHOD(string,allocatedLength_)
{
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  if ( I < 0 || I < L ) {
    mos_return(mos_error(mos_s(rangeError), "index: %d", (int) I));
  }
  
  if ( I != AL ) {
    char *v;
  
    v = mos_malloc(sizeof(V[0]) * (I + 1));
    if ( V ) {
      memcpy(v, V, sizeof(V[0]) * L);
    }
    
    AL = I;
    V = v;
  }
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Equal")
mos_METHOD(string,_equal_)
{
  if ( mos_NE(mos_RCVR, mos_ARGV[0]) ) {
    if ( L != mos_string_L(mos_ARGV[0]) )
      mos_return(mos_false);
    if ( V != mos_string_V(mos_ARGV[0]) && memcmp(V, mos_string_V(mos_ARGV[0]), L) )
      mos_return(mos_false);
  }
  mos_return(mos_true); 
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Get")
mos_METHOD(string,get_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  if ( I < 0 || I >= L ) {
    mos_return(mos_error(mos_s(rangeError), "index: %d", (int) I));
  }
  mos_return(mos_char_make(V[I]));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Subvector")
mos_METHOD(string,sliceFrom_Length_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  mos_ARGV[1] = mos_send(mos_ARGV[1], mos_s(asInteger));
  
  if ( I < 0 || I >= L ) {
    mos_return(mos_error(mos_s(rangeError), "index: %d", (int) I));
  }
  if ( J < 0 || I + J > L ) {
    mos_return(mos_error(mos_s(rangeError), "size: %d", (int) J));
  }
  
  mos_MIMPL = mos_send(mos_MIMPL, mos_s(clone));
  V += I;
  L = J;
  AL = 0; /* Always realloc substrings on resize */
  
  mos_return(mos_MIMPL);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Allocate")
mos_METHOD(string,deepen)
{
  char *v = mos_malloc(sizeof(V[0]) * (L + 1));
  if ( V )
    memcpy(v, V, sizeof(V[0]) * L);
  else
    memset(v, 0, sizeof(V[0]) * L);
  v[L] = 0;
  V = v;
  AL = L;
  FLAGS &= ~mos_READONLY;
}
mos_METHOD_END
mos_METHOD(string,makeReadonly)
{
  FLAGS |= mos_READONLY;
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Set")
mos_METHOD(string,set_Value_)
{
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  mos_ARGV[1] = mos_send(mos_ARGV[1], mos_s(asChar));
  if ( I < 0 || I >= L ) {
    mos_return(mos_error(mos_s(rangeError), "index: %d", (int) I));
  }
  V[I] = mos_CHAR(mos_ARGV[1]);
}
mos_METHOD_END


mos_METHOD(string,append_)
{
  size_t ol;
  
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  ol = L;
  mos_send(mos_RCVR, mos_s(length_), mos_integer_make(L + mos_string_L(mos_ARGV[0])));
  memcpy(V + ol, mos_string_V(mos_ARGV[0]), sizeof(V[0]) * mos_string_L(mos_ARGV[0]));
}
mos_METHOD_END
mos_METHOD(string,insert_)
{
  size_t ol;
  
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  ol = L;
  mos_send(mos_RCVR, mos_s(length_), mos_integer_make(L + mos_string_L(mos_ARGV[0])));
  memmove(V + mos_string_L(mos_ARGV[0]), V, sizeof(V[0]) * ol);
  memcpy(V, mos_string_V(mos_ARGV[0]), sizeof(V[0]) * mos_string_L(mos_ARGV[0]));
  
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Stream")
mos_ANNOT("Doc: See _ system proto stream for protocol details.")
mos_METHOD(string,asStream)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END

mos_ANNOT("Category: Output")
mos_METHOD(string,writeString_)
{
  mos_return(mos_send(mos_RCVR, mos_s(append_), mos_ARGV[0]));
}
mos_METHOD_END
mos_METHOD(string,flush)
{
  mos_return(mos_send(mos_RCVR, mos_s(append_), mos_ARGV[0]));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Input")
mos_METHOD(string,readString_)
{
  ssize_t l;
  mos_value s;
  
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  l = I;
  if ( l < 0 ) {
    mos_return(mos_error(mos_s(rangeError), "index: %d", (int) l));
  }
  if ( L == 0 && l > 0 ) {
    mos_return(mos_eos);
  }
  if ( l > L ) {
    l = L;
  }
  
  s = mos_string_make(V, l);
  V += l;
  L -= l;
  AL -= l;
  
  mos_return(s);
}
mos_METHOD_END
mos_METHOD(string,readChar)
{
  mos_return((L > 0) ? mos_char_make((L --, AL --, *(V ++))) : mos_eos);
}
mos_METHOD_END
mos_METHOD(string,peekChar)
{
  mos_return((L > 0) ? mos_char_make(*V) : mos_eos);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Structured Stream")
mos_METHOD(string,asStructuredStream)
{
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  mos_return(mos_send(mos_o(structuredStream), mos_s(newWithStream_), mos_RCVR));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT_END
mos_ANNOT_END /* End of "Category: Stream" */

mos_ANNOT("Category: Escape")
static int find_C_escape(int x, int off)
{
  static const unsigned char C_escapes[] = "\\\\\"\"\'\'\aa\bb\ff\nn\rr\tt\rr\vv";
  register const unsigned char *s = C_escapes + off;

  while ( *s ) {
    if ( *s == x )
      return *(s + (off ? -1 : 1));
    s += 2;
  }

  return 0;
}
static const char hexdigits[] = "0123456789abcdef";
mos_METHOD(string,escapeC)
{
  const unsigned char *t = (unsigned char *) V, *te = t + L;
  char *b = alloca(L * 4 + 1);
  char *s = b;
  
  for ( ; t < te; t ++ ) {
    int e;
    
    if ( *t && (e = find_C_escape(*t, 0)) ) {
      *(s ++) = '\\';
      *(s ++) = e;
    } else
    if ( isprint(*t) ) {
      *(s ++) = *t;
    } else {
      *(s ++) = '\\';
      *(s ++) = '0' + ((*t >> 6) & 7);
      *(s ++) = '0' + ((*t >> 3) & 7);
      *(s ++) = '0' + ((*t) & 7);
    }
  }
  *s = '\0';
  
  {
  mos_value S;
  
  S = mos_string_make(b, s - b);
  mos_send(S, mos_s(deepen));
  
  mos_return(S);
  }
}
mos_METHOD_END
mos_METHOD(string,unescapeC)
{
  const char *t = V, *te = t + L;
  char *b = alloca(L + 1);
  char *s = b;
  
  while ( t < te ) {
    const char *e;
    int c;
    
    if ( *t == '\\' ) {
      t ++; if ( ! (t < te) ) {
	*(s ++) = '\\';
      } else
      if ( *t == 'x' || *t == 'X' ) {
        int c = 0;
      
        t ++;
	if ( t < te && (e = strchr(hexdigits, tolower(*t))) ) {
	  c += e - hexdigits;
	  t ++;
	  if ( t < te && (e = strchr(hexdigits, tolower(*t))) ) {
	    c *= 16;
	    c += e - hexdigits;
	    t ++;
	  }
	}
	*(s ++) = c;
      } else
      if ( '0' <= *t && *t <= '7' ) {
        int c = 0;

	c += *(t ++) - '0';
	if ( t < te && '0' <= *t && *t <= '7' ) {
	  c *= 8;
	  c += *(t ++) - '0';
	  if ( t < te && '0' <= *t && *t <= '7' ) {
	    c *= 8;
	    c += *(t ++) - '0';
	  }
	}
	*(s ++) = c;
      } else
      if ( (c = find_C_escape(*t, 1)) ) {
	t ++;
        *(s ++) = c;
      }
    } else {
      *(s ++) = *(t ++);
    }
  }
  *s = '\0';
  
  {
  mos_value S;
  
  S = mos_string_make(b, s - b);
  mos_send(S, mos_s(deepen));
  
  mos_return(S);
  }
}
mos_METHOD_END


#undef SELF
#undef V
#undef L
#undef AL
#undef FLAGS
#undef I
#undef J

mos_ANNOT_END /* End of "Category: Escape" */

mos_OBJECT(string)
mos_constant_METHODS
mos_OBJECT_M(string,asString)
mos_OBJECT_M(string,asConstant)
mos_OBJECT_M(string,asChar)
mos_OBJECT_M(string,asSelector)
mos_OBJECT_M(string,asStream)
mos_OBJECT_M(string,asAnnotation)
mos_OBJECT_M(string,printOn_)
mos_OBJECT_M(string,writeOn_)
mos_OBJECT_M(string,encodeOn_)
mos_OBJECT_M(string,length)
mos_OBJECT_M(string,length_)
mos_OBJECT_M(string,allocatedLength)
mos_OBJECT_M(string,allocatedLength_)
mos_OBJECT_M(string,_equal_)
mos_OBJECT_M(string,get_)
mos_OBJECT_M(string,sliceFrom_Length_)
mos_OBJECT_M(string,escapeC)
mos_OBJECT_M(string,unescapeC)
mos_OBJECT_M(string,deepen)
mos_OBJECT_M(string,makeReadonly)
mos_OBJECT_M(string,set_Value_)
mos_OBJECT_M(string,append_)
mos_OBJECT_M(string,insert_)
mos_OBJECT_M(string,writeString_)
mos_OBJECT_M(string,flush)
mos_OBJECT_M(string,readString_)
mos_OBJECT_M(string,readChar)
mos_OBJECT_M(string,peekChar)
mos_OBJECT_M(string,asStructuredStream)
//mos_OBJECT_M(string,clone)
mos_OBJECT_SLOTS(string)
mos_OBJECT_END(protos,string,mos_string,basicMeta)

mos_ANNOT_END
mos_ANNOT_END /* End of "Module: string" */


mos_value mos_string_make(const char *_v, size_t _l)
{
  mos_string *x = mos_REFT(mos_clone(mos_o(string)),mos_string);
  x->_v = (char*) _v;
  x->_l = _l;
  x->_al = _l;
  x->_flags = 0;
  return mos_MAKE_REF(x);
}

mos_value mos_string_make_(const char *v)
{
  return mos_string_make(v, v ? strlen(v) : 0);
}

