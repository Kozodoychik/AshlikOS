#include <stdint.h>
#include <stdbool.h>
#include <types.h>

typedef struct mem_chunk mem_chunk;

struct mem_chunk{

	mem_chunk* prev;
	mem_chunk* next;

	uint32_t size;
	bool is_allocated;

};

//typedef struct mem_chunk mem_chunk;

void mm_init(uint32_t start, uint32_t size);
void* malloc(size_t size);
void free(void* ptr);
