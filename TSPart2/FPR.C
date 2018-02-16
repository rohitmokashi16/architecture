#include <cstdint>
#include <iostream>
#include <string>
#include "CDB.h"
#include "Register.h"
#include "FPR.h"

FPR * FPR::fpr = NULL;

/*
 * getInstance
 *
 * returns a pointer to the single FPR object
*/
FPR * FPR::getInstance()
{
   if (fpr == NULL)
      fpr = new FPR();
   return fpr;
}

/*
 * dump
 *
 * Outputs the contents of the floating point registers
*/
void FPR::dump()
{
   std::string headings[] = {"F0-F7:   ", "F8-F15:  ", "F16-R23: ", "F24-F31: "};
   int32_t i, j;

   std::cout << "Floating Point Registers:\n";
   for (j = 0, i = 0; j < 4; j++, i+=8)
   {
      std::cout << headings[j];
      dumpRow(i, 8);
   }
   std::cout << std::endl;
}


