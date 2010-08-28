#ifndef __rcs_id__
#ifndef __rcs_id_mos_catch_c__
#define __rcs_id_mos_catch_c__
static const char __rcs_id_mos_catch_c[] = "$Id: catch.c,v 1.4 2000-03-21 07:13:43 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include "mos/catch.h"
#include <stdlib.h> /* abort() */
#include <assert.h>

/* NOT THREAD SAFE */
static mos_catch *_currentCatch = 0;

void _mos_CATCH_BEGIN(mos_value catch)
{
  mos_catch *c = mos_REFT(catch,mos_catch);
  
  /*
  ** Link this catch with previous catch.
  */
  c->_prevCatch = _currentCatch;

  /*
  ** Save value stack pointer.
  */
  c->_spSave = _mos_sp;

  /*
  ** Make this catch the current catch.
  */
  _currentCatch = c;

  /*
  ** Mark this catch as valid.
  */
  c->_jbvalid = 1;
}
void _mos_CATCH_END(mos_value catch)
{
  mos_catch *c = mos_REFT(catch,mos_catch);

  /*
  ** Matk this catch as invalid.
  */
  c->_jbvalid = 0;

  /*
  ** Make sure the stack pointer is what was saved.
  */
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
    
    /*
    ** Restore value stack pointer.
    */
    _mos_sp = c->_spSave;

    /*
    ** Save the value and mark as thrown.
    */
    c->_slots[mos_catch_tvalue] = value;
    c->_slots[mos_catch_thrown] = mos_true;
    
    /*
    ** Get outta here!
    */
    longjmp(c->_jb, 1);

    /* NOTREACHED */
    abort();
    return mos_undef;
  } else {
    /*
    ** The catch was uses once before or went out of scope.
    */
    return mos_error(mos_s(invalidCatch), "%P", catch);
  }
}

/******************************************************************/

#define C mos_REFT(mos_MIMPL,mos_catch)

/******************************************************************/

mos_ANNOT("Module: catch")
mos_ANNOT("Doc: The catch object.  \
Catch objects are used to do non-local transfer of control.  \
Catch objects are used by block objects to transfer control to lexical closures.  \
Catch objects are also used for error handling.  \
Catch objects respond to @\"value:\" messages by transfering control to context of the @\"do:*\" message that created them.\
Example: \
<CODE>\n\
_ system proto catch clone do:[| c |\n\
  doSomething;\n\
  c value: value;\n\
  somethingNeverDone;\n\
];\n\
\n\
_ system proto catch clone do:[| c |\n\
  doSomething;\n\
  c value: value;\n\
  somethingNeverDone;\n\
] IfThrown:[| v |\n\
  \"I was thrown \" writeOn: _ system os stdio out;\n\
  v printOn: _ system os stdio out;\n\
  \"\n\" writeOn: _ system os stdio out;\n\
];\n\
</CODE>\
")

/******************************************************************/

  mos_ANNOT("Category: Coerce")

mos_METHOD(catch,asCatch)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END

  mos_ANNOT_END

/******************************************************************/

  mos_ANNOT("Category: Catch")

mos_ANNOT("Doc: Creates a catch object (C) and performs (arg0 value: C).  If (C value: X) is called then return X, otherwise return the value of (arg0 value: C).")
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
mos_ANNOT_END

mos_ANNOT("Doc: Creates a catch object (C) and performs (arg0 value: C).  If (C value: X) is done then return (arg1 value: (X value)), otherwise return result of (arg0 value: C).")
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
mos_ANNOT_END

  mos_ANNOT_END

/******************************************************************/

  mos_ANNOT("Category: Throw")

mos_ANNOT("Doc: Causes non-local exit to the @\"catch:\" or @\"catch:IfThrown:\" that created the receiver.")
mos_METHOD(catch,throw_)
{
  mos_return(mos_THROW(mos_MIMPL,mos_ARGV[0]));
}
mos_METHOD_END
mos_METHOD(catch,value_)
{
  mos_return(mos_THROW(mos_MIMPL,mos_ARGV[0]));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Doc: Same as (rcvr throw: @U).")
mos_METHOD(catch,throw)
{
  mos_return(mos_send(mos_RCVR, mos_s(throw_), mos_undef));
}
mos_METHOD_END
mos_METHOD(catch,value)
{
  mos_return(mos_send(mos_RCVR, mos_s(throw_), mos_undef));
}
mos_METHOD_END
mos_ANNOT_END

  mos_ANNOT_END

/******************************************************************/

#undef C

mos_OBJECT(catch)
mos_OBJECT_M(catch,asCatch)
mos_OBJECT_M(catch,do_)
mos_OBJECT_M(catch,do_IfThrown_)
mos_OBJECT_M(catch,throw_)
mos_OBJECT_M(catch,throw)
mos_ANNOT("Category: Internal")
mos_ANNOT("Doc: The value that was thrown.")
mos_OBJECT_A(tvalue,mos_catch_tvalue)
mos_ANNOT_END
mos_ANNOT("Doc: True if a value was thrown.")
mos_OBJECT_A(thrown,mos_catch_thrown)
mos_ANNOT_END
mos_ANNOT_END
mos_OBJECT_SLOTS(catch)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_false)
mos_OBJECT_END(protos,catch,mos_catch,basicMeta)

mos_ANNOT_END
mos_ANNOT_END

/******************************************************************/
