#ifndef _mos_objdesc_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_objdesc_h__
#define __rcs_id_mos_objdesc_h__
static const char __rcs_id_mos_objdesc_h[] = "$Id: objdesc.h,v 1.2 1999-02-19 09:26:30 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_objdesc_h

#include "mos/value.h" /* mos_value */
#include "mos/objhdr.h" /* mos_object_HDR */
#include "mos/map.h" /* mos_map */

enum {
  mos_SHARED_DESC = 1,
  	/* This desc is never copy-on-write */
  mos_COPY_ON_WRITE = 2,
  	/* Make a copy before you change this desc. */
  mos_CLONE_FLAT = 4,
  	/* Put the slots directly after allocating object hdr (one malloc() instead of 2) */
  mos_SEALED = 8,
  	/* This desc is not allowed to change. */
};

/* An object descriptor is also an object */
struct mos_objectDesc {
  mos_object_HDR;

  mos_value _meta;
  	/* The object to handle lookupMethod: and applyMethod: messages */
  mos_map _methods;
  	/* Map selectors to methods */
  size_t _nslots;
  	/* Number of slots */
  size_t _sizeof;
  	/* Size of object hdr */
  int _flags;
  	/* Flags above OR'ed  */
  mos_value _annot;
        /* The object's annotations */
};

#endif
