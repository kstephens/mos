#ifndef __rcs_id__
#ifndef __rcs_id_mos_objdesc_c__
#define __rcs_id_mos_objdesc_c__
static const char __rcs_id_mos_objdesc_c[] = "$Id: objdesc.c,v 1.4 2000-03-21 07:13:44 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"


/******************************************************************/

#define SELF mos_REFT(mos_MIMPL,mos_objectDesc)

/******************************************************************/

mos_ANNOT("Module: objdesc")
mos_ANNOT("Doc: The low-level object descriptor.  \
All objects have an object descriptor.  \
Object descriptors contain the following information about an object: \n\
  Its meta object. \n\
  Its number of value slots. \n\
  The methods that it responds to. \n\
  Its annotations. \n\
\n\
Object descriptors are like classes in class-based objects systems except that objects may share object descriptors until they are modified by copy-on-write operations.")

/******************************************************************/

  mos_ANNOT("Category: Coerce")

mos_METHOD(objectDesc,asObjectDesc)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END

  mos_ANNOT_END

/******************************************************************/

  mos_ANNOT("Category: Coder")

mos_METHOD(objectDesc,encodeOtherOn_)
{
  mos_objectDesc *o = SELF;

  /* encode the object's meta */
  mos_send(mos_ARGV[0], mos_s(encodeAsReference_), o->_meta);

  /* encode the object's size */
  mos_send(mos_ARGV[0], mos_s(encodeRawInteger_), mos_integer_make(o->_sizeof));

  /* encode the object's number of slots */
  mos_send(mos_ARGV[0], mos_s(encodeRawInteger_), mos_integer_make(o->_nslots));

  /* encode the object's flags */
  mos_send(mos_ARGV[0], mos_s(encodeRawInteger_), mos_integer_make(o->_flags));

  /* encode the object's method map */
  mos_send(mos_ARGV[0], mos_s(encodeAsReference_), mos_MAKE_REF(&o->_methods));
}
mos_METHOD_END

mos_METHOD(objectDesc,decodeOtherOn_)
{
  mos_objectDesc *o = SELF;
  mos_value methods;

  o->_meta = mos_send(mos_ARGV[0], mos_s(decode));
  o->_sizeof = mos_INT(mos_send(mos_ARGV[0], mos_s(decodeRawInteger)));
  o->_nslots = mos_INT(mos_send(mos_ARGV[0], mos_s(decodeRawInteger)));
  o->_flags = mos_INT(mos_send(mos_ARGV[0], mos_s(decodeRawInteger)));
  methods = mos_send(mos_ARGV[0], mos_s(decode));
  o->_methods = *mos_REFT(methods, mos_map);
}
mos_METHOD_END

  mos_ANNOT_END

/******************************************************************/

  mos_ANNOT("Category: ObjectDescription")

mos_METHOD(objectDesc,meta)
{
  mos_return(SELF->_meta);
}
mos_METHOD_END

mos_METHOD(objectDesc,methods)
{
  mos_return(mos_MAKE_REF(&(SELF->_methods)));
}
mos_METHOD_END

mos_METHOD(objectDesc,nslots)
{
  mos_return(mos_integer_make(SELF->_nslots));
}
mos_METHOD_END

mos_METHOD(objectDesc,sizeof)
{
  mos_return(mos_integer_make(SELF->_sizeof));
}
mos_METHOD_END

mos_METHOD(objectDesc,flags)
{
  mos_return(mos_integer_make(SELF->_flags));
}
mos_METHOD_END

  mos_ANNOT_END

/******************************************************************/

#undef SELF

mos_OBJECT(objectDesc)
mos_OBJECT_M(objectDesc,asObjectDesc)
mos_OBJECT_M(objectDesc,encodeOtherOn_)
mos_OBJECT_M(objectDesc,decodeOtherOn_)
mos_OBJECT_M(objectDesc,meta)
mos_OBJECT_M(objectDesc,methods)
mos_OBJECT_M(objectDesc,nslots)
mos_OBJECT_M(objectDesc,sizeof)
mos_OBJECT_M(objectDesc,flags)
mos_OBJECT_M(objectDesc,annotations)
mos_OBJECT_M(objectDesc,__annot)
mos_OBJECT_M(objectDesc,__annot_)
mos_OBJECT_M(objectDesc,__annot_For_)
mos_OBJECT_M(objectDesc,__annotFor_)
mos_OBJECT_SLOTS(objectDesc)
mos_OBJECT_END(protos,objectDesc,mos_objectDesc,basicMeta)

mos_ANNOT_END
mos_ANNOT_END

/******************************************************************/

mos_INIT(objectDesc,-10)
{
  mos_REFT(mos_o(objectDesc),mos_objectDesc)->_meta = mos_o(basicMeta);
  mos_REFT(mos_o(objectDesc),mos_objectDesc)->_methods = *mos_REFT(mos_o(map),mos_map);
  mos_REFT(mos_o(objectDesc),mos_objectDesc)->_nslots = 0;
  mos_REFT(mos_o(objectDesc),mos_objectDesc)->_sizeof = sizeof(mos_object);
  mos_REFT(mos_o(objectDesc),mos_objectDesc)->_flags = 0;
  return 0;
}

