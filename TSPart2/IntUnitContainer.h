#ifndef INTUNITCONTAINER_H
#define INTUNITCONTAINER_H

class IntUnitContainer: public FUnitContainer
{
   private:
      static const int32_t instrNUM = 10;
      static const Instruction instr[instrNUM];
      static const std::string unitName;
      int32_t getInstrIndex(uint32_t opcode);
   public:
      IntUnitContainer(int32_t RSct, int32_t FUct, int32_t EXct);
      bool isUnitInstr(uint32_t opcode);
      Status issue(uint32_t instruction);
      void computeResult(RStation * rstation);
      void dump();
};
#endif

