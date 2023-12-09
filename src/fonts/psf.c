#include <fonts/psf.h>

uint8_t glyph_w;
uint8_t glyph_h;
uint32_t bpg;

void* psf_load(uint8_t* data){

	uint32_t magic = *((uint32_t*)data);
	if (magic != 0x864ab572){
		if ((magic&0xffff) != 0x0436) return 0;

		psf1_header* header = (psf1_header*)data;
		glyph_w = header->size;
		glyph_h = header->size;
		bpg = header->size;
		return data+sizeof(psf1_header);
	}
	else{
		psf_header* header = (psf_header*)data;
		glyph_w = header->w;
		glyph_h = header->h;
		bpg = header->bytes_per_glyph;
		return data+sizeof(psf_header);
	}
	return 0;
}

uint8_t psf_get_glyph_width(){
	return glyph_w;
}

uint8_t psf_get_glyph_height(){
	return glyph_h;
}

uint32_t psf_get_bytes_per_glyph(){
	return bpg;
}
