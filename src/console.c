#include <console.h>
#include <mm/memman.h>

console_device* consoles;
console_device* active_console;
int console_count = 0;

void console_init(int count){

	consoles = (console_device*)malloc((count * sizeof(console_device))+8);

}

void console_register(void* read_func, void* write_func){

	consoles[console_count].read = read_func;
	consoles[console_count].write = write_func;

	console_count++;

}

void set_active_console(int index){
	active_console = &consoles[index];
}

console_device* get_active_console(){
	return active_console;
}
