#ifndef CONFIG_H
#define CONFIG_H
class Config
{
   public:
      /* Integer Units */
      static const int32_t intRS = 8;
      static const int32_t intFU = 3;
      static const int32_t intEX = 1;

      /* Trap Unit */
      static const int32_t trapRS = 4;
      static const int32_t trapFU = 1;
      static const int32_t trapEX = 1;

      /* Memory Unit */
      static const int32_t memRS = 8;
      static const int32_t memFU = 2;
      static const int32_t memEX = 2;

      /* Floating Point32_t Units */
      static const int32_t fpRS = 8;
      static const int32_t fpFU = 2;
      static const int32_t fpEX = 4;

      /* Branch unit */
      static const int32_t brRS = 1;
      static const int32_t brFU = 1;
      static const int32_t brEX = 1;
};
#endif
