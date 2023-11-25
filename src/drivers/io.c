#include "../../incl/io.h"

uint8_t inb(uint16_t port){

	uint8_t data;
	asm volatile("inb %1, %0":"=a"(data):"Nd"(port):"memory");

	return data;
}

void outb(uint16_t port, uint8_t data){

	asm volatile("outb %0, %1": :"a"(data),"Nd"(port):"memory");

}

void outb_slow(uint16_t port, uint8_t data){

	outb(port, data);
	asm volatile("jmp 1f\n1: jmp 1f\n1:");

}
