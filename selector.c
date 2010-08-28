#include "mos/mos.h"
#include "mos/constant.h"
#include "mos/selector.h"
#include <string.h>


/* A cached list of selectors */
static mos_value _selectors_list;
static mos_selector *_selectors;

mos_value _mos_selector_list(void)
{
  if ( ! mos_INITIALIZED(_selectors_list) ) {
    mos_selector *s;
    size_t l = 0;
    mos_value *v;

    /* Count the number of selectors */
    for ( s = _selectors; s; s = s->_next ) {
      l ++;
    }

    /* Create an array of selectors */
    v = mos_malloc(sizeof(v[0]) * l);
    l = 0;
    for ( s = _selectors; s; s = s->_next ) {
      v[l ++] = mos_MAKE_REF(s);
    }

    {
      mos_value V = mos_vector_make(l, v);
      mos_vector_FLAGS(V) |= mos_READONLY;
      _selectors_list = V;
    }
    
  }
  return _selectors_list;
}


static
void _mos_selector_init(mos_selector *x)
{
  /* Initialize the selectors hidden slots */
  mos_value str = mos_string_make(x->_namestr, strlen(x->_namestr));
  mos_string_FLAGS(str) |= mos_READONLY;
  x->_name = str;
  x->_next = _selectors;
  x->_getter = 0;
  x->_setter = 0;
  
#define IS_OP(X) strchr(_mos_selector_op_charset, X[0])

  /* Compute the number of arguments for the selector */
  if ( IS_OP(x->_namestr) ) {
    x->_nargs = 1;
  } else {
    const char *s = x->_namestr;
    x->_nargs = 0;
    while ( *s ) {
      if ( *(s ++) == ':' )
	x->_nargs ++;
    }
  }

  _selectors_list = mos_UNINITIALIZED;
  _selectors = x;
}


mos_value mos_selector_make_(const char *name, size_t l)
{
  mos_selector **x = &_selectors, *s;
  
  /* Compute the length, if needed */
  if ( l == 0 ) {
    l = strlen(name);
  }
  
  /* Escape the name string, if needed */
  if ( name[0] == '\01' ) {
    name = _mos_selector_escape(name + 1);
  }
  
  /* Look for an already existing selector with the same name */
  while ( (s = *x) ) {
    if ( strncmp(s->_namestr, name, l) == 0 && s->_namestr[l] == '\0' ) {
      /*
	fprintf(stderr, "\nmos_selector_name_: %s\n", s->_namestr);
      */

      /*
      ** HEURISTIC: Temporal reference locality.
      ** Put the found selector at the head of the selector list.
      */
      *x = s->_next;
      s->_next = _selectors;
      _selectors = s;

      return mos_MAKE_REF(s);
    }
    x = &(s->_next);
  }
  
  /* Create a new selector object */
  s = mos_REFT(mos_clone(mos_o(selector)),mos_selector);
  
  s->_namestr = mos_malloc(l + 1);
  memcpy((char*) s->_namestr, name, l);
  ((char*) s->_namestr)[l] = '\0';
  
  _mos_selector_init(s);
  
  return mos_MAKE_REF(s);
}


mos_value mos_getter_selector(mos_value x)
{
  return mos_send(x, mos_s(getter));
}


mos_value mos_setter_selector(mos_value x)
{
  return mos_send(x, mos_s(setter));
}


#define mos_DEF_s(X) mos_selector _mos_s_##X =  {mos_object_HDR_INIT_STATIC(selector), #X };
#include "mos/sel.def"

static mos_selector *_inits[] = {
  #define mos_DEF_s(X) &_mos_s_##X,
#include "mos/sel.def"
  0
};


mos_INIT(selector,0)
{
  mos_selector **s;
  
  /* Force selector creation */
  (void) mos_s(__ASN__);
  (void) mos_s(parseDebug);
  (void) mos_s(parseDebug_);

  mos_REFT(mos_o(selector),mos_selector)->_namestr = "";
  _mos_selector_init(mos_REFT(mos_o(selector),mos_selector));

  for ( s = _inits; *s; s ++ ) {
    (*s)->_namestr = mos_strdup(_mos_selector_escape((*s)->_namestr));
    _mos_selector_init(*s);
  }
  
  return 0;
}


#define SELF mos_REFT(mos_MIMPL,mos_selector)
mos_METHOD(selector,asSelector)
{
}
mos_METHOD_END


mos_METHOD(selector,equal_)
{
  mos_return(mos_EQ(mos_RCVR, mos_ARGV[0]) ? mos_true : mos_false);
}
mos_METHOD_END


