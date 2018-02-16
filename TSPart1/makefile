CC = g++
CFLAGS = -g -c -Wall -std=c++0x 
OBJS = Memory.o Register.o tomasulo.o GPR.o FPR.o Tools.o
.C.o:
	$(CC) $(CFLAGS) $< -o $@

tomasulo: $(OBJS)

tomasulo.o: Memory.h Register.h GPR.h FPR.h

Memory.o: Memory.h Tools.h 

Register.o: Register.h

GPR.o: GPR.h Register.h

FPR.o: FPR.h Register.h

Tools.o: Tools.h
clean:
	rm *.o

submit:
	submit can tomasulo-part1 *.C *.h makefile
