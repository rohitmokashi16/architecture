#ifndef PC_H
#define PC_H
class PC
{
   private:
      int32_t pc;
      static PC * pcPtr;
      PC();
   public:
      static PC * getInstance();
      int32_t get();
      void set(int32_t val);
};
#endif
