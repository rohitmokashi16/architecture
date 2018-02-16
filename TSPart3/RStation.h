#ifndef RSTATION_H
#define RSTATION_H
class RStation
{

   public:
      //all of these fields are public for ease of access by
      //the functional unit container
      std::string name;      //name of reservation station
      bool busy;             //is station holding an operationg
      std::string opcode;    //type of operating
      int32_t Vj;            //value of operand
      int32_t Vk;            //value of operand
      std::string Qj;        //name of reservation station producing Vj
      std::string Qk;        //name of reservation station producing Vk
      int32_t A;             //used to hold immediate field or eff address
      int32_t result;        //used to hold result
      bool resultReady;      //flag indicating result is ready to be written
      bool resultWritten;    //flag indicating the result has been written
      FUnit * fu;            //Functional unit executing this instruction
      int32_t ExStart;       //When does execution start?
      int32_t ExEnd;         //When does execution end?
      bool hideEnd;
      RStation(std::string name);
      void fillStation(uint32_t encoding, Instruction instrtype);
      void fillRegisterRS(uint32_t encoding, Instruction instrtype);
      void fillVjQj(uint32_t encoding, Instruction instrtype);
      void fillVkQk(uint32_t encoding, Instruction instrtype);
      void fillImmediate(uint32_t encoding, Instruction instrtype);
      void clear();
      void dump();
      void setDestinationRegister(int32_t number, int32_t type);
};
#endif
