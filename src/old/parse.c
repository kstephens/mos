
#ifndef __rcs_id__
#ifndef __rcs_id_old_parse_c__
#define __rcs_id_old_parse_c__
static const char __rcs_id_old_parse_c[] = "$Id: parse.c,v 1.2 1999-02-19 09:26:36 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

mos_METHOD(parser,parseExpr)
{
  mos_value tok;
  mos_value val;
  
  tok = PEEK_TOKEN();
  if ( mos_EQ(tok, mos_MAKE_CHAR('(')) ) {
    GET_TOKEN();
    tok = EXPR();
    MATCH(mos_MAKE_CHAR(')');
  } else
  if ( mos_EQ(tok, mos_MAKE_CHAR('@')) ) {
    GET_TOKEN();
    val = EXPR();
    val = mos_send(mos_RCVR, mos_s(parseExprSendCntx_), val);
  } else
  if ( mos_EQ(tok, mos_s(number))
  ||   mos_EQ(tok, mos_s(string)) ) {
    val = mos_exprConstant(GET_VALUE());
    tok = mos_send(mos_RCVR, mos_s(parseExprSendRcvr_), val);
    if ( mos_NE(tok,mos_undef) ) {
      val = tok;
    }
  }
  
  mos_return(val);
}
mos_METHOD_END

mos_METHOD(parse,parseExprSendRcvr_)
{
  mos_value rcvr = mos_ARGV[0];
  mos_value tok;
  
  tok = PEEK_TOKEN()
  if ( mos_EQ(tok, mos_s(name)
}
mos_METHOD_END

mos_METHOD(parse,parseExprSendR_)
{
  mos_value cntx = mos_ARGV[0];
  mos_value tok;
  
  tok = PEEK_TOKEN();
  
  mos_return(tok);
}
mos_METHOD_END

