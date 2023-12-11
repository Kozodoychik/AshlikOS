#include <stdint.h>

typedef struct {
	void* (*write)();
	char (*read)();
} console_device;

void console_init(int count);
void console_register(void* read_func, void* write_func);
console_device* get_active_console();
void set_active_console(int index);
console_device* get_console(int id);
