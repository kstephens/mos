# MOS - Meta Object System

V0.1
Copyright 1997-2024 [Kurt A. Stephens](mailto:ks.github@kurtstephens.com)

## Introduction

MOS is an embeddable, pure, prototype-based object development system with a meta-object protocol. Objects in MOS simply contain value and method slots. Everything in MOS is an object; every operation is achieved by a message send.

### Conventions

In this document `fixed with font` denotes:
- literal strings delimited the single quote: `'`
- regular expressions delimited by forward slash: `/`
- BNF grammar productions and terminals: `<name>`

### Semantics

The semantics of MOS are similar to the Self and SmallTalk programming environments except:

- MOS objects do not have explicit classes (like Self, unlike SmallTalk)
- MOS objects do not have an inheritance or delegation mechanism (Self has built-in delegation, SmallTalk has classes and metaclasses)
- The process of sending message to MOS objects can be redefined at run-time

## Expressions

MOS syntax is reminicent of Smalltalk (and Self).

### Atoms

Atoms always evaluate to themselves.
They are either constant or have state: e.g. strings maybe side-effected.

| Type | Example |
|------|---------|
| Integer | `5` |
| Float | `5.5` |
| Character | `'c'` |
| String | `"abc"` |
| Boolean true | `@T` |
| Boolean false | `@F` |
| Undefined value | `@U` |
| End-of-stream value | `@E` |
| Method selector named "foo" | `@"foo"` |
| Object named `"foo"` | `@N"foo"` |

### Objects

Objects are defined by slot definitions enclosed in a pair of curly braces '`{`' '`}`'. The objects are only created once, thus have a temporal extent of the inner-most inclosing method definition (or console expression). An object slot definition can be of 4 types:

| Slot | Example |
|------|---------|
| uninitialized | `x;` |
| initialized | `x -> 5;` |
| constant | `x = 5;` |
| unary method | `foo {` _..._ `}` |
| binary method | `baz: arg {` _..._ `}` |
| multiary method slot | `a: arg1 B: arg2 C: arg3 {` _..._ `}` |
| binary operator method | `+ bar {` _..._ `}` |

Example object definitions:

- `{}` - An empty object
- `{x;}` - An object with a uninitialized value slot named "`x`"
- `{x=5; y { _: (x + 5); }` - an object with a constant value named "`x`" and a method named "`y`" that returns "`10`"

### Containers

Container objects are created at run-time and are not literals.

| Type | Example | Contents |
|------|---------|----------|
| vector | `(@V, 1, "xyz", 3)` | zero-origin, length 3 |
| map | `(@M, "a" -> 1, "b" -> 2)` | mapping from `"a"` to `1`, etc. |

### Messages

Messages are built from unary, binary and multiary messages consisting of a receiver, selector and arguments.

| Type | Expression | Receiver | Selector | Arguments |
|------|------------|----------|----------|-----------|
| unary | `system`  | `_`      | `@"system"` | _(none)_ |
| binary | `foo get: 5` | `foo` | `@"get:"` | `5` |
| indexed get | `a-vector[2]` | `a-vector` | `@"get:"` | `5` |
| indexed set | `a-vector[3]: 7` | `a-vector` | `@"set:Value:"` | `7` |
| binary operator | `foo +-+ bar` | `foo` | `@"+-+"` | `bar` |
| multiary | `foo bar a: 1 B: 2 C: 3 neg + baz` | `foo bar` | `@"a:B:C:"` | `1`, `2`, `(-3 + baz)` |
| multiary | `foo bar[x]:y z` | `foo bar` | `@"set:Value:"` | `x`, `(y z)` |
| return | `_: 5 * 2` | current message | `@"return"` | `(5 * 2)` |

The receiver of a message maybe unspecified; the semantic of the message depends on the lexical scope:

