#
# SCAD3 makefile 
#
# Created from: /home/shir/pccts/support/genmk/genmk -CC -project scad3 -class sp_parser spice_gram.g
#

#/*
# *    $RCSfile: Makefile,v $
# *    $Revision: 1.10 $
# *    $Date: 2002/11/21 02:41:17 $
# */

#
TOKENS = tokens.h
#
# The following filenames must be consistent with ANTLR/DLG flags
CCC= g++ 
CC=$(CCC)
DLG_FILE = parser.dlg
ERR = err
HDR_FILE =
SCAN = DLGLexer
PCCTS = /home/eeprof/stan/locker_stan/scad3_rel/lib/pccts
#PCCTS = /users/stan/scad3/lib/pccts
ANTLR_H = $(PCCTS)/h
BIN = $(PCCTS)/bin
ANTLR = $(BIN)/antlr
DLG = $(BIN)/dlg
CFLAGS1 = -g -I. -I$(ANTLR_H) -I../../inc  -I../../stl
AFLAGS = -CC
DFLAGS = -C2 -i -CC

LIBDIR = /usr/lib/
SLIB = -lm 
#CFLAGS = -g  -I../../inc -I/usr/include/g++ +w +p -ftrap=common -mt -DSYSV -DSVR4 -xtarget=ultra2 -xarch=v8plus 
CFLAGS = -g  -Wall -I../../inc -I../../stl -I/usr/include/g++  -x c++
#CFLAGS = -g  -I../../inc -I/usr/include/g++ +p 
#CFLAGS = -g  -I../../inc -I/usr/local/SUNWspro5.0/SC5.0/include/CC
LDFLAGS = 
PROGRAM = ../../bin/scad3


SRCS =  	acan.cc\
		dtstruct.cc\
		branch.cc\
		circuits.cc\
		commands.cc\
		complex.cc\
		control.cc\
		subckt.cc\
		dddbase.cc\
		detcalc.cc\
		hashcash.cc\
		hasher.cc\
		scad3.cc\
		yainit.cc\
		list.cc\
		lookup.cc\
		mxexp.cc\
		message.cc\
		buildmna.cc\
		nextfield.cc\
		node.cc\
		parse_bch.cc\
		parse_ctrl.cc\
		rclcompx.cc\
		runstats.cc\
		signals.cc\
		utility.cc\
		timer.cc\
		approx.cc\
		coeff.cc\
		indexmx.cc\
		fcoeff.cc\
		polezero.cc\
		buildexp.cc\
		graph.cc\
		dddgc.cc\
		label.cc\
		noisean.cc\
		noise_src.cc\
		symb_gen.cc\
		xdraw.cc\
		cancel_flab_map.cc\
		new_term_gen.cc\
		new_term_gen2.cc\
		new_term_gen4.cc \
		semi_symb.cc \
		reordering.cc 


OBJS =  	acan.o\
		dtstruct.o\
		branch.o\
		circuits.o\
		commands.o\
		control.o\
		complex.o\
		subckt.o\
		dddbase.o\
		detcalc.o\
		hashcash.o\
		hasher.o\
		scad3.o\
		yainit.o\
		list.o\
		lookup.o\
		mxexp.o\
		message.o\
		buildmna.o\
		nextfield.o\
		node.o\
		parse_bch.o\
		parse_ctrl.o\
		rclcompx.o\
		runstats.o\
		signals.o\
		utility.o\
		timer.o\
		approx.o\
		coeff.o\
		indexmx.o\
		fcoeff.o\
		polezero.o\
		buildexp.o\
		graph.o\
		dddgc.o\
		label.o\
		noisean.o\
		noise_src.o\
		symb_gen.o\
		xdraw.o\
		cancel_flab_map.o\
		new_term_gen.o\
		new_term_gen2.o\
		new_term_gen4.o\
		semi_symb.o\
		reordering.o

.cc.o:
	$(CC) $(CFLAGS) -c $<

GRM = spgram.g
SRC1 = spgram.cpp \
      sp_parser.cpp \
      $(ANTLR_H)/AParser.cpp $(ANTLR_H)/DLexerBase.cpp $(ANTLR_H)/ATokenBuffer.cpp $(SCAN).cpp

OBJ1 = spgram.o \
	   sp_parser.o \
       AParser.o DLexerBase.o ATokenBuffer.o $(SCAN).o

ANTLR_SPAWN = spgram.cpp sp_parser.cpp \
              sp_parser.h $(DLG_FILE) $(TOKENS)
DLG_SPAWN = $(SCAN).cpp $(SCAN).h

#$(PROGRAM) : $(OBJ1) $(SRC1)
#	$(CCC) -o scad3 $(CFLAGS1) $(OBJ1)

TOUCHFILE = .touchfl

$(PROGRAM) : $(OBJS) $(OBJ1) $(TOUCHFILE)
	@echo -n Linking $(PROGRAM) ... 
#@$(CC) -O -o $(PROGRAM) $(OBJS) $(OBJ1) $(SLIB) 
	@$(CCC) -o $(PROGRAM) $(OBJS) $(OBJ1) $(SLIB) 
	@echo ""
	@echo done
	@touch $(TOUCHFILE)

all: $(PROGRAM)

spgram.o : $(TOKENS) $(SCAN).h spgram.cpp
	$(CCC) -c $(CFLAGS1) -o spgram.o spgram.cpp

sp_parser.o : $(TOKENS) $(SCAN).h sp_parser.cpp sp_parser.h
	$(CCC) -c $(CFLAGS1) -o sp_parser.o sp_parser.cpp

$(SCAN).o : $(SCAN).cpp $(TOKENS)
	$(CCC) -c $(CFLAGS1) -o $(SCAN).o $(SCAN).cpp

$(ANTLR_SPAWN) : $(GRM)
	$(ANTLR) $(AFLAGS) $(GRM)

$(DLG_SPAWN) : $(DLG_FILE)
	$(DLG) $(DFLAGS) $(DLG_FILE)

AParser.o : $(ANTLR_H)/AParser.cpp
	$(CCC) -c $(CFLAGS1) -o AParser.o $(ANTLR_H)/AParser.cpp

ATokenBuffer.o : $(ANTLR_H)/ATokenBuffer.cpp
	$(CCC) -c $(CFLAGS1) -o ATokenBuffer.o $(ANTLR_H)/ATokenBuffer.cpp

DLexerBase.o : $(ANTLR_H)/DLexerBase.cpp
	$(CCC) -c $(CFLAGS1) -o DLexerBase.o $(ANTLR_H)/DLexerBase.cpp

clean:
	rm -f $(OBJS) $(OBJ1) core $(PROGRAM)

scrub:
	rm -f *.o core $(PROGRAM) $(ANTLR_SPAWN) $(DLG_SPAWN)

depend:
	makedepend $(CFLAGS) $(CFLAGS1) $(CCHEAD) -- $(SRCS) $(SRC1)

# DO NOT DELETE THIS LINE -- make depend depends on it.

