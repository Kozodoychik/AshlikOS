#include <stdint.h>

#define IDT_32_BIT 0xE
#define IDT_16_BIT 0x6
#define IDT_TRAP 1
#define IDT_INTERRUPT 0
#define IDT_P 0x80

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_COMMAND 0xA0
#define PIC_SLAVE_DATA 0xA1

typedef struct  {
	uint16_t offset_low;
	uint16_t selector;
	uint8_t unused;
	uint8_t attrib;
	uint16_t offset_high;
}__attribute__((__packed__)) idt_entry;
typedef struct  {
	uint16_t limit;
	idt_entry* addr;
}__attribute__((__packed__)) idt_ptr;
void idt_setup();
void idt_reg_handler(uint8_t, uint8_t, void*);
void pic_init();

