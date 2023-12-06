#include <fs/iso9660.h>
#include <drivers/atapi.h>
#include <drivers/vga.h>
#include <mm/memman.h>


iso9660_directory_entry* find_entry(char* name, bool is_directory, iso9660_directory_entry* entry){

	char n[32];
	strcpy(n, name);
	if (!is_directory)
		strcat(n, ";1");


	while (entry->length != 0 && memcmp(entry->file_id, n, strlen(n)) != 0){
		uint8_t len = entry->length;
		entry = (uint32_t)entry + len;
	}

	if (entry->length == 0) return 0;

	return entry;

}

char* get_name_from_path(char* path, int index){

	char* temp = path;

	char* result = (char*)malloc(32);
	for (int i=0;i<32;i++) result[i] = 0;

	if (temp[0] == '/') temp++;

	for (int i=0;i<index && temp[0];temp++){
		if (temp[0]=='/') i++;
	}

	for (int i=0;temp[i]!='/' && temp[i];i++) result[i] = temp[i];

	return result;

}

int get_subdirs_count(char* path){

	int result = 0;

	if (path[0] == '/') path++;

	for (int i=0;i<strlen(path);i++){
		if (path[i] == '/') result++;
	}

	return result;

}


void* read_file(char* path){

	uint16_t* pvd_ptr = (uint16_t*)malloc(2050);
	int status = atapi_read(0x10, 1, pvd_ptr);

        if (status==1) return 0;
        else {

		iso9660_primary_volume_descriptor* pvd = (iso9660_primary_volume_descriptor*)pvd_ptr;

		uint32_t lba = pvd->root_directory_entry.lba_lsb;
		uint16_t* root_entry_ptr = (uint16_t*)malloc(pvd->root_directory_entry.length);
		status = atapi_read(lba, (pvd->root_directory_entry.length/2048)+1, (uint16_t*)root_entry_ptr);

		free(pvd_ptr);

		if (status == 1) return 0;
		else {

			iso9660_directory_entry* directory = (iso9660_directory_entry*)root_entry_ptr;
			free(root_entry_ptr);
			for (int i=0;i<=get_subdirs_count(path);i++){
				bool is_file = false;

				char* name = get_name_from_path(path, i);
				iso9660_directory_entry* file = find_entry(name, !is_file, directory);

				if (file==0) return 0;

				free(name);

				if (!(file->flags & 0x2)){
					if (i==get_subdirs_count(path)) is_file=true;
					else return 0;
				}


				uint32_t lba = file->lba_lsb;
				uint32_t size = file->data_length_lsb;

				free(directory);
				directory = (iso9660_directory_entry*)malloc(size);

				status = atapi_read(lba, (size/2048)+1, (uint16_t*)directory);

				if (status == 1) return 0;
				if (is_file) return (void*)directory;
			}
		}
	}
}
