
#ifndef REGISTER_H
#define REGISTER_H

//dlx has 32 general purpose registers and 32 floating point registers
#define NUMREG 32

//parent class for GPR and FPR
class Register
{
   protected:
      uint32_t reg[NUMREG];
      std::string Qi[NUMREG]; 
      void dumpRow(int32_t start, int32_t count);
      Register();
   public:
      void updateRegisterFile(CDB * cdb);
      std::string getQi(int32_t regnum);
      void setQi(int32_t regnum, std::string value);
      bool hasValue(int32_t regnum);
      int32_t get(int32_t regnum);

      void set(int32_t regnum, uint32_t value);
};
#endif
