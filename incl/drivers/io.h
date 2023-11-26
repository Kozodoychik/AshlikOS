#include <stdint.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);
void outb_slow(uint16_t port, uint8_t data);
uint32_t inl(uint16_t port);
void outl(uint16_t port, uint32_t data);
