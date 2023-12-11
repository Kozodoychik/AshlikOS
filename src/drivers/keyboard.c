#include <interrupts.h>
#include <drivers/io.h>
#include <drivers/keyboard.h>
#include <io/printf.h>

const char* scan_code_set1 = "  1234567890-=  qwertyuiop[]  asdfghjkl;'` \\zxcvbnm,./ *               789-456+1230.";
const wchar_t* scan_code_set1_rus = L"  1234567890-=  יצףךוםדרשחץת  פûגאןנמכהז‎¸ \\ÿקסלטעüב‏. *               789-456+1230.";

bool keyb_layout2 = false;
bool is_alt_pressed = false;

extern void isr0x21;

void __keyb_irq_handler(){

	uint8_t data = inb(PS2_KEYB_DATA);
	if (data == 0x38) is_alt_pressed = true;
	else if (data == 0xb8) is_alt_pressed = false;
	else{
		if (data == 0x2a && is_alt_pressed) keyb_layout2 = !keyb_layout2;
		else if (data < 0x80){
			if (data == 0x1c) printf("\n\r");
			else keyb_layout2 ? wprintf(L"%c", scan_code_set1_rus[data]) : printf("%c", scan_code_set1[data]);
		}
	}
	//printf("%X\n\r", data);
	pic_eoi();

}

void keyboard_init(){

	idt_reg_handler(0x21, 0x8e, &isr0x21);

}

char* keyboard_input(){

	return "";

}
