/**
 * \class Memory
 * \ingroup Hardware
 * \author Last Modified by Clay Buxton (clbx). Created by: Clay Buxton (clbx)
 * \date Last Modified 12/4/2020. Created 12/4/2020
 * \brief Memory Handling
 * 
 * Handles all memory writes and reads for the 64KB of RAM
 */

#include <stdint.h>
#include <array>


class Memory {
    public:

        /* 
         * Memory is Set up like this to make it easy to manage for debugging and such.
         * Read and Write Functions handle making sure all memory goes to the correct position
         */
        std::array<uint8_t, 0x4000> ROMBank0;  /// 16KB 0x0000 - 0x3FFF: In Cart, Fixed at Bank 00 
        std::array<uint8_t, 0x4000> ROMBank1;  /// 16KB 0x4000 - 0x7FFF: In Cart, Switchable in CGB
        std::array<uint8_t, 0x2000> VRAM;      ///  8KB 0x8000 - 0x9FFF: VRAM
        std::array<uint8_t, 0x2000> ExternRAM; ///  8KB 0xA000 - 0xBFFF: In Cart, External RAM
        std::array<uint8_t, 0x1000> WorkRAM0;  ///  4KB 0xC000 - 0xCFFF: Work Ram 0 
        std::array<uint8_t, 0x1000> WorkRAM1;  ///  4KB 0xD000 - 0xDFFF: Work Ram 1, Switchable in CGB
        std::array<uint8_t, 0x1E00> EchoRAM;   ///  7KB 0xE000 - 0xFDFF: Mirror of 0xC000 - 0xDDFF, Typically not used
        std::array<uint8_t, 0x00A0> OAMRAM;    /// 160B 0xFE00 - 0xFE9F: Sprite Table
        //96B 0xFEA0 - 0xFEFF: Not Usable      
        std::array<uint8_t, 0x007E> HRAM;      /// 126B 0xFF80 - 0xFFFE: High RAM
        uint8_t IE;                            ///   1B 0xFFFF - 0xFFFF: Interrupts Enable Register


        Memory();
        void reset();
        uint8_t read(uint16_t);
        void write(uint16_t, uint8_t);
};