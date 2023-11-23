#include <stdint.h>

#define COM1_BASE 0x3f8

void serial_init();
int serial_received();
int serial_transmit_empty();
char serial_read();
void serial_write(char data);
void serial_print(char* data);
