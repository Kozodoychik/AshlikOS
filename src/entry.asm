section .text
global start
extern kmain
bits 32
start:
	mov esp,bss_end
	call kmain
	cli
	hlt
section .bss
bss_start:
resb 4096 * 4
bss_end: