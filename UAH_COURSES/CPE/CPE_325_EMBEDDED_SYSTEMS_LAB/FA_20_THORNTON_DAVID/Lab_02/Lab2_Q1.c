/*---------------------------------------------------------
* File: Lab2_Q1.c
* Description: C program will print the sizes and ranges
*              of common data types.
* Input: float.h, limits.h
* Output: Data type sizes and ranges
* Author: David Thornton
* Lab Section: 2
* Date: September 7, 2020
* *------------------------------------------------------*/
#include <stdio.h> // For printf
#include <msp430.h>
#include <limits.h> // For min/max variables
#include <float.h> // For min/max variables

#define ZERO 0 // Eliminate magic numbers

int main()
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer to prevent time out reset
    const char hyphen = '-'; int i = 0;
    for(i = 1; i <= 76; i++) { putchar(hyphen); } printf("\n"); // Prints 76 '-'
    printf("| Data Type          | Bytes |        Minimum       |       Maximum        |\n");
    for(i = 1; i <= 76; i++) { putchar(hyphen); } printf("\n"); // Prints 76 '-'
    printf("| char               | %3d   | %20d | %20d |\n", sizeof(char), SCHAR_MIN, SCHAR_MAX);
    printf("| short              | %3d   | %20hd | %20hd |\n", sizeof(short int), SHRT_MIN, SHRT_MAX);
    printf("| int                | %3d   | %20d | %20d |\n", sizeof(int), INT_MIN, INT_MAX);
    printf("| long               | %3d   | %20ld | %20ld |\n", sizeof(long int), LONG_MIN, LONG_MAX);
    printf("| long long          | %3d   | %20lld | %20lld |\n", sizeof(long long int), LLONG_MIN, LLONG_MAX);
    printf("| unsigned char      | %3d   | %20d | %20u |\n", sizeof(unsigned char), ZERO, UCHAR_MAX);
    printf("| unsigned short     | %3d   | %20d | %20hu |\n", sizeof(unsigned short int), ZERO, USHRT_MAX);
    printf("| unsigned int       | %3d   | %20d | %20u |\n", sizeof(unsigned  int), ZERO, UINT_MAX);
    printf("| unsigned long      | %3d   | %20d | %20lu |\n", sizeof(unsigned long int), ZERO, ULONG_MAX);
    printf("| unsigned long long | %3d   | %20d | %20llu |\n", sizeof(unsigned long long int), ZERO, ULLONG_MAX);
    printf("| float              | %3d   | %20g | %20g |\n", sizeof(float), FLT_MIN, FLT_MAX);
    printf("| double             | %3d   | %20g | %20g |\n", sizeof(double), DBL_MIN, DBL_MAX);
    for(i = 1; i <= 76; i++) { putchar(hyphen); } // Prints 76 '-'
    return 0;
}

/* Calculate the range of the data type for a given size.
 * Number of Bytes * 8 = Number of Bits
 * Size = 2 ^ (bits)
 */
