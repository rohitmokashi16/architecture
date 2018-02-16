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
#include "Memory.h"
#include "Register.h"
#include "GPR.h"
#include "FPR.h"

void printUsageAndQuit();
int main(int argc, char * argv[])
{
   //check to see if the -D option was provided 
   bool dump = false;
   if (argc < 2) printUsageAndQuit();
   if (argc >= 3 && (strcmp(argv[2], "-D") == 0)) dump = true;

   Memory * mem = Memory::getInstance();
   bool loaded = mem->loadMemory(argv[1]);
   if (!loaded) printUsageAndQuit();
   mem->dump();

   GPR * gpr = GPR::getInstance();
   gpr->set(8, 0x11112222u);
   gpr->set(27, 0x23232323u);
   gpr->dump();

   FPR * fpr = FPR::getInstance();
   fpr->set(10, 0x11112222u);
   fpr->set(19, 0x23232323u);
   fpr->dump();
   return 0;
}

/*
 * printUsageAndQuit
 * 
 * called when the user enters errored input to print usage
 * information and terminate the program
*/
void printUsageAndQuit()
{
   std::cout << "Usage: tomasulo <file>.hex [-D]\n";
   std::cout << "<file>.hex contains assembled dlx code.\n";
   std::cout << "-D option causes a cycle by cycle dump of registers, ";
   std::cout << " reservation stations, and memory.\n";
   exit(1);
}

