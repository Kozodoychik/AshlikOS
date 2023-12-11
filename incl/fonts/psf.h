#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
	uint16_t magic;
	uint8_t mode;
	uint8_t size;
} psf1_header;

typedef struct{
	uint32_t magic;
	uint32_t version;
	uint32_t header_size;
	uint32_t flags;
	uint32_t glyph_count;
	uint32_t bytes_per_glyph;
	uint32_t w;
	uint32_t h;
} psf_header;

void* psf_load(uint8_t* data);
uint8_t psf_get_glyph_width();
uint8_t psf_get_glyph_height();
uint32_t psf_get_bytes_per_glyph();
uint16_t* psf_get_unicode_translation_table();
