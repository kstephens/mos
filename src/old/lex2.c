/*
lexer = @{
  lexemeProto = @{
    token = a-string,	# a string of the parsed token
    type = a-selector,  # the type of token
    value = any		# the value of the token
  }, 
  baseProto = a-number, # the default number base
  stateProto = @s"startState" # the default start state
  
  stream = a-stream,	# the stream to parse from
  c = a-char,		# the character just read
  state = a-selector,	# the current state
  base = a-number,	# the current number base
  
  startState {
  },

  lexeme = a-lexemeProto, # the lexeme parsed
}
*/

#define CASE_DIGIT
  case '0': case '1': case '2': case '3': case '4': \
  case '5': case '6': case '7': case '8': case '9'

#define CASE_LOWER case 'a': case 'b': case 'c': case 'd': case 'e': \
  case 'f': case 'g': case 'h': case 'i': case 'j': \
  case 'k': case 'l': case 'm': case 'n': case 'o': \
  case 'p': case 'q': case 'r': case 's': case 't': \
  case 'u': case 'v': case 'w': case 'x': case 'y': case 'z'

#define CASE_UPPER \
  case 'A': case 'B': case 'C': case 'D': case 'E':
  case 'F': case 'G': case 'H': case 'I': case 'J':
  case 'K': case 'L': case 'M': case 'N': case 'O':
  case 'P': case 'Q': case 'R': case 'S': case 'T':
  case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z'

#define TYPE
#define LEXEME(TYPE,VALUE) do { \
  type = TYPE; \
  value = VALUE; \
  goto done; \
} while (0)

#define STATE mos_send(mos_RCVR, mos_s(state))
#define SET_STATE(X) mos_send(mos_RCVR, mos_s(state_), X)

#define LEXEME mos_send(mos_RCVR, mos_s(lexeme))
#define SET_LEXEME(X) mos_send(mos_RCVR, mos_s(lexeme_), X)

#define BASE mos_INT(mos_send(mos_RCVR, mos_s(base))
#define SET_BASE(X) mos_send(mos_RCVR, mos_s(base), mos_integer_make(X))

#define VALUE mos_send(LEXEME, mos_s(value))
#define SET_VALUE(X) mos_send(LEXEME, mos_s(value_), X)

#define STRING VALUE
#define SET_STRING(X) SET_VALUE(X)

#define NUMBER mos_INT(VALUE)
#define SET_NUMBER(X) SET_VALUE(mos_integer_make(X))

#define TYPE mos_send(LEXEME, mos_s(type))
#define SET_TYPE(X) mos_send(LEXEME, mos_s(type_), X)

#define GOT_LEXEME(T) SET_TYPE(mos_s(T)); mos_send(mos_RCVR, mos_s(done_), mos_true)

mos_METHOD(lexer,parse)
{
  mos_value result;
  
  /* Clone the lexeme proto */
  SET_LEXEME(mos_send(mos_send(mos_RCVR, mos_s(lexemeProto)), mos_s(clone)));
  
  /* Begin at the start state */
  SET_STATE(mos_send(mos_RCVR), mos_s(stateProto));

  /* Use the default number base */
  mos_send(mos_RCVR, mos_s(base_), mos_send(mos_RCVR, mos_s(baseProto)));
  
  /* the value */
  SET_VALUE(mos_undef);
  SET_TYPE(mos_undef);
  SET_DONE(mos_false);
  
  while ( mos_EQ(DONE, mos_s(done)) ) {
    mos_value c;
    
    c = mos_send(mos_send(mos_RCVR, mos_s(stream)), mos_s(readChar));
    mos_send(c, mos_s(c_), mos_RCVR);
    if ( c == mos_eos ) {
      mos_send(mos_RCVR, mos_s(lexeme_), mos_eos);
      break;
    }
    result = mos_send(mos_RCVR, STATE);
    if ( result == mos_eos ) {
      mos_send(mos_RCVR, mos_s(lexeme_), mos_eos);
      break;
    } else
    if ( result == mos_s(ungetc) ) {
      UNGETC();
    }
  }
  
  return mos_send(mos_RCVR, mos_s(lexeme));
}

