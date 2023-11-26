extern gdt_ptr
global load_gdt
load_gdt:
	lgdt [gdt_ptr]
	jmp 0x08:gdt_loaded
	gdt_loaded:
	mov ax,0x0010
	mov es,ax
	mov ds,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	ret
