#include <types.h>
#include <drivers/pci.h>
#include <drivers/io.h>

uint32_t get_id(uint16_t bus, uint16_t device, uint16_t function, uint32_t reg){

	return 1 << 31 | ((bus & 0xff) << 16) | ((device & 0x1f) << 11) | ((function & 0x07) << 8) | (reg & 0xfc);

}

uint32_t pci_read(uint16_t bus, uint16_t device, uint16_t function, uint32_t reg){

	uint32_t id = get_id(bus, device, function, reg);
	outl(PCI_COMMAND, id);
	uint32_t result = inl(PCI_DATA);
	return result >> (8 * (reg % 4));

}

void pci_write(uint16_t bus, uint16_t device, uint16_t function, uint32_t reg, uint32_t value){

	uint32_t id = get_id(bus, device, function, reg);
	outl(PCI_COMMAND, id);
	outl(PCI_DATA, value);

}

bool pci_device_has_funcs(uint16_t bus, uint16_t device){

	return pci_read(bus, device, 0, 0x0e) & (1<<7);

}
pci_device_desc pci_get_device_descriptor(uint16_t bus, uint16_t device, uint16_t function){

	pci_device_desc d;
	d.bus = bus;
	d.device = device;
	d.function = function;
	d.vendor_id = pci_read(bus, device, function, 0x0);
	d.device_id = pci_read(bus, device, function, 0x02);
	d.class_id = pci_read(bus, device, function, 0x0b);
	d.subclass_id = pci_read(bus, device, function, 0x0a);
	d.interface = pci_read(bus, device, function, 0x09);
	d.revision = pci_read(bus, device, function, 0x08);
	d.interrupt = pci_read(bus, device, function, 0x3c);

	return d;

}
