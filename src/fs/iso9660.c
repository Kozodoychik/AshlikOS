#include <fs/iso9660.h>
#include <drivers/atapi.h>

void* read_file(char* name){

	char n[32];
	strcpy(n, name);
	strcat(n, ";1");

	int status = atapi_read(0x10, 1, (uint16_t*)0x500000);

        if (status==1) return (void*)0;
        else {

		iso9660_primary_volume_descriptor* pvd = (iso9660_primary_volume_descriptor*)0x500000;

		uint32_t lba = pvd->root_directory_entry.lba_lsb;
		int status = atapi_read(lba, (pvd->root_directory_entry.length/2048)+1, (uint16_t*)0x510000);

		if (status == 1) return (void*)0;
		else {

			iso9660_directory_entry* file = (iso9660_directory_entry*)0x510000;

			while (file->length != 0){

				uint8_t len = file->length;
				if (strcmp(file->file_id, n) == 0) break;
				file = (uint32_t)file+len;

			}

			if (file->length == 0) return (void*)0;
			else {
				lba = file->lba_lsb;

				status = atapi_read(lba, (file->data_length_lsb/2048)+1, (uint16_t*)0x520000);

				return (void*)0x520000;
			}
		}
	}

}
