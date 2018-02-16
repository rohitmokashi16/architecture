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
#include "Status.h"
#include "Clock.h"
#include "FpUnitContainer.h"
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

const Instruction FpUnitContainer::instr[instrNUM] = {
         Instruction("addf",   0x1, 0x0, FPReg, 16, FPReg, 6, FPReg, 11, false, 0,  0, false),
         Instruction("subf",   0x1, 0x1, FPReg, 16, FPReg, 6, FPReg, 11, false, 0,  0, false),
         Instruction("multf",  0x1, 0x2, FPReg, 16, FPReg, 6, FPReg, 11, false, 0,  0, false),
         Instruction("divf",   0x1, 0x3, FPReg, 16, FPReg, 6, FPReg, 11, false, 0,  0, false),
         Instruction("mult",   0x1, 0xe, FPReg, 16, FPReg, 6, FPReg, 11, false, 0,  0, false),
         Instruction("div",    0x1, 0xf, FPReg, 16, FPReg, 6, FPReg, 11, false, 0,  0, false),
         Instruction("cvtf2i", 0x1, 0x9, FPReg, 16, FPReg, 6, NoReg,  0, false, 0,  0, false),
         Instruction("cvti2f", 0x1, 0xc, FPReg, 16, FPReg, 6, NoReg,  0, false, 0,  0, false),
      };

const std::string FpUnitContainer::unitName = "FpUnit";

// I'm doing this so it will pass the tests, basically on the fp and mem unit the ExEnd is only set on the last cycle
void FpUnitContainer::execute() {
  
  FUnitContainer::execute();
  for (int i = 0; i < RS->getRScount(); i++) {
    RStation * station = RS->getRStation(i);
    if (station -> ExEnd != -1 && station -> ExEnd > Clock::clock)
      station -> hideEnd = true;
    else 
      station -> hideEnd = false;
  }
}

Status FpUnitContainer::issue(uint32_t instruction) {
  Instruction format = instr[getInstrIndex(instruction)];
  if (!RS->addToStation(instruction, format)) 
    return STALL;
  return CONTINUE;
}

int32_t FpUnitContainer::getInstrIndex(uint32_t instruction) {
  int32_t funcode = Tools::getInstructionBits(instruction, 27, 31);
  switch (funcode) {
    case 0x0: 
      return 0;
    case 0x1: 
      return 1;
    case 0x2: 
      return 2;
    case 0x3:
      return 3;
    case 0xe:
      return 4;
    case 0xf: 
      return 5;
    case 0x9:
      return 6;
    case 0xc: 
      return 7;
    default:
      return -1;
  }
}


bool FpUnitContainer::isUnitInstr(uint32_t instruction) {
  int32_t opcode = Tools::getInstructionBits(instruction, 0, 5);
  return (opcode == 0x1);
}
void FpUnitContainer::computeResult(RStation * station) {
  std::string opcode = station->opcode;
  station -> resultReady = true;
  
  float op1 = Tools::binToFloat(station->Vj);
  float op2 = Tools::binToFloat(station->Vk);
  
  float result = 0;
  if (opcode.compare("addf") == 0) 
    result = op1 + op2;
  if (opcode.compare("subf") == 0) 
    result = op1 - op2;
  if (opcode.compare("multf") == 0) 
    result = op1 * op2;
  if (opcode.compare("divf") == 0) 
    result = op1 / op2;

  station -> result = Tools::floatToBin(result);
  
  if (opcode.compare("mult") == 0) {
    station -> result = station->Vj * station->Vk;
  }
  if (opcode.compare("div") == 0) {
    station -> result = station -> Vj / station -> Vk;
  }
  if (opcode.compare("cvtf2i") == 0) {
    float value = Tools::binToFloat(station -> Vj);
    station -> result = (int32_t)value;
  }
  if (opcode.compare("cvti2f") == 0) {
    float value = (float) station -> Vj;
    station -> result = Tools::floatToBin(value);
  }
}

FpUnitContainer::FpUnitContainer(int32_t RSct, int32_t FUct, int32_t EXct)
{
   RS = new RStationManager(RSct, unitName);
   FU = new FUnitManager(FUct, EXct);
}

void FpUnitContainer::dump()
{
   std::cout << "Floating Point Unit:\n";
   RS->dump();
}

