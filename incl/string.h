#include <stdint.h>

typedef uint32_t size_t;

size_t strlen(char* str){
	size_t size = 0;
	while (str[size] != 0){
		size++;
	}
	return size;
}