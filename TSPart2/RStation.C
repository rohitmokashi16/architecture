#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>
#include "Instruction.h"
#include "FUnit.h"
#include "RStation.h"
#include "CDB.h"
#include "Register.h"
#include "GPR.h"
#include "FPR.h"
#include "Tools.h"

/*
 * Rstation
 *
 * @description: initialize a new reservation station
 * @param: name of reservation station
 */
RStation::RStation(std::string name)
{
   this->name = name;
   busy = false;
   opcode = "";
   Vj = Vk = A = 0;
   Qj = Qk = "";
   fu = NULL;
   resultReady = false;
   resultWritten = false;
   ExStart = ExEnd = -1;
}

/* 
 * fillStation
 * @description: Use the encoding and the Instruction object to
 *               fill the reservation station
 * @param: encoding - 32-bit instruction
 * @param: instrtype - Instruction object that provides the
 *         encoding information.
 */
void RStation::fillStation(uint32_t encoding, Instruction instrtype)
{
   busy = true;
   opcode = instrtype.opcode;
   result = 0;
   fu = NULL;
   resultReady = false;
   resultWritten = false;

   fillVjQj(encoding, instrtype);
   fillVkQk(encoding, instrtype);
   fillImmediate(encoding, instrtype);
   fillRegisterRS(encoding, instrtype);
}     

//set the Qi field in the GPR register or the FPR register (if necessary)
void RStation::fillRegisterRS(uint32_t encoding, Instruction instrtype)
{
    if (instrtype.dstReg == GPReg) {
      GPR * gpreg = GPR::getInstance();
      int32_t regnum = Tools::getInstructionBits(encoding, instrtype.dstRegBit, instrtype.dstRegBit + 4);
      gpreg->setQi(regnum, this->name);
    } else if (instrtype.dstReg == FPReg) {
      FPR * fpreg = FPR::getInstance();
      int32_t regnum = Tools::getInstructionBits(encoding, instrtype.dstRegBit, instrtype.dstRegBit + 4);
      fpreg->setQi(regnum, this->name);
    }
}

//grab the immediate field from the instruction if there is one
//and fill the A field in the station
void RStation::fillImmediate(uint32_t encoding, Instruction instrtype)
{
  if(instrtype.immedField) {
    if(instrtype.signExtend) {
      int32_t temp = Tools::getInstructionBits(encoding, instrtype.immedFieldSBit, instrtype.immedFieldEBit);
      temp <<= 16;
      temp >>= 16; // shift left shift right to sign extend
      A = temp;
    } else {
      A = Tools::getInstructionBits(encoding, instrtype.immedFieldSBit, instrtype.immedFieldEBit);
    }
  }
}


//grab first source from the instruction if there is one and
//fill the Vj or the Qj field in the station
void RStation::fillVjQj(uint32_t encoding, Instruction instrtype)
{
  // Get the value out of architected the register file, that's going to be either the name of a RS or a value
    if(instrtype.s1Reg == NoReg) {
      return;
    }
    Register * reg;
    // reg will either be located in the integer unit or the float unit
    if(instrtype.s1Reg == GPReg)
      reg = GPR::getInstance();
    else 
      reg = FPR::getInstance();
    // use the value in the instruction to select one of the register file
    int32_t regnum = Tools::getInstructionBits(encoding, instrtype.s1RegBit, instrtype.s1RegBit + 4);
    if (reg->hasValue(regnum)) {
      Vj = reg->get(regnum); 
    } else {
      // if the register file doesn't have the value set it in Qj 
      Qj = reg->getQi(regnum);
    }
}

//grab second source from the instruction if there is one and
//fill the Vk or the Qk field in the station
void RStation::fillVkQk(uint32_t encoding, Instruction instrtype)
{
  if (instrtype.s2Reg == NoReg) 
    return;

  Register * reg;
  
  if (instrtype.s1Reg == GPReg)
    reg = GPR::getInstance();
  else 
    reg = FPR::getInstance();
  
  int32_t regnum = Tools::getInstructionBits(encoding, instrtype.s2RegBit, instrtype.s2RegBit + 4);
  if (reg->hasValue(regnum)) {
    Vk = reg->get(regnum);    
  } else {
    Qk = reg->getQi(regnum);
  }
}

/*
 * clear
 * @description: when the result is written, this method
 *               is called to clear the reservation station
*/
void RStation::clear()
{
   busy = false;
   opcode = "";
   Vj = Vk = A = 0;
   Qj = Qk = "";
   fu = NULL;
   resultReady = false;
   resultWritten = false;
   ExStart = ExEnd = -1;
}


/*
 * dump
 * output the contents of the reservation station
 */
void RStation::dump()
{
   std::cout << std::setw(9) << std::setfill(' ') << name << " "
             << std::setw(8) << std::setfill(' ') << busy << " "
             << std::setw(8) << std::setfill(' ') << opcode << " "
             << std::setw(8) << std::setfill('0') << std::hex << Vj << " "
             << std::setw(8) << std::setfill('0') << std::hex <<  Vk << " "
             << std::setw(9) << std::setfill(' ') << Qj << " "
             << std::setw(9) << std::setfill(' ') << Qk << " "
             << std::setw(8) << std::setfill('0') << std::hex << A << " "; 
   if (resultReady == false)
   {
      std::cout << std::setw(8) << std::setfill(' ') << "--" <<  " ";
   } else
   {
      std::cout << std::setw(8) << std::setfill('0') << std::hex << result <<  " "; 
   }
   if (ExStart != -1)
   {
      std::cout << std::setw(8) << std::setfill(' ') << std::dec << ExStart << " "; 
   } else
   {
      std::cout << std::setw(8) << std::setfill(' ') << "--";
   }
   if (ExEnd != -1)
   {
      std::cout << std::setw(8) << std::setfill(' ') << std::dec << ExEnd << " "; 
   } else
   {
      std::cout << std::setw(8) << std::setfill(' ') << "--";
   }
   std::cout << std::endl;
}
