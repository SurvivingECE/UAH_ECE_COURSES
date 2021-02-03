/*---------------------------------------------------------
* File: 		Lab_7_Q1.c
* Description:	This program increases and decreases the
* 				brightness of LED1 at a frequency of .16 Hz
* Input:		S1 and S2
* Output:		LED1 gradually changes intensity, and can
* 				pause depending on button presses.
* Author: 		David Thornton
* Lab Section: 	2
* Date: 		October 21, 2020
* *------------------------------------------------------*/
#include <msp430.h>
#define SW1 ((P2IN&BIT1) == 0)	// Define switch 1
#define SW2 ((P1IN&BIT1) == 0)	// Define switch 2
#define REDLED 0x01				// LED1 - Mask for BIT0 (0000_0001b)

void main(void)
{
	_EINT();					// Enable interrupts
	WDTCTL = WDT_MDLY_32;		// Set interval to 32 milliseconds
	SFRIE1 |= WDTIE;

	P1DIR |= REDLED;			// Set LED1 as output
	P1OUT &= ~REDLED;			// Turn LED1 off at start

	P2DIR &= ~BIT1;				// SW1 direction
	P2REN |= BIT1;				// Enable the pull-up resistor at P2.1
	P2OUT |= BIT1;				// Set SW1 output to on

	P1DIR &= ~BIT1;				// SW2 direction
	P1REN |= BIT1;				// Enable the pull-up resistor at P1.1
	P1OUT |= BIT1;				// Set SW2 output to on

	TA0CCTL0 = CCIE;			// TA0 count triggers interrupt
	TA0CCR0 = 96;				// Set TA0 count value

	TA0CCTL1 = CCIE;			// TA0.1 count triggers interrupt
	TA0CCR1 = 96;				// Set TA0.1 count value

	TA0CTL = TASSEL_1 | MC_3;	// ACLK is clock source, UP/DOWN mode

	int i = 0;					// Used in for loops for debouncing
	while(1)
	{
		if(SW1)
		{
			for(i = 2000; i >= 0; i--); // Debounce
			if (SW1)
			{
				WDTCTL = WDTPW + WDTHOLD; // Stop WDT
			}
		}
		if(SW2)
		{
			for(i = 2000; i >= 0; i--); // Debounce
			if(SW2)
			{
				WDTCTL = WDT_MDLY_32; 	// Resume interval
			}
		}
	}
}


#pragma vector = WDT_VECTOR
__interrupt void wdtISR(void)
{
	static int counter = 96;
	static int pwm = 1;
	if(pwm == 0 )
	{
		if(counter == 96)
		{
			pwm = 1;
		}
		counter++;
		TA0CCR1 = counter;
	}
	if(pwm == 1)
	{
		if(counter == 0)
		{
			pwm = 0;
		}
		counter--;
		TA0CCR1 = counter;
	}
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void timerISR(void)
{
	P1OUT |= REDLED;
	TA0CCTL0 &= ~CCIFG;
}


#pragma vector = TIMER0_A1_VECTOR
__interrupt void timerISR2(void)
{
	P1OUT &= ~REDLED;
	TA0CCTL1 &= ~CCIFG;
}
