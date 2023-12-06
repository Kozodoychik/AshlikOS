#include <stdint.h>
#include <string.h>
#include <interrupts.h>
#include <gdt.h>
#include <console.h>
#include <drivers/vga.h>
#include <drivers/io.h>
#include <drivers/serial.h>
#include <drivers/keyboard.h>
#include <drivers/pci.h>
#include <drivers/atapi.h>
#include <mm/memman.h>
#include <fs/iso9660.h>
#include <io/printf.h>

void* nullptr = (void*)0;

void _ignore(){}

void kmain(void* multiboot_struct, uint32_t magic){

	vga_90x30_text_mode_init();
	cls();

	console_init(2);
	console_register(_ignore, putchar);		// VGA
	console_register(_ignore, serial_write);	// Serial
	set_active_console(0);

	serial_init();

	printf("90x30 Text mode\n\r");

	gdt_setup();

	printf("\nAshlikOS Kernel v.1.0\n\n\r");

	uint32_t heap = 10*1024*1024;
	uint32_t* upper_mem = (uint32_t*)(((uint32_t)multiboot_struct)+8);
	mm_init(heap, (*upper_mem)*1024 - heap - 10*1024);
	printf("Heap: %X\n\r", heap);

	asm volatile("cli");
	pic_init();
	idt_setup();

	printf("Initializing keyboard...");

	keyboard_init();

	printf("OK\n\r");

	asm volatile("sti");

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

		char* test = (char*)read_file("/boot/grub/grub.cfg");
		if (test != 0){
			printf(test);
			free(test);
		}
		else printf("File test.txt not found\n\r");
	}

	while(1);

	return;

}
