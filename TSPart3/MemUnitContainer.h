#ifndef MEMUNITCONTAINER_H
#define MEMUNITCONTAINER_H

class MemUnitContainer: public FUnitContainer
{
   private:
      static const int32_t instrNUM = 4;
      static const Instruction instr[instrNUM];
      static const std::string unitName;
      int32_t nextIssue, nextWrite, nextExe;
      int32_t getInstrIndex(uint32_t instruction);
      CDB * prepareCDB(RStation * station);
      bool writeToMemory(RStation * station) ;
   public:
      void execute();
      Status issue(uint32_t instruction); 
      bool isUnitInstr(uint32_t instruction);
      void computeResult(RStation * rs);
      MemUnitContainer(int32_t RSct, int32_t FUct, int32_t EXct);
      void dump();
      CDB * write(CDB * cdb);
};
#endif

