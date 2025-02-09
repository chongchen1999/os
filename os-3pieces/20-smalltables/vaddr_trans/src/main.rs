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
    next_free_page: u64,
}

impl MemoryManager {
    fn new() -> Self {
        MemoryManager {
            physical_memory: HashMap::new(),
            next_free_page: 0x6000,
        }
    }

    fn allocate_page(&mut self) -> u64 {
        let addr = self.next_free_page;
        self.next_free_page += 0x1000;
        addr
    }

    fn handle_page_fault(&mut self, current_addr: u64, level: usize) -> u64 {
        let new_page = self.allocate_page();
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

    // [63, 48] : 16-bit, 
    // [47, 39] : 9-bit, VPN[3]
    // [38, 30] : 9-bit, VPN[2]
    // [29, 21] : 9-bit, VPN[1]
    // [20, 12] : 9-bit, VPN[0]
    // [11, 0]  : 12-bit, offset

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
    let mut mem_manager = MemoryManager::new();
    let mut tlb = TLB::new(16);
    let cr3 = 0x1000;

    mem_manager.physical_memory.insert(
        0x1000,
        vec![Some((true, 0x2000))]
            .into_iter()
            .chain(vec![None; 511])
            .collect(),
    );
    mem_manager.physical_memory.insert(
        0x2000,
        vec![Some((true, 0x3000))]
            .into_iter()
            .chain(vec![None; 511])
            .collect(),
    );
    mem_manager.physical_memory.insert(
        0x3000,
        vec![Some((true, 0x4000))]
            .into_iter()
            .chain(vec![None; 511])
            .collect(),
    );
    mem_manager.physical_memory.insert(
        0x4000,
        vec![Some((true, 0x5000))]
            .into_iter()
            .chain(vec![None; 511])
            .collect(),
    );

    let test_vas = [0x0, 0x123, 0x0, 0x200000, 0x400000];
    for &va in &test_vas {
        let (pa, hit) = translate(va, cr3, &mut tlb, &mut mem_manager);
        println!(
            "Virtual 0x{:016x} -> Physical 0x{:016x}, TLB hit: {}",
            va, pa, hit
        );
    }
}
