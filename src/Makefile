#
# Makefile for the delila C progams
# 
# This code has been converted from Pascal to C using p2c.
# The conversion is not perfect for each program warnings
# are generated, the -w flag turns off these warnings.
# The warnings do not seem to impact the running of the programs.
# 
SHELL = /bin/bash
CC = gcc
INCLUDES = -I/home/mplace/bin/p2c/src
LFLAGS = -L/home/mplace/bin/p2c/src 
LIBS = -lm -lp2c

all: alist catal comp dalvec dbbk delila encode makelogo malign malin mkdb ri rseq 
.PHONEY: all

alist: alist.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

catal: catal.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

comp: comp.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

dalvec: dalvec.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

dbbk: dbbk.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

delila: delila.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

encode: encode.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

makelogo: makelogo.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

malign: malign.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

malin: malin.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

mkdb: mkdb.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

ri: ri.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

rseq: rseq.c
	$(CC) -w $? $(LFLAGS) $(LIBS) $(INCLUDES) -o $@

.PHONEY: clean
clean:
	rm -f alist
	rm -f catal
	rm -f comp
	rm -f dalvec
	rm -f dbbk
	rm -f delila
	rm -f encode
	rm -f makelogo
	rm -f malign
	rm -f malin
	rm -f mkdb
	rm -f ri
	rm -f rseq




