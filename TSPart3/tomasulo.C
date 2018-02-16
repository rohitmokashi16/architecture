/* 
 * Driver for the tomasulo simulator
 * Usage: tomasulo <file>.hex [-D]
 *
 * <file>.hex contains assembled dlx code.
 * If the -D option is provided then the program performs a cycle by cycle dump
 * of registers, reservation stations, and memory.
*/

#include <iostream>
#include <fstream>
#include <string.h>
#include "Instruction.h"
#include "CDB.h"
#include "Status.h"
#include "Memory.h"
#include "Register.h"
#include "GPR.h"
#include "FPR.h"
#include "FUnit.h"
#include "RStation.h"
#include "FUnitManager.h"
#include "RStationManager.h"
#include "FUnitContainer.h"
#include "MemUnitContainer.h" 
#include "IntUnitContainer.h"
#include "TrapUnitContainer.h"
#include "BranchUnitContainer.h"
#include "FpUnitContainer.h" 
#include "Simulator.h"

void printUsageAndQuit();
int32_t main(int32_t argc, char * argv[])
{
   //check to see if the -D option was provided 
   bool dump = false;
   if (argc < 2) printUsageAndQuit();
   if (argc >= 3 && (strcmp(argv[2], "-D") == 0)) dump = true;

   Memory * mem = Memory::getInstance();
   bool loaded = mem->loadMemory(argv[1]);
   if (!loaded) printUsageAndQuit();

   Simulator simulator(dump);
   simulator.issueExecuteWrite(); 
   return 0;
}

void printUsageAndQuit()
{
   std::cout << "Usage: tomasulo <file>.hex [-D]\n";
   std::cout << "<file>.hex contains assembled dlx code.\n";
   std::cout << "-D option causes a cycle by cycle dump of registers, reservation stations, and memory.\n";
   exit(1);
}

