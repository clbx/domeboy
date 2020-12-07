#include "cpu.h"
#include "imgui_logger.h"



class Gameboy{
    public:
        CPU cpu;
        Memory memory;   

        bool running;

        Logger logger;

        Gameboy(Logger *logger);

        /// "Loads" the ROM into memory at 0x0000
        void LoadIntoMemory(char* filepath);

        void Run();
        void Step();
};
