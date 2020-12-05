#include "gameboy.h"

Gameboy::Gameboy(){
    memory = Memory();
    cpu = CPU(memory);
}