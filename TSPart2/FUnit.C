#include <cstdint>
#include "FUnit.h"

FUnit::FUnit(int32_t maxCycles)
{
   busy = false;
   this->maxCycles = maxCycles;
   remainingCycles = 0;
}

