;-------------------------------------------------------------------------------
; File: Lab_5.asm
; Description: This program calculates the first five powers of an integer b.
; 			   This program uses software (SW) and hardware (HW) multiplication
;			   subroutines, and stores the result from each in an array.
; Input: Integer b in the range [-8, -1] U [2, 8] per assignment requirement
; Output: Two integer arrays containing the first five powers of b
; Author: David Thornton (dht0002@uah.edu)
; Lab Section: 2
; Date: September 30, 2020
;-------------------------------------------------------------------------------
			.cdecls C,LIST,"msp430.h"		; Include device header file
;-------------------------------------------------------------------------------
			.def    RESET                   ; Export program entry-point to
											; make it known to linker.
			.ref	calc_power
;-------------------------------------------------------------------------------
; 		Memory allocation
			.bss	swarr, 10				; 10 bytes for software array
			.bss	hwarr, 10				; 10 bytes for hardware array
			.data							; Declare variable in a data segment
b:			.int	3						; intput into the code
result:		.int	1						; result = 1 at start of code
;-------------------------------------------------------------------------------
			.text                           ; Assemble into program memory.
			.retain                         ; Override ELF conditional linking
											; and retain current section.
			.retainrefs                     ; And retain any sections that have
											; references to current section.
;-------------------------------------------------------------------------------
RESET       mov   #__STACK_END, SP        	; Initialize stackpointer
StopWDT     mov   #WDTPW|WDTHOLD, &WDTCTL  	; Stop watchdog timer
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
main:
			push	#swarr					; push the address of the software array to the stack
			push	#hwarr					; push the address of the hardware array to the stack
			push	result					; push result to the stack
			mov	b, 	R4						; put b into R4
			mov	#5,	R8						; counter for number of powers
			call 	#calc_power				; go to calc_power to do the operations
			jmp $							; infinite loop

lend:
			nop

;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
            .global __STACK_END
            .sect   .stack

;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET
