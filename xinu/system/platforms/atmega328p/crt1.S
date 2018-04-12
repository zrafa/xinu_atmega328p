#define SPL 0x3D
#define SPH 0x3E

    .org 0x0000
	
__vectors:
	jmp		Reset_handler 

Reset_handler:
;Initialize the microcontroller stack pointer
	ldi		r16, lo8(_stack_end)
	out		SPL, r16
	ldi		r16, hi8(_stack_end)
	out		SPH, r16

	clr		r1
	rjmp	start

