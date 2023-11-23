#include "vga.h"
#include "io.h"
#include <stdarg.h>

struct vga_char* videomem = (struct vga_char*)0xb8000;
uint8_t attrib = 0x07;

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
	uint16_t pos = (y * 80) + x;
	outb(0x3d4, 0x0f);
	outb(0x3d5, (uint8_t)(pos & 0xff));
	outb(0x3d4, 0x0e);
	outb(0x3d5, (uint8_t)((pos>>8) & 0xff));
}

char* tohexstring(int hex){
	char* result = (char*)0x500;
	char temp[sizeof(int)] = {};
	char hex_table[16] = "0123456789ABCDEF";
	uint8_t offset = 0;
	while (hex > 0){
		uint8_t dig = hex & 0xf;
		temp[offset] = hex_table[dig];
		hex = hex >> 4;
		offset++;
	}
	for (int i=0;i<offset;i++){
		result[i] = temp[offset-i-1];
	}
	return result;
}

char* todecstring(int num){
	char* result = (char*)0x600;
	char temp[sizeof(int)] = {};
	char dec_table[10] = "0123456789";
	uint8_t offset = 0;
	while (num > 0){
		uint8_t dig = num % 10;
		temp[offset] = dec_table[dig];
		num /= 10;
		offset++;
	}
	for (int i=0;i<offset;i++){
		result[i] = temp[offset-i-1];
	}
	return result;
}

void scroll(){
	for (int y=1;y<25;y++){
		for (int x=0;x<80;x++){
			videomem[x+((y-1)*80)] = videomem[x+(y*80)];
		}
	}
}

void printf(char* str, ...){
	uint8_t pos_x = (uint8_t)((get_cursor_pos()) % 80);
	uint8_t pos_y = (uint8_t)((get_cursor_pos()) / 80);
	va_list args;
	va_start(args, str); 
	uint8_t i=0;
	while (str[i] != '\0'){
		if (str[i] == '\n'){
			if (pos_y == 24){
				scroll();
			}else{
				pos_y++;
			}
		} 
		else if (str[i] == '\r'){
			pos_x = 0;
		}
		else if (str[i] == '%' && str[i+1] == 'X'){
			printf(tohexstring(va_arg(args, int)));
			pos_x += 2;
			i++;
		}
		else if (str[i] == '%' && str[i+1] == 'd'){
			printf(todecstring(va_arg(args, int)));
			pos_x += 2;
			i++;
		}
		else{
			uint16_t offset = get_cursor_pos();
			videomem[offset].c = str[i];
			videomem[offset].attrib = attrib;
			pos_x++;
		}
		i++;
		set_cursor_pos(pos_x, pos_y);
	}
	return;
}

void cls(){
	for (int i=0;i<80*25;i++){
		videomem[i].c = 0;
		videomem[i].attrib = attrib;
	}
	set_cursor_pos(0, 0);
}