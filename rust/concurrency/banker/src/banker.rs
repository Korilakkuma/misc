use std::sync::{Arc, Mutex};

#[derive(Clone)]
pub struct Banker<const NRES: usize, const NTH: usize> {
    resource: Arc<Mutex<Resource<NRES, NTH>>>,
}

impl<const NRES: usize, const NTH: usize> Banker<NRES, NTH> {
    pub fn new(availables: [usize; NRES], max: [[usize; NRES]; NTH]) -> Self {
        Banker {
            resource: Arc::new(Mutex::new(Resource::new(availables, max))),
        }
    }

    pub fn take(&self, id: usize, resource: usize) -> bool {
        match self.resource.lock() {
            Ok(mut r) => r.take(id, resource),
            Err(error) => panic!("{}", error),
        }
    }

    pub fn release(&self, id: usize, resource: usize) {
        match self.resource.lock() {
            Ok(mut r) => r.release(id, resource),
            Err(error) => panic!("{}", error),
        }
    }
}

struct Resource<const NRES: usize, const NTH: usize> {
    availables: [usize; NRES],
    allocations: [[usize; NRES]; NTH],
    max: [[usize; NRES]; NTH],
}

impl<const NRES: usize, const NTH: usize> Resource<NRES, NTH> {
    fn new(availables: [usize; NRES], max: [[usize; NRES]; NTH]) -> Self {
        Resource {
            availables,
            allocations: [[0; NRES]; NTH],
            max,
        }
    }

    fn is_safe(&self) -> bool {
        let mut ended = [false; NTH];
        let mut work = self.availables.clone();

        loop {
            let mut found = false;
            let mut number_of_trues = 0;

            for (index, allocation) in self.allocations.iter().enumerate() {
                if ended[index] {
                    number_of_trues += 1;
                    continue;
                }

                let need = self.max[index].iter().zip(allocation).map(|(m, a)| m - a);
                let is_available = work.iter().zip(need).all(|(w, n)| *w >= n);

                if is_available {
                    found = true;
                    ended[index] = true;

                    for (w, a) in work.iter_mut().zip(allocation) {
                        *w += *a;
                    }

                    break;
                }
            }

            if number_of_trues == NTH {
                return true;
            }

            if !found {
                break;
            }
        }

        false
    }

    fn take(&mut self, id: usize, resource: usize) -> bool {
        if id >= NTH
            || resource >= NRES
            || self.availables[resource] == 0
            || self.max[id][resource] == self.allocations[id][resource]
        {
            return false;
        }

        self.allocations[id][resource] += 1;
        self.availables[resource] -= 1;

        if self.is_safe() {
            true
        } else {
            self.allocations[id][resource] -= 1;
            self.availables[resource] += 1;
            false
        }
    }

    fn release(&mut self, id: usize, resource: usize) {
        if id >= NTH || resource >= NRES || self.allocations[id][resource] == 0 {
            return;
        }

        self.allocations[id][resource] -= 1;
        self.availables[resource] += 1;
    }
}
