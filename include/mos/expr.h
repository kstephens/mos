
mos_value mos_exprConstant(mos_value X);
mos_value mos_exprMemo(mos_value X);
mos_value mos_exprGroup(mos_value X);
mos_value mos_exprSelf();
mos_value mos_exprReturnSelf();
mos_value mos_exprMsg();
mos_value mos_exprMethodContext();
mos_value mos_exprVector(mos_value v);
mos_value mos_exprVectorFill(mos_value v);
mos_value mos_exprMap();


mos_value mos_exprSend(mos_value rcvr, mos_value sel, int nargs, ...);
mos_value mos_exprSend_(mos_value cntx, mos_value rcvr, mos_value sel, int nargs, ...);

mos_value mos_exprSendV(mos_value rcvr, mos_value sel, mos_value args);
mos_value mos_exprSend_V(mos_value cntx, mos_value rcvr, mos_value sel, mos_value args);

mos_value mos_exprObject(mos_value slots, mos_value body);
mos_value mos_exprSlot(mos_value sel, mos_value val);
mos_value mos_exprReadOnlySlot(mos_value sel, mos_value val);
mos_value mos_exprMethodSlot(mos_value sel, mos_value val);

mos_value mos_exprMethod(mos_value args, mos_value locals, mos_value body);
mos_value mos_exprBlock(mos_value args, mos_value locals, mos_value body);
