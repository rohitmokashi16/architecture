class Tools {
  public:
    static uint32_t buildLong(uint8_t * bytes);
    static uint8_t getByte(uint8_t byteNum, uint32_t word);
    static uint32_t setByte(uint8_t byteNum, uint8_t byteVal, uint32_t word);    
};
