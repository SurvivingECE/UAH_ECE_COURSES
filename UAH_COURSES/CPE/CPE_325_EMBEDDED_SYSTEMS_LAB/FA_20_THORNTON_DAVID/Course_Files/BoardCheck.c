/*------------------------------------------------------------------------------
 * File:        CheckBoard.c
 * Function:    Makes sure the board can function properly
 * Description: In this C program, we blink the LEDs based on which Switch is held
 *              Holding SW1 blinks LED1 with WDT
 *              Holding SW2 blinks LED2 with TimerA
 * Input:       SW1 and SW2
 * Output:      LED1 and LED2
 * Author(s):   Aleksandar Milenkovic, milenkovic@computer.org
 *              Amirahmad Ramezani, ar0070@uah.edu
 * Date:        November 2020
 * ---------------------------------------------------------------------------*/
#include <msp430F5529.h>
#include <stdbool.h>

bool LED1_Enabled = false;
bool LED2_Enabled = false;

void main(void)
{
    //Configure the Timers
    WDTCTL = WDT_MDLY_32;       //32s interval
    SFRIE1 |= WDTIE;            //Enable WDT interrupt
    TA0CCTL0 = CCIE;            //Enabled TimerA interrupt
    TA0CCR0 = 3277;             // 3277 / 32768 Hz = 0.1s
    TA0CTL = TASSEL_1 + MC_1;   // ACLK, up mode

    //Configure LEDs
    P1DIR |= 0x01;              //Configure LED1 as output
    P4DIR |= 0x80;              //Configure LED2 as output
    P1OUT |= 0x01;              //Turn LED1 on
    P4OUT |= 0x80;              //Turn LED2 on

    //Configure the Switches
    P2DIR &= ~BIT1;             //Set the direction at SW1 as input
    P2REN |= BIT1;              //Enable Pull-up resistor
    P2OUT |= BIT1;              //Required for proper IO
    P2IE |= BIT1;               //SW1 interrupt enabled
    P2IES |= BIT1;              //SW1 hi/low edge
    P2IFG &= ~BIT1;             //SW1 interrupt flag is cleared
    P1DIR &= ~BIT1;             //Set the direction at SW2 as input
    P1REN |= BIT1;              //Enable Pull-up resistor
    P1OUT |= BIT1;              //Required for proper IO
    P1IE |= BIT1;               //SW2 interrupt enabled
    P1IES |= BIT1;              //SW2 hi/low edge
    P1IFG &= ~BIT1;             //SW2 interrupt flag is cleared

    _BIS_SR(LPM0_bits + GIE);   // Enter Low Power Mode 0
}

//SW1 Interrupt Service Routine
#pragma vector = PORT2_VECTOR
__interrupt void Port2_ISR (void)
{
    P2IFG &= ~BIT1;         //SW1 interrupt flag is cleared
    LED1_Enabled ^= true;   //Toggle LED1 flag
    P2IES ^= BIT1;          //Toggle SW1 edge select
}

//SW2 Interrupt Service Routine
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR (void)
{
    P1IFG &= ~BIT1;         //SW1 interrupt flag is cleared
    LED2_Enabled ^= true;   //Toggle LED1 flag
    P1IES ^= BIT1;          //Toggle SW1 edge select
}

//Watchdog Timer Interrupt Service Routine
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    if(LED1_Enabled)
        P1OUT ^= 0x01;
}

//TimerA Interrupt Service Routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void timerA_isr()
{
    if(LED2_Enabled)
        P4OUT ^= 0x80;
}
