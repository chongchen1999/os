class TLB:
    def __init__(self, size):
        self.size = size
        self.cache = {}
        self.order = []

    def lookup(self, vpn):
        return self.cache.get(vpn, None)

    def update(self, vpn, pfn):
        if vpn in self.cache:
            self.order.remove(vpn)
        else:
            if len(self.cache) >= self.size:
                oldest = self.order.pop(0)
                del self.cache[oldest]
        self.cache[vpn] = pfn
        self.order.append(vpn)

class MemoryManager:
    def __init__(self):
        self.physical_memory = {}
        self.next_free_page = 0x6000  # Simulating next available physical page

    def allocate_page(self):
        addr = self.next_free_page
        self.next_free_page += 0x1000  # Move to next page
        return addr

    def handle_page_fault(self, current_addr, level):
        new_page = self.allocate_page()
        if current_addr not in self.physical_memory:
            self.physical_memory[current_addr] = [{'present': False}] * 512
        self.physical_memory[current_addr][level] = {'present': True, 'addr': new_page}
        return new_page

def translate(va, cr3, tlb, mem_manager):
    vpn = va >> 12
    pfn = tlb.lookup(vpn)
    if pfn is not None:
        return (pfn << 12) | (va & 0xFFF), True  # TLB hit

    indices = [
        (va >> 39) & 0x1FF,  # PML4 index
        (va >> 30) & 0x1FF,  # PDP index
        (va >> 21) & 0x1FF,  # PD index
        (va >> 12) & 0x1FF   # PT index
    ]
    offset = va & 0xFFF

    current_addr = cr3
    for i, level in enumerate(indices):
        table = mem_manager.physical_memory.get(current_addr, None)
        if not table or not table[level].get('present', False):
            current_addr = mem_manager.handle_page_fault(current_addr, level)
        else:
            current_addr = table[level]['addr']

    pa = (current_addr << 12) | offset
    tlb.update(vpn, current_addr)
    return pa, False  # TLB miss

def main():
    mem_manager = MemoryManager()
    # Preload initial memory structures
    mem_manager.physical_memory = {
        0x1000: [{'present': True, 'addr': 0x2000}] + [{'present': False}] * 511,
        0x2000: [{'present': True, 'addr': 0x3000}] + [{'present': False}] * 511,
        0x3000: [{'present': True, 'addr': 0x4000}] + [{'present': False}] * 511,
        0x4000: [{'present': True, 'addr': 0x5}] + [{'present': False}] * 511,
    }
    CR3 = 0x1000  # PML4 physical address
    tlb = TLB(size=16)

    test_vas = [0x0, 0x123, 0x0, 0x200000, 0x400000]  # Last address will cause a page fault
    for va in test_vas:
        pa, hit = translate(va, CR3, tlb, mem_manager)
        if pa is not None:
            print(f"Virtual 0x{va:016x} → Physical 0x{pa:016x}, TLB hit: {hit}")
        else:
            print(f"Virtual 0x{va:016x} → Page Fault")

if __name__ == "__main__":
    main()