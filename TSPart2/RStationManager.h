#ifndef RSTATIONMANAGER_H
#define RSTATIONMANAGER_H
class RStationManager
{
   private:
      RStation ** RS;
      int32_t RScount;
   public:
      RStationManager(int32_t count, std::string unitName);
      bool addToStation(uint32_t encoding, Instruction instrtype);
      RStation * getRStation(int32_t which);
      int32_t getRScount();
      void dump();
};
#endif   
