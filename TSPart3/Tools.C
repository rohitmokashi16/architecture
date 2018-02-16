#include <cstdlib>
#include <stdint.h>
#include "Tools.h" 

// select bits indexed from right to left out of a 32 bit intger
uint32_t Tools::getBits(uint32_t source, int32_t low, int32_t high)
{
    if (low > 31 || high > 31) {
        return 0;
    }
    if (low > high) {
        return 0;
    }
    source = source << (31 - high);
    source = source >> (31 - high);
    source = source >> low;
    return source;
}


int32_t Tools::signExtend(uint32_t original) {
  int32_t n = original << 16;
  return n >> 16;
}


union m {
  float f;
  uint32_t  b;

} m_u;

float Tools::binToFloat(uint32_t binary) {
  m n;
  n.b = binary;
  return n.f;
}
uint32_t Tools::floatToBin(float value) {
  m n;
  n.f = value;
  return n.b;
}
// select bits indexed from left to right out of a 32 bit unsigned integer
uint32_t Tools::getInstructionBits(uint32_t encoding, int32_t l1, int32_t l2) {
  encoding <<= l1;
  encoding >>= (31 - (l2 - l1));
  return encoding;
}

//select a byte out of a 32 bit unsigned integer
uint8_t Tools::getByte(uint8_t byteNum, uint32_t word) {
  uint8_t result;
  word <<= byteNum * 8;
  word >>= 24;
  result = word;
  return result;
}
// convert a 32 bit unsigned integer to an array of 4 bytes
// note: this puts the array in the heap, so you will need to call delete on the returned pointer
// so as not to cause a memory leak
uint8_t * Tools::getBytes(uint32_t word) {
  uint8_t * bytes = new uint8_t[4];
  bytes[0] = word >> 24;
  bytes[1] = (word << 8) >> 24;
  bytes[2] = (word << 16) >> 24;
  bytes[3] = (word << 24) >> 24;
  return bytes;
}

// change one of the bytes in a 32 bit unsigned integer to the value in byteVal
uint32_t Tools::setByte(uint8_t byteNum, uint8_t byteVal, uint32_t word) {
  uint32_t newVal;
  uint32_t mask = 0xffffffff;
  uint8_t resetBits = 0xff;
  resetBits <<= ((3 - byteNum) * 8);
  mask ^= resetBits;
  newVal = byteVal << ((3-byteNum) * 8);
  return word | newVal;
}

//convert an array of 4 bytes to a 32 bit unsigned integer
uint32_t Tools::buildLong(uint8_t * bytes) {
  uint32_t result = 0;
  for(int i = 0; i < 3; i++) {
    result |= *(bytes + i);
    result <<= 8;
  }
  result |= *(bytes + 3);
  return result;
}

