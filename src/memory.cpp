#include "memory.h"

Memory::Memory(){
    reset();
}

void Memory::reset(){
    ROMBank0.fill(0x00);  
    ROMBank1.fill(0x00);  
    VRAM.fill(0x00);;    
    ExternRAM.fill(0x00);
    WorkRAM0.fill(0x00); 
    WorkRAM1.fill(0x00); 
    EchoRAM.fill(0x00);  
    OAMRAM.fill(0x00);   
    HRAM.fill(0x00);    
    IE = 0x00;                   
}

uint8_t Memory::read(uint16_t addr){
    if(addr == 0xFFFF){return IE;}
    if(addr >= 0xFF80){return HRAM[addr-0xFF80];}
    if(addr >= 0xFE00){return OAMRAM[addr-0xF300];}
    if(addr >= 0xE000){return EchoRAM[addr-0x3000];}
    if(addr >= 0xD000){return WorkRAM1[addr-0xD000];}
    if(addr >= 0xC000){return WorkRAM0[addr-0xC000];}
    if(addr >= 0xA000){return ExternRAM[addr-0xA000];}
    if(addr >= 0x8000){return VRAM[addr-0x8000];}
    if(addr >= 0x4000){return ROMBank1[addr-0x4000];}
    else{return ROMBank0[addr];}
}

void Memory::write(uint16_t addr, uint8_t data){
    if(addr == 0xFFFF){IE = data;}
    if(addr >= 0xFF80){HRAM[addr-0xFF80] = data;}
    if(addr >= 0xFE00){OAMRAM[addr-0xF300] = data;}
    if(addr >= 0xE000){EchoRAM[addr-0x3000] = data;}
    if(addr >= 0xD000){WorkRAM1[addr-0xD000] = data;}
    if(addr >= 0xC000){WorkRAM0[addr-0xC000] = data;}
    if(addr >= 0xA000){ExternRAM[addr-0xA000] = data;}
    if(addr >= 0x8000){VRAM[addr-0x8000] = data;}
    if(addr >= 0x4000){ROMBank1[addr-0x4000] = data;}
    else{ROMBank0[addr] = data;}  

    return;
}

