;-------------------------------------------------------------------------------------------------------
; File: Lab_4_Q2.asm
; Description: Performs addition and subtraction of single digit base 10 numbers
; Constraints: This program can only accept the integers [0-9] as inputs, as well as
; 			   only perform the operations addition and subtraction.
;			   The scope could be easily expanded to include multiplication and division.
; Input: Hard coded string containing a mathematical expression. The input must not contain spaces.
; Output: P2OUT contians the hex result of the mathematical expression
; Author: David Thornton
; Lab Section: 2
; Date: September 22, 2020
;-------------------------------------------------------------------------------------------------------
		.cdecls C,LIST,"msp430.h"		; Include device header file
;-------------------------------------------------------------------------------------------------------
		.def    RESET					; Export program entry-point to make it known to linker
		.data 							; Declare variable in a data segment
myStr:  .cstring "9+9-6"
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
		clr.b   R5						; R5 is the running total
		clr.b   R6						; R6 is a temp register

gnext:  mov.b   @R4+, R6				; Get the next character, put it into R6, and increment R4
		cmp.b   #0, R6				   	; Check to see if R6 is the NULL character
		jeq     lend					; If yes, go to the "lend" subroutine
		cmp.b   #'+', R6				; Check to see if R6 is the '+' character
		jeq     add_only				; If yes, go to the "add_only" subroutine
		cmp.b   #'-', R6				; Check to see if R6 is the '-' character
		jeq     sub_only				; If yes, go to the "sub_only" subroutine
		mov.b   R6, R5					; Puts the first character of the string into R5,
										; a positive integer is expected.
										; The program only reaches this statement once,
										; because otherwise a subroutine is called.
		jmp     gnext					; Go to the next character

add_only
		mov.b   @R4+, R6				; Get a new character, put it into R6, and increment R4
		add     R6, R5					; Perform addition and store result in R5
		sub     #48, R5					; This is necessary to keep the "running total" correct
		jmp     gnext					; Go to the next character

sub_only
		mov.b   @R4+, R6				; Get a new character, put it into R6, and increment R4
		sub     R6, R5					; Perform subtraction and store result in R5
		add     #48, R5					; This is necessary to keep the "running total" correct
		jmp     gnext					; Go to the next character

lend:	sub     #48, R5					; Convert from ASCII to decimal value
		mov.b   R5, &P2OUT				; Write result to P2OUT (not visible on port pins)
		nop								; Required only for debugger

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
