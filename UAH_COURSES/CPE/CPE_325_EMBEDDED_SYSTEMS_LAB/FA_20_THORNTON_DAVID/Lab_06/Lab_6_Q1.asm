;-------------------------------------------------------------------------------
; File: 		Lab_6_Q1.asm
; Description:	S2 toggles GREEN_LED, S1 also toggles GREEN_LED, but after
;				blinking RED_LED 3 times at 1Hz.
; Input:		S1 and S2 on MSP-EXP430F5529LP
; Output:		Blinking LEDs 1 and 2
; Author: 		David Thornton
; Lab Section: 	2
; Date:			October 12, 2020
;-------------------------------------------------------------------------------
			.cdecls C,LIST,"msp430.h"		; Include device header file
;-------------------------------------------------------------------------------
			.def    RESET                   ; Export program entry-point to
											; make it known to linker.
			.def	S1_ISR
			.def	S2_ISR
;-------------------------------------------------------------------------------
			.text                           ; Assemble into program memory.
			.retain                         ; Override ELF conditional linking
											; and retain current section.
			.retainrefs                     ; And retain any sections that have
											; references to current section.
;-------------------------------------------------------------------------------
RESET:       mov.w   #__STACK_END,SP		; Initialize stack pointer
StopWDT:     mov.w   #WDTPW|WDTHOLD,&WDTCTL	; Stop watchdog timer
;-------------------------------------------------------------------------------
; Main loop here
;-------------------------------------------------------------------------------
Setup:
			bis.b	#0x01, &P1DIR			; Set P1.0 as output (RED_LED)
			bis.b	#0x80, &P4DIR			; Set P4.7 as output (GREEN_LED)
			bic.b	#0x01, &P1OUT			; Turn RED_LED off at start
			bic.b	#0x80, &P4OUT			; Turn GREEN_LED off at start

			; S2 input and output
			bic.b	#0x02, &P2DIR			; Set P2.1 as input for S1
			bis.b	#0x02, &P2REN			; Enable pull-up resistor at P2.1
			bis.b	#0x02, &P2OUT			; Required for proper IO set up

			; S1 input and output
			bic.b	#0x02, &P1DIR			; Set P1.1 as input for S2
			bis.b	#0x02, &P1REN			; Enable pull-up resistor at P1.1
			bis.b	#0x02, &P1OUT			; Required for proper IO set up

			bis.w	#GIE,  SR				; Enable global interrupts
			bis.b	#0x02, &P1IE			; Enable port 1 interrupt from bit 1
			bis.b	#0x02, &P1IES			; Set interrupt for high to low
			bic.b	#0x02, &P1IFG			; Clear interrupt flag
			bis.b	#0x02, &P2IE			; Enable port 2 interrupt from bit 1
			bis.b	#0x02, &P2IES			; Set interrupt for high to low
			bic.b	#0x02, &P2IFG			; Clear interrupt flag

Start:		cmp		#1, R5					; Check if R5 = 1
			jne		Red_Press				; If R5 != 1, jump to Red_Press
			clr		R5						; Clear R5
			xor.b	#0x01, &P1OUT			; Toggle P1.0 (RED_LED)

Red_Press:	cmp		#1, R6					; Check if R6 = 1
			jne 	Inf_Loop				; If R6 != 1, jump to Inf_Loop
			clr		R6						; Clear R6
			mov		#6, R5					; R5 <- 6

Cycle:		mov		#0xFFFF, R7				; R7 <- 0xFFFF

Delay:		dec		R7						; Decrement R7
			nop
			nop
			nop
			nop
			nop
			jnz		Delay					; If R7 != 0, jump to Delay
			xor.b	#0x01, &P1OUT			; Toggle P1.0 (RED_LED)
			dec		R5						; Decrement R5
			jnz 	Cycle					; If R5 != 0, jump to Cycle
			bit.b	#0x01, &P1OUT			; P1 AND 1
			xor.b	#0x80, &P4OUT			; Toggle P4.7 (GREEN_LED)
			jz		Inf_Loop				; If P4.7 changes, jump to Inf_Loop

Inf_Loop:	jmp		Start					; Loop until interrupt occurs


;-------------------------------------------------------------------------------
; P1_0 (RED_LED) / P2_1 (S1) ISR
;-------------------------------------------------------------------------------
S1_ISR:
			bic.b	#0x02, &P2IFG			; Clear interrupt flag
			bit.b	#0x02, &P2IN			; Check if S1 is pressed
			jnz		Red_Exit				; If SW is not pressed, jump to exit
			mov.b	#2000, R7				; Set to (2000 * 10 cc = 20,000 cc)

RedDelay:	dec		R7						; Decrement R7
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			jnz 	RedDelay				; Is R7 = 0? (Delay over?)
			bit.b	#0x02, &P2IN			; Verify S1 is still pressed
			jnz		Red_Exit			 	; If not, wait for S1 press
			mov.b   #1, R6					; R6 <- 1

Red_Exit:	reti					 		; Return from interrupt


;----------------------------------------------------------------------------
; P4_7 (GREEN_LED) / P1_1 (S2) ISR
;----------------------------------------------------------------------------
S2_ISR:
			bic.b	#0x02, &P1IFG			; Clear interrupt flag
			bit.b	#0x02, &P1IN			; Check if S2 is pressed
			jnz		Green_Exit				; If SW is not pressed, jump to exit
			xor.b	#0x80, &P4OUT			; Toggle P4.7 (GREEN_LED)
			mov.b	#2000, R7				; Set to (2000 * 10 cc = 20,000 cc)

GreenDelay: dec		R7						; Decrement R7
			nop
			nop
			nop
			nop
			nop
			nop
			nop
			jnz 	GreenDelay				; Is R7 = 0? (Delay over?)
			bit.b	#0x02, &P1IN			; Verify S2 is still pressed
			jnz		Green_Exit			 	; If not, wait for S2 press
			mov.b 	#1, R7					; R7 <- 1

Green_Exit:	reti					 		; Return from interrupt


;-------------------------------------------------------------------------------
; Stack Pointer definition
;-------------------------------------------------------------------------------
			.global __STACK_END
			.sect   .stack

;-------------------------------------------------------------------------------
; Interrupt Vectors
;-------------------------------------------------------------------------------
			.sect   ".reset"				; MSP430 RESET Vector
			.short  RESET
			.sect   ".int47"				; PORT2_VECTOR
			.short  S2_ISR
			.sect   ".int42"				; PORT1_VECTOR
			.short  S1_ISR
			.end
