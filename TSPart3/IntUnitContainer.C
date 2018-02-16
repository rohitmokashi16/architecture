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
#include "IntUnitContainer.h"
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

const Instruction IntUnitContainer::instr[instrNUM] = {
         Instruction("addi",    0x8,  0x0, GPReg, 11, GPReg, 6, NoReg,  0, true,  16, 31, true),
         Instruction("nop",     0x0,  0x0, NoReg,  0, NoReg, 0, NoReg,  0, false,  0,  0, false),
         Instruction("add",     0x0, 0x20, GPReg, 16, GPReg, 6, GPReg, 11, false,  0,  0, false),
         Instruction("sub",     0x0, 0x22, GPReg, 16, GPReg, 6, GPReg, 11, false,  0,  0, false),
         Instruction("and",     0x0, 0x24, GPReg, 16, GPReg, 6, GPReg, 11, false,  0,  0, false),
         Instruction("or",      0x0, 0x25, GPReg, 16, GPReg, 6, GPReg, 11, false,  0,  0, false),
         Instruction("xor",     0x0, 0x26, GPReg, 16, GPReg, 6, GPReg, 11, false,  0,  0, false),
         Instruction("movf",    0x0, 0x32, FPReg, 16, FPReg, 6, NoReg,  0, false,  0,  0, false),
         Instruction("movfp2i", 0x0, 0x34, GPReg, 16, FPReg, 6, NoReg,  0, false,  0,  0, false),
         Instruction("movi2fp", 0x0, 0x35, FPReg, 16, GPReg, 6, NoReg,  0, false,  0,  0, false)
      };

const std::string IntUnitContainer::unitName = "IntUnit";

/*
 * IntUnitContainer
 * @description: create the Reservation Station Manager and the Functional Unit
 *               Manager.
*/
IntUnitContainer::IntUnitContainer(int32_t RSct, int32_t FUct, int32_t EXct)
{
   RS = new RStationManager(RSct, unitName);
   FU = new FUnitManager(FUct, EXct);
}

//returns true if the instruction should be issued to this unit
bool IntUnitContainer::isUnitInstr(uint32_t instruction)
{
  uint32_t opcode = Tools::getBits(instruction, 26, 31);// the opcode field is 6 bits 
  // If the opcode is 0x00 or 0x8 this instruction belongs to this functional unit
  return (opcode == 0x0 || opcode == 0x8);
}

//returns instr index if the instruction should be issued to this unit
int32_t IntUnitContainer::getInstrIndex(uint32_t instruction)
{
  uint32_t opcode = Tools::getInstructionBits(instruction, 0, 5);
  if(opcode == 0x8) {
  // opcode 8 is reserved for add immediate
    return 0;
  }
  uint32_t funcode = Tools::getInstructionBits(instruction, 26, 31);

  // use the function code to index into the instr array above
  
  switch(funcode) {
    case 0x00: 
      return 1;
    case 0x20:
      return 2;
    case 0x22:
      return 3;
    case 0x24:
      return 4;
    case 0x25:
      return 5;
    case 0x26:
      return 6;
    case 0x32:
      return 7;
    case 0x34: 
      return 8;
    case 0x35:
      return 9;
    default:
      return -1;
  }
}

//finds a free reservation station and issues the
//the instruction to this unit; the addToStation
//method in the RStation class does the issue work
Status IntUnitContainer::issue(uint32_t instruction)
{
  Instruction currentInstruction = instr[getInstrIndex(instruction)];
  // if add to station returns true, we were able to successfully add to a reservation station and continue
  // if not, we have run out of reservation stations, and need to stall until one is available
  if(RS->addToStation(instruction, currentInstruction)) {
    return CONTINUE;
  } else {
    return STALL;
  }
}

//computes the value to be stored in station->result
//based upon station->opcode
void IntUnitContainer::computeResult(RStation * station)
{
  std::string opcode = station->opcode;
  // These conditions perform the arithematic for each of the integer functions
  // then set the resultReady field to indicate the station is ready for write()
  if (opcode.compare("addi") == 0) {
    station->result = station->Vj + station->A;
    station->resultReady = true;
    return;
  }
  if (opcode.compare("add") == 0) {
    station->result = station->Vk + station->Vj;
    station->resultReady = true;
    return;
  }
  if (opcode.compare("nop") == 0) {
    station->resultReady = true;
    return;
  }
  if (opcode.compare("sub") == 0) {
    station->result = station->Vj - station->Vk;
    station->resultReady = true;
  }
  if (opcode.compare("xor") == 0) {
    station->result = station->Vj ^ station->Vk;
    station->resultReady = true;
    return;
  }
  if (opcode.compare("and") == 0) {
    station->result = station->Vj & station->Vk;
    station->resultReady = true;
    return;
  }
  if (opcode.compare("or") == 0) {
    station->result = station->Vj | station->Vk;
    station->resultReady = true;
    return;
  }
  // if we are doing some type of mov instruction, we're just going to move the bits from Vj to result
  if (opcode.compare("movf") == 0 || opcode.compare("movfp2i") ==0 || opcode.compare("movi2fp") == 0) {
    station->result = station->Vj;
    station->resultReady = true;
  }  
}

/*
 * dump
 * @description: dump the contents of the Int Unit
 */
void IntUnitContainer::dump()
{
   std::cout << "Integer Unit:\n";
   RS->dump();
}

