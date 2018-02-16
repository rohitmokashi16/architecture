#include <cstdint>
#include <iostream>
#include <string>
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
#include "MemUnitContainer.h"
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


// Again this override is to fix the problem with the tests where ExEnd is not printed correctly
//

void MemUnitContainer::execute() {
  RStation * station = RS -> getRStation(nextWrite);
  if (station -> busy && station -> Qj.compare("") == 0 && station -> Qk.compare("") == 0) {

    if(station -> fu == NULL) {
      FUnit * unit = FU->getFU();
      station -> fu = unit;
      station->ExStart = Clock::clock;
      station->ExEnd = Clock::clock + unit->maxCycles - 1;
      station -> hideEnd = true;
    }
  
    if (station -> ExEnd <=  Clock::clock) {
      computeResult(station);
      station->hideEnd = false;
    }
  } 
  /**
    for (int i = 0; i < RS->getRScount(); i++) {
     RStation * station = RS->getRStation(i);
      if (station -> ExEnd != -1 && station -> ExEnd > Clock::clock)
        station -> hideEnd = false;
        //      station -> hideEnd = true;
      else 
        station -> hideEnd = false;
   } **/
}

const Instruction MemUnitContainer::instr[instrNUM] = {
         Instruction("lw",  0x23,  0x0, GPReg, 11, GPReg, 6, NoReg,  0, true, 16, 31, true),
         Instruction("sw",  0x2b,  0x0, NoReg,  0, GPReg, 6, GPReg, 11, true, 16, 31, true),
         Instruction("lf",  0x26,  0x0, FPReg, 11, GPReg, 6, NoReg,  0, true, 16, 31, true),
         Instruction("sf",  0x2e,  0x0, NoReg,  0, GPReg, 6, FPReg, 11, true, 16, 31, true),
      };

const std::string MemUnitContainer::unitName = "MemUnit";

Status MemUnitContainer::issue(uint32_t instruct) { 
  Instruction instruction = instr[getInstrIndex(instruct)];
  RStation * station = RS->getRStation(nextIssue);
  if (station -> busy)
    return STALL;
  
  station -> fillStation(instruct, instruction);

  if (nextIssue == RS->getRScount() - 1) {
    nextIssue = 0; 
  } else {
    nextIssue++;
  }
  return CONTINUE;
}



bool MemUnitContainer::isUnitInstr(uint32_t instruction) {
int32_t opcode = Tools::getInstructionBits(instruction, 0, 5);
  return (opcode == 0x23 || opcode == 0x2b || opcode == 0x26 || opcode == 0x2e);
}

int32_t MemUnitContainer::getInstrIndex(uint32_t instruction) {
  int32_t opcode = Tools::getInstructionBits(instruction, 0, 5);
  switch (opcode) {
    case 0x23:
      return 0;
    case 0x2b:
      return 1;
    case 0x26:
      return 2;
    case 0x2e: 
      return 3;
    default:
      return -1;
  }
}

void MemUnitContainer::computeResult(RStation * rs) {
  std::string opcode = rs -> opcode;
  if (opcode.compare("lw") == 0 || opcode.compare("lf") == 0) {
    // A is sign extended and added to r1
    rs -> resultReady = true;
    Memory * mem = Memory::getInstance();
    int32_t address = rs -> Vj + Tools::signExtend(rs -> A); 
    rs -> A = address;
    rs -> result = mem -> getWord(address);
  } else {
    int32_t address = rs->Vj + Tools::signExtend(rs->A);
    rs->A = address;
    rs-> resultReady = true; 
  }
}

CDB * MemUnitContainer::write(CDB * cdb) {
  if (cdb != NULL) 
    return cdb;
  
  RStation * station = RS->getRStation(nextWrite);

  if(!station -> busy || !station->resultReady)
    return cdb;

  if (nextWrite == RS->getRScount() - 1) 
    nextWrite = 0;
  else 
    nextWrite++;
  
  station->resultWritten = true;
  bool error = writeToMemory(station); 
  return prepareCDB(station);
}

// This function will write to memory and return true if there was an error
bool MemUnitContainer::writeToMemory(RStation * station) {
  std::string opcode = station->opcode;
  if (opcode.compare("lw") == 0 || opcode.compare("lf") == 0) 
    return false;
  Memory * mem = Memory::getInstance();
  mem->storeWord(station->A, station->Vk);
  return false;
}

CDB * MemUnitContainer::prepareCDB (RStation * station) {
  std::string opcode = station->opcode;
  if (opcode.compare("sw") == 0 || opcode.compare("sf") == 0) 
    return NULL;
  return new CDB(station->name, station->result); 
}

MemUnitContainer::MemUnitContainer(int32_t RSct, int32_t FUct, int32_t EXct)
{
   RS = new RStationManager(RSct, unitName);
   FU = new FUnitManager(FUct, EXct);
   nextIssue = nextWrite = 0;
}

void MemUnitContainer::dump()
{
   std::cout << "Memory Unit:\n";
   RS->dump();
}

