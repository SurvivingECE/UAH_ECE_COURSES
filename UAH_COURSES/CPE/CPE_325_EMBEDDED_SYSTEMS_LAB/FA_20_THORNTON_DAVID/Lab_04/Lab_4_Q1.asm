;-------------------------------------------------------------------------------------------------------
; File: Lab_4_Q1.asm
; Description: Counts the number of words and sentences in a string
; Input: Hard coded string
; Output: R5 and R7 contain word and sentence count respectively
; Author: David Thornton
; Lab Section: 2
; Date: September 22, 2020
;-------------------------------------------------------------------------------------------------------
		.cdecls C,LIST,"msp430.h"		; Include device header file
;-------------------------------------------------------------------------------------------------------
		.def    RESET					; Export program entry-point to make it known to linker
		.data 							; Declare variable in a data segment
myStr:  .cstring "This is a test! Is CPE 325 fun? Depends on who you ask."
w_count: 	.int 	0 					; Given word count variable
sent_count:	.int 	0 					; Given sentence count variable
										; Expected: words = 13, sentences = 3
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
		clr.b   R5						; R5 is the counter for words
		clr.b   R7						; R7 is the counter for sentences

gnext:  mov.b   @R4+, R6				; Get a new character and put it into R6
		cmp     #0, R6				   	; Check to see if it is NULL
		jeq     lend					; If yes, go to the "lend" subroutine
		cmp.b   #' ', R6				; Check to see if R6 is the ' ' character
		jeq     word_count				; If yes, go to the "word_count" subroutine
		cmp.b   #'.', R6				; Check to see if R6 is the '.' character
		jeq     sentence_count			; If yes, go to the "sentence_count" subroutine
		cmp.b   #'!', R6				; Check to see if R6 is the '!' character
		jeq     sentence_count			; If yes, go to the "sentence_count" subroutine
		cmp.b   #'?', R6				; Check to see if R6 is the '?' character
		jeq     sentence_count			; If yes, go to the "sentence_count" subroutine
		jmp     gnext					; Go to the next character

word_count
		inc.w R5						; Increment word counter
		jmp     gnext					; Go to the next character

sentence_count
		inc.w R7						; Increment sentence counter
		jmp     gnext					; Go to the next character

lend:	inc.w R5 						; Since the end of the sentence does not end with a space,
										; you have to add one to the word count.
		mov.w   R5, &w_count			; The memory address of w_count gets the contents of R5
		mov.w   R7, &sent_count			; The memory address of sent_count gets the contents of R7
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
