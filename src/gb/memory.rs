pub struct Memory{
    pub writeable: bool,
    data: Vec<i8>,
}

impl Memory{
    pub fn new(writable: bool, size: i16) -> Self{
        let data = Vec::with_capacity(size as usize);
        for i in 0..size{
            data.push(0);
        }
        Memory {
            writeable: false,
            data: data,
        }
    }

    pub fn write(&self, loc: i16, data: i8) -> bool {

        if self.writeable == false {
            println!("Tried writing to read only memory!");
            return false;
        }

        if loc > self.data.len() as i16 {
            println!("Tried writing past memory bounds!");
            return false;
        }

        &mut self.data[loc as usize] = data;

        true

        
    }
}