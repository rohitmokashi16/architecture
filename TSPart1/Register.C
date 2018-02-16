#include <iostream>
#include <iomanip>
#include <cstdint>
#include "Register.h"

/*
 * Register
 *
 * Register constructor; initializes the registers to 0 and
 * the Qi fields to empty strings.
*/
Register::Register()
{
   for (int32_t i = 0; i < NUMREG; i++)
   {
      reg[i] = 0;
      Qi[i] = "";
   }

}

/*
 * set
 *
 * set register regnum to value
 *
 * @param: regnum - number of the register
 * @param: value - value to be stored in the register
*/
void Register::set(int32_t regnum, uint32_t value)
{
  reg[regnum]=value;
}

/*
 * get
 *
 * get the value of register regnum
 *
 * @param: regnum - number of the register
 * @returns: value of register regnum 
*/
int32_t Register::get(int32_t regnum)
{
  return reg[regnum];
}

/*
 * dumpRow
 *
 * dumps the values of count registers starting with register
 * number start; if a functional unit is producing the value
 * then the Qi value is displayed instead.
 *
 * @param: start - starting register number
 * @param: count - number of registers to display
*/
void Register::dumpRow(int32_t start, int32_t count)
{
   int32_t i, k;
   i = start;
   for (k = 0; k < count; k++, i++)
   {
      if (!Qi[i].empty())
         std::cout << std::setw(8) << Qi[i] << " ";
      else
         std::cout << std::setw(8) << std::setfill('0') << std::hex << reg[i] << " ";
   }
   std::cout << "\n";
}      
