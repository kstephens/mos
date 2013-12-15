#include "mos/mos.h"
#include "mos/weak.h"
#include "mos/slots.h"
#include <string.h>

mos_ANNOT("Module: defaultBehavior")
mos_ANNOT("Doc: The basic behavior delegate.  MOS does a very low level, behind-the-scenes lookup delegation to this special object when certain messages cannot be found through the usual lookup semantics.  This is primarly for bootstrapping and low-level object messages that all objects must respond to; like @\"_clone\", @\"_meta\", @\"isConstant\", and the basic printing and encoding facilities.")

mos_ANNOT("Category: Finalize")
mos_METHOD(defaultBehavior,_finalize_)
{
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Weak Pointer")
mos_METHOD(defaultBehavior,asWeakPtr)
{
  mos_return(mos_integer_make(mos_weakPtrForObject(mos_RCVR)));
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Constant")
mos_METHOD(defaultBehavior,isConstant)
{
  mos_return(mos_false);
}
mos_METHOD_END
mos_ANNOT_END

mos_ANNOT("Category: Print")
mos_METHOD(defaultBehavior,printOnAsMethodBody_)
{
  mos_return(mos_printf(mos_ARGV[0], "{ %-P }", mos_RCVR));
}

mos_METHOD_END
mos_METHOD(defaultBehavior,printOn_AsMethodWithSelector_)
{
  mos_value selSplit = mos_send(mos_ARGV[1], mos_s(split));
  int nargs = mos_INT(mos_send(mos_ARGV[1], mos_s(nargs)));
  
  if ( nargs ) {
    int i = 0;
    
    mos_vector_LOOP(selSplit, p)
      mos_printf(mos_ARGV[0], "%W %W ", *p, mos_send(mos_send(mos_RCVR, mos_s(argumentAt_), mos_integer_make(i ++)), mos_s(asString)));
    mos_vector_LOOP_END
  } else {
    mos_printf(mos_ARGV[0], "%W ", mos_send(mos_ARGV[1], mos_s(name)));
  }
  
  mos_return(mos_send(mos_RCVR, mos_s(printOnAsMethodBody_), mos_ARGV[0]));
}
mos_METHOD_END

mos_METHOD(defaultBehavior,printSlots_On_)
{
  mos_value mmap;
  int i;
  mos_value meth, sel, val;
  const char *sep;
  
  mmap = mos_ARGV[0];
  
  mos_ARGV[1] = mos_printf(mos_ARGV[1], "%[{");

  for ( i = 0; i < mos_NSLOTS(mos_RCVR); i ++ ) {
    val = mos_SLOTS(mos_RCVR)[i];
    if ( mos_NE(mos_send(mmap, mos_s(hasValue_), meth = mos_setter_method(i)), mos_false) ) {
      sel = mos_send(mmap, mos_s(getKey_), meth);
      mos_send(mmap, mos_s(remove_), sel);
      sel = mos_send(sel, mos_s(getter));
      mos_send(mmap, mos_s(remove_), sel);
      if ( mos_NE(val,mos_undef) ) {
	sep = "->";
	mos_printf(mos_ARGV[1], "\n%W %s %-P;", mos_send(sel, mos_s(asString)), sep, val);
      } else {
	mos_printf(mos_ARGV[1], "\n%W;", mos_send(sel, mos_s(asString)));
      }
    } else
    if ( mos_NE(mos_send(mmap, mos_s(hasValue_), meth = mos_getter_method(i)), mos_false) ) {
      sel = mos_send(mmap, mos_s(getKey_), meth);
      mos_send(mmap, mos_s(remove_), sel);
      sep = "=";
      mos_printf(mos_ARGV[1], "\n%W %s %-P;", mos_send(sel, mos_s(asString)), sep, val);
    } else {
      continue;
    }
  }
  
  {
  mos_map_LOOP(mos_REFT(mmap,mos_map),ms)
    sel = ms->_key;
    val = ms->_value;
    mos_printf(mos_ARGV[1], "\n");
    mos_send(val, mos_s(printOn_AsMethodWithSelector_), mos_ARGV[1], sel);
  mos_map_LOOP_END
  }
  
  mos_return(mos_printf(mos_ARGV[1], "%]\n}"));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,printableMethods)
{
  mos_return(mos_send(mos_send(mos_RCVR, mos_s(_meta)), mos_s(methodMapForObject_), mos_RCVR));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,printSlotsOn_)
{
  mos_return(mos_send(mos_RCVR, mos_s(printSlots_On_), mos_send(mos_send(mos_RCVR, mos_s(printableMethods)), mos_s(clone)), mos_ARGV[0]));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,printOn_)
{
  mos_return(mos_send(mos_RCVR, mos_s(printSlotsOn_), mos_ARGV[0]));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,printShallowOn_)
{
  mos_value x = mos_object_name(mos_RCVR);
  if ( mos_NE(x, mos_false) ) {
    mos_return(mos_printf(mos_ARGV[0], "@N%P", x));
  }
  mos_return(mos_send(mos_RCVR, mos_s(printIdOn_), mos_ARGV[0]));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,printIdOn_)
{
  mos_return(mos_printf(mos_ARGV[0], "@#%P", mos_send(mos_RCVR, mos_s(asWeakPtr))));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,writeOn_)
{
  mos_return(mos_send(mos_RCVR, mos_s(printOn_), mos_ARGV[0]));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,writeShallowOn_)
{
  mos_return(mos_send(mos_RCVR, mos_s(writeOn_), mos_ARGV[0]));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,print)
{
  mos_printf(mos_FILE(out), "%P", mos_RCVR);
  mos_return(mos_undef);
}
mos_METHOD_END


mos_METHOD(defaultBehavior,write)
{
  mos_printf(mos_FILE(out), "%W", mos_RCVR);
  mos_return(mos_undef);
}
mos_METHOD_END

mos_ANNOT_END

mos_ANNOT("Category: Clone")

mos_METHOD(defaultBehavior,_clone)
{
  mos_return(mos_clone(mos_RCVR));
}
mos_METHOD_END
mos_METHOD(defaultBehavior,clone)
{
  mos_return(mos_clone(mos_RCVR));
}
mos_METHOD_END

mos_ANNOT_END


/************************************************************************/


  mos_ANNOT("Category: Value")

 
mos_METHOD(defaultBehavior,value)
{
}
mos_METHOD_END


mos_METHOD(defaultBehavior,value_)
{
  mos_return(mos_send(mos_RCVR, mos_s(value)));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,value_With_)
{
  mos_return(mos_send(mos_RCVR, mos_s(value_), mos_ARGV[0]));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,value_With_With_)
{
  mos_return(mos_send(mos_RCVR, mos_s(value_With_), mos_ARGV[0], mos_ARGV[1]));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,value_With_With_With_)
{
  mos_return(mos_send(mos_RCVR, mos_s(value_With_With_), mos_ARGV[0], mos_ARGV[1], mos_ARGV[2]));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,value_With_With_With_With_)
{
  mos_return(mos_send(mos_RCVR, mos_s(value_With_With_With_), mos_ARGV[0], mos_ARGV[1], mos_ARGV[2], mos_ARGV[3]));
}
mos_METHOD_END


  mos_ANNOT_END


/*******************************************************************/


  mos_ANNOT("Category: Equal")


mos_METHOD(defaultBehavior,eq_)
{
  mos_return(mos_EQ(mos_RCVR, mos_ARGV[0]) ? mos_true : mos_false);
}
mos_METHOD_END


mos_METHOD(defaultBehavior,ne_)
{
  mos_return(mos_EQ(mos_send(mos_RCVR, mos_s(eq_), mos_ARGV[0]), mos_false) ? mos_true : mos_false);
}
mos_METHOD_END


mos_METHOD(defaultBehavior,_equal_)
{
  /*
  ** If the auxillary stuff is equal they are equal.
  */
  mos_return(memcmp(mos_AUXSLOTS(mos_RCVR), mos_AUXSLOTS(mos_ARGV[0]), mos_AUXSIZE(mos_RCVR)) ? mos_false : mos_true);
}
mos_METHOD_END


mos_METHOD(defaultBehavior,equal_)
{
  /* If the objects are eq: they're obviously equal: */
  if ( mos_EQ(mos_RCVR, mos_ARGV[0]) )
    mos_return(mos_true);
  
  /*
  ** The objects must be of the same shape:
  ** 1. Same intrinsic size. (i.e. mos_string cannot be equal: to mos_object)
  ** 2. Same number of methods. (We won't compare method maps; it's too expensive)
  ** 3. Same meta.
  ** 4. Same number of slots.
  */
  if ( mos_DESC(mos_RCVR) != mos_DESC(mos_ARGV[0]) ) {
    if ( mos_SIZEOF(mos_RCVR) != mos_SIZEOF(mos_ARGV[0]) )
      mos_return(mos_false);
    if ( mos_METHODS(mos_RCVR)->_m_nslots != mos_METHODS(mos_ARGV[0])->_m_nslots )
      mos_return(mos_false);
    if ( mos_NE(mos_META(mos_RCVR), mos_META(mos_ARGV[0])) )
      mos_return(mos_false);
    if ( mos_NSLOTS(mos_RCVR) != mos_NSLOTS(mos_ARGV[0]) )
      mos_return(mos_false);
  }
  
  /*
  ** If the object's slots are not the same, they are not equal.
  */
  if ( memcmp(mos_SLOTS(mos_RCVR), mos_SLOTS(mos_ARGV[0]), sizeof(mos_SLOTS(mos_RCVR)[0]) * mos_NSLOTS(mos_RCVR)))
    mos_return(mos_false);
  
  /*
  ** Check the specific _equal: implementation.
  */
  mos_return(mos_send(mos_RCVR, mos_s(_equal_), mos_ARGV[0]));
}
mos_METHOD_END


  mos_ANNOT_END


mos_value mos_EQUAL_(mos_value X, mos_value Y)
{
  return mos_send((X), mos_s(equal_), (Y));
}


int mos_EQUAL(mos_value X, mos_value Y)
{
  return mos_NE(mos_EQUAL_(X, Y), mos_false);
}


/*******************************************************************/


  mos_ANNOT("Category: Meta")


/*******************************************************************/


  mos_ANNOT("Category: Object Descriptor")


mos_METHOD(defaultBehavior,_objectDesc)
{
  mos_return(mos_MAKE_REF(mos_DESC(mos_RCVR)));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,_objectDesc_)
{
  mos_objectDesc *desc;
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asObjectDesc));
  desc = mos_REFT(mos_ARGV[0], mos_objectDesc);
  if ( mos_DESC(mos_RCVR) == desc ) 
    mos_return(mos_RCVR);

  /*
  ** Make sure not to do something unsafe like:
  ** make the object think it has more slots or is bigger than
  ** actually allocated.
  */
  if ( mos_FLAGS(mos_RCVR) & mos_SEALED ) {
    mos_return(mos_error(mos_s(rangeError), "rcvr is sealed"));
  }
  if ( desc->_nslots > mos_DESC(mos_RCVR)->_nslots ) {
    mos_return(mos_error(mos_s(rangeError), "new objectDesc has more slots than previous descriptor"));
  }
  if ( desc->_sizeof > mos_DESC(mos_RCVR)->_sizeof ) {
    mos_return(mos_error(mos_s(rangeError), "new objectDesc has sizeof bigger than previous objectDesc"));
  }

  /* Make the descriptor copy-on-write, since it might be shared. */
  if ( ! (desc->_flags & (mos_SHARED_DESC)) ) {
    desc->_flags |= (mos_COPY_ON_WRITE);
  }

  mos_DESC(mos_RCVR) = desc;
}
mos_METHOD_END


  mos_ANNOT_END


  mos_ANNOT("Category: Object Slots")


