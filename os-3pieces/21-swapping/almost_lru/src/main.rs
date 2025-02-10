use rand::Rng;
use std::collections::{HashMap, VecDeque};

struct TLB {
    size: usize,
    cache: HashMap<u64, u64>,
    order: VecDeque<u64>,
}

impl TLB {
    fn new(size: usize) -> Self {
        TLB {
            size,
            cache: HashMap::new(),
            order: VecDeque::new(),
        }
    }

    fn lookup(&self, vpn: u64) -> Option<u64> {
        self.cache.get(&vpn).copied()
    }

    fn update(&mut self, vpn: u64, pfn: u64) {
        if self.cache.contains_key(&vpn) {
            self.order.retain(|&v| v != vpn);
        } else if self.cache.len() >= self.size {
            if let Some(oldest) = self.order.pop_front() {
                self.cache.remove(&oldest);
            }
        }
        self.cache.insert(vpn, pfn);
        self.order.push_back(vpn);
    }
}

struct MemoryManager {
    physical_memory: HashMap<u64, Vec<Option<(bool, u64)>>>,
    swap_space: HashMap<u64, u64>,
    active_list: VecDeque<u64>,
    inactive_list: VecDeque<u64>,
    next_free_page: u64,
    max_physical_pages: usize,
}

impl MemoryManager {
    fn new(max_physical_pages: usize) -> Self {
        MemoryManager {
            physical_memory: HashMap::new(),
            swap_space: HashMap::new(),
            active_list: VecDeque::new(),
            inactive_list: VecDeque::new(),
            next_free_page: 0x6000,
            max_physical_pages,
        }
    }

    fn allocate_page(&mut self) -> u64 {
        if self.active_list.len() + self.inactive_list.len() >= self.max_physical_pages {
            self.replace_page();
        }
        let addr = self.next_free_page;
        self.next_free_page += 0x1000;
        self.active_list.push_back(addr);
        addr
    }

    fn replace_page(&mut self) {
        if let Some(victim) = self.inactive_list.pop_front() {
            println!("Swapping out page: 0x{:x}", victim);
            self.swap_space.insert(victim, victim);
            self.physical_memory.remove(&victim);
        } else if let Some(active_page) = self.active_list.pop_front() {
            println!("Moving page 0x{:x} to inactive list", active_page);
            self.inactive_list.push_back(active_page);
        }
    }

    fn swap_in(&mut self, vpn: u64) -> u64 {
        if let Some(swapped_page) = self.swap_space.remove(&vpn) {
            println!("Swapping in page: 0x{:x}", swapped_page);
            self.active_list.push_back(swapped_page);
            return swapped_page;
        }
        self.allocate_page()
    }

    fn handle_page_fault(&mut self, current_addr: u64, level: usize) -> u64 {
        let new_page = self.swap_in(current_addr);
        self.physical_memory
            .entry(current_addr)
            .or_insert_with(|| vec![None; 512])[level] = Some((true, new_page));
        new_page
    }
}

fn translate(va: u64, cr3: u64, tlb: &mut TLB, mem_manager: &mut MemoryManager) -> (u64, bool) {
    let vpn = va >> 12;
    if let Some(pfn) = tlb.lookup(vpn) {
        return ((pfn << 12) | (va & 0xFFF), true);
    }

    let indices = [
        (va >> 39) & 0x1FF,
        (va >> 30) & 0x1FF,
        (va >> 21) & 0x1FF,
        (va >> 12) & 0x1FF,
    ];
    let offset = va & 0xFFF;

    let mut current_addr = cr3;
    for (_i, &level) in indices.iter().enumerate() {
        let table = mem_manager.physical_memory.get(&current_addr);
        if table.is_none() || table.unwrap()[level as usize].is_none() {
            current_addr = mem_manager.handle_page_fault(current_addr, level as usize);
        } else {
            current_addr = table.unwrap()[level as usize].unwrap().1;
        }
    }

    let pfn = current_addr;
    let pa = (pfn << 12) | offset;
    tlb.update(vpn, pfn);
    (pa, false)
}

fn main() {
    let mut mem_manager = MemoryManager::new(16);
    let mut tlb = TLB::new(16);
    let cr3 = 0x1000;

    let mut rng = rand::thread_rng();
    let random_vas: Vec<u64> = (0..100)
        .map(|_| rng.gen_range(0..0xFFFFFFFFFFFF))
        .collect();

    for va in random_vas {
        let (pa, hit) = translate(va, cr3, &mut tlb, &mut mem_manager);
        println!(
            "Virtual 0x{:016x} -> Physical 0x{:016x}, TLB hit: {}",
            va, pa, hit
        );
    }
}
