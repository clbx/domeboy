mod cpu;
mod memory;

pub struct GB {
    pub cpu: cpu::CPU,
    pub sram: memory::Memory,
    pub vram: memory::Memory,
    pub bootstrap: memory::Memory,
    //pub gpu: GPU,
    //pub framebuffer: vec<u8>,
}
 
impl GB {
    pub fn new() -> Self {
        GB {
            cpu: cpu::CPU::new(),
            sram: memory::Memory::new(true, 8*1024),
            vram: memory::Memory::new(true, 8*1024),
            bootstrap: memory::Memory::new(false, 256)
        }
    }
}