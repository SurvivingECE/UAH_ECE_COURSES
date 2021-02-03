/*---------------------------------------------------------
* File: 		Lab_9.c
* Description:	This program measures acceleration
* Input:		ADXL335 accelerometer
* Output:		Blinking LEDs and data in UAH serial app
* Author: 		David Thornton
* Lab Section: 	2
* Date: 		November 11, 2020
* *------------------------------------------------------*/
#include <msp430.h>
#include <math.h>
#define RED 0x01						// Red LED
#define GREEN 0x80						// Green LED
#define S1 ((P2IN&BIT1) == 0)			// Switch 1 push defined
#define S2 ((P1IN&BIT1) == 0)			// Switch 2 push defined

// Global variables for later use
volatile long int ADCXval, ADCYval, ADCZval = -1;
volatile float XDir, YDir, ZDir, netG = -1;

void TimerA_setup(void);
void ADC_setup(void);
void UART_putCharacter(char c);
void UART_setup(void);
void sendData(void);

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	__enable_interrupt();		// Enable interrupts globally
	SFRIE1 |= WDTIE;			// Enable WDT interrupt

	P1DIR |= RED;				// P1.0 is output direction for REDLED
	P1REN |= BIT1;		  		// Enable the pull-up resistor at P1.1
	P1OUT &= ~RED;		  		// LED is off at start

	P4DIR |= GREEN;		 		// P4.7 is output direction for GREENLED
	P2REN |= BIT1;		  		// Enable the pull-up resistor at P2.1
	P4OUT &= ~GREEN;			// LED is off at start

	// Configuring switch 1 interrupts
	P2IE  |= BIT1;
	P2IES |= BIT1;
	P2IFG &= ~BIT1;

	// Configuring switch 2 interrupts
	P1IE  |= BIT1;
	P1IES |= BIT1;
	P1IFG &= ~BIT1;

	ADC_setup();	// Setup ADC
	UART_setup();	// Setup UART Comms.
	TimerA_setup();	// Setup Timer A for 10 times a second

	while (1)	// Infinite loop
	{
		__bis_SR_register(LPM0_bits + GIE); // Enter LPM0
		sendData();		// Send out the data
	}
}

void sendData(void)
{
	XDir = ((((ADCXval * 3.3)/4095)-1.65)/.330);
	YDir = ((((ADCYval * 3.3)/4095)-1.65)/.330);
	ZDir = ((((ADCZval * 3.3)/4095)-1.80)/.330);

	// Use character pointers to send one byte at a time
	char *xpointer=(char *)&XDir;
	char *ypointer=(char *)&YDir;
	char *zpointer=(char *)&ZDir;

	unsigned int i;
	UART_putCharacter(0x55);	// Send header

	// Send floats one byte at a time
	for(i = 0; i < 4; i++)
	{
		UART_putCharacter(xpointer[i]);
	}
	for(i = 0; i < 4; i++)
	{
		UART_putCharacter(ypointer[i]);
	}
	for(i = 0; i < 4; i++)
	{
		UART_putCharacter(zpointer[i]);
	}

	// Use vector sum for calculation of netG
	netG = sqrtf((XDir*XDir) + (YDir * YDir) + (ZDir * ZDir));
	if(netG >= 2)
	{
		WDTCTL = WDT_MDLY_0_5;	// 0.5ms interval timer
		P1OUT |= RED;			// Turn on REDLED
		P4OUT &= ~GREEN;		// Make sure GREENLED is off
	}
}


void TimerA_setup(void)
{
	TA0CCTL0 = CCIE;			// Enabled interrupt
	TA0CCR0 = 3277;				// 3277 / 32768 Hz = 0.1s, 10 samples/sec
	TA0CTL = TASSEL_1 + MC_1;	// ACLK, up mode
}


#pragma vector = ADC12_VECTOR
__interrupt void ADC12ISR(void)
{
	ADCXval = ADC12MEM0;					// Move ADC12MEM0 (x) to ADCXval
	ADCYval = ADC12MEM1;					// Move ADC12MEM1 (y) to ADCYval
	ADCZval = ADC12MEM2;					// Move ADC12MEM2 (z) to ADCZval
	__bic_SR_register_on_exit(LPM0_bits);	// Exit LPM0
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void timerA_isr()
{
	ADC12CTL0 |= ADC12SC;
}


// Switch 2 Press
#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P1OUT &= ~RED;			  	// Turn Red LED off
	P4OUT &= ~GREEN;			// Turn Green LED off
	P1IFG &= ~BIT1;
}


// Switch 1 Press
#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	P1OUT &= ~RED;			  	// Turn Red LED off
	P4OUT &= ~GREEN;			// Turn Green LED off
	P2IFG &= ~BIT1;
}


// Watchdog Timer ISR.
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
	static int i = 1;
	if(i == 1000)
	{
		P4OUT ^= GREEN;	// Toggle green LED
		P1OUT ^= RED;	// Toggle the red LED
		i = 1;
	}
	i++;
}


void ADC_setup(void)
{
	P6SEL = 0x07;									// Enable A/D channel inputs for x, y, and z (0000 0111)
	ADC12CTL0 = ADC12ON + ADC12MSC + ADC12SHT0_8;	// Turn on ADC12, extend sampling time to avoid overflow of results
	ADC12CTL1 = ADC12SHP + ADC12CONSEQ_1;			// Use sampling timer, repeated sequence
	ADC12MCTL0 = ADC12INCH_0;						// ref += AVcc, channel = A0
	ADC12MCTL1 = ADC12INCH_1;						// ref += AVcc, channel = A1
	ADC12MCTL2 = ADC12INCH_2 + ADC12EOS;			// ref += AVcc, channel = A2, end sequence
	ADC12IE = 0x02;								 	// Enable ADC12IFG.1
	ADC12CTL0 |= ADC12ENC;						  	// Enable conversions
}


void UART_putCharacter(char c)
{
	while (!(UCA0IFG&UCTXIFG)); // Wait for previous character to transmit
	UCA0TXBUF = c;	// Put character into tx buffer
}


void UART_setup(void)
{
	P3SEL |= BIT3 + BIT4;	// Set USCI_A0 RXD/TXD to receive/transmit data
	UCA0CTL1 |= UCSWRST;	// Set software reset during initialization
	UCA0CTL0 = 0;			// USCI_A0 control register
	UCA0CTL1 |= UCSSEL_2;	// Clock source SMCLK
	UCA0BR0 = 0x09;			// 1048576 Hz  / 115200 lower byte
	UCA0BR1 = 0x00;			// upper byte
	UCA0MCTL |= UCBRS0;		// Modulation (UCBRS0=0x01, UCOS16=0)
	UCA0CTL1 &= ~UCSWRST;	// Clear software reset to initialize USCI state machine
}
