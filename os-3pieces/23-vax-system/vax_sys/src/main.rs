use std::collections::{HashMap, VecDeque};

const PAGE_SIZE: usize = 512; // VAX/VMS uses small 512-byte pages
const MEMORY_SIZE: usize = 4096; // Simulating a small memory of 4KB
const NUM_PAGES: usize = MEMORY_SIZE / PAGE_SIZE;

struct VirtualMemory {
    page_table: HashMap<usize, usize>, // Virtual Page Number -> Physical Frame Number
    tlb: HashMap<usize, usize>, // Translation Lookaside Buffer (TLB)
    memory: Vec<Option<usize>>, // Simulating physical memory frames
    free_frames: VecDeque<usize>, // List of free frames
}

impl VirtualMemory {
    fn new() -> Self {
        let mut free_frames = VecDeque::new();
        for i in 0..NUM_PAGES {
            free_frames.push_back(i);
        }
        Self {
            page_table: HashMap::new(),
            tlb: HashMap::new(),
            memory: vec![None; NUM_PAGES],
            free_frames,
        }
    }
    
    fn access_memory(&mut self, vpn: usize) {
        if let Some(&pfn) = self.tlb.get(&vpn) {
            println!("TLB hit: VPN {} -> PFN {}", vpn, pfn);
        } else {
            if let Some(&pfn) = self.page_table.get(&vpn) {
                println!("Page Table hit: VPN {} -> PFN {}", vpn, pfn);
                self.tlb.insert(vpn, pfn);
            } else {
                println!("Page Fault: VPN {} not in memory", vpn);
                self.handle_page_fault(vpn);
            }
        }
    }
    
    fn handle_page_fault(&mut self, vpn: usize) {
        if let Some(pfn) = self.free_frames.pop_front() {
            println!("Allocating frame {} to VPN {}", pfn, vpn);
            self.page_table.insert(vpn, pfn);
            self.tlb.insert(vpn, pfn);
            self.memory[pfn] = Some(vpn);
        } else {
            self.replace_page(vpn);
        }
    }
    
    fn replace_page(&mut self, vpn: usize) {
        let victim_pfn = self.memory.iter().enumerate().find(|(_, v)| v.is_some()).map(|(i, _)| i).unwrap();
        let victim_vpn = self.memory[victim_pfn].unwrap();
        println!("Evicting VPN {} from PFN {}", victim_vpn, victim_pfn);
        self.page_table.remove(&victim_vpn);
        self.tlb.remove(&victim_vpn);
        
        println!("Allocating frame {} to VPN {}", victim_pfn, vpn);
        self.page_table.insert(vpn, victim_pfn);
        self.tlb.insert(vpn, victim_pfn);
        self.memory[victim_pfn] = Some(vpn);
    }
}

fn main() {
    let mut vm = VirtualMemory::new();
    vm.access_memory(0);
    vm.access_memory(1);
    vm.access_memory(2);
    vm.access_memory(3);
    vm.access_memory(4); // Triggers replacement
}
