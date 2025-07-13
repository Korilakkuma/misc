use std::cell::UnsafeCell;
use std::collections::{HashMap, HashSet};
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
