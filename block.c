#include "mos/mos.h"
#include "mos/block.h"

mos_ANNOT("Module: block")
mos_ANNOT("Doc: A block object provides deferred execution of a lexical context.  All initialized block objects respond to @\"value*\" messages.")

mos_METHOD(block,asBlock)
{
}
mos_METHOD_END

mos_OBJECT(block)
mos_OBJECT_A(_lexicalContext,mos_block___lexicalContext)
mos_OBJECT_A(_methodContext,mos_block___methodContext)
mos_OBJECT_SLOTS(block)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(protos,block,mos_object,basicMeta)

mos_ANNOT_END
mos_ANNOT_END

