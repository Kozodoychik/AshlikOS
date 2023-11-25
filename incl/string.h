#include <stdint.h>

typedef uint32_t size_t;

size_t strlen(char* str){
	size_t size = 0;
	while (str[size] != 0){
		size++;
	}
	return size;
}

char* strcat(char* dest, char* source){

	size_t dest_size = strlen(dest);

	for (size_t i=0;i<strlen(source)+1;i++)
		dest[dest_size+i] = source[i];

	return dest;
}
