/*---------------------------------------------------------
* File: 		Lab_8.c
* Description:	This program user UART communication to
* 				test usernames and passwords. This program
* 				uses a baud rate of 115200.
* Input:		Typing into hyperterminal
* Output:		Hyperterminal text
* Author: 		David Thornton
* Lab Section: 	2
* Date: 		October 28, 2020
* *------------------------------------------------------*/
#include <stdio.h>
#include <msp430.h>

char username_prompt[35] = "\033[33mPlease enter the username: ";
char password_prompt[35] = "\033[34mPlease enter the password: ";
char incorrect_login[41] = "\033[31mIncorrect username or password!!!";
char login_success[41] = "\033[32mWelcome to CPE 325!!!";

char username_1[5] = "David";
char username_2[5] = "dht22";
char password_1[6] = "David1";
char password_2[6] = "abc123";

void print_username_prompt(void);
void print_password_prompt(void);
void print_incorrect_login(void);
void print_login_success(void);
void print_newline(void);
void check_username(char any_user[]);
void check_password(char any_user[], char any_pass[]);
void UART_setup(void);

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;	// Stop watchdog timer
	UART_setup();				// Initialize UART
	while(1)
	{
		unsigned int i = 0;
		unsigned int j = 0;
		char username[5];
		char password[6];

		print_username_prompt();
		do
		{
			if(i >= 5) break;
			while(!(UCA0IFG & UCRXIFG)); 	// Wait for a new character
			while(!(UCA0IFG & UCTXIFG)); 	// Wait until TXBUF is free
			UCA0TXBUF = UCA0RXBUF; 			// TXBUF <= RXBUF (echo)
			username[i] = UCA0RXBUF;     	// username[i] <- user input
			i++;
		} while(UCA0RXBUF != '\r');
		check_username(username);

		print_password_prompt();
		do
		{
			if(j >= 6) break;
			while(!(UCA0IFG & UCRXIFG)); 	// Wait for a new character
			while(!(UCA0IFG & UCTXIFG)); 	// Wait until TXBUF is free
			UCA0TXBUF = '*'; 				// TXBUF <= * (echo)
			password[j] = UCA0RXBUF;     	// password[j] <- user input
			j++;
			// if timeout flag is set, restart
			// if timeout flag is not set, keep going
		} while(UCA0RXBUF != '\r');
		check_password(username, password);
	}
}


void print_username_prompt(void)
{
	print_newline();
	unsigned int i = 0;
	for(i = 0; i <= 35; i++)
	{
		while (!(UCA0IFG & UCTXIFG)); 		// Wait for previous character to transmit
		UCA0TXBUF = username_prompt[i];	 	// Transmit a byte
	}
	print_newline();
}


void print_password_prompt(void)
{
	print_newline();
	unsigned int i = 0;
	for(i = 0; i <= 35; i++)
	{
		while (!(UCA0IFG & UCTXIFG)); 		// Wait for previous character to transmit
		UCA0TXBUF = password_prompt[i];	 	// Transmit a byte
	}
	print_newline();
}


void print_incorrect_login(void)
{
	print_newline();
	unsigned int i = 0;
	for(i = 0; i <= 41; i++)
	{
		while (!(UCA0IFG & UCTXIFG)); 		// Wait until TXBUF is free
		UCA0TXBUF = incorrect_login[i];	 	// Transmit a byte
	}
	print_newline();
}


void print_login_success(void)
{
	print_newline();
	unsigned int i = 0;
	for(i = 0; i <= 41; i++)
	{
		while (!(UCA0IFG & UCTXIFG)); 		// Wait for previous character to transmit
		UCA0TXBUF = login_success[i];	 	// Transmit a byte
	}
	print_newline();
}


void check_username(char any_user[])
{
	unsigned int i = 0;
	for (i = 0; i < 5; i++)
	{
		if ((any_user[i] != username_1[i]) && (any_user[i] != username_2[i]))
		{
			print_incorrect_login();
			main();
		}
	}
	while(!(UCA0IFG & UCTXIFG));  			// Wait until TXBUF is free
}


void check_password(char any_user[], char any_pass[])
{
	unsigned int i = 0;
	for (i = 0; i < 6; i++)
	{
		if ((any_pass[i] != password_1[i]) && (any_pass[i] != password_2[i]))
		{
			print_incorrect_login();
			main();
		}
		else if((any_user[i] == username_1[i]) && (any_pass[i] == password_2[i]))
		{
			print_incorrect_login();
			main();
		}
		else if((any_user[i] == username_2[i]) && (any_pass[i] == password_1[i]))
		{
			print_incorrect_login();
			main();
		}
	}
	print_login_success();
}


void print_newline(void)
{
	while(!(UCA0IFG & UCTXIFG));  			// Wait until TXBUF is free
	UCA0TXBUF = '\n';						// print newline
	while(!(UCA0IFG & UCTXIFG));  			// Wait until TXBUF is free
	UCA0TXBUF = '\r';						// carriage return
	while(!(UCA0IFG & UCTXIFG));  			// Wait until TXBUF is free
	UCA0TXBUF = '\0';						// reset buffer
}


void UART_setup(void)
{
	P3SEL |= BIT3 + BIT4;		// Set USCI_A0 RXD/TXD to receive/transmit data
	UCA0CTL1 |= UCSWRST;		// Set software reset during initialization
	UCA0CTL0 = 0;				// USCI_A0 control register
	UCA0CTL1 |= UCSSEL_2;		// Clock source SMCLK

	UCA0BR0 = 0x09;			 	// 1048576 Hz  / 115200 lower byte
	UCA0BR1 = 0x00;			 	// upper byte
	UCA0MCTL |= UCBRS0;		 	// Modulation (UCBRS0=0x01, UCOS16=0)

	// Clear software reset to initialize USCI state machine
	UCA0CTL1 &= ~UCSWRST;
}
