#ifndef __rcs_id__
#ifndef __rcs_id_mos_readline_c__
#define __rcs_id_mos_readline_c__
static const char __rcs_id_mos_readline_c[] = "$Id: readline.c,v 1.5 2001-08-13 09:33:48 stephens Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include "readline/readline.h" /* readline() */
#include <string.h> /* strncmp() */

#ifndef HAS_HISTORY
#define HAS_HISTORY 1
#endif
#if HAS_HISTORY
#include "readline/history.h" /* add_history() */
#endif

/*************************************************************************/
/* Completion support */

static
char *
_mos_completion_generator (text, state)
     char *text;
     int state;
{
  static int list_index, len;
  static mos_value selectors;

  char *name;

  /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
  if ( ! state ) {
    list_index = 0;
    len = strlen (text);
    selectors = _mos_selector_list();
  }

  /* Return the next name which partially matches from the command list. */
  while ( list_index < mos_vector_L(selectors) ) {
    const char *name = mos_REFT(mos_vector_V(selectors)[list_index],mos_selector)->_namestr;
    list_index ++;
    if (strncmp (name, text, len) == 0)
      return (mos_strdup(name));
  }

  /* If no names matched, then return NULL. */
  return ((char *)NULL);
}

static
char **
_mos_completion (text, start, end)
     char *text;
     int start, end;
{
  char **matches;

  matches = rl_completion_matches (text, _mos_completion_generator);

  return (matches);
}


/***********************************************************************/


mos_ANNOT("Module: readline")
mos_ANNOT("Doc: The object that represents GNU readline() as an input stream object.")

mos_METHOD(readline,asStream)
{
  mos_return(mos_MIMPL);
}
mos_METHOD_END

mos_METHOD(readline,close)
{
}
mos_METHOD_END

mos_ANNOT("Doc: Calls readline(), calls add_history(), and appends result to _buffer.  Return @e if readline() returns 0\n")
mos_METHOD(readline,_readline)
{
  char *s;
  mos_value b;

  b = mos_send(mos_RCVR, mos_s(prompt));
  b = mos_send(b, mos_s(asString));
  s = readline(mos_string_V(b));

  /*
    fprintf(stderr, "readline: %s\n", s ? s : "<NULL>");
  */

  if ( s ) {

#if HAS_HISTORY
    if ( s[0] != '\0' )
      add_history(s);
#endif

    b = mos_send(mos_RCVR, mos_s(_buffer));
    mos_writeString(b, s, strlen(s));
    mos_writeString(b, "\n", 1);
  }

  mos_return(s ? mos_true : mos_eos);
}
mos_METHOD_END
mos_ANNOT_END

  mos_ANNOT("Category: Read")

mos_METHOD(readline,readString_)
{
  mos_value s;
  size_t bytes_read;
  
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  s = mos_send(mos_string_make(0, 0), mos_s(length_), mos_ARGV[0]);
  /* IMPLEMENT */
  abort();

  mos_return(s);
}
mos_METHOD_END

mos_METHOD(readline,peekChar)
{
  mos_value c;

  c = mos_send(mos_send(mos_RCVR, mos_s(_buffer)), mos_s(peekChar));
  if ( mos_EQ(c, mos_eos) ) {
    c = mos_send(mos_RCVR, mos_s(_readline));
    if ( mos_NE(c, mos_eos) ) {
      c = mos_send(mos_send(mos_RCVR, mos_s(_buffer)), mos_s(peekChar));
    }   
  }

  mos_return(c);
}
mos_METHOD_END

mos_METHOD(readline,readChar)
{
  mos_value c;

  c = mos_send(mos_send(mos_RCVR, mos_s(_buffer)), mos_s(readChar));
  if ( mos_EQ(c, mos_eos) ) {
    c = mos_send(mos_RCVR, mos_s(_readline));
    if ( mos_NE(c, mos_eos) ) {
     c = mos_send(mos_send(mos_RCVR, mos_s(_buffer)), mos_s(readChar));
    }
  }

  if ( mos_EQ(c, mos_char_make('\n')) ) {
    mos_send(mos_RCVR, mos_s(line_), mos_send(mos_RCVR, mos_s(line)), mos_s(__ADD__), mos_integer_make(1));
  }

  mos_return(c);
}
mos_METHOD_END

  mos_ANNOT_END

mos_OBJECT(readline)
mos_OBJECT_M(readline,asStream)
mos_OBJECT_M(constant,clone)
mos_OBJECT_M(readline,close)
mos_OBJECT_M(readline,_readline)
mos_OBJECT_M(readline,readString_)
mos_OBJECT_M(readline,readChar)
mos_OBJECT_M(readline,peekChar)
mos_ANNOT("Category: Internal")
mos_OBJECT_A(_buffer,0)
mos_OBJECT_A(line,1)
mos_OBJECT_A(prompt,2)
mos_ANNOT_END
mos_OBJECT_SLOTS(readline)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(os,readline,mos_object,basicMeta)

mos_ANNOT_END
mos_ANNOT_END

mos_INIT(readline,90)
{
    /* Allow conditional parsing of the ~/.inputrc file. */
  rl_readline_name = "mos";

  /* Tell the completer that we want a crack first. */
  rl_attempted_completion_function = (CPPFunction *)_mos_completion;

#if HAS_HISTORY
  /* Initialize the history library */
  using_history();
#endif

  mos_send(mos_o(readline), mos_s(_buffer_), mos_string_make(0, 0));
  mos_send(mos_o(readline), mos_s(line_), mos_integer_make(1));
  mos_send(mos_o(readline), mos_s(prompt_), mos_string_make_(" mos> "));
  _mos_addSlot(mos_o(os), mos_s(readline), mos_o(readline));
  mos_send(mos_o(debugger), mos_s(input_), mos_o(readline));

  return 0;
}