mos_METHOD(selector,printOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "@\"%s\"", SELF->_namestr));
}
mos_METHOD_END


mos_METHOD(selector,encodeOn_)
{
  mos_send(mos_ARGV[0], mos_s(encodeAsSelector_), mos_MIMPL);
}
mos_METHOD_END


mos_METHOD(selector,name)
{
  mos_return(SELF->_name);
}
mos_METHOD_END


mos_METHOD(selector,asString)
{
  mos_return(SELF->_name);
}
mos_METHOD_END


mos_METHOD(selector,isOperator)
{
  mos_return(IS_OP(SELF->_namestr) ? mos_true : mos_false);
}
mos_METHOD_END


mos_METHOD(selector,nargs)
{
  mos_return(mos_integer_make(SELF->_nargs));
}
mos_METHOD_END


mos_METHOD(selector,getter)
{
  if ( ! SELF->_getter ) {
    int len = strlen(SELF->_namestr) - 1;
    if ( SELF->_namestr[len] == ':' ) {
      SELF->_getter = mos_selector_make_(SELF->_namestr, len);
    } else {
      SELF->_getter = mos_undef;
    }
  }
  mos_return(SELF->_getter);
}
mos_METHOD_END


mos_METHOD(selector,setter)
{
  if ( ! SELF->_setter ) {
    if ( IS_OP(SELF->_namestr) ) {
      SELF->_setter = mos_undef;
    } else {
      char buf[1024];
      strcpy(buf, SELF->_namestr);
      strcat(buf, ":");
      SELF->_setter = mos_selector_make(buf);
    }
  }
  mos_return(SELF->_setter);
}
mos_METHOD_END


mos_METHOD(selector,split)
{
  if ( ! SELF->_split ) {
    if ( IS_OP(SELF->_namestr) ) {
      SELF->_split = mos_vector_make_(1, mos_send(mos_RCVR, mos_s(name)));
    } else {
      const char *b, *e;
      
      SELF->_split = mos_vector_make_(0);
      e = SELF->_namestr;
      while ( *e ) {
        b = e;
	while ( *e ) {
	  if ( *e == ':' ) {
	    e ++;
	    break;
	  }
	  e ++;
	}
	{
	  mos_value str = mos_send(mos_string_make(b, e - b), mos_s(makeReadonly));

	  mos_send(SELF->_split, mos_s(append_), str);
	}
      }
      
      mos_send(SELF->_split, mos_s(makeReadonly));
    }
  }
  mos_return(SELF->_split);
}
mos_METHOD_END


mos_METHOD(selector,valueSelectorForNargs_)
{
  /*
  ** 0 value
  ** 1 value:
  ** 2 value:With:
  ** 3 value:With:With:
  ** ...
  */
  int nargs;
  
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  nargs = mos_INT(mos_ARGV[0]);
  
  switch ( nargs ) {
  case 0: mos_return(mos_s(value));
  case 1: mos_return(mos_s(value_));
  case 2: mos_return(mos_s(value_With_));
  case 3: mos_return(mos_s(value_With_With_));
  default: {
      char buf[1024] = "value:";
      char *s;
      
      s = buf;
      
      while ( *s ) {
        s ++;
      }
      
      while ( -- nargs ) {
        const char *t = "With:";
	while ( *t ) {
	  *(s ++) = *(t ++);
	}
      }
      
      *s = '\0';
      
      mos_return(mos_selector_make_(buf, s - buf));
    }
  }
}
mos_METHOD_END


mos_ANNOT("Returns a vector of all selectors.")
mos_METHOD(selector,selectors)
{
  mos_return(_mos_selector_list());
}
mos_METHOD_END
mos_ANNOT_END


#undef SELF


mos_OBJECT(selector)
mos_constant_METHODS
mos_OBJECT_M(selector,asSelector)
mos_OBJECT_M(selector,equal_)
mos_OBJECT_M(selector,printOn_)
mos_OBJECT_M(selector,encodeOn_)
mos_OBJECT_M(selector,name)
mos_OBJECT_M(selector,asString)
mos_OBJECT_M(selector,isOperator)
mos_OBJECT_M(selector,getter)
mos_OBJECT_M(selector,setter)
mos_OBJECT_M(selector,split)
mos_OBJECT_M(selector,nargs)
mos_OBJECT_M(selector,valueSelectorForNargs_)
mos_OBJECT_M(selector,selectors)
mos_OBJECT_SLOTS(selector)
mos_OBJECT_END(protos,selector,mos_selector,basicMeta)



