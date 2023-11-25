#include <stdint.h>
#include "../incl/vga.h"
#include "../incl/string.h"
#include "../incl/interrupts.h"
#include "../incl/gdt.h"
#include "../incl/io.h"
#include "../incl/serial.h"
#include "../incl/keyboard.h"

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

	printf("%X, %X, %X, %X\n\r", 0x20, 0x1234, 0x21, 0x55AAFF);

	while(1);

	return;

}
