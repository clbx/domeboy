/**
 * Gameboy runs off the Z80 processor
 */

pub struct CPU {

    pub sp: u16,
    pub pc: u16,
    pub flag: u8,

    pub a: u8,
    pub b: u8,
    pub c: u8,
    pub d: u8,
    pub e: u8,
    pub h: u8,
    pub l: u8,


}

impl CPU{
    pub fn new() -> Self{
        CPU {
            sp: 0,
            pc: 0,
            flag: 0,
            a: 0,
            b: 0,
            c: 0,
            d: 0,
            e: 0,
            h: 0,
            l: 0,
        }
    }


    pub fn exec(op: u8){
            
    }


}

