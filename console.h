#include <stdint.h>

typedef struct {
	char name[9];
	void* (*write)(char);
	char (*read)();
} console_device;

void console_init();
