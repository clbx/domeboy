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

        Memory mem;


    public:

        struct Instruction{
            const char* mnemonic;
            int bytes;
            int cycles;
            void* op; 
        };

        uint16_t pc;  //Program Counter
        uint16_t sp;  //Stack Pointer
        uint8_t st;   //Flag Register

        uint8_t a;
        uint8_t f;

        uint8_t b;
        uint8_t c;
        
        uint8_t d;
        uint8_t e;
        
        uint8_t h;
        uint8_t l;

        CPU();
        CPU(Memory);




};