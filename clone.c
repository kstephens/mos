#ifndef __rcs_id__
#ifndef __rcs_id_mos_clone_c__
#define __rcs_id_mos_clone_c__
static const char __rcs_id_mos_clone_c[] = "$Id: clone.c,v 1.3 1999-12-26 20:05:43 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include "mos/memcpy.h"

void _mos_deepenSlots(mos_object *x)
{
  size_t s = sizeof(x->_slots[0]) * x->_desc->_nslots;
  mos_value *v = mos_malloc(s);
  if ( x->_slots ) {
    mos_memcpy(v, x->_slots, s);
    x->_slots = v;
  } else {
    s = x->_desc->_nslots;
    x->_slots = v;
    while ( s -- ) {
      *(v ++) = mos_undef;
    }
  }
}

mos_objectDesc *_mos_clone_desc(mos_objectDesc *d)
{
  mos_objectDesc *nd;
  
  nd = mos_malloc(sizeof(*nd));
  *nd = *d;
  nd->_methods._m_slots = mos_malloc(sizeof(nd->_methods._m_slots[0]) * (nd->_methods._m_nslots + 1));
  mos_memcpy(nd->_methods._m_slots, d->_methods._m_slots, (nd->_methods._m_nslots + 1) * sizeof(nd->_methods._m_slots[0]));

  if ( mos_INITIALIZED(nd->_annot) ) {
    nd->_annot = mos_send(nd->_annot, mos_s(clone));
  }

  return nd;
}

static
mos_object *_mos_allocTall(mos_objectDesc *desc)
{
  mos_object *y;
  y = mos_malloc(desc->_sizeof);
  y->_desc = desc;
  y->_slots = mos_malloc(sizeof(y->_slots[0]) * desc->_nslots);
  return y;
}

static
mos_object *_mos_cloneTall(mos_object *x)
{
  mos_object *y;
  y = mos_malloc(x->_desc->_sizeof);
  mos_memcpy(y, x, x->_desc->_sizeof);
  y->_slots = mos_malloc(x->_desc->_nslots * sizeof(y->_slots[0]));
  mos_memcpy(y->_slots, x->_slots, x->_desc->_nslots * sizeof(x->_slots[0]));
  return y;
}

static
mos_object *_mos_allocFlat(mos_objectDesc *desc)
{
  mos_object *y;
  y = mos_malloc(desc->_sizeof + desc->_nslots * sizeof(y->_slots[0]));
  y->_slots = (mos_value*) (((char*) y) + desc->_sizeof);
  return y;
}

static
mos_object *_mos_cloneFlat(mos_object *x)
{
  mos_object *y;
  y = mos_malloc(x->_desc->_sizeof + x->_desc->_nslots * sizeof(x->_slots[0]));
  mos_memcpy(y, x, x->_desc->_sizeof);
  y->_slots = (mos_value*) (((char*) y) + x->_desc->_sizeof);
  mos_memcpy(y->_slots, x->_slots, x->_desc->_nslots * sizeof(x->_slots[0]));
  return y;
}


mos_object *_mos_clone(mos_object *x)
{
  /*
  ** If the object's descriptor was sealed,
  ** make a copy of the descriptor
  */
  if ( x->_desc->_flags & (mos_SEALED) ) {
    mos_objectDesc *nd = _mos_clone_desc(x->_desc);
    nd->_flags &= ~(mos_SEALED|mos_COPY_ON_WRITE);
    x = _mos_cloneFlat(x);
    x->_desc = nd;
    return x;
  } else {
    /* Share a copy of the object's descriptor */
    x->_desc->_flags |= mos_COPY_ON_WRITE;
    if ( x->_desc->_flags & (mos_CLONE_FLAT) ) {
      return _mos_cloneFlat(x);
    } else {
      return _mos_cloneTall(x);
    }
  }
}

mos_object *_mos_create(mos_objectDesc *desc)
{
  if ( desc->_flags & (mos_SEALED) ) {
    mos_object *x;
    mos_objectDesc *nd = _mos_clone_desc(desc);
    nd->_flags &= ~(mos_SEALED|mos_COPY_ON_WRITE);
    x = _mos_allocFlat(nd);
    x->_desc = nd;
    return x;    
  } else {
    /* Share a copy of the object's descriptor */
    desc->_flags |= mos_COPY_ON_WRITE;
    if ( desc->_flags & (mos_CLONE_FLAT) ) {
      return _mos_allocFlat(desc);
    } else {
      return _mos_allocTall(desc);
    }
  }
}

mos_value mos_clone(mos_value x)
{
  return mos_MAKE_REF(_mos_clone(mos_REF(x)));
}

mos_value mos_create(mos_value x)
{
  return mos_MAKE_REF(_mos_create(mos_REFT(x,mos_objectDesc)));
}


