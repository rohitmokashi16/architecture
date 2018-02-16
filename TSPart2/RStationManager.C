#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <iostream>
#include "Instruction.h"
#include "FUnit.h"
#include "RStation.h"
#include "RStationManager.h"

/*
 * RStationManager
 * @description: construct an array of count RStation objects, assigning each
 *               a unique name
 * @param: count - the number of RStation objects to create
 * @param: unitName - the prefix of the name
 */
RStationManager::RStationManager(int32_t count, std::string unitName)  
{
   int32_t i;
   RS = new RStation*[count];
   RScount = count;
   for (i = 0; i < RScount; i++) RS[i] = new RStation(unitName + std::to_string((long long int)i));
}

/*
 * getRSCount
 * @description: return the count of the number of RStation objects
 *               being managed
 * @returns: RScount
 */
int32_t RStationManager::getRScount()
{
   return RScount;
}

//return the station with the index which or NULL
//if which is out of range 
RStation * RStationManager::getRStation(int32_t which)
{
  return RS[which];
}

//find a free reservation station in the array (one that is not busy)
//and call fillStation on that RStation to fill it.
//if all are busy, return false.
bool RStationManager::addToStation(uint32_t encoding, Instruction instrtype)
{
  for(int i = 0; i < RScount; i++) {
    RStation * rs = RS[i];
    if(!rs->busy) {
      rs->fillStation(encoding, instrtype);
      return true;
    }
  }
  return false;
}

/*
 * dump
 * @descripton: dump the contents of the reservation stations
 *              being managed by this RStationManager
 */
void RStationManager::dump()
{
   int32_t i;
   std::cout << std::setw(9) << std::setfill(' ') << "Name" << " "
             << std::setw(8) << std::setfill(' ') << "Busy"  << " "
             << std::setw(8) << std::setfill(' ') << "Op"  << " "
             << std::setw(8) << std::setfill(' ') << "Vj"  << " "
             << std::setw(8) << std::setfill(' ') << "Vk"  << " "
             << std::setw(9) << std::setfill(' ') << "Qj"  << " "
             << std::setw(9) << std::setfill(' ') << "Qk"  << " "
             << std::setw(8) << std::setfill(' ') << "A" <<  " "
             << std::setw(8) << std::setfill(' ') << "Result" <<  " "
             << std::setw(8) << std::setfill(' ') << "X-Start" << " "
             << std::setw(8) << std::setfill(' ') << "X-End" << std::endl;

   for (i = 0; i < RScount; i++)
       if (RS[i]->busy) RS[i]->dump();
   std::cout << std::endl;
}
