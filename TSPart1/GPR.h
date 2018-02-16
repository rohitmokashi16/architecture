#ifndef GPR_H
#define GPR_H
class GPR: public Register
{
   private:
      static GPR * gpr;
   public:
      static GPR * getInstance();
      void dump();
};
#endif
