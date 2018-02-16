#ifndef TRAPUNITCONTAINER_H
#define TRAPUNITCONTAINER_H

class TrapUnitContainer: public FUnitContainer
{
   private:
      static const int32_t instrNUM = 4;
      static const Instruction instr[instrNUM];
      static const std::string unitName;
      int32_t getInstrIndex(uint32_t opcode);
      int32_t nextIssue, nextWrite;
   public:
      TrapUnitContainer(int32_t RSct, int32_t FUct, int32_t EXct);
      bool isUnitInstr(uint32_t opcode);
      Status issue(uint32_t instruction);
      void dump();
      void execute();
      CDB * write(CDB * cdb);
      void computeResult(RStation *);
};
#endif

