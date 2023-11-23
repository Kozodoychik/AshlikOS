#include <stdint.h>

struct vga_char {
	uint8_t c;
	uint8_t attrib;
};

void set_attrib(uint8_t attr);
void set_cursor_pos(uint8_t x, uint8_t y);
uint16_t get_cursor_pos();
void printf(char* str, ...);
void cls();
