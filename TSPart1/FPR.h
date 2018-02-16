#ifndef FPR_H
#define FPR_H
class FPR: public Register
{
   private:
      static FPR * fpr;
   public:
      static FPR * getInstance();
      void dump();
};
#endif
