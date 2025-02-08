class Segment:
    def __init__(self, name, base, size, grows_positive=True):
        self.name = name
        self.base = base
        self.size = size
        self.grows_positive = grows_positive

    def translate(self, virtual_address):
        if self.grows_positive:
            offset = virtual_address
        else:
            offset = self.size - (virtual_address + 1)

        if 0 <= offset < self.size:
            physical_address = self.base + offset
            return physical_address
        else:
            raise ValueError(f"Segmentation fault in segment {self.name}: Address {virtual_address} out of bounds")

class MemoryManager:
    def __init__(self):
        self.segments = {}

    def add_segment(self, name, base, size, grows_positive=True):
        self.segments[name] = Segment(name, base, size, grows_positive)

    def translate(self, segment_name, virtual_address):
        if segment_name not in self.segments:
            raise ValueError(f"Segment {segment_name} not found")

        segment = self.segments[segment_name]
        return segment.translate(virtual_address)

# Define the memory layout
memory_manager = MemoryManager()

# Add segments: code, heap, and stack
memory_manager.add_segment("code", base=32 * 1024, size=2 * 1024)  # Base: 32 KB, Size: 2 KB
memory_manager.add_segment("heap", base=34 * 1024, size=3 * 1024)  # Base: 34 KB, Size: 3 KB
memory_manager.add_segment("stack", base=28 * 1024, size=2 * 1024, grows_positive=False)  # Base: 28 KB, Size: 2 KB

# Test translations
try:
    print("Translating code segment, virtual address 100:")
    print("Physical address:", memory_manager.translate("code", 100))

    print("\nTranslating heap segment, virtual address 4200:")
    print("Physical address:", memory_manager.translate("heap", 4200 - 4096))  # Offset into heap

    print("\nTranslating stack segment, virtual address 15 KB:")
    print("Physical address:", memory_manager.translate("stack", 15 * 1024 - 14 * 1024))

    print("\nAttempting illegal address in heap segment:")
    print("Physical address:", memory_manager.translate("heap", 7000))  # Out of bounds
except ValueError as e:
    print(e)