mos_METHOD(defaultBehavior,_objectSlots)
{
  mos_return(mos_send(mos_META(mos_RCVR), mos_s(slotVectorForObject_), mos_RCVR));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,_objectSlots_)
{
  mos_return(mos_send(mos_META(mos_RCVR), mos_s(slotVectorForObject_), mos_RCVR));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,_deepenSlots)
{
  _mos_deepenSlots(mos_REF(mos_RCVR));
}
mos_METHOD_END

  mos_ANNOT_END


  mos_ANNOT("Category: Object Meta")


mos_METHOD(defaultBehavior,_meta)
{
  mos_return(mos_META(mos_RCVR));
}
mos_METHOD_END


mos_METHOD(defaultBehavior,_meta_)
{
  _mos_setMeta(mos_RCVR, mos_ARGV[0]);
}
mos_METHOD_END


  mos_ANNOT_END


/************************************************************************/


  mos_ANNOT("Category: Object Slots")


mos_METHOD(defaultBehavior,_addSlots_)
{
  _mos_addSlots(mos_RCVR, mos_ARGV[0], 0);
}
mos_METHOD_END


mos_METHOD(defaultBehavior,_addSlotsIfAbsent_)
{
  _mos_addSlots(mos_RCVR, mos_ARGV[0], 1);
}
mos_METHOD_END


