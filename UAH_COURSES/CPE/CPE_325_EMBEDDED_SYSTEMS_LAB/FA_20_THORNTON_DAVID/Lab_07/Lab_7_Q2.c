/*---------------------------------------------------------
* File: 		Lab_7_Q2.c
* Description:	This program uses the WDT to blink LED2
* Input:		None
* Output:		Blinking LED2
* Author: 		David Thornton
* Lab Section: 	2
* Date: 		October 21, 2020
* *------------------------------------------------------*/
#include  <msp430.h>

#define GREENLED 0x80				// LED2 - Mask for BIT7 (1000_0000b)

void main(void)
{
	WDTCTL = WDT_MDLY_0_5;			// 0.5 ms interrupt
	P4DIR |= GREENLED;				// Configure the P4.7 as output.
	_EINT();						// Enable interrupts
	SFRIE1 |= WDTIE;				// Enable WDT interrupt

	P2IE  |= BIT1;					// Enable interrupt at P2.1 for S2
	P2IES |= BIT1;					// Enable hi->lo edge for interrupt
	P2IFG &= ~BIT1;					// Clear any errornous interrupt flag

	_BIS_SR(LPM0_bits + GIE);		// Enter LPM0 with interrupt
}


// SW1 key press ISR
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
	P2IFG &= ~BIT1;					// Clear the flag
	P4OUT ^= GREENLED;				// Toggle LED
}


// WDT ISR
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
	static int i = 0;
	i++;
	if (i == 1000)
	{
		P2IFG |= BIT1;	// Clear the flag
		i = 0;			// Reset i
	}
}
