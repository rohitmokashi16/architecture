#ifndef FUNITMANAGER_H
#define FUNITMANAGER_H
class FUnitManager
{
   private:
      FUnit ** fUnit;
      int32_t fUnitCount;
   public:
      FUnitManager(int32_t fUnitCount, int32_t maxCycles);
      FUnit * getFU();
};
#endif

