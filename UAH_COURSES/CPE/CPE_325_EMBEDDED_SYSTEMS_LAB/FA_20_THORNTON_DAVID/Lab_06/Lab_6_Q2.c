/*------------------------------------------------------------------------------
* File: 		Lab_6_Q2.c
* Description:	This program varies the clock frequency depending on S1 and S2.
* Input:		S1 and S2 on MSP-EXP430F5529LP
* Output:		Blinking LEDs 1 and 2 at various frequencies
* Author: 		David Thornton
* Lab Section: 	2
* Date:			October 12, 2020
* *---------------------------------------------------------------------------*/
#include  <msp430.h>

#define S1 ((P2IN & BIT1) == 0)		// Red LED switch
#define S2 ((P1IN & BIT1) == 0)		// Green LED switch
#define REDLED 0x01					// LED1 - Mask for BIT0 (0000_0001b)
#define GREENLED 0x80				// LED2 - Mask for BIT7 (1000_0000b)

void configure_clock_sources();		// Function prototype: Configure clock
inline void _1Mhz();				// Function prototype: Change CF to 1 Mhz
inline void _2Mhz();				// Function prototype: Change CF to 2 Mhz
inline void _4Mhz();				// Function prototype: Change CF to 4 Mhz
inline void _8Mhz();				// Function prototype: Change CF to 8 Mhz

int status = 1;						// Status to check blink frequency

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;		// Stopping the watchdog timer

	P1DIR &= ~BIT1;					// Set P1.1 as input (S2)
	P1REN |= BIT1;					// Enable pull-up resistor
	P1OUT |= BIT1;					// Turn P1 output on

	P2DIR &= ~BIT1;					// Set P2.1 as input (S1)
	P2REN |= BIT1;					// Enable pull-up resistor
	P2OUT |= BIT1;					// Turn P2 output on

	_EINT();						// Enable interrupts
	P1IE  |= BIT1;					// Enable interrupt at P1.1 for S1
	P1IES |= BIT1;					// Enable hi->lo edge for interrupt
	P1IFG &= ~BIT1;					// Clear any errornous interrupt flag

	P2IE  |= BIT1;					// Enable interrupt at P2.1 for S2
	P2IES |= BIT1;					// Enable hi->lo edge for interrupt
	P2IFG &= ~BIT1;					// Clear any errornous interrupt flag

	configure_clock_sources();		// Configure the clock sources
	_1Mhz();						// Set initial blinking to 1 Mhz

	P1DIR |= REDLED;				// Configure P1.0 as output
	P4DIR |= GREENLED;				// Configure P4.7 as output

	P1OUT = P1OUT | REDLED;			// Turn on REDLED
	P4OUT = P4OUT & ~GREENLED;		// Turn off GREENLED

	while(1)						// Infinite loop
	{
		P1OUT ^= REDLED;			// Toggle P1.0
		P4OUT ^= GREENLED;			// Toggle P4.7
		__delay_cycles(500000);		// Delay of 250ms when CF is 1 Mhz
	}
}


#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)	// ISR to handle S2 press (decrease CF)
{
	P1IFG &= ~BIT1; 				// Clear the interrupt flag
	__delay_cycles(25000);

	if(S2 != 0)						// If S2 is not pressed
		return;						// Return

	if(status == 8)					// Are LEDs blinking at 8 Mhz?
	{
		_4Mhz();			 		// Decrease to 4Mhz
		status = 4;					// Adjust status
	}
	else if(status == 4)			// Are LEDs blinking at 4 Mhz?
	{
		_2Mhz();			 		// Decrease to 2Mhz
		status = 2;					// Adjust status
	}
	else if(status == 2)			// Are LEDs blinking at 2 Mhz?
	{
		_1Mhz();			 		// Decrease to 1Mhz
		status = 1;					// Adjust status
	}
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)	// ISR to handle S1 press (increase CF)
{
	P2IFG &= ~BIT1; 				// Clear the interrupt flag
	__delay_cycles(25000);

	if(S1 != 0)						// If S1 is not pressed
		return;						// Return

	if(status == 4)					// Are LEDs blinking at 4 Mhz?
	{
		_8Mhz();			 		// Increase to 8Mhz
		status = 8;					// Adjust status
	}
	else if(status == 2)			// Are LEDs blinking at 2 Mhz?
	{
		_4Mhz();			 		// Increase to 4Mhz
		status = 4;					// Adjust status
	}
	else if(status == 1)			// Are LEDs blinking at 1 Mhz?
	{
		_2Mhz();			 		// Increase to 2Mhz
		status = 2;					// Adjust status
	}
}


