CC = g++
CFLAGS = -g -c -Wall -std=c++0x 
OBJS = RStationManager.o RStation.o Instruction.o Memory.o Tools.o Register.o tomasulo.o \
 GPR.o FPR.o IntUnitContainer.o MemUnitContainer.o BranchUnitContainer.o FpUnitContainer.o FUnitContainer.o FUnitManager.o FUnit.o Simulator.o \
 PC.o TrapUnitContainer.o CDB.o Clock.o  \

.C.o:
	$(CC) $(CFLAGS) $< -o $@

tomasulo: $(OBJS)

tomasulo.o: Instruction.h CDB.h Status.h Memory.h Register.h GPR.h FPR.h FUnit.h \
 RStation.h FUnitManager.h RStationManager.h FUnitContainer.h FpUnitContainer.h MemUnitContainer.h IntUnitContainer.h \
 TrapUnitContainer.h Simulator.h 

Memory.o: Memory.h Tools.h

Tools.o: Tools.h 

Register.o: Register.h CDB.h

GPR.o: CDB.h GPR.h Register.h

FPR.o: CDB.h FPR.h Register.h

Instruction.o: Instruction.h

RStation.o: RStation.h Instruction.h FUnit.h CDB.h Register.h GPR.h \
FPR.h Tools.h

RStationManager.o: RStation.h RStationManager.h Instruction.h FUnit.h



BranchUnitContainer.o:  Instruction.h CDB.h FUnit.h RStation.h FUnitManager.h \
 RStationManager.h FUnitContainer.h Status.h BranchUnitContainer.h Tools.h

MemUnitContainer.o:  Instruction.h CDB.h FUnit.h RStation.h FUnitManager.h \
 RStationManager.h FUnitContainer.h Status.h MemUnitContainer.h Tools.h

IntUnitContainer.o: Instruction.h CDB.h FUnit.h RStation.h FUnitManager.h \
 RStationManager.h FUnitContainer.h Status.h IntUnitContainer.h Tools.h

FpUnitContainer.o: Instruction.h CDB.h FUnit.h RStation.h FUnitManager.h \
 RStationManager.h FUnitContainer.h Status.h FpUnitContainer.h Tools.h


FUnitContainer.o: Instruction.h Clock.h CDB.h FUnit.h RStation.h \
 RStationManager.h FUnitManager.h FUnitContainer.h

FUnit.o: FUnit.h

FUnitManager.o: Instruction.h FUnit.h RStation.h FUnitManager.h

CDB.o: CDB.h

Simulator.o: Instruction.h Status.h Memory.h Config.h CDB.h FUnit.h \
RStation.h FUnitManager.h RStationManager.h FUnitContainer.h IntUnitContainer.h BranchUnitContainer.h FpUnitContainer.h MemUnitContainer.h\
TrapUnitContainer.h Simulator.h PC.h Register.h GPR.h FPR.h Tools.h Clock.h

PC.o: PC.h

TrapUnitContainer.o: Memory.h Clock.h Instruction.h CDB.h FUnit.h RStation.h \
FUnitManager.h RStationManager.h FUnitContainer.h Status.h TrapUnitContainer.h \
Tools.h

Clock.o: Clock.h

clean:
	rm *.o
submit:
	submit can tomasulo-part2 *.C *.h makefile
