#include <mm/memman.h>

mem_chunk* first;

void mm_init(uint32_t start, uint32_t size){

	if (size < sizeof(mem_chunk)){
		first = 0;
		return;
	}

	first = (mem_chunk*)start;
	first->prev = 0;
	first->next = 0;
	first->size = size - sizeof(mem_chunk);
	first->is_allocated = false;

}

void* malloc(size_t size){

	mem_chunk* result = 0;

	for (mem_chunk* chunk = first;chunk!=0 && result==0;chunk=chunk->next){
		if (chunk->size > size && !chunk->is_allocated)
			result = chunk;
	}

	if (result==0) return 0;

	if (result->size >= size + sizeof(mem_chunk) + 1){
		mem_chunk* temp = (mem_chunk*)((size_t)result + sizeof(mem_chunk) + size);
		temp->prev = result;
		temp->next = result->next;
		temp->size = result->size - size - sizeof(mem_chunk);
		if (temp->next != 0) temp->next->prev = temp;

		result->next = temp;
		result->size = size;
	}

	result->is_allocated = true;

	return (void*)((uint32_t)result + sizeof(mem_chunk));

}

void free(void* ptr){

	mem_chunk* chunk = (mem_chunk*)((uint32_t)ptr - sizeof(mem_chunk));

	chunk->is_allocated = false;

	if (chunk->prev != 0 && !chunk->prev->is_allocated){
		chunk->prev->next = chunk->next;
		chunk->prev->size += chunk->size + sizeof(mem_chunk);

		if (chunk->next != 0) chunk->next->prev = chunk->prev;

		chunk = chunk->prev;
	}

	if (chunk->next != 0 && !chunk->next->is_allocated){
		chunk->size += chunk->next->size + sizeof(mem_chunk);
		chunk->next = chunk->next->next;

		if (chunk->next != 0) chunk->next->prev = chunk;
	}
}
