#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <cctype>
#include "Tools.h"
#include "Memory.h"

Memory * Memory::memInstance = NULL;

/** 
 * Memory constructor
 * initializes the mem array to 0
 */
Memory::Memory()
{
   for (int32_t i = 0; i < MEMSIZE; i++) mem[i] = 0;
}

/**
 * getInstance
 * if memInstance is null then creates a Memory object
 * and sets memInstance to point it; returns memInstance
 */
Memory * Memory::getInstance()
{
   if (memInstance == NULL)
   {
      memInstance = new Memory();
   }
   return memInstance;
}

/*
 * loadMemory
 *
 * Checks the name of the file passed to the method. It
 * should end with a .hex suffix.  Then, opens up the file,
 * reads the contents, and loads the contents into memory. 
 * 
 * @param: fileName - name of the file containing dlx hex.
 *         should end with a .hex suffix
 * @returns: true if the file successfully loaded
*/
bool Memory::loadMemory(std::string fileName)
{
  std::string extension = fileName.substr(fileName.length() - 3, 3);
  if(extension.compare("hex") != 0 && extension.compare("HEX") != 0) {
    std::cout << "Pass a .hex file to run in the simulator";
    return false;
  }
  std::ifstream file(fileName.c_str());
  std::string line; 
 
  //set up an array of 8 bit int's to build a 32 bit word
  uint8_t bytes[4] = {0,0,0,0};
  while(std::getline(file, line)) {
      
    
      std::string addressString = line.substr(0, 8);
      //get the address of the line in the file
      uint32_t address = std::stoul(addressString, 0, 16);
      //the position of the first byte on the line eg 00000000: 00
      uint32_t byteStart = 10;
      //if we're not QPU aligned, set the byteNum to the first position we should be at 
      uint8_t byteNum = address % 4;
      //calculate the position in the mem array
      address /= 4;
      while(true) {
        std::string byteString = line.substr(byteStart, 2);
        // we're at the end of the line, store whateer we have at address
        if(byteString.compare("\t#") == 0 || byteString.compare("  ") == 0) {
          mem[address] = Tools::buildLong(bytes);
          break;
        } 
        // we've finished building a 32 bit word. Store it in memory
        else if(byteNum == 3) {
          uint8_t byte = std::stoul(byteString, 0, 16);
          bytes[3] = byte;
          mem[address] = Tools::buildLong(bytes);
          // reset the contents of the words byte array
          for(int i = 0; i < 4; i++) {
            bytes[i] = 0;
          }
          //increment the position in the array and reset the position in the word
          address++; byteNum =0;
        } else {
          // we're not done building a 32 bit word, take the byte and position it in the array
          uint8_t byte = std::stoul(byteString, 0, 16);
          bytes[byteNum] = byte;
          byteNum++;
        }
        //move the cursor up the line
        byteStart+=2;
      }
  }
  return true;
}


/**
 * dump
 * Output the contents of memory (mem array), eight 32-bit words per line.
 * Rather than output memory that contains a lot of 0s, it outputs
 * a * after a line to indicate that the values in memory up to the next
 * line displayed are identical.
 */
void Memory::dump()
{
   uint32_t prevLine[8] = {0, 0, 0, 0, 0, 0, 0, 0};
   uint32_t currLine[8] = {0, 0, 0, 0, 0, 0, 0, 0};
   int32_t i;
   bool star = false;

   std::cout << "Memory:";
   for (i = 0; i < MEMSIZE; i+=8)
   {
      //get the values for the current line
      for (int32_t j = 0; j < 8; j++) currLine[j] = mem[i + j];

      //if they are the same as the values in the previous line then
      //don't display them, but always display the first line
      if (i == 0 || isDifferent(prevLine, currLine, 8))
      {
         std::cout << std::endl << std::setw(4) << std::setfill('0') << std::hex << i*4 << ": "; 
         for (int32_t j = 0; j < 8; j++) 
             std::cout << std::setw(8) << std::setfill('0') << currLine[j] << " ";
         star = false;
      } else
      {
         //if this line is exactly like the previous line then
         //just print a * if one hasn't been printed already
         if (star == false)
         {
            std::cout << "*";
         } 
         star = true;
      }
      for (int32_t j = 0; j < 8; j++) prevLine[j] = currLine[j];
   }
   std::cout << std::endl << std::endl;
}

/*
 * isDifferent
 *
 * Takes two arrays of unsigned ints and returns true if the two arrays
 * are different.  This function is used by dump to determine whether the
 * array of ints should be output.
 *
 * @param: prevLine - the previous array of unsigned ints that was displayed
 * @param: currLine - the next array of unsigned ints to be displayed if
 *                    different
 * @param: len - number of array elements
 *
*/
bool Memory::isDifferent(uint32_t * prevLine, uint32_t * currLine, int32_t len)
{
   for (int32_t i = 0; i < len; i++)
      if (prevLine[i] != currLine[i]) return true;
   return false;
}
