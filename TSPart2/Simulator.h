#ifndef SIMULATOR_H
#define SIMULATOR_H
class Simulator
{
   private:
      IntUnitContainer * intUnit;
      TrapUnitContainer * trapUnit;
      bool doDump;
      void dump();
      Status issue(uint32_t instruction);
      void execute();
      void clear();
      CDB * write();
      void updateRStations(CDB * cdb);
      bool done();
   public:
      Simulator(bool doDump);
      void issueExecuteWrite();
};

#endif
