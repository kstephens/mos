#ifndef _mos_weak_h
#define _mos_weak_h

#include "mos/mos.h"

mos_int mos_weakPtrForObject(mos_value x);
mos_value mos_objectForWeakPtr(mos_int x);

#endif
