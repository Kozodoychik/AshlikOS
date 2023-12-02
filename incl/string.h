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

	for (size_t i=0;i<=strlen(source);i++)
		dest[dest_size+i] = source[i];

	return dest;
}

int strcmp(char* first_str, char* second_str){

	for (size_t i=0;i<=strlen(first_str);i++){
		if (first_str[i] != second_str[i]) return first_str[i] - second_str[i];
	}

	return 0;

}

char* strcpy(char* dest, char* src){

	for (size_t i=0;i<=strlen(src);i++)
		dest[i] = src[i];
	return dest;

}