- if the selector is `@"_msg"`, the current message object is accessed.
- if the selector is `@"self"`, the message accesses the current message's receiver.
- in a block context; `@"self"`, accesses the block's lexical message receiver.
- if the selector is an identifier (or a keyword) and is a formal parameter list of the enclosing method-slot defntion, the message is a `@"get:"` or `@"set:Value:"` operation on the message's arguments.
- if the selector is an identifier (or a keyword) and is a local variable in the enclosing method-slot defintion, the message is a `@"get:"` or `@"set:Value:"` message on the message's local variable.

### Memos

Memos are lexical and evaluate their expressions _only_ once.

`@'(@V, 1, 2, 3)` evaluates to the same vector object.


### Lobby

The lobby where objects are defined, the scope of interactive sessions and source files.
It is specified with "`_`" identifier and is always the root receiver expression/
The lobby always contains a reference to itself and the '`system`' object.

It always contains a reference to the `(_ system)` object and itself `(_ _)`:

``` text
_ _addSlots: {
  foo -> 5;
  bar;
  baz = 2;
};
_ _loadFile: "src/delegate.mos";
```

## Examples


``` text
mos> "xyz" length;
 3

mos> 5 neg + 2 * 3;
 -21

mos> "xyz" get: 1;
 'y'

 # Note: X[Y] is the same as (X get: Y):
mos> "xyz"[1];
 'y'

mos> "xyz" set: 1 Value: 'b';
 "xbz"

 # Note: X[Y]: Z is the same as (X set: Y Value: Z):
mos> "xyz"[1];
 'y'

 # Constructs a vector object.
mos> (@V, 1, "xyz", 3)[2];
 "xyz"

 # Constructs a map object. |#`
mos> (@M, "a" -> 1, "b" -> 2);
 (@M, "a" -> 1, "b" -> 2)

mos> (@M, "a" -> 1, "b" -> 2)["a"];
 1
```

### Object Annotations

Annotations are strings that are attached to objects, value slots or methods. Annotations are used for on-line documentation, slot grouping, and for browsing. Parts of an annotation are flagged by using keywords like "Doc:" or "Category:".

Annotations can be accessed for any object: `(_obj_ _annot)` or `(_obj_ _annotFor: _slot_).`

Annotations can be added to any object: `(_obj_ _annot: _docstring_) or (_obj_ _annot: _docstring_ For: slot).`

Annotations can be defined in source code: "`#{ ANNOTATION-TEXT...\n`" and delimited scope with "`#}`".

Example:

``` text
#{this object
  { # This object's annotation is "this object"
    #{this slot
       x = 5; # This slot's annotation is "this object\\0x7fthis slot
    #}
  }
#}annotation end
```

Nested annotations are concatenated using the `'\x7f'` character.

### Object Cloning

When an object is cloned via the low-level `(obj _clone)` message:

1. A bitwise copy of the object is made,
2. A bitwise copy of the object's value slot vector is made,
3. The object's descriptor object is marked as copy-on-write.

If an object with a copy-on-write descriptor has a method added, redefined, or removed; or a value slot is added or deleted, a new clone of the descriptor is made and referenced by the object, before any modifications to the descriptor are made. Since this new descriptor is not referenced by any other objects, the copy-on-write flag is turned off.

## System Objects

### `(_)`

The lobby (user namespace).

### `(_ system)`

Contains all system objects.

### `(_ system protos)`

Contains the system prototype objects from which new objects are created.

### `(_ system metas)`

Contains the system meta objects.

### `(_ system oddballs)`

Contains the one-of-a-kind system objects, like `@t`, `@f`, `@u` and `@e`. These objects are never cloned.

### `(_ system behaviors)`

Contains the system behavior delegates.

### `(_ system behaviors defaultBehavior)`

MOS does a very low-level, behind-the-scenes lookup delegation to this special object when certain messages cannot be found through the usual lookup semantics. This is primarly for bootstrapping and low-level object messages that all objects must respond to; like `@"_clone"`, `@"_meta"`, `@"isConstant"` and the basic printing and encoding facilities.

### `(_ system os)`

User interface to the host operating system (most ANSI C and POSIX system calls).

### `(_ system named)`

User interface to named objects.

### `(_ system gc)`

User interface to the garbage collector.

### `(_ system debugger)`

User interface to the debugger.

### `(_ system protos selector selectors)`

Returns a constant vector of all selectors.

### `(_ system compiler)`

User interface to the (bytecode) compiler

### `(_ system protos stream)`

Stream objects represent open files (socket, pipes).

`(stream open: "filename")` - a new stream object opened to filename with mode `mode`.

`(stream close)` - closes the stream object's file handle.

The `(stream object)` is expected to perform `(stream readString: length)` and `(stream writeString: string)` messages.

### `(_ system protos coder)`

The coder prototype contains methods for encoding and decoding objects to and from a stream. The coder object can be used as an encoder or decoder. When a `(coder object)` is used for encoding the coder stream is written to; when a coder object is used for decoding the `(coder stream)` object is read from.

The `(coder table)` object is used to remember which objects have already been written to preserve referential equality after decoding.

## Meta-Object Protocol

### Messaging Protocol

MOS has a meta-object protocol that implements the low-level message `send(object, selector, args)` operation as a compostion of `method = lookup(object, selector)` and `apply(object, method, args)` messages to meta-objects. Each MOS object has a "`_meta`" object associated with it.

The MOS `send(receiver, selector, arguments)` primitive is implemented by creating a "`message`" object containing the message's receiver, the message's method selector, and the message's arguments. Then the receiver's meta object gets a meta-message to find a method in the receiver and store it in the message object. Then the message's method's meta object gets a meta-message to apply the message's method to the message's receiver. In pseudo-code, `send()` might be implemented as:

``` text
send(rcvr, sel, args) {
  message = new Message(rcvr, sel, args);
  message.rcvr.\_meta.lookupMethod(message);
  return message.method.\_meta.applyMethod(message);
}
```

A MOS object's `_meta` object can be changed to implement different method lookup and application behaviors, thus classes, delegation and proxy objects can be implemented with meta-objects that implement "`lookupMethod:`" methods. Likewise, different method application behaviors, such as: tracing and multi-methods can be implemented by defining new method objects with new meta-objects that implement "`applyMethod:`" methods.

Two standard meta-object primitives are:

- `(_ system metas basicMeta)` which implements "`lookupMethod:`"
- `(_ system metas methodMeta)` which implements all of the `basicMeta` object and implements the "`applyMethod:`" operation.

### Reflection Protocol

`(obj _meta)`, `(obj _meta: _newMeta_)`

An object's meta-object can be retrieved or set with the above messages.

`(obj _objectDesc)`, `(obj _objectDesc: _newObjDesc_)`

An object's decscriptor is an object that contains the object's size, number of slots and its method map. Object descriptors may be shared between many objects (to reduce memory usage and to provide type information to the compiler) but are shared using copy-on-write updating.

## Examples

``` text
  # Define 4 variable slots in the lobby:
_addSlots: { myObject; encoder; stream; decoder; };

  # Create basic object:
myObject: { a -> 1; b -> 2; };

  # Create a coder object:
encoder: _ system protos coder clone;

  # Create an empty string stream:
encoder stream: (("" clone) deepen);

  # Encode myObject:
encoder encode: myObject;

  # Get the string stream:
stream: (encoder stream);

  # Create a decoder object.  Note: the stream proto is cloned, first:
decoder: (_ system protos coder clone stream: (stream clone));

  # Decode an object (myObject) from the decoder stream:
decoder decode;
```

## Syntax

### Comments

Comments begin at the `'#'`and continue through to the end of the line. Multi-line comments can be specified with `'#|'`and terminate with `'|#'`. Multi-line comments can nested. Comments are treated as whitespace.

### Numbers

Numbers must begin with a digit `/[0-9]/`, decimal point `'.'`, `'+'`or `'-'`sign.

### Integers

Integers can be specified with different radices prefixes; the default is base 10:

- octal (base 8): `/0[0-8]*/`
- hexadecimal (base 16): `/0[xX][0-9a-fA-F]+/`
- binary (base 2): `/0[bB][0-1]+/`

### Reals

Reals are sequences of decimal digits with `'.'` or exponents specified with `/\[eEfFgG\]/`.

