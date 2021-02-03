;-------------------------------------------------------------------------------------------------------
; File: Lab_4_Q3.asm
; Description: Capitalizes all letters in a string
; Input: Hard coded string
; Output: Upper case string in the same memory location as input
; Author: David Thornton
; Lab Section: 2
; Date: September 22, 2020
;-------------------------------------------------------------------------------------------------------
		.cdecls C,LIST,"msp430.h"		; Include device header file
;-------------------------------------------------------------------------------------------------------
		.def    RESET					; Export program entry-point to make it known to linker
		.data 							; Declare variable in a data segment
myStr:  .cstring "I enjoy learning msp430"
;-------------------------------------------------------------------------------------------------------
		.text							; Assemble into program memory
		.retain							; Override ELF conditional linking and retain current section
		.retainrefs						; Retain any sections that have references to current section
;-------------------------------------------------------------------------------------------------------
RESET:  mov.w   #__STACK_END, SP		; Initialize stack pointer
		mov.w   #WDTPW|WDTHOLD, &WDTCTL	; Stop watchdog timer
;-------------------------------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------------------------------
main:	mov.w   #myStr, R4				; Load the starting address of the string into R4

gnext:  mov.b   @R4+, R6				; Get the next character, put it into R6, and increment R4
		cmp.b   #0, R6				   	; Check to see if R6 is the NULL character
		jeq     lend					; If yes, go to the "lend" subroutine
		cmp.b   #97, R6					; Check to see if R6 is upper case
		jc      to_upper				; If yes, go to the "to_upper" subroutine
		jmp     gnext					; Go to the next character

to_upper
		cmp.b   #123, R6				; Check if the character is within the lower case values
		jc      gnext					; If R6 is outside the range, go to the "gnext" subroutine
		sub     #32, R6					; If R6 is inside the range [97, -122], subtract 32
		mov.b   R6, -1(R4)				; Move the new (upper case) character (R6) to the
										; previous position in R4 (original string).
										; We use -1 as the index since R4 was already incremented.
		jmp     gnext					; Go to the next character

lend:	nop								; Required only for debugger

;-------------------------------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------------------------------
		.global __STACK_END
		.sect   .stack
;-------------------------------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------------------------------
		.sect   ".reset"				; MSP430 RESET Vector
		.short  RESET
		.end
