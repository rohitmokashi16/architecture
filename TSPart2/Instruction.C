#include <cstdint>
#include <string>
#include "Instruction.h"

/*
 * Instruction:
 * @description: initialize an instruction object.
 * @params:
      std::string opcode;     //string representation of the opcode (for example, "add")

      uint32_t opcodeNo;      //numeric opcode value

      uint32_t funCode;       //numeric function code  value or 0 if no function code

      REGTYPE dstReg;         // NoReg, GPReg, FPReg depending upon whether the 
                              // instruction writes to a destination register 

      int32_t dstRegBit;      // starting bit within instruction that begins 
                              // the destination register field (11 or 16)

      REGTYPE s1Reg;          // NoReg, GPReg, FPReg (this info is used to 
                              // initialize Vj and Qj)

      int32_t s1RegBit;       // starting bit within instruction that begins the
                              // source1 register field

      REGTYPE s2Reg;          // NoReg, GPReg, FPReg (this info is used to 
                              // initialize Vk and Qk)

      int32_t s2RegBit;       // starting bit within instruction that begins 
                              // the source2 register field

      bool immedField;        // true or false depending upon whether instruction has
                              // an immediate field that is used to initialize station.A

      int32_t immedFieldSBit; // starting bit of immediate field

      int32_t immedFieldEBit; // ending bit of immediate field
      
      bool signExtend;        // should immediate field be sign extended?
*/
Instruction::Instruction(std::string opcode, int32_t opcodeNo, int32_t funCode, REGTYPE dstReg,
                  int32_t dstRegBit, REGTYPE s1Reg, int32_t s1RegBit, REGTYPE s2Reg,
                  int32_t s2RegBit, bool immedField, int32_t immedFieldSBit, 
                  int32_t immedFieldEBit, bool signExtend)
{
   this->opcode = opcode;
   this->opcodeNo = opcodeNo;
   this->funCode = funCode;
   this->dstReg = dstReg;
   this->dstRegBit = dstRegBit;
   this->s1Reg = s1Reg;
   this->s1RegBit = s1RegBit;
   this->s2Reg = s2Reg;
   this->s2RegBit = s2RegBit;
   this->immedField = immedField;
   this->immedFieldSBit = immedFieldSBit;
   this->immedFieldEBit = immedFieldEBit;
   this->signExtend = signExtend;
}
