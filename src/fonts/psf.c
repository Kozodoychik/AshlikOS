#include <fonts/psf.h>
#include <mm/memman.h>

uint8_t glyph_w;
uint8_t glyph_h;
uint16_t glyph_count;
uint32_t flags;
uint32_t bpg;
uint16_t* unicode = 0;

void* psf_load(uint8_t* data){

	uint32_t magic = *((uint32_t*)data);
	bool is_psf1 = false;

	if (magic != 0x864ab572){
		if ((magic&0xffff) != 0x0436) return 0;

		psf1_header* header = (psf1_header*)data;
		glyph_w = 8;
		glyph_h = header->size;
		glyph_count = header->mode&1 ? 512 : 256;
		bpg = header->size;
		flags = header->mode;
		is_psf1 = true;
	}
	else{
		psf_header* header = (psf_header*)data;
		glyph_w = header->w;
		glyph_h = header->h;
		glyph_count = header->glyph_count;
		bpg = header->bytes_per_glyph;
		flags = header->flags;
	}

	unicode = (uint16_t*)malloc(USHRT_MAX * 2);

	uint16_t glyph = 0;
	uint16_t* c = (uint16_t*)((uint32_t)data+(glyph_count*bpg));

	if (flags){
		while (glyph<glyph_count){
			uint16_t uc = (uint16_t)(c[0]);

			if (uc==0xffff){
				glyph++;
				c++;
				continue;
			}

			/*else if (uc & 128){
				if ((uc & 32) == 0){
					uc = ((c[0]&0x1f)<<6)+(c[1]&0x3f);
					c++;
				}
				else if ((uc & 16) == 0){
					uc = ((((c[0]&0xf)<<6)+(c[1]&0x3f))<<6)+(c[2]&0x3f);
					c+=2;
				}
				else if ((uc & 8) == 0){
					uc = ((((((c[0]&0x7)<<6)+(c[1]&0x3f))<<6)+(c[2]&0x3f))<<6)+(c[3]&0x3f);
					c+=3;
				}
				else uc=0;
			}*/

			unicode[uc] = glyph;
			c++;
		}
	}

	return data+(is_psf1 ? 4 : 32);
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

uint16_t* psf_get_unicode_translation_table(){
	return unicode;
}
