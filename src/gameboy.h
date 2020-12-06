#include "cpu.h"

class Gameboy{
    public:
        CPU cpu;
        Memory memory;   

        bool running;

        Gameboy();

        /// "Loads" the ROM into memory at 0x0000
        void LoadIntoMemory(char* filepath);

        void Run();
        void Step();
};
