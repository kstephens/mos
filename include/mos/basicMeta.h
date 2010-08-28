#define mos_basicMeta_METHODS \
mos_OBJECT_M(basicMeta,lookupMethod_) \
mos_OBJECT_M(basicMeta,lookupMethodForSelector_InObject_) \
mos_OBJECT_M(basicMeta,addMethod_ForSelector_ToObject_) \
mos_OBJECT_M(basicMeta,addReadOnlyValue_ForSelector_ToObject_) \
mos_OBJECT_M(basicMeta,addValue_ForSelector_ToObject_) \
mos_OBJECT_M(basicMeta,removeMethodForSelector_FromObject_) \
mos_OBJECT_M(basicMeta,methodMapForObject_) \
mos_OBJECT_M(basicMeta,objectDescForObject_) \
mos_OBJECT_M(basicMeta,slotVectorForObject_)

mos_value mos_addSlot(mos_value x, mos_value sel, mos_value val);
mos_value mos_addReadOnlySlot(mos_value x, mos_value sel, mos_value val);
mos_value mos_addMethod(mos_value x, mos_value sel, mos_value val);

mos_value mos_EQUAL_(mos_value X, mos_value Y);

