#include <stdint.h>
#include "../types.h"

#define PCI_COMMAND 0xcf8
#define PCI_DATA 0xcfc

typedef struct {
	uint32_t port_base;
	uint32_t interrupt;
	uint16_t bus;
	uint16_t device;
	uint16_t function;
	uint16_t vendor_id;
	uint16_t device_id;
	uint8_t class_id;
	uint8_t subclass_id;
	uint8_t interface;
	uint8_t revision;
}__attribute__((packed))  pci_device_desc;

uint32_t pci_read(uint16_t, uint16_t, uint16_t, uint32_t);
void pci_write(uint16_t, uint16_t, uint16_t, uint32_t, uint32_t);
bool pci_device_has_funcs(uint16_t, uint16_t);
pci_device_desc pci_get_device_descriptor(uint16_t, uint16_t, uint16_t);
