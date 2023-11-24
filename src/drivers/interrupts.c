#include <stdint.h>
#include "../../incl/interrupts.h"
#include "../../incl/io.h"

idt_entry idt[256];
idt_ptr idt_p;

void idt_ignore(){

	asm volatile("iret");

}

void idt_setup(){

	pic_init();

        idt_p.limit = (uint16_t)((sizeof(idt_entry) * 256) - 1);
        idt_p.addr = idt;

	for (uint16_t i=0;i<32;i++){
		idt_reg_handler(i, 0x8f, &idt_ignore);
	}

	asm volatile("lidt %0" : : "m" (idt_p));

}

void idt_reg_handler(uint8_t i, uint8_t attrib, void* handler){

	idt_entry* desc = &idt[i];

	desc->offset_low = ((uint32_t)handler & 0xffff);
	desc->attrib = attrib;
	desc->unused = 0;
	desc->selector = 0x8;
	desc->offset_high = ((uint32_t)handler) >> 16;

	//printf("Register interrupt %d handler\n\r\n\rattrib=%X\n\rselector=%X\n\r", i, attrib, 0x8);
}

void pic_init(){

	outb(PIC_MASTER_COMMAND, 0x11);
	outb(PIC_SLAVE_COMMAND, 0x11);

	outb(PIC_MASTER_DATA, 0x20);
	outb(PIC_SLAVE_DATA, 0x28);

	outb(PIC_MASTER_DATA, 0x04);
	outb(PIC_SLAVE_DATA, 0x02);

	outb(PIC_MASTER_DATA, 0x01);
	outb(PIC_SLAVE_DATA, 0x01);

	outb(PIC_MASTER_DATA, 0);
	outb(PIC_SLAVE_DATA, 0);

}
