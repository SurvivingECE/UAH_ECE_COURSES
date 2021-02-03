/*---------------------------------------------------------
* File: Lab2_Q4.c
* Description: This programs performs matrix multiplication
*              on two 8x8 matrices.
* Input: Two random 8x8 matrices
* Output:
* Author: David Thornton
* Lab Section: 2
* Date: September 7, 2020
* *------------------------------------------------------*/
#include <time.h> // For srand()
#include <stdio.h> // For printf
#include <msp430.h>
#include <stdlib.h> // For srand()

#define ARRAYSIZE 8 // This program only works on square matrices.

void printMatrix(int anyMatrix[][]); // Function prototype

int main()
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer to prevent time out reset
    srand(time(NULL)); // Random number seed based on system time
    int x[ARRAYSIZE] [ARRAYSIZE]; int y[ARRAYSIZE] [ARRAYSIZE]; int result[ARRAYSIZE] [ARRAYSIZE];
    unsigned int i,j,k; // Loop counters
    const unsigned int maxRange = 101; // Sets the range to [0, 100]
    const unsigned int toNegative = maxRange/2; // Shifts the range down to include negative numbers. This causes some uneven distribution.

    // This loop populates the two arrays with random numbers in the range [-50, 50]
    for(i = 0; i <= ARRAYSIZE-1; i++)
        {
            for(j = 0; j <= ARRAYSIZE-1; j++)
            {
                int randX = rand() % maxRange - toNegative;
                int randY = rand() % maxRange - toNegative;
                x[i][j] = randX;
                y[i][j] = randY;
            }
        }
    // This loop calculates the value for each cell of the result matrix
    for(i = 0; i <= ARRAYSIZE-1; i++)
    {
        for(j = 0; j <= ARRAYSIZE-1; j++)
        {
            result[i][j] = 0; // Set everything to 0 as a failsafe
            for(k = 0; k <= ARRAYSIZE-1; k++)
            {
                result[i][j]+=x[i][k]*y[k][j];
            }
        }
    }
    printf("A:\n");
    printMatrix(x);
    printf("\nB:\n");
    printMatrix(y);
    printf("\nAB:\n");
    printMatrix(result);
}

// This function prints a 2 dimensional array
void printMatrix(int anyMatrix[ARRAYSIZE][ARRAYSIZE])
{
    unsigned int i,j; // Loop counters
    for(i = 0; i <= ARRAYSIZE-1; i++)
        {
            for(j = 0; j <= ARRAYSIZE-1; j++)
            {
                printf("%d\t", anyMatrix[i][j]);
            }
            printf("\n");
        }
}
