;-------------------------------------------------------------------------------
; File: Lab_5_SW_Mult.asm
; Description: This subroutine calculates the first five powers of B using
;			   a shift and add multiplication algorithm.
; Input: Integers A, B
; Output: Integer product of A and B, C
; Author: David Thornton (dht0002@uah.edu)
; Lab Section: 2
; Date: September 30, 2020
;-------------------------------------------------------------------------------
			.cdecls C,LIST,"msp430.h"      ; Include device header file
			.def SW_Mult
			.text

SW_Mult:
			mov #16, R9 					; max bit counter per assignment
			mov #0, R10 					; clear result variable (C)
			mov R4, R11 					; copy b to A
			jmp check

check:
			mov R12, R13 					; temp so we don't overwrite B
			and #0x01, R13					; bitwise and temp (B) and 0x01
			cmp #0x01, R13 					; compare LSB of temp to 1
			jne skip						; if LSB of B is not 1, jump to skip
			add R11, R10					; add A to C
			jmp skip						; jump to skip subroutine

skip:
			rrc R12							; rotate right B
			rla R11							; rotate left A
			dec R9 							; bit counter--
			jnz check						; jump to check if bit counter >0
			mov R12, R13					; temp so we don't overwrite B
			and #0x10, R13					; bitwise and temp (B) and 0x10
			cmp #0x10, R13 					; compare MSB of temp to 1
			jeq neg							; jump if temp is negative
			jmp qend						; else jump to qend

neg:
			sub R11, R10					; subtract A from C
			jmp qend						; jump to qend

qend:
			mov R10, 0(R5)					; move result into the SW array
			mov R10, R12 					; move result into B for next loop
			ret								; return from subroutine
			.end
