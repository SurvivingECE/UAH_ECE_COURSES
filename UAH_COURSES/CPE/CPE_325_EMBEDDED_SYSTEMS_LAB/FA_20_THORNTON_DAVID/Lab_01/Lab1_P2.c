/*---------------------------------------------------------
* File: Lab1_P1.c
* Description: Counts the number of upper case, lower case,
*              and numbers (0-9) in a string.
* Input: Unsigned integer
* Output: Multiplication table
* Author: David Thornton
* Lab Section: 2
* Date: August 25, 2020
* *------------------------------------------------------*/
#include <stdio.h> // For printf
#include <string.h>
#include <msp430.h>

int main()
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer to prevent time out reset
    const char input[] = "Hello! Welcome to CPE325. It’s a great day, isn’t it?";
    unsigned int i = 0; int lowerCase = 0; int upperCase = 0; int number = 0;
    for(i = 0; i<=strlen(input); i++)
    {
        if(input[i] >= 48 && input[i] <= 57)
            number++;
        if(input[i] >= 65 && input[i] <= 90)
            upperCase++;
        if(input[i] >= 97 && input[i] <= 122)
            lowerCase++;
    }
    printf("The string contains %d upper case and %d lower case characters and %d digits.", upperCase, lowerCase, number);
    return 0;
}
