#include <stdint.h>

typedef struct {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t flags_limit;
	uint8_t base_high;
}__attribute__((packed)) gdt_entry;
typedef struct {
	uint16_t limit;
	gdt_entry* ptr;
}__attribute__((packed)) gdtr;

void gdt_setup();
