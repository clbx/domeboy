#include "gameboy.h"

Gameboy::Gameboy(){
    memory = Memory();
    cpu = CPU(&memory);
}


void Gameboy::Run(){
    running = true;

    while(running){
        //4.194304 MHz cpu freq
        //59.73hz vsync
        //70,221 cycles per vblank
        int cyclespassed = 0;
        while(cyclespassed > 70221){
            cpu.step();
        }
    }

}

void Gameboy::Step(){
    cpu.step();
}