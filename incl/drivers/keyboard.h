#include <stdint.h>
#include <stdbool.h>

#define PS2_KEYB_DATA 0x60
#define PS2_KEYB_COMMAND 0x64

void keyboard_init();
char* keyboard_input();
