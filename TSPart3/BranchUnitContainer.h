#define BRANCHUNITCONTAINER_H

class BranchUnitContainer: public FUnitContainer
{
   private:
      static const int instrNUM = 5;
      static const Instruction intInstr[instrNUM];
      static const std::string unitName;
   public:
      void computeResult(RStation * rs);
      bool isUnitInstr(uint32_t instruction);
      int32_t getInstrIndex(uint32_t instruction);
      Status issue(uint32_t instruction);
      CDB * write(CDB * cdb);
      void execute();

      BranchUnitContainer(int32_t RSct, int32_t FUct, int32_t EXct);
      void dump();
};
