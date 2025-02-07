class AddressTranslator:
    def __init__(self, base, bounds):
        self.base = base
        self.bounds = bounds

    def translate(self, virtual_address):
        """Translate a virtual address to a physical address."""
        if 0 <= virtual_address < self.bounds:
            physical_address = virtual_address + self.base
            return physical_address
        else:
            return None

def main():
    print("\nWelcome to the Address Translation Simulator!")

    # Input base and bounds registers
    base = int(input("Enter the base register (starting physical address): "))
    bounds = int(input("Enter the bounds register (size of the address space): "))

    translator = AddressTranslator(base, bounds)

    while True:
        try:
            # Input a virtual address to translate
            virtual_address = int(input("\nEnter a virtual address to translate (-1 to exit): "))

            if virtual_address == -1:
                print("Exiting the simulator. Goodbye!")
                break

            # Perform address translation
            physical_address = translator.translate(virtual_address)

            if physical_address is not None:
                print(f"Virtual Address {virtual_address} -> Physical Address {physical_address}")
            else:
                print(f"Virtual Address {virtual_address} is OUT OF BOUNDS.")

        except ValueError:
            print("Invalid input. Please enter an integer value.")

if __name__ == "__main__":
    main()
