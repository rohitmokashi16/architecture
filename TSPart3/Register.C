#include <iostream>
#include <iomanip>
#include <cstdint>
#include "CDB.h"
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
// get the value in Qi
std::string Register::getQi(int32_t regnum) {
  return Qi[regnum];
}
// set the value in Qi 
void Register::setQi(int32_t regnum, std::string value) {
  Qi[regnum] = value;
}
// returns true if Qi == "" to indicate that reg[regnum] represents a value and were not
// waiting on a reservation station
bool Register::hasValue(int32_t regnum) {
  if(Qi[regnum].compare("") == 0) 
    return true;
  return false;
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

// If we recieve an update cdb object, use that to update registers that are currently waiting
// on that reservation station
void Register::updateRegisterFile(CDB * cdb) {
  for (int i = 0; i < NUMREG; i++) {
    if (Qi[i].compare(cdb->rsName) == 0) {
      reg[i] = cdb->value;
      Qi[i] = "";
    }
  }
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
        std::cout << std::setw(8) << std::setfill(' ') << Qi[i] << " ";
      else
         std::cout << std::setw(8) << std::setfill('0') << std::hex << reg[i] << " ";
   }
   std::cout << "\n";
}      