mos_METHOD(lexer,startState)
{
  whitespace:
  if ( isspace(c) ) {
    while ( isspace((c = GETC()) ) ;
      return mos_s(ungetc);
  }
  if ( c == '#' ) {
    while ( (c = GETC()) != '\n' ) ;
    goto whitespace;
  }
  if ( c == EOF ) {
    return mos_eos;
  }
  
  switch ( c ) {
    case '-':
      SET_STATE(mos_s(numberState));
      break;

    case '0':
      SET_BASE(8);
      /* FALL THROUGH */
	      case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      SET_STATE(mos_s(numberState));
      SET_VALUE(mos_integer_make(c - '0'));
      break;
      
    CASE_LOWER: case '_':
      SET_STATE(mos_s(identifierState));
      SET_TYPE(mos_s(identifier));
      SET_STRING(mos_string_make(0, 0));
      return mos_s(ungetc);
      break;
      
    CASE_UPPER:
      SET_STATE(mos_s(identifierState));
      SET_TYPE(mos_s(capword));
      SET_STRING(mos_string_make(0, 0));
      return mos_s(ungetc);
      break;
      
    case '"':
      SET_STATE(mos_s(stringState));
      SET_TYPE(mos_s(string));
      SET_STRING(mos_string_make(0, 0));
      break;

    default:
      /* DO OPERATOR STUFF */
      GOT_LEXEME();
      SET_TYPE(mos_s(char));
      SET_STRING(mos_string_make(&C, 1));
      return mos_undef;
      break;
  }
  break;
  
  return mos_true;
}

mos_METHOD(lexer,numberState)
{
  switch ( c ) {
    case '-':
      SET_STATE(mos_s(operator));
      mos_writeString(VALUE, "--", 2);
      break;
      
    case 'x': case 'X':
      SET_BASE(16);
      SET_STATE(mos_s(number2State);
      break;
      
    default:
      SET_STATE(mos_s(number2State);
      return mos_s(ungetc);
      break;
  }
}

mos_METHOD(lexer,number2State)
{
  int cc = c;
  
  switch ( cc ) {
    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
      cc -= 'a' - 'A';
      /* FALL THROUGH */
      
    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
      cc -= 'a' + 10 - '0';
      /* FALL THROUGH */
      
    CASE_DIGIT:
      cc -= '0';
      if ( ! (0 <= cc && cc < BASE) ) {
 	GOT_LEXEME();
	return mos_s(ungetc);
      }
      SET_NUMBER(NUMBER * BASE);
      SET_NUMBER(NUMBER + cc - '0');
      break;
     
    default:
      return mos_undef;
  }
}

mos_METHOD(lexer,operatorState)
{
  if ( mos_IS_OPERATOR(c) ) {
    mos_writeString(STRING, &c, 1);
  } else {
    GOT_LEXEME();
    SET_TYPE(mos_s(operator));
    SET_VALUE(mos_selector_make(STRING));
    return mos_s(ungetc);
  }
}

mos_METHOD(lexer,identifierState)
{
  mos_value rtnval = mos_true;
  int colon = 0;
  
  switch ( c ) {
    CASE_UPPER: CASE_LOWER: CASE_DIGIT: case '_':
      mos_writeString(string, &C, 1);
      break;
      
    case ':':
      colon = 1;
      /* FALL THROUGH */
      
    default:
      if ( ! colon ) {
	rtnval = mos_s(ungetc);
      } else {
	mos_writeString(string, &C, 1);
      }
      
      if ( TYPE == mos_s(capword) ) {
	if ( ! colon ) {
	  SET_TYPE(mos_s(error));
	  SET_VALUE(mos_string_make("expected colon after capword", -1));
	}
      } else
      if ( TYPE == mos_s(identifier) ) {
	if ( colon ) {
	  SET_TYPE(mos_s(keyword));
	}
      }
      SET_VALUE(mos_selector_make(STRING));
      SET_DONE();
      break;
  }
  
  return rtnval;
}


mos_METHOD(lexer,stringState)
{
  switch ( c ) {
    case '"':
      GOT_LEXEME(string);
      SET_VALUE(mos_send(STRING, mos_s(decodeCString)));
      SET_DONE();
      break;
      
    case '\\':
      SET_STATE(mos_s(string2State));
      /* FALL THROUGH */
	
    default:
      mos_writeString(STRING, &C, 1);
      break;
  }
  
  return mos_true;
}

mos_METHOD(lexer,string2State)
{
  SET_STATE(mos_s(stringState));
  mos_writeString(STRING, &C, 1);
  return mos_true;  
}

/* convience */
mos_METHOD(lexer,parseFromStream_)
{
  mos_RCVR = mos_send(mos_RCVR, mos_s(clone));
  mos_send(mos_RCVR, mos_s(stream_), mos_ARGV[0]);
  return LEXEME;
}
