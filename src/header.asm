section .multiboot_header
	dd 0x1badb002
	dd (1<<0 | 1<<1 | 1<<2)
	dd -(0x1badb002 + (1<<0 | 1<<1 | 1<<2))
	times 5 dd 0
	dd 0
	dd 640
	dd 480
	dd 32
