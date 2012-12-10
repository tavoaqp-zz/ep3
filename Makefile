# Makefile for C/C++

include ../../Makefile.common
include depends.inc

EXECS = MatrixMultiplication 

.cpp.o:
	$(CCC) $(CFLAGS) -I../../include -c $<

all: $(EXECS)

MatrixMultiplication: MatrixMultiplication.o utils.o
	$(CCLINKER) -o $@ MatrixMultiplication.o utils.o -L../../lib -lcgm $(MPILDFLAGS)
