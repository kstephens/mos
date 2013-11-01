#ifndef _mos_namedobj_h
#define _mos_namedobj_h

mos_value mos_object_name(mos_value obj);
mos_value mos_object_named(mos_value name);
mos_value mos_name_object(mos_value name, mos_value obj);
mos_value mos_name_object_(const char *n1, const char *n2, mos_value obj);

#endif
