#include <drivers/vga.h>
#include <drivers/io.h>

struct vga_char* txt_videomem = (struct vga_char*)0xb8000;
uint8_t* framebuffer;

uint8_t* font;
uint8_t font_w = 0;
uint8_t font_h = 0;
uint32_t bytes_per_glyph;

uint8_t attrib = 0x07;
uint32_t w = 80;
uint32_t h = 25;
uint8_t bpp = 0;

bool graphics = true;

uint32_t cursor_x = 0;
uint32_t cursor_y = 0;

void vga_init(uint32_t fb, uint32_t width, uint32_t height, uint32_t depth){
	framebuffer = (uint8_t*)fb;
	w = width;
	h = height;
	bpp = depth;
}

void vga_load_font(uint8_t* data, uint8_t w, uint8_t h, uint32_t bpg){
	font = data;
	font_w = w;
	font_h = h;
	bytes_per_glyph = bpg;
}

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

uint8_t txt_90x30[] =
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

void write_regs(uint8_t* reg){

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
}

void write_90x30_text_mode(){

	write_regs(txt_90x30);
	w = 90;
	h = 30;
	bpp = 0;
	graphics = false;

}

void scroll(){

	if (!graphics){
		for (uint32_t y=1;y<h;y++){
			for (uint32_t x=0;x<w;x++){
				txt_videomem[x+((y-1)*w)] = txt_videomem[x+(y*w)];
			}
		}
		for (uint32_t x=0;x<w;x++) txt_videomem[x+((h-1)*w)].c = 0;
	}
	else{
		for (uint32_t y=0;y<h;y++){
			for (uint32_t x=0;x<w;x++){
				uint32_t offset1 = ((y-font_h)*(w*bpp)) + x*bpp;
				uint32_t offset2 = (y*(w*bpp)) + x*bpp;
				
				framebuffer[offset1] = framebuffer[offset2];
				framebuffer[offset1+1] = framebuffer[offset2+1];
				framebuffer[offset1+2] = framebuffer[offset2+2];
				
				framebuffer[offset2] = 0;
				framebuffer[offset2+1] = 0;
				framebuffer[offset2+2] = 0;
			}
		}
	}

}

void vga_putpixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b){

	uint32_t offset = (y*(w*bpp)) + x*bpp;

	framebuffer[offset] = r;
	framebuffer[offset+1] = g;
	framebuffer[offset+2] = b;

}

void vga_putchar(char c){

	if (!graphics){
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
				txt_videomem[get_cursor_pos()].c = c;
				txt_videomem[get_cursor_pos()+1].attrib = attrib;
				x++;
		}

		if (y >= h-1){
			scroll();
			y--;
		}

		set_cursor_pos(x, y);
	}
	else{
		if (cursor_x == (w/font_w)){
			cursor_x = 0;
			cursor_y++;
		}
		if (cursor_y == (h/font_h)){
			scroll();
			cursor_y--;
		}
		switch (c){
			case '\n':
				cursor_y++;
				break;
			case '\r':
				cursor_x = 0;
				break;
			default:
				uint8_t* ch = (uint8_t*)((uint32_t)font+(c*bytes_per_glyph));
				for (int y=0;y<font_h;y++){
					uint8_t line = ch[y];
					for (int x=0;x<font_w;x++){
						uint8_t pixel = (line << x) & 0x80;
						if (pixel) vga_putpixel(x+(cursor_x*font_w), y+(cursor_y*font_h), 255, 255, 255);
					}
				}
				cursor_x++;
				break;
		}
	}

}

void cls(){

	for (uint32_t i=0;i<w*h;i++){
		txt_videomem[i].c = 0;
		txt_videomem[i].attrib = attrib;
	}

	set_cursor_pos(0, 0);
}
