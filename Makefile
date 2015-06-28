# 
#
all:

EXE=#.exe

NAME = mos

CFILES_OPTIMIZED =

CFILES_DEBUG = 

GC=../GC
GC_BEGIN=
GC_END=

CFILES_DEBUG_OPTIMIZED = \
	share.c \
	lex.c \
	mem.c \
	map.c \
        init.c \
	clone.c \
	slots.c \
	basicMeta.c \
	defaultBehavior.c \
	constant.c \
	undef.c \
	bool.c \
	integer.c \
	real.c \
	char.c \
	str.c \
	vector.c \
	selsupp.c \
	selector.c \
	method.c \
	accessor.c \
	block.c \
	srcInfo.c \
	message.c \
	stream.c \
	print.c \
	error.c \
	catch.c \
	objdesc.c \
	namedobj.c \
	system.c \
	os.c \
	debugger.c \
	traceMethod.c \
	bc.c \
	bcMethod.c \
	bcCompile.c \
	bcDecomp.c \
	expr.c \
	weak.c \
	load.c \
	comptest.c \
	strstrm.c \
	readline.c \
	coder.c \
	annot.c \
	regexp.c \
	defaults.c \
	main.c

CFILES_NDEBUG = \
	send.c \
	debug.c

CFILES = $(CFILES_OPTIMIZED) $(CFILES_DEBUG_OPTIMIZED) $(CFILES_DEBUG) $(CFILES_NDEBUG)

YFILES = parse.y

INC_DIR=include/mos

mos_ANNOT_DIR=lib/mos/annot

HFILES = \
	$(INC_DIR)/mos.h \
	$(INC_DIR)/send.h \
	$(INC_DIR)/message.h \
	$(INC_DIR)/vector.h \
	$(INC_DIR)/memcpy.h \
	$(INC_DIR)/bc.h \
	$(INC_DIR)/bc.def \
	$(INC_DIR)/weak.h

DEFS = \
	config.h \
	init.def \
	annot.def \
	$(INC_DIR)/sel.def \
	$(INC_DIR)/obj.def \
	$(INC_DIR)/meth.def

#####################################################################

init.o : init.def
annot.o : annot.def
selector.o : $(INC_DIR)/sel.def $(INC_DIR)/op.def
integer.o: $(INC_DIR)/op.def
real.o: $(INC_DIR)/op.def
system.o : $(INC_DIR)/obj.def
namedobj.o : $(INC_DIR)/obj.def $(INC_DIR)/meth.def
bc.o : $(INC_DIR)/bc.def
bcCompile.o : $(INC_DIR)/bc.def
bcMethod.o : $(INC_DIR)/bc.def
share.o : share.def $(INC_DIR)/obj.def
parse.c : parse.y
lex.o : parse.c
defaults.o : config.h

#####################################################################

LD_MODE = # -static
LD_C_FLAGS = -I. -Iinclude -Isrc/include -Wall

#LIBS_GC += -lgc
LIBS_GC = 
LIBS_READLINE += -lreadline
LIBS_HISTORY = -lhistory
LIBS_HISTORY =
LIBS_CURSES = -lncurses
#LIBS_CURSES =
LIBS = $(LIBS_GC) $(LIBS_READLINE) $(LIBS_HISTORY) $(LIBS_CURSES) -lm

CFLAGS_DEBUG = -g
CFLAGS_OPTIMIZE = # -O2 # -O

LDFLAGS = $(LD_C_FLAGS) $(CFLAGS_DEBUG) 
CFLAGS_OPTIMIZED = $(LD_C_FLAGS) $(CFLAGS_OPTIMIZE) $(LD_MODE)
CFLAGS_NDEBUG = $(LD_C_FLAGS) $(LD_MODE)
CFLAGS_DEBUG_OPTIMIZED = $(LD_C_FLAGS) $(CFLAGS_DEBUG) $(CFLAGS_OPTIMIZE) $(LD_MODE)
CFLAGS = -Wall -Werror $(CFLAGS_NDEBUG) $(CFLAGS_DEBUG) $(CFLAGS_OPTIMIZE)

#####################################################################

CAT=cat
RM=rm
MV=mv
ECHO=echo
TOUCH=/usr/bin/touch
TOUCH=touch
MKDIR=mkdir
MKDIRS=$(MKDIR) -p
CPP=$(CC) $(CFLAGS) -E
PERL=perl

#####################################################################

all:

.SUFFIXES:
.SUFFIXES: .c .h .y .i .s .o


_PREPROC_C_ = $(CPP) $*.c | grep -v -e '^\#' > $*.i
_COMPILE_I_ = $(CC) $(CFLAGS) -c -o $@ $*.i
_COMPILE_C_ = $(_PREPROC_C_) && $(_COMPILE_I_) && $(RM) $*.i
_COMPILE_C_ = $(CC) $(CFLAGS) -c -o $@ $*.c

.c.o:
	$(_COMPILE_C_)

.c.i:
	$(CPP) $*.c > $*.i

.c.s:
	$(CPP) $*.c > $*.i
	$(CC) $(CFLAGS) -c -S -o $@ $*.i
	$(RM) $*.i

BISON = bison -t --locations --token-table
.y.c .y.h:
	$(BISON) --name-prefix=_mos_yy --defines=$(<:.y=.h) --graph=$(<:.y=.out) --output=$(<:.y=.c) $<

