
#ifndef __rcs_id__
#ifndef __rcs_id_mos_vector_c__
#define __rcs_id_mos_vector_c__
static const char __rcs_id_mos_vector_c[] = "$Id: vector.c,v 1.2 1999-02-19 09:26:26 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include <string.h>

#define SELF mos_REFT(mos_MIMPL,mos_vector)
#define V SELF->_v
#define L SELF->_l
#define AL SELF->_al
#define FLAGS SELF->_flags
#define I mos_INT(mos_ARGV[0])
#define J mos_INT(mos_ARGV[1])

mos_METHOD(vector,asVector)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END

mos_METHOD(vector,asConstant)
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

mos_METHOD(vector,printOn_)
{
  size_t i;
  
  mos_ARGV[0] = mos_printf(mos_ARGV[0], "(@V");
  
  for ( i = 0; i < L; i ++ ) {
    mos_printf(mos_ARGV[0], ", %-P", V[i]);
  }
  
  mos_return(mos_printf(mos_ARGV[0], ")"));
}
mos_METHOD_END

mos_METHOD(vector,printShallowOn_)
{
  mos_return(mos_send(mos_RCVR, mos_s(printIdOn_), mos_ARGV[0]));
}
mos_METHOD_END

mos_METHOD(vector,encodeOtherOn_)
{
  mos_vector *o = SELF;
  size_t i;
  mos_send(mos_ARGV[0], mos_s(encodeRawInteger_), mos_integer_make(o->_l));
  mos_send(mos_ARGV[0], mos_s(encodeRawInteger_), mos_integer_make(o->_al));
  for ( i = 0; i < o->_l; i ++ ) {
    mos_send(mos_ARGV[0], mos_s(encodeAsReference_), o->_v[i]);
  }
}
mos_METHOD_END

mos_METHOD(vector,decodeOtherOn_)
{
  mos_vector *o = SELF;
  size_t i;

  o->_l = mos_INT(mos_send(mos_ARGV[0], mos_s(decodeRawInteger)));
  o->_al = mos_INT(mos_send(mos_ARGV[0], mos_s(decodeRawInteger)));
  o->_v = mos_malloc(sizeof(o->_v[0]) * o->_al);
  for ( i = 0; i < o->_l; i ++ ) {
    o->_v[i] = mos_send(mos_ARGV[0], mos_s(decode));
  }
  for ( ; i < o->_al; i ++ ) {
    o->_v[i] = mos_undef;
  }
}
mos_METHOD_END

mos_METHOD(vector,length)
{
  mos_return(mos_integer_make(L));
}
mos_METHOD_END

mos_METHOD(vector,allocatedLength)
{
  mos_return(mos_integer_make(AL));
}
mos_METHOD_END

mos_METHOD(vector,_equal_)
{
  if ( mos_NE(mos_RCVR, mos_ARGV[0]) ) {
    if ( L != mos_vector_L(mos_ARGV[0]) )
      mos_return(mos_false);
    if ( V != mos_vector_V(mos_ARGV[0]) ) {
      size_t i;

      for ( i = 0; i < L; i ++ ) {
	if ( ! mos_EQUAL(V[i], mos_vector_V(mos_ARGV[0])[i]) )
	  mos_return(mos_false);
      }
    }
  }
  mos_return(mos_true); 
}
mos_METHOD_END

mos_METHOD(vector,get_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  if ( I < 0 || I >= L ) {
    mos_return(mos_error(mos_s(rangeError), "index: %d", (int) I));
  }
  mos_return(V[I]);
}
mos_METHOD_END

mos_METHOD(vector,sliceFrom_Length_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  mos_ARGV[1] = mos_send(mos_ARGV[1], mos_s(asInteger));
  if ( I < 0 || I > L ) {
    mos_return(mos_error(mos_s(rangeError), "index: %d", (int) I));
  }
  if ( J < 0 || I + J > L ) {
    mos_return(mos_error(mos_s(rangeError), "size: %d", (int) J));
  }

  mos_MIMPL = mos_send(mos_MIMPL, mos_s(clone));
  V += I;
  L = J;
  AL -= I;
  
  mos_return(mos_MIMPL);
}
mos_METHOD_END

mos_METHOD(vector,foreachDo_)
{
  mos_vector_LOOP(mos_MIMPL,ep)
    mos_send(mos_ARGV[0], mos_s(value_), *ep);
  mos_vector_LOOP_END
}
mos_METHOD_END

mos_METHOD(vector,deepen)
{
  mos_value *v = mos_malloc(sizeof(V[0]) * L);
  
  if ( V ) {
    memcpy(v, V, sizeof(V[0]) * L);
  } else {
    size_t i;
    for ( i = 0; i < L; i ++ ) {
      v[i] = mos_undef;
    }
  }
  V = v;
  AL = L;
  FLAGS &= ~mos_READONLY;
}
mos_METHOD_END

mos_METHOD(vector,makeReadonly)
{
  FLAGS |= mos_READONLY;
}
mos_METHOD_END

/************************************************************************
**
** Mutable vector
**
*/
mos_METHOD(vector,length_)
{
  mos_value *v;
  
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  if ( I < 0 ) {
    mos_return(mos_error(mos_s(rangeError), "index: %d", (int) I));
  }
  
  if ( AL < I ) {
    while ( AL < I ) {
      AL += AL + 1;
    }
    v = mos_malloc(sizeof(V[0]) * (AL));
    if ( V ) {
      memcpy(v, V, sizeof(V[0]) * (I < L ? I : L));
    }
    V = v;
  }
  
  {
    size_t i;
    
    for ( i = L; i < I; i ++ ) {
      V[i] = mos_undef;
    }
  }
  
  L = I;
}
mos_METHOD_END


