#include <interrupts.h>
#include <drivers/io.h>
#include <drivers/keyboard.h>
#include <drivers/vga.h>

const char* scan_code_set1 = "  1234567890-=  qwertyuiop[]  asdfghjkl;'` \\zxcvbnm,./ *               789-456+1230.";

extern void isr0x21;

void __keyb_irq_handler(){

	uint8_t data = inb(PS2_KEYB_DATA);
	if (data < 0x80){
		if (data == 0x1c) printf("\n\r");
		else printf("%c", scan_code_set1[data]);
	}
	pic_eoi();

}

void keyboard_init(){

	idt_reg_handler(0x21, 0x8e, &isr0x21);

}

char* keyboard_input(){

	return "";

}
