extern __interrupt_handler
extern __keyb_irq_handler

%macro ISR_Handler 2
isr%1:
	pusha
	call %2
	popa
	iret
global isr%1
%endmacro

ISR_Handler 0x00, __interrupt_handler
ISR_Handler 0x21, __keyb_irq_handler
