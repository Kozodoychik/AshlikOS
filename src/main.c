#include <stdint.h>
#include "../incl/vga.h"
#include "../incl/string.h"
#include "../incl/interrupts.h"
#include "../incl/gdt.h"
#include "../incl/io.h"
#include "../incl/serial.h"
#include "../incl/keyboard.h"
#include "../incl/pci.h"

void kmain(){

	cls();

	gdt_setup();

	printf("AshlikOS Kernel v.1.0\n\r");

	asm volatile("cli");
	pic_init();
	idt_setup();

	printf("Initializing keyboard...");

	keyboard_init();

	printf("OK\n\r");

	asm volatile("sti");


	printf("Initializing serial...");

	serial_init();
	serial_print("AshlikOS Kernel v.1.0 COM1\n\r");

	printf("OK\n\r");

	for (int bus=0;bus<8;bus++){
		for (int device=0;device<32;device++){
			int funcs = pci_device_has_funcs(bus, device) ? 8 : 1;
			for (int func=0;func<funcs;func++){
				pci_device_desc d = pci_get_device_descriptor(bus, device, func);
				if (d.vendor_id == 0 || d.vendor_id == 0xffff)
					break;
				printf("PCI: bus=%X  device=%X  function=%X  vendor_id=%X  device_id=%X\n\r", bus, device, func,
					d.vendor_id, d.device_id);
			}
		}
	}
	//printf("%X, %X, %X, %X\n\r", 0x20, 0x1234, 0x21, 0x55AAFF);

	while(1);

	return;

}
