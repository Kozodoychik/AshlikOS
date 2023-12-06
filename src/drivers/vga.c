#include <drivers/vga.h>
#include <drivers/io.h>
#include <stdarg.h>

struct vga_char* videomem = (struct vga_char*)0xb8000;
uint8_t attrib = 0x07;
uint8_t w = 80;
uint8_t h = 25;

void set_attrib(uint8_t attr){

	attrib = attr;

}

uint16_t get_cursor_pos(){

	uint16_t pos = 0;

	outb(0x3d4, 0x0f);
	pos |= inb(0x3d5);
	outb(0x3d4, 0x0e);
	pos |= ((uint16_t)inb(0x3d5)) << 8;

	return pos;
}

void set_cursor_pos(uint8_t x, uint8_t y){

	uint16_t pos = (y * w) + x;

	outb(0x3d4, 0x0f);
	outb(0x3d5, (uint8_t)(pos & 0xff));
	outb(0x3d4, 0x0e);
	outb(0x3d5, (uint8_t)((pos>>8) & 0xff));

}

void vga_90x30_text_mode_init(){

	unsigned char g_90x30_text[] =
	{
	/* MISC */
		0xE7,
	/* SEQ */
		0x03, 0x01, 0x03, 0x00, 0x02,
	/* CRTC */
		0x6B, 0x59, 0x5A, 0x82, 0x60, 0x8D, 0x0B, 0x3E,
		0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
		0xEA, 0x0C, 0xDF, 0x2D, 0x10, 0xE8, 0x05, 0xA3,
		0xFF,
	/* GC */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
		0xFF,
	/* AC */
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
		0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		0x0C, 0x00, 0x0F, 0x08, 0x00,
	};

	unsigned char* reg = g_90x30_text;

	outb(VGA_MISC_WRITE, *reg);
	reg++;

	for (uint8_t i=0;i<5;i++){
		outb(VGA_SEQ_INDEX, i);
		outb(VGA_SEQ_DATA, *reg);
		reg++;
	}

	outb(VGA_CRTC_INDEX, 0x03);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
	outb(VGA_CRTC_INDEX, 0x11);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);

	reg[0x03] |= 0x80;
	reg[0x11] &= ~0x80;

	for (uint8_t i=0;i<25;i++){
		outb(VGA_CRTC_INDEX, i);
		outb(VGA_CRTC_DATA, *reg);
		reg++;
	}

	for (uint8_t i=0;i<9;i++){
		outb(VGA_GC_INDEX, i);
		outb(VGA_GC_DATA, *reg);
		reg++;
	}

	for (uint8_t i=0;i<21;i++){
		(void)inb(VGA_INSTAT_READ);
		outb(VGA_AC_INDEX, i);
		outb(VGA_AC_WRITE, *reg);
		reg++;
	}

	(void)inb(VGA_INSTAT_READ);
	outb(VGA_AC_INDEX, 0x20);

	w = 90;
	h = 30;

}

void scroll(){

	for (int y=1;y<h;y++){
		for (int x=0;x<w;x++){
			videomem[x+((y-1)*w)] = videomem[x+(y*w)];
		}
	}

}

void putchar(char c){
	uint16_t pos = get_cursor_pos();
	uint8_t x = (uint8_t)(pos % w);
	uint8_t y = (uint8_t)(pos / w);
	switch(c){
		case '\n':
			y++;
			break;
		case '\r':
			x = 0;
			break;
		default:
			videomem[get_cursor_pos()].c = c;
			videomem[get_cursor_pos()+1].attrib = attrib;
			x++;
	}
	if (y == 25){
		scroll();
		y--;
	}
	set_cursor_pos(x, y);
}

void cls(){

	for (int i=0;i<w*h;i++){
		videomem[i].c = 0;
		videomem[i].attrib = attrib;
	}

	set_cursor_pos(0, 0);
}
