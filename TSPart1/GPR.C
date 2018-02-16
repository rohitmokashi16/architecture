#include <cstdint>
#include <iostream>
#include <string>
#include "Register.h"
#include "GPR.h"

GPR * GPR::gpr = NULL;

/*
 * getInstance
 *
 * returns a pointer to the single GPR object
*/
GPR * GPR::getInstance()
{
   if (gpr == NULL)
      gpr = new GPR();
   return gpr;
}

/*
 * dump
 *
 * Outputs the contents of the general purpose registers
*/
void GPR::dump()
{
   std::string headings[] = {"R0-R7:   ", "R8-R15:  ", "R16-R23: ", "R24-R31: "};
   int32_t i, j;

   std::cout << "General Purpose Registers:\n";
   for (j = 0, i = 0; j < 4; j++, i+=8)
   {
      std::cout << headings[j];
      dumpRow(i, 8);
   }
   std::cout << "\n";
}