void _1Mhz() 						// Change the clock frequency to 1 Mhz
{
	__bis_SR_register(SCG0);		// Disable the FLL control loop
	UCSCTL1 = DCORSEL_3;			// Select DCO range _1Mhz operation
	UCSCTL2 = 32;					// Set DCO Multiplier for 1Mhz
									// (N + 1) * FLLRef = Fdco
									// (32 + 1) * 32768 = 1Mhz
	__bic_SR_register(SCG0);		// Enable the FLL control loop
	__delay_cycles(33792);			// 32 x 32 x 1 MHz / 32,768 Hz
									// 33792 = MCLK cycles for DCO to settle
}


void _2Mhz() 						// Change the clock frequency to 2 Mhz
{
	__bis_SR_register(SCG0);		// Disable the FLL control loop
	UCSCTL1 = DCORSEL_4;			// Select DCO range 2Mhz operation,
	UCSCTL2 = 62;					// Set DCO Multiplier for 2Mhz
									// (N + 1) * FLLRef = Fdco
									// (62 + 1) * 32768 = 2Mhz
	__bic_SR_register(SCG0);		// Enable the FLL control loop
	__delay_cycles(62500);			// 32 x 32 x 2 MHz / 32,768 Hz
									// 62500 = MCLK cycles for DCO to settle
}


void _4Mhz() 						// Change the clock frequency to 4 Mhz
{
	__bis_SR_register(SCG0);		// Disable the FLL control loop
	UCSCTL1 = DCORSEL_4;			// Select DCO range 4 Mhz operation
	UCSCTL2 = 124;					// Set DCO Multiplier for 4 Mhz
									// (N + 1) * FLLRef = Fdco
									// (124 + 1) * 32768 = 4 Mhz
	__bic_SR_register(SCG0);		// Enable the FLL control loop
	__delay_cycles(125000);			// 32 x 32 x 4 MHz / 32,768 Hz
									// 125000 = MCLK cycles for DCO to settle
}


void _8Mhz() 						// Change the clock frequency to 8 Mhz
{
	__bis_SR_register(SCG0);		// Disable the FLL control loop
	UCSCTL1 = DCORSEL_5;			// Select DCO range 8 Mhz operation
	UCSCTL2 = 249;					// Set DCO Multiplier for 8 Mhz
									// (N + 1) * FLLRef = Fdco
									// (249 + 1) * 32768 = 8 Mhz
	__bic_SR_register(SCG0);		// Enable the FLL control loop
	__delay_cycles(250000);			// 32 x 32 x 8 MHz / 32,768 Hz
									// 250000 = MCLK cycles for DCO to settle
}


void configure_clock_sources()
{
	UCSCTL3 = SELREF_2;				// Set DCO FLL reference = REFO
	UCSCTL4 |= SELA_2;				// Set ACLK = REFO
	UCSCTL0 = 0x0000;				// Set lowest possible DCOx, MODx

	// Loop until XT1, XT2, and DCO stabilizes.
	// In this case only DCO has to stabilize.
	do
	{
		UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);	// Clear XT2, XT1, DCO flags
		SFRIFG1 &= ~OFIFG;							// Clear fault flags
	}
	while (SFRIFG1 & OFIFG);			// Test oscillator fault flag
}
