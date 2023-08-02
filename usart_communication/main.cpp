/*
 * main.cpp
 *
 *  Created on: Aug 2, 2023
 *      Author: quiet
 */
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

#define uart0_ucsrA UCSR0A
#define uart0_ucsrB UCSR0B
#define uart0_ucsrC UCSR0C
#define uart0_UDR UDR0
#define uart0_baudrate UBRR0

void UART_Init0() {

	// 103 fpr 9600
	uart0_baudrate = 103;
	uart0_ucsrB = (1 << TXEN0) | (1 << RXEN0);  // 00001000, 00010000
	uart0_ucsrC = (1 << UCSZ00) | (1 << UCSZ01);

}

void UART_TxChar0(uint16_t data) {
	while ((uart0_ucsrA & (1 << UDRE0)) == 0)
		; //00100000
	uart0_UDR = data;

}

void UART_RxChar0() {
	while (!(uart0_ucsrA & (1 << RXC0)))
		;
	return uart0_UDR;

}

// stores the incoming string buffer

void rxString0(char *buff) {
	int i = 0;
	char myValue;
	do {
		myValue = UART_RxChar0();
		if (myValue != '\n') {
			buff[i] = myValue;
			UART_TxChar0(buff[i]);
			i++;
		} else {
			buff[i] = '\0';
			break;
		}
	}

	while (!(uart0_ucsrA & (1 << RXC0)));
	printString0(buff);
//	return myValue;
}

void printString0(const char *myString) {
	while (*myString) {
		UART_TxChar0(*myString++);
	}
}

int main(void) {

	UART_Init0();
	char rec[100];
	printString0("Hello this is sohan");

	while(1){
		printString0("Write a string");
		rxString0();
	}
}

