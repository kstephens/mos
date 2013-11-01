#ifndef _mos_objdesc_h
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
  const char *_name;
        /* The objects descriptor name. */
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
