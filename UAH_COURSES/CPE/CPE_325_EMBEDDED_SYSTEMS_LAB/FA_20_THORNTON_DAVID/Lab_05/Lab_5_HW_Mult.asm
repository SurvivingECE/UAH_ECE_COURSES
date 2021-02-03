;-------------------------------------------------------------------------------
; File: Lab_5_HW_Mult.asm
; Description: This subroutine uses a hardware multiplier to multiply numbers
; Input: Integers b, result
; Output: Integer product of b and result
; Author: David Thornton (dht0002@uah.edu)
; Lab Section: 2
; Date: September 30, 2020
;-------------------------------------------------------------------------------
			.cdecls C,LIST,"msp430.h"      ; Include device header file
			.def HW_Mult
			.text

HW_Mult:
			mov		R4, &MPY			; move b into multiplication register
			mov		R7, &OP2			; move result to general purpose operator
			nop
			nop
			nop
			mov		RESLO, R7			; move the product to R7 (result)
			mov		R7, 0(R6)			; move result into the HW array
			ret							; return from subroutine
			.end
