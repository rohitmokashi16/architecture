
#ifndef MEMORY_H
#define MEMORY_H

#define MEMSIZE 4000
class Memory
{
   private:
      static Memory * memInstance;
      uint32_t mem[MEMSIZE];
      Memory();
      bool isDifferent(uint32_t *, uint32_t *, int32_t);
   public:
      bool loadMemory(std::string fileName);
      static Memory * getInstance();
      void dump();
};
#endif

      
      
