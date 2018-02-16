
#include <stdint.h>
#include <stdio.h>
#include "Tools.h" 

int main() {
  printf("getInstructionBits(%x, 11, 15) = %x\n", 0x20010003, Tools::getInstructionBits(0x20010003, 11, 15));
}
