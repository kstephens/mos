#include "mos/mos.h"
#include "mos/memcpy.h"
#include "mos/mapfuncs.h"

extern mos_objectDesc *_mos_clone_desc(mos_objectDesc *d);

__inline
void _mos_copyDescForWrite(mos_object *x)
{
  /* If the object is sealed, don't allow any modifications. */
  if ( x->_desc->_flags & mos_SEALED ) {
    mos_error(mos_s(sealedObjectError), "%O", mos_MAKE_REF(x));
  } else
  /* If the desc is not shared, */
  if ( ! (x->_desc->_flags & mos_SHARED_DESC) ) {
    /* If it's copy-on-write, */
    if ( x->_desc->_flags & mos_COPY_ON_WRITE ) {
      /* Clone the desc and clear the copy-on-write flag */
      x->_desc = _mos_clone_desc(x->_desc);
      x->_desc->_flags &= ~ mos_COPY_ON_WRITE;
    }
  }
}

__inline
void _mos_setMeta(mos_value x, mos_value m)
{
  if ( mos_NE(mos_META(x), m) ) {
    _mos_copyDescForWrite(mos_REF(x));
    mos_META(x) = m;
  }
}

__inline
void _mos_reallocSlots(mos_object *x, int nslots)
{
  mos_value *slots;
  
  _mos_copyDescForWrite(x);
  slots = mos_malloc(sizeof(x->_slots[0]) * nslots);
  mos_memcpy(slots, x->_slots, sizeof(x->_slots[0]) * (nslots < x->_desc->_nslots ? nslots : x->_desc->_nslots));
  x->_slots = slots;
  x->_desc->_nslots = nslots;
}

__inline
void _mos_addMethod(mos_object *x, mos_value sel, mos_value meth)
{
  _mos_copyDescForWrite(x);
  _mos_map_setOrAdd(&(x->_desc->_methods), sel, meth);
}

__inline
void _mos_removeMethod(mos_object *x, mos_value sel)
{
  _mos_copyDescForWrite(x);
  _mos_map_remove(&(x->_desc->_methods), sel);
}

__inline
int _mos_addReadOnlySlotForced(mos_object *x, mos_value sel, mos_value val)
{
  int sloti;
  
  /* If the object has a shared desc that's actually shared, raise an error */
  /* Rational: A shared desc cannot have accessor methods to slots that do not exist */
  if ( (x->_desc->_flags & mos_SHARED_DESC) && (x->_desc->_flags & mos_COPY_ON_WRITE) ) {
    abort();
    mos_error(mos_s(sharedDescError), "slot %P", sel);
    return -1;
  }

  _mos_copyDescForWrite(x);
  sloti = x->_desc->_nslots;
  _mos_reallocSlots(x, sloti + 1);
  x->_slots[sloti] = val;
  _mos_addMethod(x, sel, mos_getter_method(sloti));
  
  return sloti;
}

__inline
void _mos_addReadOnlySlot(mos_object *x, mos_value sel, mos_value val)
{
  _mos_copyDescForWrite(x);
  _mos_addMethod(x, sel, mos_constant_method(val));
}


__inline
void _mos_addSlot(mos_object *x, mos_value sel, mos_value val)
{
  int sloti  = _mos_addReadOnlySlotForced(x, sel, val);
  
  if ( sloti != -1 ) {
    _mos_addMethod(x, mos_setter_selector(sel), mos_setter_method(sloti));
  }
}

__inline
void _mos_addSlotIfAbsent(mos_object *x, mos_value sel, mos_value val)
{
  void *ms;

  if ( ! (ms = _mos_map_find(&(x->_desc->_methods), sel)) ) {
    _mos_addSlot(x, sel, val);
  }
}

/*
** Add slots to x from src,
** ifAbsent is true, only add slots that don't already exist
** in x.
*/
void _mos_addSlots(mos_value x, mos_value src, int ifAbsent)
{
  mos_value mmap;
  int i;
  mos_value meth, sel, val;
  const char *sep;
  mos_value xmeta;
  int hasSlot;
  
  xmeta = mos_send(x, mos_s(_meta));
  /*
  ** Get the src object's methodMap so we can find
  ** accessor methods quickly.  We create a copy
  ** so we can modify it to keep track of accessors .vs. other methods.
  */
  mmap = mos_send(mos_send(mos_send(src, mos_s(_meta)), mos_s(methodMapForObject_), src), mos_s(clone));
  
  /*
  ** Loop through all the writable slots
  */
  for ( i = 0; i < mos_NSLOTS(src); i ++ ) {
    val = mos_SLOTS(src)[i];
    
    /*
    ** if the src's method map has a setter method for slot i,
    ** if the dst's object has an existing method for the selector of src's slot i,
    **   Send a message to dst to change it's value,
    ** Otherwise,
    **   Add a new writable slot to dst.
    */
    if ( mos_NE(mos_send(mmap, mos_s(hasValue_), meth = mos_setter_method(i)), mos_false) ) {
      sel = mos_send(mmap, mos_s(getKey_), meth);
      mos_send(mmap, mos_s(remove_), sel);
      
      sel = mos_send(sel, mos_s(getter));
      mos_send(mmap, mos_s(remove_), sel);
      
      hasSlot = mos_NE(mos_send(xmeta, mos_s(lookupMethodForSelector_InObject_), sel, x), mos_undef);
      
      if ( hasSlot ) {
        if ( ! ifAbsent ) {
          mos_send(x, sel, val);
	}
      } else { 
        _mos_addSlot(mos_REF(x), sel, val);
      }
    } else
    /*
    ** If the src only has a getter method (this is the case if the slot was
    ** writable, but it's setter method was removed)
    ** Add a read only slot.
    */
    if ( mos_NE(mos_send(mmap, mos_s(hasValue_), meth = mos_getter_method(i)), mos_false) ) {
      sel = mos_send(mmap, mos_s(getKey_), meth);
      mos_send(mmap, mos_s(remove_), sel);
      
      hasSlot = mos_NE(mos_send(xmeta, mos_s(lookupMethodForSelector_InObject_), sel, x), mos_undef);

      if ( hasSlot ) {
        if ( ! ifAbsent ) {
          mos_send(x, sel, val);
	}
      } else { 
        _mos_addReadOnlySlot(mos_REF(x), sel, val);
      }
    }
  }

  /*
  ** The remaining methods may simply be installed.
  ** This loop also inserts constant value methods created
  ** by _mos_addReadOnlySlot().
  */
  {
  mos_map_LOOP(mos_REFT(mmap,mos_map),ms)
    sel = ms->_key;
    val = ms->_value;
    
    hasSlot = mos_NE(mos_send(xmeta, mos_s(lookupMethodForSelector_InObject_), sel, x), mos_undef);
    if ( ! (ifAbsent && hasSlot) ) {
      _mos_addMethod(x, sel, val);
    }
  mos_map_LOOP_END
  }
}

