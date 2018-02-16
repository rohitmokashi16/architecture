#include <cstdint>
#include <string>
#include <iostream>
#include "Instruction.h"
#include "Clock.h"
#include "CDB.h"
#include "FUnit.h"
#include "RStation.h"
#include "RStationManager.h"
#include "FUnitManager.h"
#include "FUnitContainer.h"

//start execution or continue execution of instructions
//in the reservation stations.  During the last execution
//cycle of an instruction, call compute result
void FUnitContainer::execute()
{
  int32_t rsCount = this->RS->getRScount();
  // find a reservation station that can begin execution
  for(int i = 0; i < rsCount; i++) {
    RStation * station = this->RS->getRStation(i);
    // if the station meets the following criteria:
      // an instruction waiting to execute
      // values in Vk and Vj and not waiting on values at locations in Qj or Qk
      // it has not been assigned a functional unit
      // it is not waiting to write it's result
    // we can begin execution
    if (station->busy && station->Qj.compare("") == 0 && station->Qk.compare("") == 0 && station->fu == NULL && !station->resultReady) {
      // try to find the station a functional unit
      FUnit * unit = FU->getFU();
      // we can't begin executing this instruction because we are out of functional units.
      if (unit == NULL) {
        return;
      }

      station->fu = unit;
      station->ExStart = Clock::clock;
      // The cycle ends when the funit is complete
      station->ExEnd = Clock::clock + unit-> maxCycles - 1; 
    }
    // if we are on the last cycle we can compute the result
    if(station->ExEnd == Clock::clock) {
      computeResult(station); 
    }
  }
}
//return true if no instructions in the reservation stations
bool FUnitContainer::empty()
{
  int32_t rsCount = RS->getRScount();
  for (int i = 0; i < rsCount; i++) {
    RStation * station = RS->getRStation(i);
    // if one of the stations we control are busy, we are not empty
    if (station->busy)
      return false;
  }
  return true;
}

//if an instruction is ready to write its result and
//the cdb is NULL, create a CDB object, initialize it, and
//return it. Otherwise, return the cdb object passed
//in.
CDB * FUnitContainer::write(CDB * cdb)
{
  // If we are passed in a CDB object then something has already reserved the CDB for this cycle. 
  // Just return that object and don't change anything in our reservation station
  if(cdb != NULL) {
    return cdb;
  }
  int32_t rsCount = RS->getRScount();
  for (int i = 0; i < rsCount; i++) {
    RStation * station = RS->getRStation(i);
    if (station->resultReady) {
      // create a new cdb object
      CDB * cdb = new CDB(station->name, station->result);
      station->resultWritten = true;
      return cdb;
    }
  }
  return NULL;
}

//update the reservation stations using the CDB
//object
void FUnitContainer::updateRStations(CDB * cdb)
{
  int32_t rsCount = RS->getRScount();
  for (int i = 0; i < rsCount; i++) {
    RStation * station = RS->getRStation(i);
    // if this station has a matching Qj or Qk field with the passed CDB object
    // revert Qj or Qk and place the value in Vj or Vk
    if (station -> Qj.compare(cdb->rsName) == 0) {
      station -> Qj = "";
      station -> Vj = cdb->value;
    } 
    if (station -> Qk.compare(cdb->rsName) == 0) {
      station -> Qk = "";
      station -> Vk = cdb->value;
    }
  }
}

//if the result is ready, clear the functional unit field
//if the result is written, clear the reservation station
void FUnitContainer::clear()
{
  for(int i = 0; i < RS->getRScount(); i++) {
    RStation * station = RS->getRStation(i);
    if (station->resultReady) {
      // release the functional unit back to the pool
      station->fu = NULL;
    }
    if (station->resultWritten) {
      // once the result is written we can clear the reservation station
      station->clear(); 
    }
  }
}