#$(CFILES_NDEBUG:.c=.o) :
#	$(CPP) $*.c > $*.i
#	$(CC) $(CFLAGS_NDEBUG) -c -o $@ $*.i
#	$(RM) $*.i

#$(CFILES_DEBUG_OPTIMIZED:.c=.o) :
#	$(CPP) $*.c > $*.i
#	$(CC) $(CFLAGS_DEBUG_OPTIMIZED) -c -o $@ $*.i
#	$(RM) $*.i

#$(CFILES_OPTIMIZED:.c=.o) :
#	$(CPP) $*.c > $*.i
#	$(CC) $(CFLAGS_OPTIMIZED) -c -S -o $@ $*.i
#	$(RM) $*.i

#####################################################################

OFILES = $(YFILES:.y=.o) $(CFILES:.c=.o)

SRCFILES = Makefile $(CFILES) $(HFILES) $(YFILES)

GARBAGE = .saves* core *.core $(OFILES) $(OFILES:.o=.i) $(OFILES:.o=.s) $(NAME)$(EXE)
ALL_GARBAGE = $(GARBAGE) $(YFILES:.y=.c) $(YFILES:.y=.h) $(YFILES:.y=.out) defs $(DEFS)

#####################################################################

MOS=$(NAME)$(EXE)

all : $(MOS)

$(MOS) : defs $(OFILES) Makefile
	@-$(RM) -f $(NAME)old$(EXE)
	@-$(MV) $@ $(NAME)old$(EXE)
	$(CC) $(LDFLAGS) -o $@ $(GC_BEGIN) $(OFILES) $(LIBS) $(GC_END)
	@-$(RM) -f $(NAME)old$(EXE)

$(DEFS) : Makefile

#$(OFILES) : $(HFILES)

DEFSCANFILES = $(YFILES:.y=.c) $(CFILES)

DEFSCANFILES : $(DEFSCANFILES)

defs : $(DEFSCANFILES)
	-rm $(DEFS)
	make DEFSCANFILES
	make config.h init.def annot.def
	make $(INC_DIR)/obj.def $(INC_DIR)/meth.def
	make $(INC_DIR)/sel.def
	touch $@

config.h : Makefile PKG
	. PKG; \
	$(ECHO) '#define mos_VERSION "'$${VERSION}'"' > config.h
	$(ECHO) '#define mos_SRC_DIR "'`pwd`'"' >> config.h
	$(ECHO) '#define mos_ROOT_DIR "'`pwd`'"' >> config.h
	$(ECHO) '#define mos_ANNOT_DIR "'$(mos_ANNOT_DIR)'"' >> config.h
	$(ECHO) '#define mos_SRC_SUFFIX ".mos"' >> config.h

init.def : parse.c init.pl $(mos_ANNOT_DIR)
	$(CAT) $(DEFSCANFILES) | $(PERL) init.pl > $@

$(mos_ANNOT_DIR) :
	$(MKDIRS) $@

annot.def : parse.c annot.pl selesc$(EXE)
	for f in $(DEFSCANFILES) ; \
	do \
	  $(CPP) \
		-Dmos_ANNOT=mos_ANNOT \
		-Dmos_METHOD=mos_METHOD \
		-Dmos_OBJECT=mos_OBJECT \
		$$f || exit 1; \
	done > /tmp/$$$$ ;\
	$(PERL) annot.pl /tmp/$$$$ > $@.temp ;\
        $(RM) /tmp/$$$$
	$(MV) $@.temp $@

$(INC_DIR)/sel.def : sel.pl $(INC_DIR)/meth.def $(INC_DIR)/obj.def $(INC_DIR)/sel2def.h
	for f in $(DEFSCANFILES) $(INC_DIR)/sel2def.h ; \
	do \
	  $(CPP) -Dmos_s=mos_s $$f || exit 1; \
	done | \
	$(PERL) sel.pl > $@.temp
	$(MV) $@.temp $@

$(INC_DIR)/obj.def : obj.pl
	$(CAT) $(DEFSCANFILES) | $(PERL) obj.pl > $@

$(INC_DIR)/meth.def : meth.pl
	for f in $(DEFSCANFILES) ; \
	do \
	  $(CPP) -Dmos_METHOD=mos_METHOD $$f || exit 1; \
	done | \
	$(PERL) meth.pl > $@.temp
	$(MV) $@.temp $@

selesc$(EXE) : selesc.c selsupp.c
	$(CC) $(LDFLAGS) -o $@ selesc.c selsupp.c

clean :
	$(RM) -f $(GARBAGE)

veryclean :
	$(RM) -f $(ALL_GARBAGE)
	$(RM) -f $(mos_ANNOT_DIR)/*.*

ad=..
archive : veryclean
	find . -name '*~' -print | xargs rm -f
	sleep 10 # wait for .nfs* files to disappear
	(cd .. && tar -cvf - $(NAME)) | gzip -9 > $(ad)/$(NAME).tgz

VALGRIND_OPTS=-v --track-fds=yes --time-stamp=yes --read-var-info=yes
#VALGRIND_OPTS+= --keep-stacktraces=alloc-and-free 
VALGRIND_OPTS+= --track-origins=yes --malloc-fill=5c --free-fill=e3
VALGRIND_OPTS+= --leak-check=full
VALGRIND=valgrind $(VALGRIND_OPTS)

valgrind : all
	$(VALGRIND) ./$(MOS)