### Strings

Strings are delimited by double quote (`'"'`) characters; and use the standard C backslash (`'\'`) escape sequences for special characters.

### Characters

Characters are special string instances specified as one character delimited by single quote (`"'"`) characters.

### Special Sequences

Special sequences start with the ampersand (`'@'`) character and a dispatch character. Standard special sequences are:

- True object constant: `@t`
- False object constant: `@f`
- "End-of-stream" constant: `@e`
- "undefinedValue" constant: `@u`
- message selector constant: `/@"<selector-charset>*"/`
- vector constructor sequence: `@v`
- map constructor sequence: `@m`
- named object: `@n"NAME"`
- memo: `@'`

### Selectors

Selectors are used to look up methods for messages and are broken down by:

### Identifiers

`/[_a-z][_a-zA-Z0-9]*/`

Specify unary message expressions (no arguments).

### Keywords

`/[_a-z][_a-zA-Z0-9]*:/`

Specify binary message expressions (one argument) and are used to begin multi-argument message expressions (see Capwords).

### Capwords

`/[A-Z][_a-zA-Z0-9]*:/`

Specify multi-argument message expressions.

### Operators

`/[~!$%^&*+-<>?/,|=]+/`

Specify binary message expressions. Operator expressions always group left-to-right regardless of expected mathematic precedence. `5+2*3` means `(5+2)*3`.

### Memo Expressions

The back-quote character (``'`'``) begins a memo expression. Memo expressions are evaluated only once.

### Object Literals

An object literal is an expression that creates a memoized object. The object may have slots. An object literal body is delimited with `{` and `}`. Within the object body are any number of object slots.

### Object Slots

Object slots come in four types:

- Uninitialzed data slots  _`<identifier>`_`;`
- Initialized data slots: _`<identifier>`_ '->' _`<expr>`_ `';'`
- Read-only data slots: _`<identifier>`_ '=' _`<expr>`_ `';'`
- method slots:
  - _`<identifier>`_ '{' _`<method-body>` `'}'`
  - _`<operator>`_ _`<identifier>`_ '{' _`<method-body>`_ `'}'`
  - _`<keyword>`_ _`<identifier>`_ _`<capword-identifier-list>`_ '{' _`<method-body>`_ `'}'`


## Others

- A weak pointer - `/@#<number>/`

## Grammar

The MOS grammar extended BNF:

``` text
<expr> :
   <constant> |
   <object> |
   <block> |
   '(' <expr> ')' |
   <expr> <identifier> |
   <expr> <keyword> <expr> { <capword> <expr> } * |
   <expr> <operator> expr |
   <expr> '[' <expr> ']' |
   <expr> '[' <expr> ']' ':' <expr> |
   <memo>
   ;
<memo> : '@\'' <expr>
<object> : '{' <slot> + '}' ;

<slot> : <value-slot> | <method-slot> ;
<value-slot> :
  <uninitialized-slot>  |
  <readonly-slot>       |
  <initialized-slot>    ;
<uninitialized-slot> : <identifier> ';' ;
<readonly-slot>      : <identifier> '=' <expr> ';' ;
<initialized-slot>   : <identifier> '->' <expr> ';' ;

<method-slot> :
  <unary-method-slot> |
  <binary-method-slot> |
  <multiary-method-slot> ;

<unary-method-slot> : <identifier> <method-body> ;

<binary-method-slot> :
  <keyword> <formal> <method-body> |
  <operator> <formal> <method-body> ;

<multiary-method-slot> :
  <keyword> <formal> (<capword> <formal>)+ <method-body> ;

<method-body> : '{' <locals> ? <stmt> * '}' ;

<locals> : '@' <local> * '@' ;
<local>  : <value-slot> ;

<stmt> : <expr> ';' ;
<block> : '[' <block-formals> <stmt> * ']' ;
<block-formals> : '(' <identifier> * ')' ;
```


## Requirements

- GCC or clang
- GNU bison
- perl 5.x
- GNU readline
- GNU history
- PCRE
- Boehm GC
