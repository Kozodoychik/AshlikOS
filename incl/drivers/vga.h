#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ		0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
#define VGA_CRTC_INDEX		0x3D4
#define VGA_CRTC_DATA		0x3D5
#define	VGA_INSTAT_READ		0x3DA

struct vga_char {
	uint8_t c;
	uint8_t attrib;
};

void vga_init(uint32_t, uint32_t, uint32_t, uint32_t);
void vga_load_font(uint8_t *data, uint8_t w, uint8_t h, uint32_t bpg);
void set_attrib(uint8_t attr);
void set_cursor_pos(uint8_t x, uint8_t y);
void write_90x30_text_mode();
uint16_t get_cursor_pos();
void vga_90x30_text_mode_init();
void vga_putpixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
void vga_putchar(char c);
void cls();
