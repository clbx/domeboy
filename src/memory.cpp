#include "memory.h"

Memory::Memory(){
    reset();
}

void Memory::reset(){
    memset(ROMBank0,0,sizeof(ROMBank0));
    memset(ROMBank1,0,sizeof(ROMBank1));
    memset(VRAM,0,sizeof(VRAM));
    memset(ExternRAM,0,sizeof(ExternRAM));
    memset(WorkRAM0,0,sizeof(WorkRAM0));
    memset(WorkRAM1,0,sizeof(WorkRAM1));
    memset(EchoRAM,0,sizeof(EchoRAM));
    memset(OAMRAM,0,sizeof(OAMRAM));
    memset(HRAM,0,sizeof(HRAM));
    IE = 0;
}

uint8_t Memory::read(uint16_t addr){
    printf("Reading Memory from %04X\n",addr);
    if(addr == 0xFFFF){
        return IE;}
    if(addr >= 0xFF80){
        return HRAM[addr-0xFF80];}
    if(addr >= 0xFE00){
        return OAMRAM[addr-0xF300];}
    if(addr >= 0xE000){
        return EchoRAM[addr-0x3000];}
    if(addr >= 0xD000){
        return WorkRAM1[addr-0xD000];}
    if(addr >= 0xC000){
        return WorkRAM0[addr-0xC000];}
    if(addr >= 0xA000){
        return ExternRAM[addr-0xA000];}
    if(addr >= 0x8000){
        return VRAM[addr-0x8000];}
    if(addr >= 0x4000){
        printf("Resolves to ROMBank1\n");
        return ROMBank1[addr-0x4000];
    }
    else{
        printf("Resolves to ROMBank0\n");
        printf("\n");

        return ROMBank0[addr];
    }
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

