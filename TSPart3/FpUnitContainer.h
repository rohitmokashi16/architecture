#ifndef FPUNITCONTAINER_H
#define FPUNITCONTAINER_H

class FpUnitContainer: public FUnitContainer
{
   private:
      static const int instrNUM = 8;
      static const Instruction instr[instrNUM];
      static const std::string unitName;
      int32_t getInstrIndex(uint32_t instruction);
   public:
      void execute();
      bool isUnitInstr(uint32_t instruction);
      Status issue(uint32_t instruction);
      FpUnitContainer(int32_t RS, int32_t FU, int32_t EX);
      void computeResult(RStation * station);
      void dump();
};
#endif

