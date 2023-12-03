#include <drivers/atapi.h>
#include <drivers/io.h>

uint16_t atapi_base = 0x1f0;
uint16_t atapi_detect(){

	outb(ATA_PRIMARY_BASE + ATA_COMMAND, 0x90);
	outb(ATA_SECONDARY_BASE + ATA_COMMAND, 0x90);

	for(uint8_t i=0;i<4;i++){}

	uint16_t signature = (inb(ATA_PRIMARY_BASE + ATA_LBA_MID) << 0x08) | inb(ATA_PRIMARY_BASE + ATA_LBA_HIGH);

	if (signature == 0x14EB){
		atapi_base = ATA_PRIMARY_BASE;
		return atapi_base;
	}

	signature = (inb(ATA_SECONDARY_BASE + ATA_LBA_MID) << 0x08) | inb(ATA_SECONDARY_BASE + ATA_LBA_HIGH);

	if (signature == 0x14EB){
                atapi_base = ATA_SECONDARY_BASE;
                return atapi_base;
        }

	return 1;

}

int atapi_read(uint32_t lba, uint32_t count, uint16_t* buffer){

	atapi_detect();

	volatile uint8_t cmd[12] = {0xa8, 0,
		(lba>>0x18) & 0xff, (lba>>0x10) & 0xff, (lba>>0x08) & 0xff, lba & 0xff,
		(count>>0x18) & 0xff, (count>>0x10) & 0xff, (count>>0x8) & 0xff, count & 0xff,
		0, 0};

	outb(atapi_base + ATA_DRIVE_SELECT, 0xa0 & (0 << 4));
	for(uint8_t i=0;i<4;i++){}
	outb(atapi_base + ATA_ERR, 0);
	outb(atapi_base + ATA_LBA_MID, 2048 & 0xff);
	outb(atapi_base + ATA_LBA_HIGH, 2048 >> 8);
	outb(atapi_base + ATA_COMMAND, 0xa0);

	for(uint8_t i=0;i<4;i++){}

	while(1){
		uint8_t status = inb(atapi_base + ATA_COMMAND);
		if (status & 1) return 1;
		if (!(status & 0x80) && (status & 0x08)) break;
		for(uint8_t i=0;i<4;i++){}
	}

	outsw(atapi_base + ATA_DATA, ((uint16_t*)cmd), 6);

	for (uint32_t i=0;i<count;i++){
		while(1){
			uint8_t status = inb(atapi_base + ATA_COMMAND);
			if (status & 1) return 1;
                	if (!(status & 0x80) && (status & 0x08)) break;
		}

		int size = inb(atapi_base + ATA_LBA_HIGH) << 8 | inb(atapi_base + ATA_LBA_MID);
		insw(atapi_base + ATA_DATA, (uint16_t*)((uint8_t*)buffer+i*0x800), size/2);
	}

	return 0;

}

