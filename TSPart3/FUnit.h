#ifndef FUNIT_H
#define FUNIT_H
class FUnit
{
   public:
      bool busy;                //is functional unit busy?
      int32_t maxCycles;        //maximum number of execution cycles
      int32_t remainingCycles;  //remaining execution cycles
      FUnit(int32_t maxCycles);
};
#endif

