#include <stdint.h>
#include <gdt.h>

gdt_entry gdt[3];
gdtr gdt_ptr;
extern void load_gdt();
void gdt_setup(){
	gdt[0].limit_low = 0;
	gdt[0].base_low = 0;
	gdt[0].access = 0;
	gdt[0].flags_limit = 0;
	gdt[0].base_middle = 0;
	gdt[0].base_high = 0;

	gdt[1].limit_low = 0xffff;
	gdt[1].base_low = 0;
	gdt[1].access = 0b10011010;
	gdt[1].flags_limit = 0b11001111;
	gdt[1].base_middle = 0;
	gdt[1].base_high = 0;

	gdt[2].limit_low = 0xffff;
	gdt[2].base_low = 0;
	gdt[2].access = 0b10010010;
	gdt[2].flags_limit = 0b11001111;
	gdt[2].base_middle = 0;
	gdt[2].base_high = 0;

	gdt_ptr.limit = sizeof(gdt) - 1;
	gdt_ptr.ptr = gdt;
	asm volatile("cli");
	load_gdt();
	//asm volatile("lgdt %0" : : "m" (gdt_ptr));
}
