from collections import OrderedDict

class TLB:
    def __init__(self, size=16):
        """Initialize a TLB with a given number of entries (default: 16)."""
        self.size = size
        self.entries = OrderedDict()  # Keeps track of TLB entries in LRU order

    def lookup(self, virtual_page_number):
        """Check if VPN (Virtual Page Number) exists in TLB."""
        if virtual_page_number in self.entries:
            # Move to the end (LRU update)
            self.entries.move_to_end(virtual_page_number)
            print(f"TLB HIT: VPN {virtual_page_number} -> PPN {self.entries[virtual_page_number]}")
            return self.entries[virtual_page_number]  # Return physical page number
        else:
            print(f"TLB MISS: VPN {virtual_page_number}")
            return None  # TLB miss

    def update(self, virtual_page_number, physical_page_number):
        """Insert a new entry into the TLB, using LRU replacement if needed."""
        if virtual_page_number in self.entries:
            # Update existing entry and move to end
            self.entries.move_to_end(virtual_page_number)
        elif len(self.entries) >= self.size:
            # Evict least recently used (LRU) entry
            evicted_vpn, evicted_ppn = self.entries.popitem(last=False)
            print(f"TLB EVICT: Removing VPN {evicted_vpn} -> PPN {evicted_ppn}")

        # Insert the new entry
        self.entries[virtual_page_number] = physical_page_number
        print(f"TLB UPDATE: VPN {virtual_page_number} -> PPN {physical_page_number}")

class PageTable:
    def __init__(self):
        """Simulated page table mapping virtual pages to physical pages."""
        self.table = {}  # Virtual Page Number (VPN) -> Physical Page Number (PPN)

    def resolve_page_fault(self, virtual_page_number):
        """Simulate handling a page fault by allocating a new physical page."""
        if virtual_page_number not in self.table:
            physical_page_number = len(self.table) + 1  # Simple allocation strategy
            self.table[virtual_page_number] = physical_page_number
            print(f"PAGE TABLE: Mapping VPN {virtual_page_number} -> PPN {physical_page_number}")
        return self.table[virtual_page_number]

# Simulating the TLB System
tlb = TLB(size=4)  # Small TLB with 4 entries
page_table = PageTable()

# Simulated memory access pattern
virtual_addresses = [0x1A, 0x2B, 0x1A, 0x3C, 0x4D, 0x5E, 0x2B, 0x6F]

for va in virtual_addresses:
    vpn = va >> 12  # Extract VPN (assuming 4KB pages, shift by 12 bits)

    # Lookup in TLB
    ppn = tlb.lookup(vpn)

    if ppn is None:  # TLB miss, resolve via page table
        ppn = page_table.resolve_page_fault(vpn)
        tlb.update(vpn, ppn)

print("\nFinal TLB State:", tlb.entries)
