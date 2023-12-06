#include <stdint.h>

typedef struct {

	uint8_t type;
	char id[5];
	uint8_t version;

}__attribute__((packed)) iso9660_volume_descriptor;

typedef struct {

	char year[4];
	char month[2];
	char day[2];
	char hour[2];
	char minute[2];
	char second[2];
	char hundredths_of_second[2];
	uint8_t timezone;

}__attribute__((packed)) iso9660_ascii_date_time;

typedef struct {

	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	uint8_t gmt_offset;

}__attribute__((packed)) iso9660_date_time;

typedef struct {

	uint8_t length;
	uint8_t attr_record_length;

	uint32_t lba_lsb;
	uint32_t lba_msb;

	uint32_t data_length_lsb;
	uint32_t data_length_msb;

	iso9660_date_time time;

	uint8_t flags;
	uint8_t unit_size;
	uint8_t gap_size;

	uint16_t sequence_num_lsb;
	uint16_t sequence_num_msb;

	uint8_t file_id_length;

	char file_id[1];

}__attribute__((packed)) iso9660_directory_entry;

typedef struct {

	iso9660_volume_descriptor d;

	uint8_t unused;

	char system_id[32];
	char volume_id[32];

	uint64_t unused2;

	uint32_t volume_space_size_lsb;
	uint32_t volume_space_size_msb;

	uint8_t unused3[32];

	uint16_t volume_set_size_lsb;
	uint16_t volume_set_size_msb;

	uint16_t sequence_num_lsb;
	uint16_t sequence_num_msb;

	uint16_t logical_block_size_lsb;
	uint16_t logical_block_size_msb;

	uint32_t path_table_size_lsb;
	uint32_t path_table_size_msb;

	uint32_t path_table_l_ptr;
	uint32_t optional_path_table_l_ptr;

	uint32_t path_table_m_ptr;
	uint32_t optional_path_table_m_ptr;

	iso9660_directory_entry root_directory_entry;

	char volume_set_id[128];
	char publisher_id[128];
	char data_preparer_id[128];
	char app_id[128];
	char copyright_file_id[37];
	char abstract_file_id[37];
	char bibliographic_file_id[37];

	iso9660_ascii_date_time volume_creation_date;
	iso9660_ascii_date_time volume_expiration_date;
	iso9660_ascii_date_time volume_effective_date;

	uint8_t version;

}__attribute__((packed)) iso9660_primary_volume_descriptor;


char* get_name_from_path(char* path, int index);
int get_subdirs_count(char* path);
void* read_file(char* path);
