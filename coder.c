#include "mos/mos.h"
#include <string.h> /* memcpy */
#include <stdlib.h> /* abort() */
#include <assert.h>

#define STREAM mos_send(mos_RCVR, mos_s(stream))
#define TABLE mos_send(mos_RCVR, mos_s(table))

mos_ANNOT("Module: coder")
mos_ANNOT("Doc: A coder object contains a stream (to encode or decode) the objects to and a reference table to encode objects with referential integrity.")

mos_METHOD(coder,asCoder)
{
}
mos_METHOD_END

mos_ANNOT("Category: Clone")
mos_METHOD(coder,clone)
{
  mos_RCVR = mos_send(mos_RCVR, mos_s(_clone));
  mos_send(mos_RCVR, mos_s(table_), mos_send(TABLE, mos_s(clone)));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Internal")
mos_METHOD(coder,reset)
{
  mos_send(mos_RCVR, mos_s(table_), mos_send(mos_o(map), mos_s(clone)));
}
mos_METHOD_END
mos_METHOD(coder,flush)
{
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Low-level")
mos_METHOD(coder,readCharSize)
{
  mos_value c = mos_send(STREAM, mos_s(readChar));
  if ( mos_EQ(c, mos_eos) )
    mos_return(mos_error(mos_s(coderError), "readSizeChar: end-of-stream"));
  mos_return(mos_integer_make(mos_CHAR(c)));
}
mos_METHOD_END
mos_METHOD(coder,readStringSize_)
{
  mos_int x_size = mos_INT(mos_ARGV[0]);
  mos_value str = mos_send(STREAM, mos_s(readString_), mos_integer_make(x_size));
  if ( mos_EQ(str, mos_eos) )
    mos_return(mos_error(mos_s(coderError), "readStringSize: end-of-stream"));
  if ( mos_string_L(str) != x_size )
    mos_return(mos_error(mos_s(coderError), "readStringSize: expected %ld chars, read only %ld", (long) x_size, (long) mos_string_L(str)));
  mos_return(str);
}
mos_METHOD_END
mos_METHOD(coder,encodeRawInteger_)
{
  mos_uint x = mos_INT(mos_ARGV[0]);
  unsigned char buf[sizeof(x) + 1];
  mos_string str = { mos_object_HDR_INIT_STATIC(string), (char *) buf, sizeof(buf), sizeof(buf) };
  buf[0] = sizeof(x);
  switch ( sizeof(x) ) {
  default: abort(); break;
  case 8: buf[8] = x >> 56;
  case 7: buf[7] = x >> 48;
  case 6: buf[6] = x >> 40;
  case 5: buf[5] = x >> 32;
  case 4: buf[4] = x >> 24;
  case 3: buf[3] = x >> 16;
  case 2: buf[2] = x >>  8;
  case 1: buf[1] = x >>  0;
  }
  mos_send(STREAM, mos_s(writeString_), mos_MAKE_REF(&str));
}
mos_METHOD_END
mos_METHOD(coder,decodeRawInteger)
{
  mos_int sizeof_x;
  mos_value str;
  unsigned char *buf;
  mos_uint x;

  sizeof_x = mos_INT(str = mos_send(mos_RCVR, mos_s(readCharSize)));
  str = mos_send(mos_RCVR, mos_s(readStringSize_), str);
  buf = (void *) mos_string_V(str);

  x = 0; buf --;
  switch ( sizeof_x ) {
  default: abort(); break;
  case 8: x |= ((mos_uint) buf[8]) << 56;
  case 7: x |= ((mos_uint) buf[7]) << 48;
  case 6: x |= ((mos_uint) buf[6]) << 40;
  case 5: x |= ((mos_uint) buf[5]) << 32;
  case 4: x |= ((mos_uint) buf[4]) << 24;
  case 3: x |= ((mos_uint) buf[3]) << 16;
  case 2: x |= ((mos_uint) buf[2]) <<  8;
  case 1: x |= ((mos_uint) buf[1]) <<  0;
  }
  mos_return(mos_integer_make(x));
}
mos_METHOD_END
mos_METHOD(coder,encodeRawReal_)
{
  double x = mos_REAL(mos_ARGV[0]);
  unsigned char buf[sizeof(x) + 1];
  buf[0] = sizeof(x);
  memcpy(buf + 1, &x, sizeof(x));
  mos_string str = { mos_object_HDR_INIT_STATIC(string), (char*) buf, sizeof(buf), sizeof(buf) };
  mos_send(STREAM, mos_s(writeString_), mos_MAKE_REF(&str));
}
mos_METHOD_END
mos_METHOD(coder,decodeRawReal)
{
  mos_int sizeof_x;
  mos_value str;
  double x;
  sizeof_x = mos_INT(str = mos_send(mos_RCVR, mos_s(readCharSize)));
  if ( sizeof_x != sizeof(x) )
    mos_return(mos_error(mos_s(coderError), "decodeRawReal: unexpected size %ld", (long) sizeof_x));
  str = mos_send(mos_RCVR, mos_s(readStringSize_), str);
  x = *(double*) mos_string_V(str);
  mos_return(mos_real_make(x));
}
mos_METHOD_END
mos_METHOD(coder,encodeRawString_)
{
  mos_send(mos_RCVR, mos_s(encodeRawInteger_), (mos_send(mos_ARGV[0], mos_s(length))));
  mos_send(STREAM, mos_s(writeString_), mos_ARGV[0]);
}
mos_METHOD_END
mos_METHOD(coder,decodeRawString)
{
  mos_value len = mos_send(mos_RCVR, mos_s(decodeRawInteger));
  mos_value str = mos_send(STREAM, mos_s(readStringSize_), len);
  mos_return(str);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Encode")
mos_ANNOT("Category: Atomic")
mos_METHOD(coder,encodeAsInteger_)
{
  mos_value obj = mos_send(mos_ARGV[0], mos_s(asInteger));
  mos_send(STREAM, mos_s(writeString_), mos_char_make('i'));
  mos_send(mos_RCVR, mos_s(encodeRawInteger_), obj);
}
mos_METHOD_END
mos_METHOD(coder,encodeAsReal_)
{
  mos_value obj = mos_send(mos_ARGV[0], mos_s(asReal));
  mos_send(STREAM, mos_s(writeString_), mos_char_make('f'));
  mos_send(mos_RCVR, mos_s(encodeRawReal_), obj);
}
mos_METHOD_END
mos_METHOD(coder,encodeAsChar_)
{
  mos_value obj = mos_send(mos_ARGV[0], mos_s(asChar));
  mos_send(STREAM, mos_s(writeString_), mos_char_make('c'));
  mos_send(mos_RCVR, mos_s(encodeRawInteger_), mos_integer_make(mos_CHAR(obj)));
}
mos_METHOD_END
mos_METHOD(coder,encodeAsString_)
{
  mos_value obj = mos_send(mos_ARGV[0], mos_s(asString));
  mos_send(STREAM, mos_s(writeString_), mos_char_make('s'));
  mos_send(mos_RCVR, mos_s(encodeRawString_), obj);
}
mos_METHOD_END
mos_METHOD(coder,encodeAsVector_)
{
  mos_value obj = mos_send(mos_ARGV[0], mos_s(asVector)), len;
  mos_send(STREAM, mos_s(writeString_), mos_char_make('v'));
  len = mos_send(obj, mos_s(length));
  mos_send(mos_RCVR, mos_s(encodeRawInteger_), len);
  {
    size_t nslots = mos_INT(len), i;
    for ( i = 0; i < nslots; i ++ ) {
      mos_send(mos_RCVR, mos_s(encode_), mos_vector_V(obj)[i]);
    }
  }
}
mos_METHOD_END
mos_METHOD(coder,encodeAsSelector_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(name));
  mos_send(STREAM, mos_s(writeString_), mos_char_make('S'));
  mos_send(mos_RCVR, mos_s(encodeRawString_), mos_ARGV[0]);
}
mos_METHOD_END
mos_METHOD(coder,encodeAsObject_WithName_)
{
  mos_ARGV[1] = mos_send(mos_ARGV[1], mos_s(asString));
  mos_send(STREAM, mos_s(writeString_), mos_char_make('N'));
  mos_send(mos_RCVR, mos_s(encodeRawString_), mos_ARGV[1]);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Complex")
mos_METHOD(coder,encodeAsReference_)
{
  mos_value id = mos_send(TABLE, mos_s(get_Or_), mos_ARGV[0], mos_undef);
  if ( mos_EQ(id, mos_undef) ) {
    id = mos_send(TABLE, mos_s(length));
    /* Create an entry into the reference table */
    mos_send(TABLE, mos_s(set_Value_), mos_ARGV[0], id);
    /* 'R' is the define reference tag */
    mos_send(STREAM, mos_s(writeString_), mos_char_make('R'));
    /* encode the reference id */
    mos_send(mos_RCVR, mos_s(encodeRawInteger_), id);
    /* encode the object */
    mos_send(mos_ARGV[0], mos_s(encodeOn_), mos_RCVR);
  } else {
    /* We've already encoded the object, so just incode the reference id */
    mos_send(STREAM, mos_s(writeString_), mos_char_make('r'));
    mos_send(mos_RCVR, mos_s(encodeRawInteger_), id);
  }
}
mos_METHOD_END
mos_METHOD(coder,encodeAsObject_)
{
  mos_value o = mos_ARGV[0];
  /* 'O' is the object tag */
  mos_send(STREAM, mos_s(writeString_), mos_char_make('O'));
  /* encode the object's descriptor */
  mos_send(mos_RCVR, mos_s(encodeAsReference_), mos_MAKE_REF(mos_DESC(o)));
  /* encode the object's slots */
  {
    size_t nslots = mos_NSLOTS(o), i;
    for ( i = 0; i < nslots; i ++ ) {
      mos_send(mos_RCVR, mos_s(encodeAsReference_), mos_SLOTS(o)[i]);
    }
  }
  /* encode any additional state */
  mos_send(mos_ARGV[0], mos_s(encodeOtherOn_), mos_RCVR);
}
mos_METHOD_END
mos_ANNOT_END

mos_METHOD(coder,encode_)
{
  mos_value obj = mos_ARGV[0];
  obj = mos_send(obj, mos_s(beforeEncodeOn_), mos_RCVR);
  mos_send(mos_RCVR, mos_s(encodeAsReference_), obj);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Decode")
mos_METHOD(coder,decodeAsVector)
{
  mos_value obj;
  size_t len;
  len = mos_INT(mos_send(mos_RCVR, mos_s(decodeRawInteger)));
  obj = mos_vector_make(len, 0);
  {
    size_t i;
    for ( i = 0; i < len; i ++ ) {
      mos_send(mos_RCVR, mos_s(decode_), mos_vector_V(obj)[i]);
    }
  }
}
mos_METHOD_END
mos_METHOD(coder,decode)
{
  mos_value o;
  int c;

  o = mos_send(STREAM, mos_s(readChar));
  if ( mos_EQ(o, mos_eos) )
    mos_return(o);
  c = mos_CHAR(o);
  switch ( c ) {
  case 'i':
    o = mos_send(mos_RCVR, mos_s(decodeRawInteger));
    break;
  case 'f':
    o = mos_send(mos_RCVR, mos_s(decodeRawReal));
    break;
  case 'c':
    o = mos_char_make(mos_INT(mos_send(mos_RCVR, mos_s(decodeRawInteger))));
    break;
  case 's':
    o = mos_send(mos_RCVR, mos_s(decodeRawString));
    break;
  case 'v':
    o = mos_send(mos_RCVR, mos_s(decodeAsVector));
    break;
  case 'S':
    o = mos_send(mos_send(mos_RCVR, mos_s(decodeRawString)), mos_s(asSelector));
    break;

  case 'N':
    o = mos_send(mos_RCVR, mos_s(decodeRawString));
    o = mos_send(mos_o(named), mos_s(objectNamed_), o);
    break;

  case 'R': 
    {
      mos_value id;
      
      /* Decode the reference id */
      id = mos_send(mos_RCVR, mos_s(decodeRawInteger));
      
      /* FIXME: DOES NOT WORK FOR SELF-REFERENCING OBJECTS. */
      /* Decode the object */
      o = mos_send(mos_RCVR, mos_s(decode));
      
      /* Remember the refid -> object mapping */
      mos_send(TABLE, mos_s(set_Value_), id, o);
    }
    break;
    
  case 'r':
    /* Decode the reference id */
    o = mos_send(mos_RCVR, mos_s(decodeRawInteger));
    
    /* Get the remembered object for the refid */
    o = mos_send(TABLE, mos_s(get_), o);
    break;
    
  case 'O':
    {
      mos_value desc;
      
      /* Get the descriptor */
      desc = mos_send(mos_RCVR, mos_s(decode));
      
      /* Create an object from the descriptor */
      o = mos_create(desc);
      
      /* Read the object's slots */
      {
	size_t nslots = mos_NSLOTS(o), i;
	mos_value *slots = mos_SLOTS(o);
	
	for ( i = 0; i < nslots; i ++ ) {
	  slots[i] = mos_send(mos_RCVR, mos_s(decode));
	}
      }
      
      /* Read any additional state */
      o = mos_send(o, mos_s(decodeOtherOn_), mos_RCVR);
    }
    break;


  default:
    o = mos_error(mos_s(coderError), "%P invalid coding key", o);
    break;
  }
  /* Give object chance to replace itself. */
  o = mos_send(o, mos_s(afterDecodeOn_), mos_RCVR);
  mos_return(o);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Test")
mos_METHOD(coder,_test_)
{
  mos_value encoder, out_stream, in_stream, decoder, o, o2;

  encoder = mos_send(mos_RCVR, mos_s(clone));
  mos_send(encoder, mos_s(stream_), mos_string_make(0, 0));

  o = mos_ARGV[0];
  mos_send(encoder, mos_s(encode_), o);
  mos_send(encoder, mos_s(encode_), o);

  out_stream = mos_send(encoder, mos_s(stream));
  mos_printf(mos_FILE(err), "coder: out_stream %P\n", out_stream);

  in_stream = mos_send(out_stream, mos_s(clone));
  assert(out_stream != in_stream);
  mos_send(in_stream, mos_s(deepen));

  decoder = mos_send(mos_RCVR, mos_s(clone));
  mos_send(decoder, mos_s(stream_), in_stream);
  o = mos_send(decoder, mos_s(decode));
  o2 = mos_send(decoder, mos_s(decode));

  o = mos_vector_make_(4, mos_ARGV[0], out_stream, o, o2);

  mos_return(o);
}
mos_METHOD_END
mos_ANNOT_END

mos_OBJECT(coder)
mos_OBJECT_M(coder,asCoder)
mos_OBJECT_M(coder,clone)
mos_OBJECT_M(coder,reset)
mos_OBJECT_M(coder,readCharSize)
mos_OBJECT_M(coder,readStringSize_)
mos_OBJECT_M(coder,encodeRawInteger_)
mos_OBJECT_M(coder,decodeRawInteger)
mos_OBJECT_M(coder,encodeRawReal_)
mos_OBJECT_M(coder,decodeRawReal)
mos_OBJECT_M(coder,encodeRawString_)
mos_OBJECT_M(coder,decodeRawString)
mos_OBJECT_M(coder,encodeAsInteger_)
mos_OBJECT_M(coder,encodeAsReal_)
mos_OBJECT_M(coder,encodeAsChar_)
mos_OBJECT_M(coder,encodeAsString_)
mos_OBJECT_M(coder,encodeAsSelector_)
mos_OBJECT_M(coder,encodeAsVector_)
mos_OBJECT_M(coder,encodeAsObject_WithName_)
mos_OBJECT_M(coder,encodeAsReference_)
mos_OBJECT_M(coder,encodeAsObject_)
mos_OBJECT_M(coder,encode_)
mos_OBJECT_M(coder,decode)
mos_OBJECT_M(coder,decodeAsVector)
mos_OBJECT_M(coder,_test_)
mos_OBJECT_A(stream,0)
mos_OBJECT_A(table,1)
mos_OBJECT_SLOTS(coder)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_o(map))
mos_OBJECT_END(protos,coder,mos_object,basicMeta);

mos_ANNOT_END
mos_ANNOT_END

