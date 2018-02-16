#include <cstdint>
#include <iostream>
#include "PC.h"

PC * PC::pcPtr = NULL;

/* 
 * PC
 * @description: set pc to 0
 */
PC::PC()
{
   pc = 0;
}

/*
 * getInstance
 * @description: create and/or return a singleton PC
 *               object
 */
PC * PC::getInstance()
{
   if (pcPtr == NULL)
      pcPtr = new PC();
   return pcPtr;
}

/*
 * get
 * @description: return the current pc value
 * @returns: pc
 */
int32_t PC::get() { return pc; }

/*
 * set
 * @description: change the current pc value
 * @params: value to store in pc
 */
void PC::set(int32_t val) 
{ 
   pc = val; 
}

