use std::cell::UnsafeCell;
use std::collections::{HashMap, HashSet};
use std::ops::Drop;
use std::sync::atomic::{fence, AtomicU64, Ordering};

const SIZE_OF_STRIPE: usize = 8; // u64, 8 Bytes (It is require to be 2^{n}

const MEMORY_SIZE: usize = 512; // 51s Bytes

pub struct Memory {
    mem: Vec<u8>,
    lock_ver: Vec<AtomicU64>,
    global_clock: AtomicU64,
    shift_size: u32,
}

impl Memory {
    pub fn new() -> Self {
        let mem = [0].repeat(MEMORY_SIZE);

        let shift_size = SIZE_OF_STRIPE.trailing_zeros();

        let mut lock_ver = vec![];

        for _ in 0..MEMORY_SIZE >> shift_size {
            lock_ver.push(AtomicU64::new(0));
        }

        Memory {
            mem,
            lock_ver,
            global_clock: AtomicU64::new(0),
            shift_size,
        }
    }

    fn inc_global_clock(&mut self) -> u64 {
        self.global_clock.fetch_add(1, Ordering::AcqRel)
    }

    fn get_addr_ver(&self, addr: usize) -> u64 {
        let index = addr >> self.shift_size;
        let n = self.lock_ver[index].load(Ordering::Relaxed);

        n & !(1 << 63)
    }

    fn test_not_modify(&self, addr: usize, rv: u64) -> bool {
        let index = addr >> self.shift_size;
        let n = self.lock_ver[index].load(Ordering::Relaxed);

        n <= rv
    }

    fn lock_addr(&mut self, addr: usize) -> bool {
        let index = addr >> self.shift_size;

        match self.lock_ver[index].fetch_update(Ordering::Relaxed, Ordering::Relaxed, |val| {
            let n = val & (1 << 63);

            if n == 0 {
                Some(val | (1 << 63))
            } else {
                None
            }
        }) {
            Ok(_) => true,
            Err(_) => false,
        }
    }

    fn unlock_addr(&mut self, addr: usize) {
        let index = addr >> self.shift_size;

        self.lock_ver[index].fetch_and(!(1 << 63), Ordering::Relaxed);
    }
}

pub struct ReadTrans<'a> {
    read_ver: u64,
    is_abort: bool,
    mem: &'a Memory,
}

impl<'a> ReadTrans<'a> {
    fn new(mem: &'a Memory) -> Self {
        ReadTrans {
            read_ver: mem.global_clock.load(Ordering::Acquire),
            is_abort: false,
            mem,
        }
    }

    pub fn load(&mut self, addr: usize) -> Option<[u8; SIZE_OF_STRIPE]> {
        if self.is_abort {
            return None;
        }

        assert_eq!(addr & (SIZE_OF_STRIPE - 1), 0);

        if !self.mem.test_not_modify(addr, self.read_ver) {
            self.is_abort = true;
            return None;
        }

        fence(Ordering::Acquire);

        let mut mem = [0; SIZE_OF_STRIPE];

        for (dest, src) in mem
            .iter_mut()
            .zip(self.mem.mem[addr..addr + SIZE_OF_STRIPE].iter())
        {
            *dest = *src;
        }

        fence(Ordering::SeqCst);

        if !self.mem.test_not_modify(addr, self.read_ver) {
            self.is_abort = true;
            return None;
        }

        Some(mem)
    }
}

pub struct WriteTrans<'a> {
    read_ver: u64,
    read_set: HashSet<usize>,
    write_set: HashMap<usize, [u8; SIZE_OF_STRIPE]>,
    locked: Vec<usize>,
    is_abort: bool,
    mem: &'a mut Memory,
}

impl<'a> WriteTrans<'a> {
    fn new(mem: &'a mut Memory) -> Self {
        WriteTrans {
            read_set: HashSet::new(),
            write_set: HashMap::new(),
            locked: Vec::new(),
            is_abort: false,
            read_ver: mem.global_clock.load(Ordering::Acquire),
            mem,
        }
    }

    pub fn store(&mut self, addr: usize, val: [u8; SIZE_OF_STRIPE]) {
        assert_eq!(addr & (SIZE_OF_STRIPE - 1), 0);

        self.write_set.insert(addr, val);
    }

    pub fn load(&mut self, addr: usize) -> Option<[u8; SIZE_OF_STRIPE]> {
        if self.is_abort {
            return None;
        }

        assert_eq!(addr & (SIZE_OF_STRIPE - 1), 0);

        self.read_set.insert(addr);

        if let Some(m) = self.write_set.get(&addr) {
            return Some(*m);
        }

        if !self.mem.test_not_modify(addr, self.read_ver) {
            self.is_abort = true;
            return None;
        }

        fence(Ordering::Acquire);

        let mut mem = [0; SIZE_OF_STRIPE];

        for (dest, src) in mem
            .iter_mut()
            .zip(self.mem.mem[addr..addr + SIZE_OF_STRIPE].iter())
        {
            *dest = *src;
        }

        fence(Ordering::SeqCst);

        if !self.mem.test_not_modify(addr, self.read_ver) {
            self.is_abort = true;
            return None;
        }

        Some(mem)
    }

    fn lock_write_set(&mut self) -> bool {
        for (addr, _) in self.write_set.iter() {
            if self.mem.lock_addr(*addr) {
                self.locked.push(*addr);
            } else {
                return false;
            }
        }

        true
    }

    fn validate_read_set(&self) -> bool {
        for addr in self.read_set.iter() {
            if self.write_set.contains_key(addr) {
                let ver = self.mem.get_addr_ver(*addr);

                if ver > self.read_ver {
                    return false;
                }
            } else {
                if !self.mem.test_not_modify(*addr, self.read_ver) {
                    return false;
                }
            }
        }

        true
    }

    fn commit(&mut self, ver: u64) {
        for (addr, val) in self.write_set.iter() {
            let addr = *addr as usize;

            for (dest, src) in self.mem.mem[addr..addr + SIZE_OF_STRIPE]
                .iter_mut()
                .zip(val)
            {
                *dest = *src
            }
        }

        fence(Ordering::Release);

        for (addr, _) in self.write_set.iter() {
            let index = addr >> self.mem.shift_size;

            self.mem.lock_ver[index].store(ver, Ordering::Relaxed);
        }

        self.locked.clear();
    }
}

impl<'a> Drop for WriteTrans<'a> {
    fn drop(&mut self) {
        for addr in self.locked.iter() {
            self.mem.unlock_addr(*addr);
        }
    }
}

pub enum STMResult<T> {
    Ok(T),
    Retry,
    Abort,
}
