#include <stdint.h>
#include "../incl/vga.h"
#include "../incl/string.h"
#include "../incl/interrupts.h"
#include "../incl/gdt.h"
#include "../incl/serial.h"

void debug_int(){

	asm volatile("cli");
	printf("!!!DEBUG INTERRUPT!!!\n\r");
	asm volatile("hlt");
	while(1){};

}

void kmain(){

	cls();

	gdt_setup();

	asm volatile("cli");
	idt_setup();
	idt_reg_handler(0x1, 0x8e, &debug_int);
	asm volatile("sti");

	printf("AshlikOS Kernel v.1.0\n\r");

	printf("Initializing serial...");

	serial_init();
	serial_print("AshlikOS Kernel v.1.0 COM1\n\r");

	printf("OK\n\r");

	//asm volatile("int $1");
	return;

}
