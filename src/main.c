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
#include <fonts/psf.h>
#include <fs/iso9660.h>
#include <io/printf.h>

void* nullptr = (void*)0;

void _ignore(){}

void kmain(void* multiboot_struct, uint32_t magic){

	uint32_t* framebuffer_addr = (uint32_t*)(((uint32_t)multiboot_struct)+88);
	uint8_t* depth = (uint8_t*)(((uint32_t)multiboot_struct)+88);

	uint32_t heap = 10*1024*1024;
	uint32_t* upper_mem = (uint32_t*)(((uint32_t)multiboot_struct)+8);
	mm_init(heap, (*upper_mem)*1024 - heap - 10*1024);

	console_init(2);
	console_register(_ignore, vga_putchar);		// VGA
	console_register(_ignore, serial_write);	// Serial
	set_active_console(1);

	serial_init();

	gdt_setup();

	vga_init(*framebuffer_addr, 640, 480, 4);

	asm volatile("cli");
	pic_init();
	idt_setup();

	keyboard_init();

	asm volatile("sti");
	
	uint16_t atapi_base = atapi_detect();

	if (atapi_base != 1) {


		uint8_t* font = (uint8_t*)read_file("/font.psf");
		if (font != 0){
			uint8_t* data = (uint8_t*)psf_load(font);
			vga_load_font(data, 8, psf_get_glyph_height(), psf_get_bytes_per_glyph());
		}
		else printf("File font.psf not found\n\r");
		set_active_console(0);
		printf("sychonok\n\r");
		printf("AshlikOS VESA + PSF font test\n\r");
	}

	printf("\nAshlikOS Kernel v.1.0\n\n\r");
	printf("Heap: %X\n\r", heap);
	printf("ATAPI I/O port base: %X\n\r", atapi_base);

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
	while(1);

	return;

}