mos_METHOD(vector,allocatedLength_)
{
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  if ( I < 0 || I < L ) {
    mos_return(mos_error(mos_s(rangeError), "index: %d", (int) I));
  }
  if ( I != AL ) {
    mos_value *v;
    
    v = mos_malloc(sizeof(V[0]) * (I));
    if ( V ) {
      memcpy(v, V, sizeof(V[0]) * L);
    }
    AL = I;
    V = v;
  }
}
mos_METHOD_END

mos_METHOD(vector,append_)
{
  size_t l = L;
  
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  mos_send(mos_RCVR, mos_s(length_), mos_integer_make(l + 1));
  V[l] = mos_ARGV[0];
}
mos_METHOD_END

mos_METHOD(vector,__COM__)
{
  mos_return(mos_send(mos_RCVR, mos_s(append_), mos_ARGV[0]));
}
mos_METHOD_END

mos_METHOD(vector,insert_)
{
  size_t l = L;
  
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  mos_send(mos_RCVR, mos_s(length_), mos_integer_make(l + 1));
  memmove(V + 1, V, sizeof(V[0]) * l);
  V[0] = mos_ARGV[0];
}
mos_METHOD_END



mos_METHOD(vector,set_Value_)
{
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  if ( I < 0 || I >= L ) {
    mos_return(mos_error(mos_s(rangeError), "index: %d", (int) I));
  }
  V[I] = mos_ARGV[1];
}
mos_METHOD_END

mos_METHOD(vector,fill_)
{
  mos_value *v = V;
  mos_value *ve = v + L;

  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  while ( v < ve ) {
    *(v ++) = mos_ARGV[0];
  }
}
mos_METHOD_END

mos_METHOD(vector,foreachApply_)
{
  if ( FLAGS & mos_READONLY ) {
    mos_vector_LOOP(mos_MIMPL,ep);
      mos_send(mos_ARGV[0], mos_s(value_), *ep);
    mos_vector_LOOP_END
  } else {
    mos_vector_LOOP(mos_MIMPL,ep);
      *ep = mos_send(mos_ARGV[0], mos_s(value_), *ep);
    mos_vector_LOOP_END
  }
}
mos_METHOD_END

/**************************************************************************
**
** Vector as stack
*/
mos_METHOD(vector,push_)
{
  mos_return(mos_send(mos_RCVR, mos_s(append_), mos_ARGV[0]));
}
mos_METHOD_END

mos_METHOD(vector,top)
{
  if ( ! L ) {
    mos_return(mos_error(mos_s(rangeError), "empty"));
  }
  mos_return(V[L - 1]);
}
mos_METHOD_END

mos_METHOD(vector,pop)
{
  mos_value x;
  size_t i;
  
  if ( FLAGS & mos_READONLY ) {
    mos_return(mos_error(mos_s(readonlyError), 0));
  }

  if ( ! L ) {
    mos_return(mos_error(mos_s(rangeError), "empty"));
  }
  
  i = L - 1;
  x = V[i];
  mos_send(mos_RCVR, mos_s(length_), mos_integer_make(i));
  
  mos_return(x);
}
mos_METHOD_END

#undef SELF
#undef V
#undef L
#undef AL
#undef FLAGS
#undef I
#undef J

mos_OBJECT(vector)
mos_OBJECT_M(vector,asVector)
mos_OBJECT_M(vector,asConstant)
mos_OBJECT_M(vector,printOn_)
mos_OBJECT_M(vector,printShallowOn_)
mos_OBJECT_M(vector,encodeOtherOn_)
mos_OBJECT_M(vector,decodeOtherOn_)
mos_OBJECT_M(vector,length)
mos_OBJECT_M(vector,allocatedLength)
mos_OBJECT_M(vector,_equal_)
mos_OBJECT_M(vector,get_)
mos_OBJECT_M(vector,sliceFrom_Length_)
mos_OBJECT_M(vector,foreachDo_)
mos_OBJECT_M(vector,deepen)
mos_OBJECT_M(vector,makeReadonly)
mos_OBJECT_M(vector,set_Value_)
mos_OBJECT_M(vector,fill_)
mos_OBJECT_M(vector,length_)
mos_OBJECT_M(vector,allocatedLength_)
mos_OBJECT_M(vector,append_)
mos_OBJECT_M(vector,__COM__)
mos_OBJECT_M(vector,insert_)
mos_OBJECT_M(vector,foreachApply_)
mos_OBJECT_M(vector,push_)
mos_OBJECT_M(vector,top)
mos_OBJECT_M(vector,pop)
mos_OBJECT_SLOTS(vector)
mos_OBJECT_END(protos,vector,mos_vector,basicMeta)

mos_value mos_vector_make(size_t _l, const mos_value *_v)
{
  mos_vector *x = mos_REFT(mos_clone(mos_o(vector)),mos_vector);
  x->_v = (mos_value*) _v;
  x->_l = _l;
  x->_al = _l;
  x->_flags = 0;
  return mos_MAKE_REF(x);
}

mos_value mos_vector_makev(size_t _l, va_list *_vap)
{
  mos_value *_v = mos_malloc(sizeof(_v[0]) * _l);
  size_t i;
  mos_value v;
  
  for ( i = 0; i < _l; i ++ ) {
    _v[i] = va_arg(*_vap,mos_value);
  }

  return mos_vector_make(_l, _v);
}

mos_value mos_vector_make_(size_t _l, ...)
{
  mos_value v;
  va_list vap;
  
  va_start(vap,_l);
  v = mos_vector_makev(_l, &vap);
  va_end(vap);
   
  return v;
}

