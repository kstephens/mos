
#define mos_expr(X) mos_send(mos_o(X), mos_s(clone))
#define mos_exprConstant(X) mos_send(mos_expr(X), mos_s(value_), (X))
