/*---------------------------------------------------------
* File: Lab3.c
* Description: Uses S1 and S2 to blink LED's
* Input: Buttons on MSP-EXP430F5529LP
* Output: LED lights blinking
* Author: David Thornton
* Lab Section: 2
* Date: September 15, 2020
* *------------------------------------------------------*/

#include <msp430.h>
#define S1 ((P2IN & BIT1) == 0)
#define S2 ((P1IN & BIT1) == 0)
#define REDLED 0x01                     // LED1 - Mask for BIT0 (0000_0001b)
#define GREENLED 0x80                   // LED2 - Mask for BIT7 (1000_0000b)

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;           // Stop watchdog timer

    P1DIR |= REDLED;                    // Set P1.0 to output direction (0000_0001b)
    P1REN |= BIT1;                      // Enable the pull-up resistor at P1.1
    P1OUT |= BIT1;                      // Turn P1 output on

    P4DIR |= GREENLED;                  // Set P4.7 to output direction (1000_0000b)
    P2REN |= BIT1;                      // Enable the pull-up resistor at P2.1
    P2OUT |= BIT1;                      // Turn P2 output on

    unsigned int i = 0;                 // Loop counter
    while(1)                            // Infinite loop
    {
        if (S1 && S2)                   // If S1 and S2 are both pressed
        {
            for (i = 2000; i > 0; i--); // Debounce 20 ms
            P4OUT |= GREENLED;          // Turn on GREENLED
            P1OUT &= ~REDLED;           // Clear REDLED
            while(S1 && S2)             // Blink REDLED and GREENLED alternately at 8Hz (0.125 s)
            {
                __delay_cycles(62500);  // Delay of ~0.0625 s (0.125 s / 2)
                P4OUT ^= GREENLED;      // Toggle GREENLED
                P1OUT ^= REDLED;        // Toggle REDLED
            }
        }
        else if (S1)                    // If only S1 is pressed
        {
            for (i = 2000; i > 0; i--); // Debounce 20 ms
            while (S1 && !S2)
            {
                P4OUT &= ~GREENLED;     // Turn GREENLED off
                __delay_cycles(250000); // Delay of ~0.5 s (2Hz)
                P1OUT ^= REDLED;        // Toggle REDLED
            }
        }
        else if (S2)                    // If only S2 is pressed
        {
            for (i = 2000; i > 0; i--); // Debounce 20 ms
            while(S2 && !S1)
            {
                P1OUT |= REDLED;        // Turn REDLED on
                __delay_cycles(100000); // Delay of ~0.2 s (5Hz)
                P4OUT ^= GREENLED;      // Toggle GREENLED
            }
        }
        else
        {
        P4OUT |= GREENLED;              // GREENLED on at begining of program
        P1OUT &= ~REDLED;               // Clear REDLED
        }
    }
}

/*
*   The processor clock frequency is approximately 1 MHz
*   1/frequency = seconds  1/1MHz = 1 µs = 1000 ms
*   2Hz = 1/2 s = 0.5 s   = 500 ms = 500 * 1000 cycles / 2 = 250000 cycles
*   5Hz = 1/5 s = 0.2 s   = 200 ms = 200 * 1000 cycles / 2 = 100000 cycles
*   8Hz = 1/8 s = 0.125 s = 125 ms = 125 * 1000 cycles / 2 = 62500 cycles
*   */
