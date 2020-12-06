#include "cpu.h"


CPU::CPU(){
}

CPU::CPU(Memory *memory, Logger *logger){
    this->memory = memory;
    this->logger = *logger;

    //Set all instructions to undefined
    for(int i = 0; i < 256; i++){
        InstructionSet[i] = (Instruction){"UND",0,1,&CPU::Undefined};
    }



}

void CPU::step(){
    uint8_t opcode = memory->read(pc);                          //Fetch
    Instruction currentInstruction = InstructionSet[opcode];   //Decode
    (this->*currentInstruction.op)();                          //Execute
    pc += currentInstruction.bytes;
};



void CPU::Undefined(){
    printf("!!! Unimplemented Opcode [%02X] @ %04X !!!\n",this->memory->read(this->pc),pc);
}



