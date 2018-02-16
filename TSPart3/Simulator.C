#include <cstdint>
#include <string>
#include <iostream>
#include <iomanip>
#include "Instruction.h"
#include "Status.h"
#include "Memory.h"
#include "Config.h"
#include "CDB.h"
#include "FUnit.h"
#include "RStation.h"
#include "FUnitManager.h"
#include "RStationManager.h"
#include "FUnitContainer.h"
#include "FpUnitContainer.h"
#include "IntUnitContainer.h"
#include "TrapUnitContainer.h"
#include "BranchUnitContainer.h"
#include "MemUnitContainer.h"
#include "Simulator.h"
#include "PC.h"
#include "Register.h"
#include "GPR.h"
#include "FPR.h"
#include "Tools.h"
#include "Clock.h"

/*
 * Simulator
 * @description: constructor
 * @param: doDump flag to indicate whether the units should be dump after
 *         clock cycle
 */
Simulator::Simulator(bool doDump)
{
   this->doDump = doDump;
   intUnit = new IntUnitContainer(Config::intRS, Config::intFU, Config::intEX);
   trapUnit = new TrapUnitContainer(Config::trapRS, Config::trapFU, Config::trapEX);
   memUnit = new MemUnitContainer(Config::memRS, Config::memFU, Config::memEX);
   fpUnit = new FpUnitContainer(Config::fpRS, Config::fpFU, Config::fpEX);
   brUnit = new BranchUnitContainer(Config::brRS, Config::brFU, Config::brEX);
} 

/*
 * issueExecuteWrite
 * @description: driver method for the simulator
 */
void Simulator::issueExecuteWrite()
{
   int32_t instruction;
   PC * pc = PC::getInstance();
   CDB * cdb = NULL;
   Memory * mem = Memory::getInstance();
   pc->set(0);
   Status status = CONTINUE;
   while (status != HALT || done() == false)
   {
      if (cdb != NULL) delete cdb;  //avoid memory leak
      cdb = write();
      execute();         

      //stop fetching instructions when a halt is fetched
      //or a branch is in the branch unit
      if (status != HALT && brUnit -> empty())
      {
         instruction = mem->getWord(pc->get()); 
         status = issue(instruction);
         if (status == CONTINUE && brUnit -> empty()) 
            pc->set(pc->get() + 4);
      }
      if (cdb != NULL) 
      {
         updateRStations(cdb);
         GPR::getInstance()->updateRegisterFile(cdb);
         FPR::getInstance()->updateRegisterFile(cdb);
      }

      //free up functional units and reservation stations that are done
      clear();
      if (doDump) dump();
      Clock::clock++;
   }
}

/*
 * done
 * @description: checks whether each unit is empty
 * @returns: true if all reservations in all units are not busy
 */
bool Simulator::done()
{
   return intUnit->empty() && trapUnit->empty();
}

/*
 * clear
 * @description: frees up a functional unit if is no longer needed;
 *               and frees up a reservation station if no longer needed
 */
void Simulator::clear()
{
   //release FU that have computed results
   //release RS whose results have been written
   brUnit->clear();
   memUnit->clear();
   intUnit->clear();
   trapUnit->clear();
   fpUnit->clear();
}

/* 
 * write
 * @description: calls write on each of the units;
 *               if the unit needs to write to the 
 *               cdb and there isn't already a cdb object
 *               then the unit will create and return
 *               one.
 * @returns: CDB object
 */ 
CDB * Simulator::write()
{
   CDB * cdb = NULL;
   //cdb object is passed to each write so that
   //a unit will know whether another unit is already
   //writing to the cdb (and not do so if otherwise)
   //note that traps do not write to cdb; they
   //may write to standard out
//   cdb = memUnit->write(cdb);
   cdb = memUnit->write(cdb);
   cdb = fpUnit->write(cdb);
   cdb = intUnit->write(cdb);
   cdb = trapUnit->write(cdb);
   cdb = brUnit->write(cdb);
   return cdb;
}

/*
 * updateRStations
 * @decription: call each unit to update their
 *              reservation stations based upon
 *              the name/value in the CDB object
 * @param: CDB contains a RSname and a value
 */
void Simulator::updateRStations(CDB * cdb)
{
  brUnit->updateRStations(cdb); 
  intUnit->updateRStations(cdb);
   trapUnit->updateRStations(cdb);
   memUnit->updateRStations(cdb);
   fpUnit->updateRStations(cdb);
}

/*
 * execute
 * @decription: ask each unit to execute
 *              its pending instructions
 */
void Simulator::execute()
{
   intUnit->execute();
   trapUnit->execute();
   fpUnit -> execute();
   memUnit -> execute();
  brUnit -> execute();
}


/*
 * issue
 * @description: ask each unit if the instruction
 *               to be issued should be issued to
 *               that unit; if so call issue to
 *               do the issue.  If no valid unit
 *               return HALT.
 * @param: 32-bit instruction
 * @returns: CONTINUE, HALT, or STALL. HALT is
 *           returned if a halt is issued or the
 *           instruction is undefined.  STALL is
 *           returned if no reservation station
 *           is available.
 */
Status Simulator::issue(uint32_t instruction)
{
   if (intUnit->isUnitInstr(instruction))
   {
      return intUnit->issue(instruction);
   } else if (trapUnit->isUnitInstr(instruction))
   {
      return trapUnit->issue(instruction);
   } else if (memUnit -> isUnitInstr(instruction)) {
      return memUnit->issue(instruction);
   } else if (fpUnit -> isUnitInstr(instruction)) {
      return fpUnit -> issue(instruction);
   } else if (brUnit -> isUnitInstr(instruction)) {
      return brUnit -> issue(instruction);
   }
   return HALT;
}

/*
 * dump
 * @description: call dump on each unit, registers,
 *               and memory to output their current
 *               contents
 */
void Simulator::dump()
{
   std::cout << "End of clock cycle " 
             << std::setw(3) << std::setfill(' ') << std::dec << Clock::clock 
             << std::endl;
   std::cout << "----------------------" << std::endl;

   //dump the contents of the units
   if (!intUnit->empty()) intUnit->dump();
   if (!trapUnit->empty()) trapUnit->dump();
   if (!fpUnit->empty()) fpUnit->dump();
   if (!memUnit->empty()) memUnit->dump();
   if (!brUnit->empty()) brUnit->dump();
   //dump general purpose registers, floating point registers, and memory
   GPR::getInstance()->dump();
   FPR::getInstance()->dump();
   Memory::getInstance()->dump();
}

