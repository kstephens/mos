#ifndef _mos_weak_h
#define _mos_weak_h

#include "mos/mos.h"

unsigned long mos_weakPtrForObject(mos_value x);
mos_value mos_objectForWeakPtr(unsigned long x);

#endif
