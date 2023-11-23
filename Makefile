CC=i686-linux-gnu-gcc
CFLAGS=-ffreestanding -O2 -Wall -Wextra -nostdlib -std=gnu99
OUTPUT=kern.bin
LDFLAGS=-T link.ld -o $(OUTPUT) -ffreestanding -O2 -nostdlib -lgcc

all: build iso clear
build:
	nasm src/entry.asm -o entry.o -f elf
	nasm src/header.asm -o header.o -f elf
	$(CC) $(CFLAGS) -c src/main.c -o main.o
	$(CC) $(CFLAGS) -c incl/io.c -o io.o
	$(CC) $(CFLAGS) -c incl/vga.c -o vga.o
	$(CC) $(CFLAGS) -c incl/interrupts.c -o interrupts.o
	$(CC) $(CFLAGS) -c incl/gdt.c -o gdt.o
	$(CC) $(CFLAGS) -c incl/serial.c -o serial.o
	$(CC) $(LDFLAGS) header.o entry.o io.o vga.o interrupts.o gdt.o serial.o main.o
run:
	qemu-system-x86_64 -kernel kern.bin -d int -no-reboot
run-iso:
	qemu-system-x86_64 -cdrom os.iso
clear:
	rm *.o
iso:
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp kern.bin iso/boot/kern.bin
	echo 'menuentry "AshlikOS" {' >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/kern.bin' >> iso/boot/grub/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output os.iso iso
	rm -rf iso
