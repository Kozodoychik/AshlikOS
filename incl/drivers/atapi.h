#include "ata.h"

uint16_t atapi_detect();
int atapi_read(uint32_t lba, uint32_t count,uint16_t* buffer);
