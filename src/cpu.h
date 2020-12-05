/**
 * \class CPU
 * \ingroup hardware
 * \author Last Modified by Clay Buxton (clbx). Created by: Clay Buxton (clbx)
 * \date Last Modified 12/4/2020. Created 12/4/2020
 * \brief CPU emulation of the Gameboy's Sharp LR35902
 * 
 * This class contains all of the instruction handling of the CPU
 */



#include <stdint.h>
#include "memory.h"

class CPU {
    private:
        uint8_t r[8]; //General Registers
        uint16_t pc;  //Program Counter
        uint16_t sp;  //Stack Pointer
        uint8_t st;   //Flag Register

        Memory mem;


    public:

        struct Instruction{
            const char* mnemonic;
            int bytes;
            int cycles;
            void* op; 
        };

        CPU();
        CPU(Memory);




};