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

int memcmp(uint8_t* first_ptr, uint8_t* second_ptr, int c){

	for (int i=0;i<c;i++)
		if (first_ptr[i] != second_ptr[i]) return first_ptr[i]-second_ptr[i];

	return 0;

}
