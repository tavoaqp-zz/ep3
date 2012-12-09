# Makefile for C/C++

include ../../Makefile.common
include depends.inc

EXECS = SendReceiveExample

.cpp.o:
	$(CCC) $(CFLAGS) -I../../include -c $<

all: $(EXECS)

SendReceiveExample: SendReceiveExample.o
	$(CCLINKER) -o $@ SendReceiveExample.o -L../../lib -lcgm $(MPILDFLAGS)