acan.o: /usr/include/math.h /usr/include/features.h /usr/include/sys/cdefs.h
acan.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
acan.o: /usr/include/gnu/stubs-64.h /usr/include/bits/huge_val.h
acan.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
acan.o: ../../inc/circuits.h ../../inc/globals.h ../../inc/unixstd.h
acan.o: /usr/include/stdio.h /usr/include/bits/types.h
acan.o: /usr/include/bits/typesizes.h /usr/include/libio.h
acan.o: /usr/include/_G_config.h /usr/include/wchar.h
acan.o: /usr/include/bits/wchar.h /usr/include/gconv.h
acan.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
acan.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
acan.o: /usr/include/bits/endian.h /usr/include/stdlib.h
acan.o: /usr/include/sys/types.h /usr/include/time.h
acan.o: /usr/include/sys/select.h /usr/include/bits/select.h
acan.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
acan.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
acan.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
acan.o: ../../inc/message.h ../../inc/utility.h ../../inc/node.h
acan.o: ../../inc/branch.h ../../inc/rclcompx.h ../../inc/complex.h
acan.o: /usr/include/errno.h /usr/include/bits/errno.h
acan.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
acan.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
acan.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
acan.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
acan.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
acan.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
acan.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
acan.o: ../../inc/control.h ../../inc/acan.h ../../inc/xdraw.h
acan.o: ../../inc/coeff.h ../../inc/fcoeff.h ../../inc/mxexp.h
acan.o: ../../inc/graph_out.h ../../inc/approx.h
dtstruct.o: ../../inc/dtstruct.h /usr/include/stdio.h /usr/include/features.h
dtstruct.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
dtstruct.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
dtstruct.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
dtstruct.o: /usr/include/libio.h /usr/include/_G_config.h
dtstruct.o: /usr/include/wchar.h /usr/include/bits/wchar.h
dtstruct.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
dtstruct.o: /usr/include/bits/sys_errlist.h ../../inc/unixstd.h
dtstruct.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
dtstruct.o: /usr/include/bits/endian.h /usr/include/stdlib.h
dtstruct.o: /usr/include/sys/types.h /usr/include/time.h
dtstruct.o: /usr/include/sys/select.h /usr/include/bits/select.h
dtstruct.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
dtstruct.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
dtstruct.o: /usr/include/alloca.h ../../inc/ddd_base.h ../../inc/compable.h
dtstruct.o: ../../inc/object.h ../../inc/stddefs.h ../../inc/classdef.h
dtstruct.o: ../../inc/copyr.h ../../inc/mem.h ../../inc/hasher.h
dtstruct.o: ../../inc/list.h ../../inc/hashcach.h ../../inc/tnode.h
dtstruct.o: ../../inc/floatobj.h ../../inc/rclcompx.h ../../inc/globals.h
dtstruct.o: /usr/include/assert.h ../../inc/message.h ../../inc/utility.h
dtstruct.o: /usr/include/math.h /usr/include/bits/huge_val.h
dtstruct.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
dtstruct.o: ../../inc/complex.h /usr/include/errno.h
dtstruct.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
dtstruct.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
dtstruct.o: /usr/include/asm-generic/errno.h
dtstruct.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
dtstruct.o: ../../inc/node.h ../../inc/branch.h
branch.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
branch.o: /usr/include/features.h /usr/include/sys/cdefs.h
branch.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
branch.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
branch.o: /usr/include/bits/typesizes.h /usr/include/libio.h
branch.o: /usr/include/_G_config.h /usr/include/wchar.h
branch.o: /usr/include/bits/wchar.h /usr/include/gconv.h
branch.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
branch.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
branch.o: /usr/include/bits/endian.h /usr/include/stdlib.h
branch.o: /usr/include/sys/types.h /usr/include/time.h
branch.o: /usr/include/sys/select.h /usr/include/bits/select.h
branch.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
branch.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
branch.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
branch.o: ../../inc/message.h ../../inc/utility.h ../../inc/branch.h
branch.o: ../../inc/node.h ../../inc/rclcompx.h /usr/include/math.h
branch.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
branch.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
branch.o: /usr/include/errno.h /usr/include/bits/errno.h
branch.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
branch.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
branch.o: /usr/include/asm-generic/errno-base.h
circuits.o: /usr/include/assert.h /usr/include/features.h
circuits.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
circuits.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
circuits.o: ../../inc/circuits.h ../../inc/globals.h ../../inc/unixstd.h
circuits.o: /usr/include/stdio.h /usr/include/bits/types.h
circuits.o: /usr/include/bits/typesizes.h /usr/include/libio.h
circuits.o: /usr/include/_G_config.h /usr/include/wchar.h
circuits.o: /usr/include/bits/wchar.h /usr/include/gconv.h
circuits.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
circuits.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
circuits.o: /usr/include/bits/endian.h /usr/include/stdlib.h
circuits.o: /usr/include/sys/types.h /usr/include/time.h
circuits.o: /usr/include/sys/select.h /usr/include/bits/select.h
circuits.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
circuits.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
circuits.o: /usr/include/alloca.h ../../inc/stddefs.h ../../inc/message.h
circuits.o: ../../inc/utility.h ../../inc/node.h ../../inc/branch.h
circuits.o: ../../inc/rclcompx.h /usr/include/math.h
circuits.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
circuits.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
circuits.o: /usr/include/errno.h /usr/include/bits/errno.h
circuits.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
circuits.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
circuits.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
circuits.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
circuits.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
circuits.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
circuits.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
circuits.o: ../../inc/control.h ../../inc/acan.h ../../inc/xdraw.h
circuits.o: ../../inc/coeff.h ../../inc/fcoeff.h ../../inc/ckt_call.h
circuits.o: ../../inc/mxexp.h ../../inc/cancel_flab_map.h
commands.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
commands.o: /usr/include/features.h /usr/include/sys/cdefs.h
commands.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
commands.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
commands.o: /usr/include/bits/typesizes.h /usr/include/libio.h
commands.o: /usr/include/_G_config.h /usr/include/wchar.h
commands.o: /usr/include/bits/wchar.h /usr/include/gconv.h
commands.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
commands.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
commands.o: /usr/include/bits/endian.h /usr/include/stdlib.h
commands.o: /usr/include/sys/types.h /usr/include/time.h
commands.o: /usr/include/sys/select.h /usr/include/bits/select.h
commands.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
commands.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
commands.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
commands.o: ../../inc/message.h ../../inc/utility.h ../../inc/circuits.h
commands.o: ../../inc/node.h ../../inc/branch.h ../../inc/rclcompx.h
commands.o: /usr/include/math.h /usr/include/bits/huge_val.h
commands.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
commands.o: ../../inc/complex.h /usr/include/errno.h
commands.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
commands.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
commands.o: /usr/include/asm-generic/errno.h
commands.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
commands.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
commands.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
commands.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
commands.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
commands.o: ../../inc/control.h ../../inc/acan.h ../../inc/xdraw.h
commands.o: ../../inc/coeff.h ../../inc/fcoeff.h /usr/include/sys/times.h
commands.o: ../../inc/timer.h /usr/include/sys/time.h
commands.o: /usr/include/sys/resource.h /usr/include/bits/resource.h
commands.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
commands.o: /usr/include/bits/confname.h /usr/include/getopt.h
commands.o: ../../inc/mxexp.h
complex.o: ../../inc/message.h /usr/include/stdio.h /usr/include/features.h
complex.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
complex.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
complex.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
complex.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
complex.o: /usr/include/bits/wchar.h /usr/include/gconv.h
complex.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
complex.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
complex.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
complex.o: /usr/include/bits/posix2_lim.h ../../inc/complex.h
complex.o: /usr/include/math.h /usr/include/bits/huge_val.h
complex.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
complex.o: /usr/include/errno.h /usr/include/bits/errno.h
complex.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
complex.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
complex.o: /usr/include/asm-generic/errno-base.h
control.o: ../../inc/control.h ../../inc/globals.h ../../inc/unixstd.h
control.o: /usr/include/stdio.h /usr/include/features.h
control.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
control.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
control.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
control.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
control.o: /usr/include/bits/wchar.h /usr/include/gconv.h
control.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
control.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
control.o: /usr/include/bits/endian.h /usr/include/stdlib.h
control.o: /usr/include/sys/types.h /usr/include/time.h
control.o: /usr/include/sys/select.h /usr/include/bits/select.h
control.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
control.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
control.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
control.o: ../../inc/message.h ../../inc/utility.h
subckt.o: ../../inc/circuits.h ../../inc/globals.h ../../inc/unixstd.h
subckt.o: /usr/include/stdio.h /usr/include/features.h
subckt.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
subckt.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
subckt.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
subckt.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
subckt.o: /usr/include/bits/wchar.h /usr/include/gconv.h
subckt.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
subckt.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
subckt.o: /usr/include/bits/endian.h /usr/include/stdlib.h
subckt.o: /usr/include/sys/types.h /usr/include/time.h
subckt.o: /usr/include/sys/select.h /usr/include/bits/select.h
subckt.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
subckt.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
subckt.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
subckt.o: ../../inc/message.h ../../inc/utility.h ../../inc/node.h
subckt.o: ../../inc/branch.h ../../inc/rclcompx.h /usr/include/math.h
subckt.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
subckt.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
subckt.o: /usr/include/errno.h /usr/include/bits/errno.h
subckt.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
subckt.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
subckt.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
subckt.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
subckt.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
subckt.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
subckt.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
subckt.o: ../../inc/control.h ../../inc/ckt_call.h ../../inc/acan.h
subckt.o: ../../inc/xdraw.h ../../inc/coeff.h ../../inc/fcoeff.h
dddbase.o: ../../inc/ddd_base.h ../../inc/compable.h ../../inc/object.h
dddbase.o: /usr/include/stdlib.h /usr/include/features.h
dddbase.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
dddbase.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
dddbase.o: /usr/include/sys/types.h /usr/include/bits/types.h
dddbase.o: /usr/include/bits/typesizes.h /usr/include/time.h
dddbase.o: /usr/include/endian.h /usr/include/bits/endian.h
dddbase.o: /usr/include/sys/select.h /usr/include/bits/select.h
dddbase.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
dddbase.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
dddbase.o: /usr/include/alloca.h ../../inc/stddefs.h ../../inc/classdef.h
dddbase.o: ../../inc/copyr.h ../../inc/mem.h ../../inc/hasher.h
dddbase.o: ../../inc/list.h ../../inc/hashcach.h ../../inc/tnode.h
dddbase.o: ../../inc/floatobj.h ../../inc/dtstruct.h /usr/include/stdio.h
dddbase.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
dddbase.o: /usr/include/bits/wchar.h /usr/include/gconv.h
dddbase.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
dddbase.o: ../../inc/unixstd.h /usr/include/string.h /usr/include/ctype.h
dddbase.o: ../../inc/rclcompx.h ../../inc/globals.h /usr/include/assert.h
dddbase.o: ../../inc/message.h ../../inc/utility.h /usr/include/math.h
dddbase.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
dddbase.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
dddbase.o: /usr/include/errno.h /usr/include/bits/errno.h
dddbase.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
dddbase.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
dddbase.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
dddbase.o: ../../inc/node.h ../../inc/branch.h
detcalc.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
detcalc.o: /usr/include/features.h /usr/include/sys/cdefs.h
detcalc.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
detcalc.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
detcalc.o: /usr/include/bits/typesizes.h /usr/include/libio.h
detcalc.o: /usr/include/_G_config.h /usr/include/wchar.h
detcalc.o: /usr/include/bits/wchar.h /usr/include/gconv.h
detcalc.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
detcalc.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
detcalc.o: /usr/include/bits/endian.h /usr/include/stdlib.h
detcalc.o: /usr/include/sys/types.h /usr/include/time.h
detcalc.o: /usr/include/sys/select.h /usr/include/bits/select.h
detcalc.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
detcalc.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
detcalc.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
detcalc.o: ../../inc/message.h ../../inc/utility.h ../../inc/node.h
detcalc.o: ../../inc/branch.h ../../inc/rclcompx.h /usr/include/math.h
detcalc.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
detcalc.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
detcalc.o: /usr/include/errno.h /usr/include/bits/errno.h
detcalc.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
detcalc.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
detcalc.o: /usr/include/asm-generic/errno-base.h ../../inc/mxexp.h
detcalc.o: ../../inc/ddd_base.h ../../inc/compable.h ../../inc/object.h
detcalc.o: ../../inc/classdef.h ../../inc/copyr.h ../../inc/mem.h
detcalc.o: ../../inc/hasher.h ../../inc/list.h ../../inc/hashcach.h
detcalc.o: ../../inc/tnode.h ../../inc/floatobj.h ../../inc/dtstruct.h
detcalc.o: ../../inc/mna_form.h ../../inc/coeff.h ../../inc/fcoeff.h
hashcash.o: ../../inc/hashcach.h ../../inc/compable.h ../../inc/object.h
hashcash.o: /usr/include/stdlib.h /usr/include/features.h
hashcash.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hashcash.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
hashcash.o: /usr/include/sys/types.h /usr/include/bits/types.h
hashcash.o: /usr/include/bits/typesizes.h /usr/include/time.h
hashcash.o: /usr/include/endian.h /usr/include/bits/endian.h
hashcash.o: /usr/include/sys/select.h /usr/include/bits/select.h
hashcash.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
hashcash.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
hashcash.o: /usr/include/alloca.h ../../inc/stddefs.h ../../inc/classdef.h
hashcash.o: ../../inc/copyr.h ../../inc/mem.h
hasher.o: /usr/include/string.h /usr/include/features.h
hasher.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hasher.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
hasher.o: /usr/include/stdlib.h /usr/include/sys/types.h
hasher.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
hasher.o: /usr/include/time.h /usr/include/endian.h
hasher.o: /usr/include/bits/endian.h /usr/include/sys/select.h
hasher.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
hasher.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
hasher.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
hasher.o: ../../inc/list.h ../../inc/object.h ../../inc/stddefs.h
hasher.o: ../../inc/classdef.h ../../inc/copyr.h ../../inc/mem.h
hasher.o: ../../inc/compable.h ../../inc/hasher.h
scad3.o: /usr/include/stdio.h /usr/include/features.h
scad3.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
scad3.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
scad3.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
scad3.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
scad3.o: /usr/include/bits/wchar.h /usr/include/gconv.h
scad3.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
scad3.o: ../../inc/yasis.h ../../inc/globals.h ../../inc/unixstd.h
scad3.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
scad3.o: /usr/include/bits/endian.h /usr/include/stdlib.h
scad3.o: /usr/include/sys/types.h /usr/include/time.h
scad3.o: /usr/include/sys/select.h /usr/include/bits/select.h
scad3.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
scad3.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
scad3.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
scad3.o: ../../inc/message.h ../../inc/utility.h
yainit.o: /usr/include/stdlib.h /usr/include/features.h
yainit.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
yainit.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
yainit.o: /usr/include/sys/types.h /usr/include/bits/types.h
yainit.o: /usr/include/bits/typesizes.h /usr/include/time.h
yainit.o: /usr/include/endian.h /usr/include/bits/endian.h
yainit.o: /usr/include/sys/select.h /usr/include/bits/select.h
yainit.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
yainit.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
yainit.o: /usr/include/alloca.h /usr/include/stdio.h /usr/include/libio.h
yainit.o: /usr/include/_G_config.h /usr/include/wchar.h
yainit.o: /usr/include/bits/wchar.h /usr/include/gconv.h
yainit.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
yainit.o: /usr/include/ctype.h /usr/include/assert.h /usr/include/string.h
yainit.o: ../../inc/globals.h ../../inc/unixstd.h ../../inc/stddefs.h
yainit.o: ../../inc/message.h ../../inc/utility.h ../../inc/branch.h
yainit.o: ../../inc/node.h ../../inc/rclcompx.h /usr/include/math.h
yainit.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
yainit.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
yainit.o: /usr/include/errno.h /usr/include/bits/errno.h
yainit.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
yainit.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
yainit.o: /usr/include/asm-generic/errno-base.h ../../inc/circuits.h
yainit.o: ../../inc/mna_form.h ../../inc/dtstruct.h ../../inc/ddd_base.h
yainit.o: ../../inc/compable.h ../../inc/object.h ../../inc/classdef.h
yainit.o: ../../inc/copyr.h ../../inc/mem.h ../../inc/hasher.h
yainit.o: ../../inc/list.h ../../inc/hashcach.h ../../inc/tnode.h
yainit.o: ../../inc/floatobj.h ../../inc/control.h ../../inc/timer.h
yainit.o: /usr/include/sys/time.h /usr/include/sys/resource.h
yainit.o: /usr/include/bits/resource.h
list.o: /usr/include/stdlib.h /usr/include/features.h
list.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
list.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
list.o: /usr/include/sys/types.h /usr/include/bits/types.h
list.o: /usr/include/bits/typesizes.h /usr/include/time.h
list.o: /usr/include/endian.h /usr/include/bits/endian.h
list.o: /usr/include/sys/select.h /usr/include/bits/select.h
list.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
list.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
list.o: /usr/include/alloca.h ../../inc/list.h ../../inc/object.h
list.o: ../../inc/stddefs.h ../../inc/classdef.h ../../inc/copyr.h
list.o: ../../inc/mem.h ../../inc/compable.h
lookup.o: /usr/include/stdio.h /usr/include/features.h
lookup.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
lookup.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
lookup.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
lookup.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
lookup.o: /usr/include/bits/wchar.h /usr/include/gconv.h
lookup.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
lookup.o: /usr/include/ctype.h /usr/include/endian.h
lookup.o: /usr/include/bits/endian.h
mxexp.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
mxexp.o: /usr/include/features.h /usr/include/sys/cdefs.h
mxexp.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
mxexp.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
mxexp.o: /usr/include/bits/typesizes.h /usr/include/libio.h
mxexp.o: /usr/include/_G_config.h /usr/include/wchar.h
mxexp.o: /usr/include/bits/wchar.h /usr/include/gconv.h
mxexp.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
mxexp.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
mxexp.o: /usr/include/bits/endian.h /usr/include/stdlib.h
mxexp.o: /usr/include/sys/types.h /usr/include/time.h
mxexp.o: /usr/include/sys/select.h /usr/include/bits/select.h
mxexp.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
mxexp.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
mxexp.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
mxexp.o: ../../inc/message.h ../../inc/utility.h /usr/include/math.h
mxexp.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
mxexp.o: /usr/include/bits/mathcalls.h /usr/include/limits.h
mxexp.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
mxexp.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
mxexp.o: ../../inc/circuits.h ../../inc/node.h ../../inc/branch.h
mxexp.o: ../../inc/rclcompx.h ../../inc/complex.h /usr/include/errno.h
mxexp.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
mxexp.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
mxexp.o: /usr/include/asm-generic/errno.h
mxexp.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
mxexp.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
mxexp.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
mxexp.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
mxexp.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
mxexp.o: ../../inc/control.h ../../inc/mxexp.h
message.o: ../../inc/unixstd.h /usr/include/stdio.h /usr/include/features.h
message.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
message.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
message.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
message.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
message.o: /usr/include/bits/wchar.h /usr/include/gconv.h
message.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
message.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
message.o: /usr/include/bits/endian.h /usr/include/stdlib.h
message.o: /usr/include/sys/types.h /usr/include/time.h
message.o: /usr/include/sys/select.h /usr/include/bits/select.h
message.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
message.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
message.o: /usr/include/alloca.h ../../inc/message.h
buildmna.o: /usr/include/unistd.h /usr/include/features.h
buildmna.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
buildmna.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
buildmna.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
buildmna.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
buildmna.o: /usr/include/getopt.h ../../inc/circuits.h ../../inc/globals.h
buildmna.o: ../../inc/unixstd.h /usr/include/stdio.h /usr/include/libio.h
buildmna.o: /usr/include/_G_config.h /usr/include/wchar.h
buildmna.o: /usr/include/bits/wchar.h /usr/include/gconv.h
buildmna.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
buildmna.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
buildmna.o: /usr/include/bits/endian.h /usr/include/stdlib.h
buildmna.o: /usr/include/sys/types.h /usr/include/time.h
buildmna.o: /usr/include/sys/select.h /usr/include/bits/select.h
buildmna.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
buildmna.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
buildmna.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
buildmna.o: ../../inc/message.h ../../inc/utility.h ../../inc/node.h
buildmna.o: ../../inc/branch.h ../../inc/rclcompx.h /usr/include/math.h
buildmna.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
buildmna.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
buildmna.o: /usr/include/errno.h /usr/include/bits/errno.h
buildmna.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
buildmna.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
buildmna.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
buildmna.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
buildmna.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
buildmna.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
buildmna.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
buildmna.o: ../../inc/control.h ../../inc/mxexp.h ../../inc/ckt_call.h
buildmna.o: ../../inc/acan.h ../../inc/xdraw.h ../../inc/coeff.h
buildmna.o: ../../inc/fcoeff.h
nextfield.o: /usr/include/stdio.h /usr/include/features.h
nextfield.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
nextfield.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
nextfield.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
nextfield.o: /usr/include/libio.h /usr/include/_G_config.h
nextfield.o: /usr/include/wchar.h /usr/include/bits/wchar.h
nextfield.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
nextfield.o: /usr/include/bits/sys_errlist.h /usr/include/ctype.h
nextfield.o: /usr/include/endian.h /usr/include/bits/endian.h
node.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
node.o: /usr/include/features.h /usr/include/sys/cdefs.h
node.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
node.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
node.o: /usr/include/bits/typesizes.h /usr/include/libio.h
node.o: /usr/include/_G_config.h /usr/include/wchar.h
node.o: /usr/include/bits/wchar.h /usr/include/gconv.h
node.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
node.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
node.o: /usr/include/bits/endian.h /usr/include/stdlib.h
node.o: /usr/include/sys/types.h /usr/include/time.h
node.o: /usr/include/sys/select.h /usr/include/bits/select.h
node.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
node.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
node.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
node.o: ../../inc/message.h ../../inc/utility.h ../../inc/branch.h
node.o: ../../inc/node.h ../../inc/rclcompx.h /usr/include/math.h
node.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
node.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
node.o: /usr/include/errno.h /usr/include/bits/errno.h
node.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
node.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
node.o: /usr/include/asm-generic/errno-base.h
parse_bch.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
parse_bch.o: /usr/include/features.h /usr/include/sys/cdefs.h
parse_bch.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
parse_bch.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
parse_bch.o: /usr/include/bits/typesizes.h /usr/include/libio.h
parse_bch.o: /usr/include/_G_config.h /usr/include/wchar.h
parse_bch.o: /usr/include/bits/wchar.h /usr/include/gconv.h
parse_bch.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
parse_bch.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
parse_bch.o: /usr/include/bits/endian.h /usr/include/stdlib.h
parse_bch.o: /usr/include/sys/types.h /usr/include/time.h
parse_bch.o: /usr/include/sys/select.h /usr/include/bits/select.h
parse_bch.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
parse_bch.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
parse_bch.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
parse_bch.o: ../../inc/message.h ../../inc/utility.h ../../inc/rclcompx.h
parse_bch.o: /usr/include/math.h /usr/include/bits/huge_val.h
parse_bch.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
parse_bch.o: ../../inc/complex.h /usr/include/errno.h
parse_bch.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
parse_bch.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
parse_bch.o: /usr/include/asm-generic/errno.h
parse_bch.o: /usr/include/asm-generic/errno-base.h ../../inc/circuits.h
parse_bch.o: ../../inc/node.h ../../inc/branch.h ../../inc/mna_form.h
parse_bch.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
parse_bch.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
parse_bch.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
parse_bch.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
parse_bch.o: ../../inc/control.h ../../inc/acan.h ../../inc/xdraw.h
parse_bch.o: ../../inc/coeff.h ../../inc/fcoeff.h ../../inc/ckt_call.h
parse_ctrl.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
parse_ctrl.o: /usr/include/features.h /usr/include/sys/cdefs.h
parse_ctrl.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
parse_ctrl.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
parse_ctrl.o: /usr/include/bits/typesizes.h /usr/include/libio.h
parse_ctrl.o: /usr/include/_G_config.h /usr/include/wchar.h
parse_ctrl.o: /usr/include/bits/wchar.h /usr/include/gconv.h
parse_ctrl.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
parse_ctrl.o: /usr/include/string.h /usr/include/ctype.h
parse_ctrl.o: /usr/include/endian.h /usr/include/bits/endian.h
parse_ctrl.o: /usr/include/stdlib.h /usr/include/sys/types.h
parse_ctrl.o: /usr/include/time.h /usr/include/sys/select.h
parse_ctrl.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
parse_ctrl.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
parse_ctrl.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
parse_ctrl.o: ../../inc/stddefs.h /usr/include/assert.h ../../inc/message.h
parse_ctrl.o: ../../inc/utility.h ../../inc/circuits.h ../../inc/node.h
parse_ctrl.o: ../../inc/branch.h ../../inc/rclcompx.h /usr/include/math.h
parse_ctrl.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
parse_ctrl.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
parse_ctrl.o: /usr/include/errno.h /usr/include/bits/errno.h
parse_ctrl.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
parse_ctrl.o: /usr/include/asm-x86_64/errno.h
parse_ctrl.o: /usr/include/asm-generic/errno.h
parse_ctrl.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
parse_ctrl.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
parse_ctrl.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
parse_ctrl.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
parse_ctrl.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
parse_ctrl.o: ../../inc/control.h ../../inc/acan.h ../../inc/xdraw.h
parse_ctrl.o: ../../inc/coeff.h ../../inc/fcoeff.h
rclcompx.o: /usr/include/limits.h /usr/include/features.h
rclcompx.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
rclcompx.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
rclcompx.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
rclcompx.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
rclcompx.o: ../../inc/rclcompx.h ../../inc/globals.h ../../inc/unixstd.h
rclcompx.o: /usr/include/stdio.h /usr/include/bits/types.h
rclcompx.o: /usr/include/bits/typesizes.h /usr/include/libio.h
rclcompx.o: /usr/include/_G_config.h /usr/include/wchar.h
rclcompx.o: /usr/include/bits/wchar.h /usr/include/gconv.h
rclcompx.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
rclcompx.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
rclcompx.o: /usr/include/bits/endian.h /usr/include/stdlib.h
rclcompx.o: /usr/include/sys/types.h /usr/include/time.h
rclcompx.o: /usr/include/sys/select.h /usr/include/bits/select.h
rclcompx.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
rclcompx.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
rclcompx.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
rclcompx.o: ../../inc/message.h ../../inc/utility.h /usr/include/math.h
rclcompx.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
rclcompx.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
rclcompx.o: /usr/include/errno.h /usr/include/bits/errno.h
rclcompx.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
rclcompx.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
rclcompx.o: /usr/include/asm-generic/errno-base.h
runstats.o: /usr/include/sys/types.h /usr/include/features.h
runstats.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
runstats.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
runstats.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
runstats.o: /usr/include/time.h /usr/include/endian.h
runstats.o: /usr/include/bits/endian.h /usr/include/sys/select.h
runstats.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
runstats.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
runstats.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/times.h
runstats.o: /usr/include/stdio.h /usr/include/libio.h
runstats.o: /usr/include/_G_config.h /usr/include/wchar.h
runstats.o: /usr/include/bits/wchar.h /usr/include/gconv.h
runstats.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
runstats.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
runstats.o: /usr/include/bits/confname.h /usr/include/getopt.h
signals.o: /usr/include/signal.h /usr/include/features.h
signals.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
signals.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
signals.o: /usr/include/bits/sigset.h /usr/include/bits/types.h
signals.o: /usr/include/bits/typesizes.h /usr/include/bits/signum.h
signals.o: /usr/include/time.h /usr/include/bits/siginfo.h
signals.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
signals.o: /usr/include/bits/sigstack.h /usr/include/bits/pthreadtypes.h
signals.o: /usr/include/bits/sigthread.h ../../inc/mxexp.h
signals.o: ../../inc/unixstd.h /usr/include/stdio.h /usr/include/libio.h
signals.o: /usr/include/_G_config.h /usr/include/wchar.h
signals.o: /usr/include/bits/wchar.h /usr/include/gconv.h
signals.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
signals.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
signals.o: /usr/include/bits/endian.h /usr/include/stdlib.h
signals.o: /usr/include/sys/types.h /usr/include/sys/select.h
signals.o: /usr/include/bits/select.h /usr/include/bits/time.h
signals.o: /usr/include/sys/sysmacros.h /usr/include/alloca.h
signals.o: ../../inc/ddd_base.h ../../inc/compable.h ../../inc/object.h
signals.o: ../../inc/stddefs.h ../../inc/classdef.h ../../inc/copyr.h
signals.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
signals.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
signals.o: ../../inc/dtstruct.h ../../inc/rclcompx.h ../../inc/globals.h
signals.o: /usr/include/assert.h ../../inc/message.h ../../inc/utility.h
signals.o: /usr/include/math.h /usr/include/bits/huge_val.h
signals.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
signals.o: ../../inc/complex.h /usr/include/errno.h /usr/include/bits/errno.h
signals.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
signals.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
signals.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
signals.o: ../../inc/node.h ../../inc/branch.h
utility.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
utility.o: /usr/include/features.h /usr/include/sys/cdefs.h
utility.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
utility.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
utility.o: /usr/include/bits/typesizes.h /usr/include/libio.h
utility.o: /usr/include/_G_config.h /usr/include/wchar.h
utility.o: /usr/include/bits/wchar.h /usr/include/gconv.h
utility.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
utility.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
utility.o: /usr/include/bits/endian.h /usr/include/stdlib.h
utility.o: /usr/include/sys/types.h /usr/include/time.h
utility.o: /usr/include/sys/select.h /usr/include/bits/select.h
utility.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
utility.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
utility.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
utility.o: ../../inc/message.h ../../inc/utility.h /usr/include/math.h
utility.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
utility.o: /usr/include/bits/mathcalls.h
timer.o: ../../inc/timer.h /usr/include/sys/time.h /usr/include/features.h
timer.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
timer.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
timer.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
timer.o: /usr/include/time.h /usr/include/bits/time.h
timer.o: /usr/include/sys/select.h /usr/include/bits/select.h
timer.o: /usr/include/bits/sigset.h /usr/include/sys/resource.h
timer.o: /usr/include/bits/resource.h ../../inc/object.h
timer.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/endian.h
timer.o: /usr/include/bits/endian.h /usr/include/sys/sysmacros.h
timer.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
timer.o: ../../inc/stddefs.h ../../inc/classdef.h ../../inc/copyr.h
timer.o: ../../inc/mem.h
approx.o: /usr/include/stdio.h /usr/include/features.h
approx.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
approx.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
approx.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
approx.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
approx.o: /usr/include/bits/wchar.h /usr/include/gconv.h
approx.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
approx.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
approx.o: /usr/include/endian.h /usr/include/bits/endian.h
approx.o: /usr/include/sys/select.h /usr/include/bits/select.h
approx.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
approx.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
approx.o: /usr/include/alloca.h /usr/include/string.h /usr/include/limits.h
approx.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
approx.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
approx.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/ctype.h
approx.o: ../../inc/stddefs.h /usr/include/assert.h ../../inc/message.h
approx.o: ../../inc/utility.h ../../inc/ddd_base.h ../../inc/compable.h
approx.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
approx.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
approx.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
approx.o: ../../inc/dtstruct.h ../../inc/rclcompx.h /usr/include/math.h
approx.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
approx.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
approx.o: /usr/include/errno.h /usr/include/bits/errno.h
approx.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
approx.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
approx.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
approx.o: ../../inc/node.h ../../inc/branch.h ../../inc/control.h
approx.o: ../../inc/circuits.h ../../inc/mxexp.h ../../inc/approx.h
approx.o: ../../inc/acan.h ../../inc/xdraw.h ../../inc/coeff.h
approx.o: ../../inc/fcoeff.h
coeff.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
coeff.o: /usr/include/features.h /usr/include/sys/cdefs.h
coeff.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
coeff.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
coeff.o: /usr/include/bits/typesizes.h /usr/include/libio.h
coeff.o: /usr/include/_G_config.h /usr/include/wchar.h
coeff.o: /usr/include/bits/wchar.h /usr/include/gconv.h
coeff.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
coeff.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
coeff.o: /usr/include/bits/endian.h /usr/include/stdlib.h
coeff.o: /usr/include/sys/types.h /usr/include/time.h
coeff.o: /usr/include/sys/select.h /usr/include/bits/select.h
coeff.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
coeff.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
coeff.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
coeff.o: ../../inc/message.h ../../inc/utility.h ../../inc/complex.h
coeff.o: /usr/include/math.h /usr/include/bits/huge_val.h
coeff.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
coeff.o: /usr/include/errno.h /usr/include/bits/errno.h
coeff.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
coeff.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
coeff.o: /usr/include/asm-generic/errno-base.h ../../inc/node.h
coeff.o: ../../inc/branch.h ../../inc/rclcompx.h ../../inc/mxexp.h
coeff.o: ../../inc/ddd_base.h ../../inc/compable.h ../../inc/object.h
coeff.o: ../../inc/classdef.h ../../inc/copyr.h ../../inc/mem.h
coeff.o: ../../inc/hasher.h ../../inc/list.h ../../inc/hashcach.h
coeff.o: ../../inc/tnode.h ../../inc/floatobj.h ../../inc/dtstruct.h
coeff.o: ../../inc/mna_form.h ../../inc/acan.h ../../inc/xdraw.h
coeff.o: ../../inc/coeff.h ../../inc/fcoeff.h
indexmx.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
indexmx.o: /usr/include/features.h /usr/include/sys/cdefs.h
indexmx.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
indexmx.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
indexmx.o: /usr/include/bits/typesizes.h /usr/include/libio.h
indexmx.o: /usr/include/_G_config.h /usr/include/wchar.h
indexmx.o: /usr/include/bits/wchar.h /usr/include/gconv.h
indexmx.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
indexmx.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
indexmx.o: /usr/include/bits/endian.h /usr/include/stdlib.h
indexmx.o: /usr/include/sys/types.h /usr/include/time.h
indexmx.o: /usr/include/sys/select.h /usr/include/bits/select.h
indexmx.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
indexmx.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
indexmx.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
indexmx.o: ../../inc/message.h ../../inc/utility.h /usr/include/math.h
indexmx.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
indexmx.o: /usr/include/bits/mathcalls.h /usr/include/limits.h
indexmx.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
indexmx.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
indexmx.o: ../../inc/circuits.h ../../inc/node.h ../../inc/branch.h
indexmx.o: ../../inc/rclcompx.h ../../inc/complex.h /usr/include/errno.h
indexmx.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
indexmx.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
indexmx.o: /usr/include/asm-generic/errno.h
indexmx.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
indexmx.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
indexmx.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
indexmx.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
indexmx.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
indexmx.o: ../../inc/control.h ../../inc/mxexp.h
fcoeff.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
fcoeff.o: /usr/include/features.h /usr/include/sys/cdefs.h
fcoeff.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
fcoeff.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
fcoeff.o: /usr/include/bits/typesizes.h /usr/include/libio.h
fcoeff.o: /usr/include/_G_config.h /usr/include/wchar.h
fcoeff.o: /usr/include/bits/wchar.h /usr/include/gconv.h
fcoeff.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
fcoeff.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
fcoeff.o: /usr/include/bits/endian.h /usr/include/stdlib.h
fcoeff.o: /usr/include/sys/types.h /usr/include/time.h
fcoeff.o: /usr/include/sys/select.h /usr/include/bits/select.h
fcoeff.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
fcoeff.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
fcoeff.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
fcoeff.o: ../../inc/message.h ../../inc/utility.h ../../inc/control.h
fcoeff.o: ../../inc/complex.h /usr/include/math.h
fcoeff.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
fcoeff.o: /usr/include/bits/mathcalls.h /usr/include/errno.h
fcoeff.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
fcoeff.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
fcoeff.o: /usr/include/asm-generic/errno.h
fcoeff.o: /usr/include/asm-generic/errno-base.h ../../inc/node.h
fcoeff.o: ../../inc/branch.h ../../inc/rclcompx.h ../../inc/mxexp.h
fcoeff.o: ../../inc/ddd_base.h ../../inc/compable.h ../../inc/object.h
fcoeff.o: ../../inc/classdef.h ../../inc/copyr.h ../../inc/mem.h
fcoeff.o: ../../inc/hasher.h ../../inc/list.h ../../inc/hashcach.h
fcoeff.o: ../../inc/tnode.h ../../inc/floatobj.h ../../inc/dtstruct.h
fcoeff.o: ../../inc/mna_form.h ../../inc/acan.h ../../inc/xdraw.h
fcoeff.o: ../../inc/coeff.h ../../inc/fcoeff.h ../../inc/cancel_flab_map.h
polezero.o: /usr/include/limits.h /usr/include/features.h
polezero.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
polezero.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
polezero.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
polezero.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
polezero.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
polezero.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
polezero.o: /usr/include/libio.h /usr/include/_G_config.h
polezero.o: /usr/include/wchar.h /usr/include/bits/wchar.h
polezero.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
polezero.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
polezero.o: /usr/include/ctype.h /usr/include/endian.h
polezero.o: /usr/include/bits/endian.h /usr/include/stdlib.h
polezero.o: /usr/include/sys/types.h /usr/include/time.h
polezero.o: /usr/include/sys/select.h /usr/include/bits/select.h
polezero.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
polezero.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
polezero.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
polezero.o: ../../inc/message.h ../../inc/utility.h ../../inc/ddd_base.h
polezero.o: ../../inc/compable.h ../../inc/object.h ../../inc/classdef.h
polezero.o: ../../inc/copyr.h ../../inc/mem.h ../../inc/hasher.h
polezero.o: ../../inc/list.h ../../inc/hashcach.h ../../inc/tnode.h
polezero.o: ../../inc/floatobj.h ../../inc/dtstruct.h ../../inc/rclcompx.h
polezero.o: /usr/include/math.h /usr/include/bits/huge_val.h
polezero.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
polezero.o: ../../inc/complex.h /usr/include/errno.h
polezero.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
polezero.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
polezero.o: /usr/include/asm-generic/errno.h
polezero.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
polezero.o: ../../inc/node.h ../../inc/branch.h ../../inc/control.h
polezero.o: ../../inc/circuits.h ../../inc/mxexp.h ../../inc/approx.h
polezero.o: ../../inc/acan.h ../../inc/xdraw.h ../../inc/coeff.h
polezero.o: ../../inc/fcoeff.h
buildexp.o: /usr/include/math.h /usr/include/features.h
buildexp.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
buildexp.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
buildexp.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
buildexp.o: /usr/include/bits/mathcalls.h ../../inc/circuits.h
buildexp.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
buildexp.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
buildexp.o: /usr/include/libio.h /usr/include/_G_config.h
buildexp.o: /usr/include/wchar.h /usr/include/bits/wchar.h
buildexp.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
buildexp.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
buildexp.o: /usr/include/ctype.h /usr/include/endian.h
buildexp.o: /usr/include/bits/endian.h /usr/include/stdlib.h
buildexp.o: /usr/include/sys/types.h /usr/include/time.h
buildexp.o: /usr/include/sys/select.h /usr/include/bits/select.h
buildexp.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
buildexp.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
buildexp.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
buildexp.o: ../../inc/message.h ../../inc/utility.h ../../inc/node.h
buildexp.o: ../../inc/branch.h ../../inc/rclcompx.h ../../inc/complex.h
buildexp.o: /usr/include/errno.h /usr/include/bits/errno.h
buildexp.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
buildexp.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
buildexp.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
buildexp.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
buildexp.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
buildexp.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
buildexp.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
buildexp.o: ../../inc/control.h ../../inc/acan.h ../../inc/xdraw.h
buildexp.o: ../../inc/coeff.h ../../inc/fcoeff.h ../../inc/mxexp.h
buildexp.o: ../../inc/graph_out.h ../../inc/approx.h
buildexp.o: ../../inc/cancel_flab_map.h
graph.o: /usr/include/math.h /usr/include/features.h /usr/include/sys/cdefs.h
graph.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
graph.o: /usr/include/gnu/stubs-64.h /usr/include/bits/huge_val.h
graph.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
graph.o: ../../inc/circuits.h ../../inc/globals.h ../../inc/unixstd.h
graph.o: /usr/include/stdio.h /usr/include/bits/types.h
graph.o: /usr/include/bits/typesizes.h /usr/include/libio.h
graph.o: /usr/include/_G_config.h /usr/include/wchar.h
graph.o: /usr/include/bits/wchar.h /usr/include/gconv.h
graph.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
graph.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
graph.o: /usr/include/bits/endian.h /usr/include/stdlib.h
graph.o: /usr/include/sys/types.h /usr/include/time.h
graph.o: /usr/include/sys/select.h /usr/include/bits/select.h
graph.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
graph.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
graph.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
graph.o: ../../inc/message.h ../../inc/utility.h ../../inc/node.h
graph.o: ../../inc/branch.h ../../inc/rclcompx.h ../../inc/complex.h
graph.o: /usr/include/errno.h /usr/include/bits/errno.h
graph.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
graph.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
graph.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
graph.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
graph.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
graph.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
graph.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
graph.o: ../../inc/control.h ../../inc/acan.h ../../inc/xdraw.h
graph.o: ../../inc/coeff.h ../../inc/fcoeff.h
dddgc.o: ../../inc/circuits.h ../../inc/globals.h ../../inc/unixstd.h
dddgc.o: /usr/include/stdio.h /usr/include/features.h
dddgc.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
dddgc.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
dddgc.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
dddgc.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
dddgc.o: /usr/include/bits/wchar.h /usr/include/gconv.h
dddgc.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
dddgc.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
dddgc.o: /usr/include/bits/endian.h /usr/include/stdlib.h
dddgc.o: /usr/include/sys/types.h /usr/include/time.h
dddgc.o: /usr/include/sys/select.h /usr/include/bits/select.h
dddgc.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
dddgc.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
dddgc.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
dddgc.o: ../../inc/message.h ../../inc/utility.h ../../inc/node.h
dddgc.o: ../../inc/branch.h ../../inc/rclcompx.h /usr/include/math.h
dddgc.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
dddgc.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
dddgc.o: /usr/include/errno.h /usr/include/bits/errno.h
dddgc.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
dddgc.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
dddgc.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
dddgc.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
dddgc.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
dddgc.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
dddgc.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
dddgc.o: ../../inc/control.h ../../inc/acan.h ../../inc/xdraw.h
dddgc.o: ../../inc/coeff.h ../../inc/fcoeff.h ../../inc/mxexp.h
label.o: /usr/include/math.h /usr/include/features.h /usr/include/sys/cdefs.h
label.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
label.o: /usr/include/gnu/stubs-64.h /usr/include/bits/huge_val.h
label.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
label.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
label.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
label.o: /usr/include/bits/posix2_lim.h /usr/include/unistd.h
label.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
label.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
label.o: /usr/include/getopt.h ../../inc/globals.h ../../inc/unixstd.h
label.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
label.o: /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h
label.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
label.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
label.o: /usr/include/bits/endian.h /usr/include/stdlib.h
label.o: /usr/include/sys/types.h /usr/include/time.h
label.o: /usr/include/sys/select.h /usr/include/bits/select.h
label.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
label.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
label.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
label.o: ../../inc/message.h ../../inc/utility.h ../../inc/circuits.h
label.o: ../../inc/node.h ../../inc/branch.h ../../inc/rclcompx.h
label.o: ../../inc/complex.h /usr/include/errno.h /usr/include/bits/errno.h
label.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
label.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
label.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
label.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
label.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
label.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
label.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
label.o: ../../inc/control.h ../../inc/ckt_call.h ../../inc/acan.h
label.o: ../../inc/xdraw.h ../../inc/coeff.h ../../inc/fcoeff.h
label.o: ../../inc/mxexp.h
noisean.o: /usr/include/math.h /usr/include/features.h
noisean.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
noisean.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
noisean.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
noisean.o: /usr/include/bits/mathcalls.h ../../inc/circuits.h
noisean.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
noisean.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
noisean.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
noisean.o: /usr/include/bits/wchar.h /usr/include/gconv.h
noisean.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
noisean.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
noisean.o: /usr/include/bits/endian.h /usr/include/stdlib.h
noisean.o: /usr/include/sys/types.h /usr/include/time.h
noisean.o: /usr/include/sys/select.h /usr/include/bits/select.h
noisean.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
noisean.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
noisean.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
noisean.o: ../../inc/message.h ../../inc/utility.h ../../inc/node.h
noisean.o: ../../inc/branch.h ../../inc/rclcompx.h ../../inc/complex.h
noisean.o: /usr/include/errno.h /usr/include/bits/errno.h
noisean.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
noisean.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
noisean.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
noisean.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
noisean.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
noisean.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
noisean.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
noisean.o: ../../inc/control.h ../../inc/acan.h ../../inc/xdraw.h
noisean.o: ../../inc/coeff.h ../../inc/fcoeff.h ../../inc/mxexp.h
noisean.o: ../../inc/approx.h
noise_src.o: /usr/include/math.h /usr/include/features.h
noise_src.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
noise_src.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
noise_src.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
noise_src.o: /usr/include/bits/mathcalls.h ../../inc/acan.h
noise_src.o: ../../inc/unixstd.h /usr/include/stdio.h
noise_src.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
noise_src.o: /usr/include/libio.h /usr/include/_G_config.h
noise_src.o: /usr/include/wchar.h /usr/include/bits/wchar.h
noise_src.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
noise_src.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
noise_src.o: /usr/include/ctype.h /usr/include/endian.h
noise_src.o: /usr/include/bits/endian.h /usr/include/stdlib.h
noise_src.o: /usr/include/sys/types.h /usr/include/time.h
noise_src.o: /usr/include/sys/select.h /usr/include/bits/select.h
noise_src.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
noise_src.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
noise_src.o: /usr/include/alloca.h ../../inc/globals.h ../../inc/stddefs.h
noise_src.o: /usr/include/assert.h ../../inc/message.h ../../inc/utility.h
noise_src.o: ../../inc/ddd_base.h ../../inc/compable.h ../../inc/object.h
noise_src.o: ../../inc/classdef.h ../../inc/copyr.h ../../inc/mem.h
noise_src.o: ../../inc/hasher.h ../../inc/list.h ../../inc/hashcach.h
noise_src.o: ../../inc/tnode.h ../../inc/floatobj.h ../../inc/dtstruct.h
noise_src.o: ../../inc/rclcompx.h ../../inc/complex.h /usr/include/errno.h
noise_src.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
noise_src.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
noise_src.o: /usr/include/asm-generic/errno.h
noise_src.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
noise_src.o: ../../inc/node.h ../../inc/branch.h ../../inc/xdraw.h
noise_src.o: ../../inc/coeff.h ../../inc/fcoeff.h
symb_gen.o: /usr/include/stdio.h /usr/include/features.h
symb_gen.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
symb_gen.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
symb_gen.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
symb_gen.o: /usr/include/libio.h /usr/include/_G_config.h
symb_gen.o: /usr/include/wchar.h /usr/include/bits/wchar.h
symb_gen.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
symb_gen.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
symb_gen.o: /usr/include/sys/types.h /usr/include/time.h
symb_gen.o: /usr/include/endian.h /usr/include/bits/endian.h
symb_gen.o: /usr/include/sys/select.h /usr/include/bits/select.h
symb_gen.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
symb_gen.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
symb_gen.o: /usr/include/alloca.h /usr/include/string.h /usr/include/limits.h
symb_gen.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
symb_gen.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
symb_gen.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/ctype.h
symb_gen.o: ../../inc/stddefs.h /usr/include/assert.h ../../inc/message.h
symb_gen.o: ../../inc/utility.h ../../inc/ddd_base.h ../../inc/compable.h
symb_gen.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
symb_gen.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
symb_gen.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
symb_gen.o: ../../inc/dtstruct.h ../../inc/rclcompx.h /usr/include/math.h
symb_gen.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
symb_gen.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
symb_gen.o: /usr/include/errno.h /usr/include/bits/errno.h
symb_gen.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
symb_gen.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
symb_gen.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
symb_gen.o: ../../inc/node.h ../../inc/branch.h ../../inc/control.h
symb_gen.o: ../../inc/mxexp.h ../../inc/acan.h ../../inc/xdraw.h
symb_gen.o: ../../inc/coeff.h ../../inc/fcoeff.h ../../inc/circuits.h
symb_gen.o: ../../inc/ckt_call.h
xdraw.o: /usr/include/math.h /usr/include/features.h /usr/include/sys/cdefs.h
xdraw.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
xdraw.o: /usr/include/gnu/stubs-64.h /usr/include/bits/huge_val.h
xdraw.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
xdraw.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
xdraw.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
xdraw.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
xdraw.o: /usr/include/bits/wchar.h /usr/include/gconv.h
xdraw.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
xdraw.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
xdraw.o: /usr/include/bits/endian.h /usr/include/stdlib.h
xdraw.o: /usr/include/sys/types.h /usr/include/time.h
xdraw.o: /usr/include/sys/select.h /usr/include/bits/select.h
xdraw.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
xdraw.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
xdraw.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
xdraw.o: ../../inc/message.h ../../inc/utility.h ../../inc/xdraw.h
cancel_flab_map.o: ../../inc/globals.h ../../inc/unixstd.h
cancel_flab_map.o: /usr/include/stdio.h /usr/include/features.h
cancel_flab_map.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
cancel_flab_map.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
cancel_flab_map.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
cancel_flab_map.o: /usr/include/libio.h /usr/include/_G_config.h
cancel_flab_map.o: /usr/include/wchar.h /usr/include/bits/wchar.h
cancel_flab_map.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
cancel_flab_map.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
cancel_flab_map.o: /usr/include/ctype.h /usr/include/endian.h
cancel_flab_map.o: /usr/include/bits/endian.h /usr/include/stdlib.h
cancel_flab_map.o: /usr/include/sys/types.h /usr/include/time.h
cancel_flab_map.o: /usr/include/sys/select.h /usr/include/bits/select.h
cancel_flab_map.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
cancel_flab_map.o: /usr/include/sys/sysmacros.h
cancel_flab_map.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
cancel_flab_map.o: ../../inc/stddefs.h /usr/include/assert.h
cancel_flab_map.o: ../../inc/message.h ../../inc/utility.h
cancel_flab_map.o: ../../inc/cancel_flab_map.h
new_term_gen.o: /usr/include/stdio.h /usr/include/features.h
new_term_gen.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
new_term_gen.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
new_term_gen.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
new_term_gen.o: /usr/include/libio.h /usr/include/_G_config.h
new_term_gen.o: /usr/include/wchar.h /usr/include/bits/wchar.h
new_term_gen.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
new_term_gen.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
new_term_gen.o: /usr/include/sys/types.h /usr/include/time.h
new_term_gen.o: /usr/include/endian.h /usr/include/bits/endian.h
new_term_gen.o: /usr/include/sys/select.h /usr/include/bits/select.h
new_term_gen.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
new_term_gen.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
new_term_gen.o: /usr/include/alloca.h /usr/include/string.h
new_term_gen.o: /usr/include/limits.h /usr/include/bits/posix1_lim.h
new_term_gen.o: /usr/include/bits/local_lim.h /usr/include/linux/limits.h
new_term_gen.o: /usr/include/bits/posix2_lim.h ../../inc/globals.h
new_term_gen.o: ../../inc/unixstd.h /usr/include/ctype.h ../../inc/stddefs.h
new_term_gen.o: /usr/include/assert.h ../../inc/message.h ../../inc/utility.h
new_term_gen.o: ../../inc/ddd_base.h ../../inc/compable.h ../../inc/object.h
new_term_gen.o: ../../inc/classdef.h ../../inc/copyr.h ../../inc/mem.h
new_term_gen.o: ../../inc/hasher.h ../../inc/list.h ../../inc/hashcach.h
new_term_gen.o: ../../inc/tnode.h ../../inc/floatobj.h ../../inc/dtstruct.h
new_term_gen.o: ../../inc/rclcompx.h /usr/include/math.h
new_term_gen.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
new_term_gen.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
new_term_gen.o: /usr/include/errno.h /usr/include/bits/errno.h
new_term_gen.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
new_term_gen.o: /usr/include/asm-x86_64/errno.h
new_term_gen.o: /usr/include/asm-generic/errno.h
new_term_gen.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
new_term_gen.o: ../../inc/node.h ../../inc/branch.h ../../inc/control.h
new_term_gen.o: ../../inc/mxexp.h ../../inc/approx.h ../../inc/acan.h
new_term_gen.o: ../../inc/xdraw.h ../../inc/coeff.h ../../inc/fcoeff.h
new_term_gen2.o: /usr/include/stdio.h /usr/include/features.h
new_term_gen2.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
new_term_gen2.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
new_term_gen2.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
new_term_gen2.o: /usr/include/libio.h /usr/include/_G_config.h
new_term_gen2.o: /usr/include/wchar.h /usr/include/bits/wchar.h
new_term_gen2.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
new_term_gen2.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
new_term_gen2.o: /usr/include/sys/types.h /usr/include/time.h
new_term_gen2.o: /usr/include/endian.h /usr/include/bits/endian.h
new_term_gen2.o: /usr/include/sys/select.h /usr/include/bits/select.h
new_term_gen2.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
new_term_gen2.o: /usr/include/sys/sysmacros.h
new_term_gen2.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
new_term_gen2.o: /usr/include/string.h /usr/include/limits.h
new_term_gen2.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
new_term_gen2.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
new_term_gen2.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/ctype.h
new_term_gen2.o: ../../inc/stddefs.h /usr/include/assert.h
new_term_gen2.o: ../../inc/message.h ../../inc/utility.h ../../inc/ddd_base.h
new_term_gen2.o: ../../inc/compable.h ../../inc/object.h ../../inc/classdef.h
new_term_gen2.o: ../../inc/copyr.h ../../inc/mem.h ../../inc/hasher.h
new_term_gen2.o: ../../inc/list.h ../../inc/hashcach.h ../../inc/tnode.h
new_term_gen2.o: ../../inc/floatobj.h ../../inc/dtstruct.h
new_term_gen2.o: ../../inc/rclcompx.h /usr/include/math.h
new_term_gen2.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
new_term_gen2.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
new_term_gen2.o: /usr/include/errno.h /usr/include/bits/errno.h
new_term_gen2.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
new_term_gen2.o: /usr/include/asm-x86_64/errno.h
new_term_gen2.o: /usr/include/asm-generic/errno.h
new_term_gen2.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
new_term_gen2.o: ../../inc/node.h ../../inc/branch.h ../../inc/control.h
new_term_gen2.o: ../../inc/mxexp.h ../../inc/approx.h ../../inc/acan.h
new_term_gen2.o: ../../inc/xdraw.h ../../inc/coeff.h ../../inc/fcoeff.h
new_term_gen4.o: /usr/include/stdio.h /usr/include/features.h
new_term_gen4.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
new_term_gen4.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
new_term_gen4.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
new_term_gen4.o: /usr/include/libio.h /usr/include/_G_config.h
new_term_gen4.o: /usr/include/wchar.h /usr/include/bits/wchar.h
new_term_gen4.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
new_term_gen4.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
new_term_gen4.o: /usr/include/sys/types.h /usr/include/time.h
new_term_gen4.o: /usr/include/endian.h /usr/include/bits/endian.h
new_term_gen4.o: /usr/include/sys/select.h /usr/include/bits/select.h
new_term_gen4.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
new_term_gen4.o: /usr/include/sys/sysmacros.h
new_term_gen4.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
new_term_gen4.o: /usr/include/string.h /usr/include/limits.h
new_term_gen4.o: /usr/include/bits/posix1_lim.h /usr/include/bits/local_lim.h
new_term_gen4.o: /usr/include/linux/limits.h /usr/include/bits/posix2_lim.h
new_term_gen4.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/ctype.h
new_term_gen4.o: ../../inc/stddefs.h /usr/include/assert.h
new_term_gen4.o: ../../inc/message.h ../../inc/utility.h ../../inc/ddd_base.h
new_term_gen4.o: ../../inc/compable.h ../../inc/object.h ../../inc/classdef.h
new_term_gen4.o: ../../inc/copyr.h ../../inc/mem.h ../../inc/hasher.h
new_term_gen4.o: ../../inc/list.h ../../inc/hashcach.h ../../inc/tnode.h
new_term_gen4.o: ../../inc/floatobj.h ../../inc/dtstruct.h
new_term_gen4.o: ../../inc/rclcompx.h /usr/include/math.h
new_term_gen4.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
new_term_gen4.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
new_term_gen4.o: /usr/include/errno.h /usr/include/bits/errno.h
new_term_gen4.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
new_term_gen4.o: /usr/include/asm-x86_64/errno.h
new_term_gen4.o: /usr/include/asm-generic/errno.h
new_term_gen4.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
new_term_gen4.o: ../../inc/node.h ../../inc/branch.h ../../inc/control.h
new_term_gen4.o: ../../inc/mxexp.h ../../inc/approx.h ../../inc/acan.h
new_term_gen4.o: ../../inc/xdraw.h ../../inc/coeff.h ../../inc/fcoeff.h
semi_symb.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
semi_symb.o: /usr/include/features.h /usr/include/sys/cdefs.h
semi_symb.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
semi_symb.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
semi_symb.o: /usr/include/bits/typesizes.h /usr/include/libio.h
semi_symb.o: /usr/include/_G_config.h /usr/include/wchar.h
semi_symb.o: /usr/include/bits/wchar.h /usr/include/gconv.h
semi_symb.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
semi_symb.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
semi_symb.o: /usr/include/bits/endian.h /usr/include/stdlib.h
semi_symb.o: /usr/include/sys/types.h /usr/include/time.h
semi_symb.o: /usr/include/sys/select.h /usr/include/bits/select.h
semi_symb.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
semi_symb.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
semi_symb.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
semi_symb.o: ../../inc/message.h ../../inc/utility.h ../../inc/circuits.h
semi_symb.o: ../../inc/node.h ../../inc/branch.h ../../inc/rclcompx.h
semi_symb.o: /usr/include/math.h /usr/include/bits/huge_val.h
semi_symb.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
semi_symb.o: ../../inc/complex.h /usr/include/errno.h
semi_symb.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
semi_symb.o: /usr/include/asm/errno.h /usr/include/asm-x86_64/errno.h
semi_symb.o: /usr/include/asm-generic/errno.h
semi_symb.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
semi_symb.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
semi_symb.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
semi_symb.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
semi_symb.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
semi_symb.o: ../../inc/control.h ../../inc/mxexp.h ../../inc/acan.h
semi_symb.o: ../../inc/xdraw.h ../../inc/coeff.h ../../inc/fcoeff.h
reordering.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/stdio.h
reordering.o: /usr/include/features.h /usr/include/sys/cdefs.h
reordering.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
reordering.o: /usr/include/gnu/stubs-64.h /usr/include/bits/types.h
reordering.o: /usr/include/bits/typesizes.h /usr/include/libio.h
reordering.o: /usr/include/_G_config.h /usr/include/wchar.h
reordering.o: /usr/include/bits/wchar.h /usr/include/gconv.h
reordering.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
reordering.o: /usr/include/string.h /usr/include/ctype.h
reordering.o: /usr/include/endian.h /usr/include/bits/endian.h
reordering.o: /usr/include/stdlib.h /usr/include/sys/types.h
reordering.o: /usr/include/time.h /usr/include/sys/select.h
reordering.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
reordering.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
reordering.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
reordering.o: ../../inc/stddefs.h /usr/include/assert.h ../../inc/message.h
reordering.o: ../../inc/utility.h ../../inc/circuits.h ../../inc/node.h
reordering.o: ../../inc/branch.h ../../inc/rclcompx.h /usr/include/math.h
reordering.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
reordering.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
reordering.o: /usr/include/errno.h /usr/include/bits/errno.h
reordering.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
reordering.o: /usr/include/asm-x86_64/errno.h
reordering.o: /usr/include/asm-generic/errno.h
reordering.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
reordering.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
reordering.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
reordering.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
reordering.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
reordering.o: ../../inc/control.h ../../inc/mxexp.h ../../inc/acan.h
reordering.o: ../../inc/xdraw.h ../../inc/coeff.h ../../inc/fcoeff.h
spgram.o: /usr/include/stdio.h /usr/include/features.h
spgram.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
spgram.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
spgram.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
spgram.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
spgram.o: /usr/include/bits/wchar.h /usr/include/gconv.h
spgram.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
spgram.o: tokens.h ../../inc/globals.h ../../inc/unixstd.h
spgram.o: /usr/include/string.h /usr/include/ctype.h /usr/include/endian.h
spgram.o: /usr/include/bits/endian.h /usr/include/stdlib.h
spgram.o: /usr/include/sys/types.h /usr/include/time.h
spgram.o: /usr/include/sys/select.h /usr/include/bits/select.h
spgram.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
spgram.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
spgram.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
spgram.o: ../../inc/message.h ../../inc/utility.h sp_parser.h DLGLexer.h
spgram.o: ../../inc/circuits.h ../../inc/node.h ../../inc/branch.h
spgram.o: ../../inc/rclcompx.h /usr/include/math.h
spgram.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
spgram.o: /usr/include/bits/mathcalls.h ../../inc/complex.h
spgram.o: /usr/include/errno.h /usr/include/bits/errno.h
spgram.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
spgram.o: /usr/include/asm-x86_64/errno.h /usr/include/asm-generic/errno.h
spgram.o: /usr/include/asm-generic/errno-base.h ../../inc/mna_form.h
spgram.o: ../../inc/dtstruct.h ../../inc/ddd_base.h ../../inc/compable.h
spgram.o: ../../inc/object.h ../../inc/classdef.h ../../inc/copyr.h
spgram.o: ../../inc/mem.h ../../inc/hasher.h ../../inc/list.h
spgram.o: ../../inc/hashcach.h ../../inc/tnode.h ../../inc/floatobj.h
spgram.o: ../../inc/control.h ../../inc/ckt_call.h ../../inc/acan.h
spgram.o: ../../inc/xdraw.h ../../inc/coeff.h ../../inc/fcoeff.h
sp_parser.o: /usr/include/stdio.h /usr/include/features.h
sp_parser.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
sp_parser.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
sp_parser.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
sp_parser.o: /usr/include/libio.h /usr/include/_G_config.h
sp_parser.o: /usr/include/wchar.h /usr/include/bits/wchar.h
sp_parser.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
sp_parser.o: /usr/include/bits/sys_errlist.h tokens.h sp_parser.h
sp_parser.o: ../../inc/globals.h ../../inc/unixstd.h /usr/include/string.h
sp_parser.o: /usr/include/ctype.h /usr/include/endian.h
sp_parser.o: /usr/include/bits/endian.h /usr/include/stdlib.h
sp_parser.o: /usr/include/sys/types.h /usr/include/time.h
sp_parser.o: /usr/include/sys/select.h /usr/include/bits/select.h
sp_parser.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
sp_parser.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
sp_parser.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
sp_parser.o: ../../inc/message.h ../../inc/utility.h
DLGLexer.o: tokens.h ../../inc/globals.h ../../inc/unixstd.h
DLGLexer.o: /usr/include/stdio.h /usr/include/features.h
DLGLexer.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
DLGLexer.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-64.h
DLGLexer.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
DLGLexer.o: /usr/include/libio.h /usr/include/_G_config.h
DLGLexer.o: /usr/include/wchar.h /usr/include/bits/wchar.h
DLGLexer.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
DLGLexer.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
DLGLexer.o: /usr/include/ctype.h /usr/include/endian.h
DLGLexer.o: /usr/include/bits/endian.h /usr/include/stdlib.h
DLGLexer.o: /usr/include/sys/types.h /usr/include/time.h
DLGLexer.o: /usr/include/sys/select.h /usr/include/bits/select.h
DLGLexer.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
DLGLexer.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
DLGLexer.o: /usr/include/alloca.h ../../inc/stddefs.h /usr/include/assert.h
DLGLexer.o: ../../inc/message.h ../../inc/utility.h DLGLexer.h
