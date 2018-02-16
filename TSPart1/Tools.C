#include <cstdlib>
#include <stdint.h>
#include "Tools.h" 


uint8_t getByte(uint8_t byteNum, uint32_t word) {
  uint8_t result;
  word <<= byteNum * 8;
  word >>= 24;
  result = word;
  return result;
}

uint32_t Tools::setByte(uint8_t byteNum, uint8_t byteVal, uint32_t word) {
  uint32_t newVal;
  uint32_t mask = 0xffffffff;
  uint8_t resetBits = 0xff;
  resetBits <<= ((3 - byteNum) * 8);
  mask ^= resetBits;
  newVal = byteVal << ((3-byteNum) * 8);
  return word | newVal;
}

uint32_t Tools::buildLong(uint8_t * bytes) {
  uint32_t result = 0;
  for(int i = 0; i < 3; i++) {
    result |= *(bytes + i);
    result <<= 8;
  }
  result |= *(bytes + 3);
  return result;
}