mos_METHOD(defaultBehavior,_addSlot_Value_)
{
  mos_value meta = mos_META(mos_RCVR);
  mos_send(meta, mos_s(addValue_ForSelector_ToObject_), mos_ARGV[1], mos_ARGV[0], mos_RCVR);
}
mos_METHOD_END


mos_METHOD(defaultBehavior,_addReadOnlySlot_Value_)
{
  mos_value meta = mos_META(mos_RCVR);
  mos_send(meta, mos_s(addReadOnlyValue_ForSelector_ToObject_), mos_ARGV[1], mos_ARGV[0], mos_RCVR);
}
mos_METHOD_END


mos_METHOD(defaultBehavior,_addMethodSlot_Value_)
{
  mos_value meta = mos_META(mos_RCVR);
  mos_send(meta, mos_s(addMethod_ForSelector_ToObject_), mos_ARGV[1], mos_ARGV[0], mos_RCVR);
}
mos_METHOD_END


  mos_ANNOT_END

  mos_ANNOT_END


mos_ANNOT("Category: Coder")
mos_METHOD(defaultBehavior,encodeOn_)
{
  /* If rcvr is named, encode as named, otherwise encode as a raw object */
  mos_value name = mos_send(mos_o(named), mos_s(objectName_), mos_RCVR);
  if ( mos_EQ(name, mos_false) ) {
    mos_send(mos_ARGV[0], mos_s(encodeAsObject_), mos_RCVR);
  } else {
    mos_send(mos_ARGV[0], mos_s(encodeAsObject_WithName_), mos_RCVR, name);
  }
}
mos_METHOD_END
mos_METHOD(defaultBehavior,encodeOtherOn_)
{
}
mos_METHOD_END
mos_METHOD(defaultBehavior,decodeOtherOn_)
{
}
mos_METHOD_END
mos_METHOD(defaultBehavior,beforeEncodeOn_)
{
}
mos_METHOD_END
mos_METHOD(defaultBehavior,afterDecodeOn_)
{
}
mos_METHOD_END
mos_ANNOT_END


