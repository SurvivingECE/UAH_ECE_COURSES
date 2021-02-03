/*---------------------------------------------------------
* File: Lab2_Q3.c
* Description: This programs computes the dot product
* Input: Two random int arrays of size ARRAYSIZE
* Output: Dot product
* Author: David Thornton
* Lab Section: 2
* Date: September 7, 2020
* *------------------------------------------------------*/
#include <time.h> // For srand()
#include <stdio.h> // For printf
#include <msp430.h>
#include <stdlib.h> // For srand()

#define ARRAYSIZE 13 // With a size of 13 and range of [-50, 50],
                     // this program will never reach INT_MAX or INT_MIN.
                     // 50 x 50 (max result) = 2500. 2500 x 13 (array size) = 32500. 32500 < INT_MAX (32767).
                     // The same calculation can be done with negative numbers for INT_MIN.
                     // ARRAYSIZE and maxRange can be adjusted based on design requirements.

int dotProduct(int arrayA[], int arrayB[], int sizeA, int sizeB); // Function prototype

int main()
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer to prevent time out reset
    srand(time(NULL)); // Random number seed based on system time
    int x[ARRAYSIZE]; int y[ARRAYSIZE]; unsigned int i;
    const unsigned int maxRange = 101; // Sets the range to [0, 100]
    const unsigned int toNegative = maxRange/2;
    // Shifts the range down to include negative numbers. This causes some uneven distribution.

    for(i = 0; i <= ARRAYSIZE-1; i++) // This loop populates the two arrays with random numbers in the range [-50, 50]
    {
        int randX = rand() % maxRange - toNegative;
        int randY = rand() % maxRange - toNegative;
        x[i] = randX;
        y[i] = randY;
    }
    printf("X Array: \n");
    for(i = 0; i <= ARRAYSIZE-1; i++)
    {
        printf("Value %02d: %3d", i+1, x[i]);
        printf("\n");
    }

    printf("\nY Array: \n");
    for(i = 0; i <= ARRAYSIZE-1; i++)
    {
        printf("Value %02d: %3d", i+1, y[i]);
        printf("\n");
    }

    unsigned int sizeX = (sizeof(x)/sizeof((x)[0]));
    unsigned int sizeY = (sizeof(y)/sizeof((y)[0]));

    int result = dotProduct(x, y, sizeX, sizeY);
    printf("\nThe dot product is: %d", result);
    return 0;
}

int dotProduct(int arrayA[], int arrayB[], int sizeA, int sizeB)
{
    int product = 0; int i = 0;
    if (sizeA == sizeB) // This size check is not needed in this case, as array size is a defined constant ARRAYSIZE.
                        // I am still including it as a best practice and a failsafe.
    {
        for (i = 0; i <= sizeA-1; i++) // Loop to calculate dot product
        {
            product += (arrayA[i] * arrayB[i]);
        }
        return product;
    }
    else if (sizeA != sizeB)
    {
        printf("Array sizes must match. Exiting program.");
        exit(0);
    }
    return 0;
}
