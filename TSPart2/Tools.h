class Tools {
  public:
    static uint32_t buildLong(uint8_t * bytes);
    static uint32_t getBits(uint32_t source, int32_t low, int32_t high);
    static uint8_t * getBytes(uint32_t word);
    static uint32_t getInstructionBits(uint32_t encoding, int32_t l1, int32_t l2);
    static uint8_t getByte(uint8_t byteNum, uint32_t word);
    static uint32_t setByte(uint8_t byteNum, uint8_t byteVal, uint32_t word);    
};
