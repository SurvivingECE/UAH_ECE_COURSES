;-------------------------------------------------------------------------------
; File: Lab_5_calc_power.asm
; Description: This subroutine calls the SW and HW subroutines
; Input: None
; Output: None
; Author: David Thornton (dht0002@uah.edu)
; Lab Section: 2
; Date: September 30, 2020
;-------------------------------------------------------------------------------
			.cdecls C,LIST,"msp430.h"		; Include device header file
			.def 	calc_power
			.ref	HW_Mult
			.ref	SW_Mult
			.text

calc_power:
			mov 6(SP), R5 					; SW array address
			mov 4(SP), R6 					; HW array address
			mov 2(SP), R7 					; result variable
			mov #1, R12 					; B, not b
			jmp SW_Loop

SW_Loop:
			call #SW_Mult					; call subroutine
			incd R5							; go to the next array index
			dec R8							; counter--
			jnz SW_Loop						; jump if the counter is not zero
			mov #5, R8						; reset counter for the HW loop

HW_Loop:
			call #HW_Mult					; call subroutine
			incd R6							; go to the next array index
			dec R8							; counter--
			jnz HW_Loop						; jump if the counter is not zero

lend       ret								; return from subroutine
			.end