mos_OBJECT(defaultBehavior)
mos_OBJECT_M(defaultBehavior,_finalize_)

mos_OBJECT_M(defaultBehavior,asWeakPtr)

mos_OBJECT_M(defaultBehavior,printOnAsMethodBody_)
mos_OBJECT_M(defaultBehavior,printOn_AsMethodWithSelector_)
mos_OBJECT_M(defaultBehavior,printSlots_On_)
mos_OBJECT_M(defaultBehavior,printableMethods)
mos_OBJECT_M(defaultBehavior,printSlotsOn_)
mos_OBJECT_M(defaultBehavior,printOn_)
mos_OBJECT_M(defaultBehavior,printShallowOn_)
mos_OBJECT_M(defaultBehavior,printIdOn_)
mos_OBJECT_M(defaultBehavior,writeOn_)
mos_OBJECT_M(defaultBehavior,writeShallowOn_)
mos_OBJECT_M(defaultBehavior,print)
mos_OBJECT_M(defaultBehavior,write)

mos_OBJECT_M(defaultBehavior,isConstant)

mos_OBJECT_M(defaultBehavior,_clone)
mos_OBJECT_M(defaultBehavior,clone)

mos_OBJECT_M(defaultBehavior,value)
mos_OBJECT_M(defaultBehavior,value_)
mos_OBJECT_M(defaultBehavior,value_With_)
mos_OBJECT_M(defaultBehavior,value_With_With_)
mos_OBJECT_M(defaultBehavior,value_With_With_With_)

mos_OBJECT_M(defaultBehavior,eq_)
mos_OBJECT_M(defaultBehavior,ne_)
mos_OBJECT_M(defaultBehavior,equal_)
mos_OBJECT_M(defaultBehavior,_equal_)

mos_OBJECT_M(defaultBehavior,_objectDesc)
mos_OBJECT_M(defaultBehavior,_objectDesc_)

mos_OBJECT_M(defaultBehavior,_objectSlots)
mos_OBJECT_M(defaultBehavior,_objectSlots_)

mos_OBJECT_M(defaultBehavior,_deepenSlots)

mos_OBJECT_M(defaultBehavior,_meta)
mos_OBJECT_M(defaultBehavior,_meta_)

mos_OBJECT_M(defaultBehavior,_addSlots_)
mos_OBJECT_M(defaultBehavior,_addSlotsIfAbsent_)
mos_OBJECT_M(defaultBehavior,_addSlot_Value_)
mos_OBJECT_M(defaultBehavior,_addReadOnlySlot_Value_)
mos_OBJECT_M(defaultBehavior,_addMethodSlot_Value_)

mos_OBJECT_M(defaultBehavior,encodeOn_)
mos_OBJECT_M(defaultBehavior,encodeOtherOn_)
mos_OBJECT_M(defaultBehavior,decodeOtherOn_)
mos_OBJECT_M(defaultBehavior,beforeEncodeOn_)
mos_OBJECT_M(defaultBehavior,afterDecodeOn_)

mos_OBJECT_M(defaultBehavior,_annot)
mos_OBJECT_M(defaultBehavior,_annotFor_)
mos_OBJECT_M(defaultBehavior,_annot_)
mos_OBJECT_M(defaultBehavior,_annot_For_)

mos_OBJECT_SLOTS(defaultBehavior)
mos_OBJECT_END(behaviors,defaultBehavior,mos_object,basicMeta)


mos_ANNOT_END


mos_ANNOT_END
