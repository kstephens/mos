#include "mos/mos.h"
#include "mos/catch.h"
#include <assert.h>

/*
_ system proto catch clone do:[| c |
  c throw: value;
];

_ system proto catch clone do:[| c |
  c throw: value;
] IfThrown:[| v |
  "I was thrown " writeOn: _ system os stdio out;
  v printOn: _ system os stdio out;
  "\n" writeOn: _ system os stdio out;
];

@{
  do: blk {
    # setjmp _jb
    _: blk value: self;
  }
  throw: val {
    value: val;
    # longjmp(1)
  }
};
*/

/* NOT THREAD SAFE */
static mos_catch *_currentCatch = 0;

void _mos_CATCH_BEGIN(mos_value catch)
{
  mos_catch *c = mos_REFT(catch,mos_catch);
  
  c->_jbvalid = 1;
  c->_prevCatch = _currentCatch;
  c->_spSave = _mos_sp;
  _currentCatch = c;
}
void _mos_CATCH_END(mos_value catch)
{
  mos_catch *c = mos_REFT(catch,mos_catch);

  c->_jbvalid = 0;
  assert(c->_spSave == _mos_sp);
}

mos_value mos_THROW(mos_value catch, mos_value value)
{
  mos_catch *c = mos_REFT(catch,mos_catch);

  if ( c->_jbvalid ) {
    /*
    ** Invalidate all catches between the current one and the we are
    ** longjmping to.
    */
    while ( _currentCatch != c ) {
      _currentCatch->_jbvalid = 0;
      _currentCatch = _currentCatch->_prevCatch;
    }
    
    /*
    ** Invalidate ourselves and make the _currentCatch the previous one
    */
    mos_REFT(c,mos_catch)->_jbvalid = 0;
    _currentCatch = c->_prevCatch;
    
    _mos_sp = c->_spSave;

    /*
    ** Save the value and mark as thrown.
    */
    c->_slots[mos_catch_value] = value;
    c->_slots[mos_catch_thrown] = mos_true;
    
    /*
    ** Get outta here!
    */
    longjmp(c->_jb, 1);
    
    return mos_undef;
  } else {
    return mos_error(mos_s(invalidCatch), "%P", catch);
  }
}



#define C mos_REFT(mos_MIMPL,mos_catch)

mos_METHOD(catch,asCatch)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END

mos_METHOD(catch,do_)
{
  mos_value val;
  
  mos_CATCH_BEGIN(mos_MIMPL)
    val = mos_send(mos_ARGV[0], mos_s(value_), mos_RCVR);
  mos_CATCH_CAUGHT
    val = mos_send(mos_RCVR, mos_s(value));
  mos_CATCH_END
  
  mos_return(val);
}
mos_METHOD_END

mos_METHOD(catch,do_IfThrown_)
{
  mos_value val;
  
  mos_CATCH_BEGIN(mos_MIMPL)
    val = mos_send(mos_ARGV[0], mos_s(value_), mos_RCVR);
  mos_CATCH_CAUGHT
    val = mos_send(mos_RCVR, mos_s(value));
    val = mos_send(mos_ARGV[1], mos_s(value_), val);
  mos_CATCH_END
  
  mos_return(val);
}
mos_METHOD_END


mos_METHOD(catch,throw_)
{
  mos_return(mos_THROW(mos_MIMPL,mos_ARGV[0]));
}
mos_METHOD_END

mos_METHOD(catch,throw)
{
  mos_return(mos_send(mos_RCVR, mos_s(throw_), mos_undef));
}
mos_METHOD_END

#undef C

mos_OBJECT(catch)
mos_OBJECT_M(catch,asCatch)
mos_OBJECT_M(catch,do_)
mos_OBJECT_M(catch,do_IfThrown_)
mos_OBJECT_M(catch,throw_)
mos_OBJECT_M(catch,throw)
mos_OBJECT_A(value,mos_catch_value)
mos_OBJECT_A(thrown,mos_catch_thrown)
mos_OBJECT_SLOTS(catch)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_false)
mos_OBJECT_END(protos,catch,mos_catch,basicMeta)
