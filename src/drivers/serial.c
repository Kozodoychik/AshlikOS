#include <drivers/serial.h>
#include <drivers/io.h>

void serial_init(){

	outb(COM1_BASE+1, 0);
	outb(COM1_BASE+3, 0x80);
	outb(COM1_BASE, 0x03);
	outb(COM1_BASE+1, 0);
	outb(COM1_BASE+3, 0x03);
	outb(COM1_BASE+2, 0xc7);
	outb(COM1_BASE+4, 0x0b);
	outb(COM1_BASE+4, 0x1e);
	outb(COM1_BASE, 0xae);

	if (inb(COM1_BASE) != 0xae){
		return;
	}

	outb(COM1_BASE+4, 0x0f);

	return;
}

int serial_received(){
	return inb(COM1_BASE+5) & 1;
}

int serial_transmit_empty(){
	return inb(COM1_BASE+5) & 0x20;
}

char serial_read(){
	while(serial_received() == 0);
	return inb(COM1_BASE);
}

void serial_write(char data){
	while(serial_transmit_empty() == 0);
	outb(COM1_BASE, data);
}

void serial_print(char* data){
	int i=0;
	while (data[i] != 0) {
		serial_write(data[i]);
		i++;
	}
}
