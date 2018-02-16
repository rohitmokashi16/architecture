#include <cstdint>
#include <iostream>
#include <string>
#include <cstdio>
#include "Memory.h"
#include "Clock.h"
#include "Instruction.h"
#include "CDB.h"
#include "FUnit.h"
#include "RStation.h"
#include "FUnitManager.h"
#include "RStationManager.h"
#include "FUnitContainer.h"
#include "Status.h"
#include "TrapUnitContainer.h"
#include "Tools.h"

/* parameters passed to Instruction constructor:
std::string opcode;     string representation of the opcode (for example, "add")
uint32_t opcodeNo;      numeric opcode value
uint32_t funCode;       numeric function code  value or 0 if no function code
REGTYPE dstReg;         NoReg, GPReg, FPReg depending upon whether the
                        instruction writes to a destination register
int32_t dstRegBit;      starting bit within instruction that begins
                        the destination register field (11 or 16)
REGTYPE s1Reg;          NoReg, GPReg, FPReg (this info is used to
                        initialize Vj and Qj)
int32_t s1RegBit;       starting bit within instruction that begins the
                        source1 register field
REGTYPE s2Reg;          NoReg, GPReg, FPReg (this info is used to
                        initialize Vk and Qk)
int32_t s2RegBit;       starting bit within instruction that begins
                        the source2 register field
bool immedField;        true or false depending upon whether instruction has
                        an immediate field that is used to initialize station.A
int32_t immedFieldSBit; starting bit of immediate field
int32_t immedFieldEBit; ending bit of immediate field
bool signExtend;        should immediate field be sign extended?
*/

const Instruction TrapUnitContainer::instr[instrNUM] = {
         Instruction("halt",  0x11,  0x0, NoReg, 0, NoReg, 6, NoReg,  0, false, 0, 0, false),
         Instruction("dumpgpr",  0x11,  0x1, NoReg, 0, GPReg, 6, NoReg,  0, false, 0, 0, false),
         Instruction("dumpfpr",  0x11,  0x2, NoReg, 0, FPReg, 6, NoReg,  0, false, 0, 0, false),
         Instruction("dumpstr",  0x11,  0x3, NoReg, 0, GPReg, 6, NoReg,  0, false, 0, 0, false),
      };

const std::string TrapUnitContainer::unitName = "TrapUnit";

/*
 * TrapUnitContainer
 * @description: create the Reservation Station Manager and the
 *               Functional Unit Manager
 */
TrapUnitContainer::TrapUnitContainer(int32_t RSct, int32_t FUct, int32_t EXct)
{
   RS = new RStationManager(RSct, unitName);
   FU = new FUnitManager(FUct, EXct);
   nextIssue = nextWrite = 0;
}

//return true if the instruction should be issued to a
//trap unit reservation station
bool TrapUnitContainer::isUnitInstr(uint32_t instruction)
{
  return (instruction >> 26 == 0x11);
}

//returns the index of trap instruction in the instr array if
//the instruction passed in is a trap instruction
int32_t TrapUnitContainer::getInstrIndex(uint32_t instruction)
{
  // grab the function code off the end of the instruction
  int32_t funcode = Tools::getInstructionBits(instruction, 27, 31);
  return funcode;
}

//checks to see if the nextIssue reservation station is
//free and issues the trap instruction to that reservation 
//station if it is free; increments nextIssue to point
//to next reservation station (resets to 0 if necessary)
//returns CONTINUE if issued and instruction not a HALT
//returns HALL if instruction issued was a HALT
//returns STALL if no free reservation station
Status TrapUnitContainer::issue(uint32_t instruction)
{
  // use the instruction index generated above to grab one of the instructions out of the array 
  Instruction instruct = instr[getInstrIndex(instruction)];
  
  // get the station we are supposed to be issuing to
  RStation * station =  RS->getRStation(nextIssue);
  
  // if we can't load this instruction into a reservation station, we have a problem and need to stall.
  if (station->busy)
    return STALL;
  
  station->fillStation(instruction, instruct);

  // change next issue in a circular pattern
  if (nextIssue == RS->getRScount() -1) {
    nextIssue = 0;
  } else {
    nextIssue++;
  }
  // if this instruction is a halt, tell the rest of the program by returning the value
  if (instruct.opcode.compare("halt") == 0) 
    return HALT;
  // otherwise just continue
  return CONTINUE;
}

//begins (if ready) or continues execution of the trap instruction
//in reservation station nextWrite; sets resultReady
//during last cycle
void TrapUnitContainer::execute()
{
  RStation * station = RS -> getRStation(nextWrite);
  if (station->busy && station->Qj.compare("") == 0 && station->Qk.compare("") == 0) {
    if (station->fu == NULL) {
      FUnit * unit = FU->getFU();
      // get a functional unit and compute the start and end cycles
      station->ExStart = Clock::clock;
      station->ExEnd = Clock::clock + unit->maxCycles - 1; 
    } 
    // mark this station as having it's result ready so it can write
    if (station->ExEnd <= Clock::clock) {
      station->resultReady = true;
    }
  }    
}

/* 
 * computeResult
 * @description: This method is never called, but because it
 *               is declared virtual and abstract in the parent
 *               class, the compiler requires that it is
 *               implemented.
 */
void TrapUnitContainer::computeResult(RStation * station)
{

}

// When resultReady is true, this method performs
// the behavior of the instruction in the nextWrite
// reservation station.  resultWritten is set to
// true (to allow instruction to be cleared) and
// nextWrite is circularly incremented.
// The cdb value passed in is simply return since
// trap instructions don't need the CDB
CDB * TrapUnitContainer::write(CDB * cdb)
{
  RStation * station = RS -> getRStation(nextWrite);
  if (station->resultReady) {
    // update the reservation station so this instruction can be cleared
    station->resultWritten = true;
    // dump the result on stdout
    if(station->opcode.compare("dumpfpr") == 0) {
      // this is left empty since we are not yet issuing floating point instructions
      // will be completed at a later date
      //      std::printf("%f", station->Vj);
    }
    if(station->opcode.compare("dumpgpr") == 0) {
      // print the value in the register as a decimal number.
      std::printf("%d", station->Vj);  
    }
    if(station->opcode.compare("dumpstr") == 0) {
      // get a reference to the memory singleton 
      Memory * mem = Memory::getInstance();
      // pass the address in Vj to my Memory::readString method to get the output value as a std::string
      std::cout << mem->readString(station->Vj);
    }
    // We've finished this write so we may update the nextWrite index and continue.
    if (nextWrite == RS->getRScount() - 1) {
      nextWrite = 0;
    } else {
      nextWrite++;
    }
  }
  // since a trap does not write to the common data bus, just let the result passed fall through
  return cdb;
}

/*
 * dump
 * @description: output the contents of the 
 *               trap unit reservation stations.
 */
void TrapUnitContainer::dump()
{
   std::cout << "Trap Unit:\n";
   RS->dump();
}
