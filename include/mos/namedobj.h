#ifndef _mos_namedobj_h

#ifndef __rcs_id__
#ifndef __rcs_id_mos_namedobj_h__
#define __rcs_id_mos_namedobj_h__
static const char __rcs_id_mos_namedobj_h[] = "$Id: namedobj.h,v 1.2 1999-02-19 09:26:30 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#define _mos_namedobj_h

mos_value mos_object_name(mos_value obj);
mos_value mos_object_named(mos_value name);
mos_value mos_name_object(mos_value name, mos_value obj);
mos_value mos_name_object_(const char *n1, const char *n2, mos_value obj);

#endif
