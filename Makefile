CC=i686-linux-gnu-gcc
CFLAGS=-ffreestanding -O2 -Wall -Wextra -nostdlib -std=gnu99 -Iincl
OUTPUT=kern.bin
LDFLAGS=-T link.ld -o $(OUTPUT) -ffreestanding -O2 -nostdlib -lgcc

all: build iso clean
build:
	nasm src/entry.asm -o entry.o -f elf
	nasm src/header.asm -o header.o -f elf
	nasm src/interrupts.asm -o isr.o -f elf
	nasm src/gdt.asm -o loadgdt.o -f elf
	$(CC) $(CFLAGS) -c src/main.c -o main.o
	$(CC) $(CFLAGS) -c src/interrupts.c -o interrupts.o
	$(CC) $(CFLAGS) -c src/gdt.c -o gdt.o
	$(CC) $(CFLAGS) -c src/drivers/io.c -o io.o
	$(CC) $(CFLAGS) -c src/drivers/vga.c -o vga.o
	$(CC) $(CFLAGS) -c src/drivers/serial.c -o serial.o
	$(CC) $(CFLAGS) -c src/drivers/keyboard.c -o keyboard.o
	$(CC) $(CFLAGS) -c src/drivers/pci.c -o pci.o
	$(CC) $(CFLAGS) -c src/drivers/atapi.c -o atapi.o
	$(CC) $(CFLAGS) -c src/fs/iso9660.c -o iso9660.o
	$(CC) $(LDFLAGS) header.o entry.o io.o vga.o interrupts.o isr.o loadgdt.o gdt.o serial.o keyboard.o pci.o atapi.o iso9660.o main.o
run:
	qemu-system-x86_64 -kernel kern.bin -d int -no-reboot
run-iso:
	qemu-system-x86_64 -cdrom os.iso
clean:
	rm *.o
iso:
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp kern.bin iso/boot/kern.bin
	echo 'menuentry "AshlikOS" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/kern.bin' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	echo "Sova\r" >> iso/test.txt
	grub-mkrescue --output os.iso iso
	rm -rf iso
