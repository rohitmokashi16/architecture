#include <cstdint>
#include <iostream>
#include <string>
#include "Instruction.h"
#include "CDB.h"
#include "FUnit.h"
#include "RStation.h"
#include "FUnitManager.h"
#include "RStationManager.h"
#include "FUnitContainer.h"
#include "Clock.h"
#include "Status.h"
#include "BranchUnitContainer.h"
#include "Tools.h"
#include "Register.h"
#include "GPR.h"
#include "PC.h"

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

const Instruction BranchUnitContainer::intInstr[instrNUM] = {
         Instruction("beqz",  0x4, 0x0, NoReg, 0, GPReg, 6, NoReg, 6, true, 16, 31, true),
         Instruction("j",     0x2, 0x0, NoReg, 0, NoReg, 0, NoReg, 0, true,  6, 31, true),
         Instruction("jal",   0x3, 0x0, NoReg, 0, NoReg, 0, NoReg, 0, true,  6, 31, true),
         Instruction("jr",   0x12, 0x0, NoReg, 0, GPReg, 6, NoReg, 0, false, 0,  0, false),
         Instruction("jalr", 0x13, 0x0, NoReg, 0, GPReg, 6, NoReg, 0, false, 0,  31, false)
      };

const std::string BranchUnitContainer::unitName = "BrUnit";

Status BranchUnitContainer::issue(uint32_t instruction) {
  RStation * station = RS -> getRStation(0);
  // if the reservation station is busy we can't do anything because we only have one 
  if (station->busy) 
    return STALL;
  Instruction format = intInstr[getInstrIndex(instruction)];
  if (format.opcode.compare("jal") == 0|| format.opcode.compare("jalr") == 0) {
    station->setDestinationRegister(31, GPReg); 
  }
  station->fillStation(instruction, format);
  return CONTINUE;
}


int32_t BranchUnitContainer::getInstrIndex(uint32_t instruction) {
  int32_t opcode = Tools::getInstructionBits(instruction, 0, 5);
  switch (opcode) {
    case 0x4:
      return 0;
    case 0x2:
      return 1;
    case 0x3:
      return 2;
    case 0x12: 
      return 3;
    case 0x13: 
      return 4;
    default:
      return -1;
  }
}


void BranchUnitContainer::execute() {
  RStation * station = RS->getRStation(0);
  if (station -> busy && station -> Qj.compare("") == 0 && station -> Qk.compare("") == 0) {

    if (station -> fu == NULL) {
      FUnit * unit = FU->getFU();
      station->fu = unit;
      
      station->ExStart = Clock::clock;
      station->ExEnd = Clock::clock + (unit -> maxCycles - 1); 
      computeResult(station);
    }
  }
}

void BranchUnitContainer::computeResult(RStation * rs) {
  rs->resultReady = true;
  std::string opcode = rs->opcode;
  if (opcode.compare("j") == 0) {
    rs-> result = 1;
  }
  if (opcode.compare("beqz") == 0) {
    rs->result = (rs->Vj == 0)? 1: 0;
  }
  if (opcode.compare("jr") == 0) {
    rs->result = 1;
  }
  if (opcode.compare("jal") == 0) {
    rs->result = 1;
  }
  if (opcode.compare("jalr") == 0) {
    rs->result = 1;
  }

}


CDB * BranchUnitContainer::write(CDB * cdb) {
  PC * pc = PC::getInstance();
  RStation * station = RS->getRStation(0);
  std::string opcode = station -> opcode;
  if(!station->resultReady) {
    return cdb;
  }
  station -> resultWritten = true;
  if (opcode.compare("beqz") == 0) {
    if (station->Vj == 0) {
      pc->set(station->A + pc->get() + 4);
    } else {
      pc->set(pc->get() + 4); 
    }
  }
 if (opcode.compare("j") == 0)  {
      uint32_t offset = Tools::signExtend(station -> A); 
      pc->set(pc->get() + 4 + offset);
 }
 if (opcode.compare("jr") == 0) {
      PC * pc = PC::getInstance();
      pc->set(station->Vj);
 }
 if (opcode.compare("jal") == 0) {
      uint32_t address = Tools::signExtend(station -> A); 
      address += pc->get() + 4;
      cdb = new CDB(station->name, pc->get() + 4);
      pc->set(address);
 }
 if (opcode.compare("jalr") == 0) {
      cdb = new CDB(station->name, pc->get() + 4);
      pc->set(station->Vj);
 }
  return cdb;
}

bool BranchUnitContainer::isUnitInstr(uint32_t instruction) {
  int32_t opcode = Tools::getInstructionBits(instruction, 0, 5);
  return (opcode == 0x4 || opcode == 0x2 || opcode == 0x3 || opcode == 0x12 || opcode == 0x13);
}

BranchUnitContainer::BranchUnitContainer(int32_t RSct, int32_t FUct, int32_t EXct)
{
   RS = new RStationManager(RSct, unitName);
   FU = new FUnitManager(FUct, EXct);
}

void BranchUnitContainer::dump()
{
   std::cout << "Branch Unit:\n";
   RS->dump();
}

