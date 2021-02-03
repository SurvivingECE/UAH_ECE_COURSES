/*---------------------------------------------------------
* File: Lab1_P1.c
* Description: Prints the first 10 lines of a
*              multiplication table for an unsigned integer.
* Input: Unsigned integer
* Output: Multiplication table
* Author: David Thornton
* Lab Section: 2
* Date: August 25, 2020
* *------------------------------------------------------*/
#include <stdio.h> // For printf
#include <msp430.h>

void print_multiplication_table(unsigned int); // Function prototype

int main()
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer to prevent time out reset
	const unsigned int a = 25;
	print_multiplication_table(a);
	return 0;
}

void print_multiplication_table(unsigned int any_int)
{
    int i;
    for (i = 1; i <= 10; i++)
    {
        int result = any_int * i;
        printf("%d X %d = %d \n", any_int, i, result);
    }

}
