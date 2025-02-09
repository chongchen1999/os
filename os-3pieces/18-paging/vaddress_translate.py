import random

# Constants defining our simulated system
PAGE_SIZE = 4096  # 4 KB pages
ENTRIES_PER_TABLE = 512  # Number of entries in each page table (like x86_64)

class PageTable:
    def __init__(self):
        self.entries = [None] * ENTRIES_PER_TABLE

    def allocate_entry(self, index):
        if self.entries[index] is None:
            self.entries[index] = PageTable() if index < 3 else random.randint(0x100000, 0xFFFFFFF)  # Last level maps to physical
        return self.entries[index]

def translate_virtual_address(root_table, virtual_address):
    """Simulates 4-level page table translation."""
    offsets = [
        (virtual_address >> 39) & 0x1FF,  # Level 1 index
        (virtual_address >> 30) & 0x1FF,  # Level 2 index
        (virtual_address >> 21) & 0x1FF,  # Level 3 index
        (virtual_address >> 12) & 0x1FF   # Level 4 index
    ]
    page_offset = virtual_address & 0xFFF  # Offset within the page

    table = root_table
    for level in range(4):
        entry = table.allocate_entry(offsets[level])
        if isinstance(entry, int):  # Last level: found physical address
            return entry + page_offset
        table = entry
    return None  # Should not reach here

# Initialize root page table
root_page_table = PageTable()

# Example: Translate a random virtual address
virtual_address = random.randint(0x000000000000, 0x0000FFFFFFFFF)  # 48-bit virtual address
physical_address = translate_virtual_address(root_page_table, virtual_address)

print(f"Virtual Address: 0x{virtual_address:X}")
print(f"Translated Physical Address: 0x{physical_address:X}")
