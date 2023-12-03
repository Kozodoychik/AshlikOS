#include <stdint.h>
#include <interrupts.h>
#include <drivers/io.h>
#include <drivers/vga.h>

idt_entry idt[256];
idt_ptr idt_p;
extern void isr0x00;
void __interrupt_handler(){
	printf("interrupt\n\r");
	asm volatile("cli; hlt");
	//asm volatile("iret");

}

void idt_setup(){

        idt_p.limit = (uint16_t)((sizeof(idt_entry) * 256) - 1);
        idt_p.addr = idt;

	for (uint16_t i=0;i<32;i++){
		idt_reg_handler(i, 0x8f, &isr0x00);
	}
	outb(PIC_MASTER_DATA, 0xfd);
	outb(PIC_SLAVE_DATA, 0xff);
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
	uint8_t m1, m2;
	m1 = inb(PIC_MASTER_DATA);
	m2 = inb(PIC_SLAVE_DATA);

	outb_slow(PIC_MASTER_COMMAND, 0x11);
	outb_slow(PIC_SLAVE_COMMAND, 0x11);

	outb_slow(PIC_MASTER_DATA, 0x20);
	outb_slow(PIC_SLAVE_DATA, 0x28);

	outb_slow(PIC_MASTER_DATA, 0x04);
	outb_slow(PIC_SLAVE_DATA, 0x02);

	outb_slow(PIC_MASTER_DATA, 0x01);
	outb_slow(PIC_SLAVE_DATA, 0x01);

	outb_slow(PIC_MASTER_DATA, 0);
	outb_slow(PIC_SLAVE_DATA, 0);

	outb(PIC_MASTER_DATA, m1);
	outb(PIC_SLAVE_DATA, m2);

}
void pic_eoi(){

	outb(PIC_MASTER_COMMAND, 0x20);
	outb(PIC_SLAVE_COMMAND, 0x20);

}
