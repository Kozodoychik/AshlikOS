#include <stdint.h>
#include "../incl/string.h"
#include "../incl/interrupts.h"
#include "../incl/gdt.h"
#include "../incl/drivers/vga.h"
#include "../incl/drivers/io.h"
#include "../incl/drivers/serial.h"
#include "../incl/drivers/keyboard.h"
#include "../incl/drivers/pci.h"
#include "../incl/drivers/atapi.h"

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
					continue;
				printf("PCI: bus=%X  device=%X  function=%X  vendor_id=%X  device_id=%X\n\r", bus, device, func,
					d.vendor_id, d.device_id);
			}
		}
	}

	uint16_t atapi_base = atapi_detect();

	if (atapi_base != 1) {
		printf("ATAPI I/O port base: %X\n\r", atapi_base);
		int status = atapi_read(0x10, 1, (uint16_t*)0x500000);
		if (status==1) printf("atapi_read error\n\r");
		else {
			printf((char*)0x500000);
		}
	}

	while(1);

	return;

}
