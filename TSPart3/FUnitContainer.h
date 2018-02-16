#ifndef FUNITCONTAINER_H
#define FUNITCONTAINER_H
class FUnitContainer
{
   protected: 
      RStationManager * RS;
      FUnitManager * FU;
   public:
      virtual void execute();
      void clear();
      CDB * write(CDB * write);
      void updateRStations(CDB *);
      bool empty();
      virtual void computeResult(RStation * rs) = 0;
};   
#endif

