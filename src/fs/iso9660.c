#include <fs/iso9660.h>
#include <drivers/atapi.h>
#include <mm/memman.h>

void* read_file(char* name){

	char n[32];
	strcpy(n, name);
	strcat(n, ";1");

	uint16_t* pvd_ptr = (uint16_t*)malloc(2048);
	int status = atapi_read(0x10, 1, pvd_ptr);

        if (status==1) return (void*)0;
        else {

		iso9660_primary_volume_descriptor* pvd = (iso9660_primary_volume_descriptor*)pvd_ptr;

		uint32_t lba = pvd->root_directory_entry.lba_lsb;
		uint16_t* root_entry_ptr = (uint16_t*)malloc(pvd->root_directory_entry.length);
		int status = atapi_read(lba, (pvd->root_directory_entry.length/2048)+1, (uint16_t*)root_entry_ptr);

		free(pvd_ptr);

		if (status == 1) return (void*)0;
		else {

			iso9660_directory_entry* file = (iso9660_directory_entry*)root_entry_ptr;

			while (file->length != 0){

				uint8_t len = file->length;
				if (strcmp(file->file_id, n) == 0) break;

				file = (uint32_t)file+len;

			}

			if (file->length == 0) return (void*)0;
			else {
				lba = file->lba_lsb;
				uint16_t* file_content_ptr = (uint16_t*)malloc(file->data_length_lsb);
				status = atapi_read(lba, (file->data_length_lsb/2048)+1, (uint16_t*)file_content_ptr);

				free(root_entry_ptr);

				return (void*)file_content_ptr;
			}
		}
	}

}
