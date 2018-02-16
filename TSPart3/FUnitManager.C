#include <cstdint>
#include <string>
#include "Instruction.h"
#include "FUnit.h"
#include "RStation.h"
#include "FUnitManager.h"

/*
 * FUnitManager
 * @description: create and initialize an array of FUnit objects
 */
FUnitManager::FUnitManager(int32_t fUnitCount, int32_t maxCycles)
{
   this->fUnitCount = fUnitCount;
   fUnit = new FUnit*[fUnitCount];
   for (int32_t i = 0; i < fUnitCount; i++)
      fUnit[i] = new FUnit(maxCycles);
}

//return a free functional unit
FUnit * FUnitManager::getFU()
{
  for (int i = 0; i < fUnitCount; i++) {
    if (!fUnit[i]->busy)
      return fUnit[i];
  }
  return NULL;
}


