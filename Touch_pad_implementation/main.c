/*
 * main.c
 *
 *  Created on: Aug 1, 2023
 *      Author: quiet
 */

#include <avr/io.h>
#include <util/delay.h>
#include "uarts.h"
#include "stdlib.h"


/*
 * R1 : PB0
 * R2 : PB1
 * R3 : PB2
 * R4 : PB3
 * C1 : PB4
 * C2 : PB5
 * C3 : PB6

 */


#define R1 PB0
#define R2 PB1
#define R3 PB2
#define R4 PB3
#define C1 PB4
#define C2 PB5
#define C3 PB6


uint8_t getKey(){

	uint8_t keypressed = 0;

	DDRB = 0b00001111; // Column as input (PB6,PB5,PB4 th bit) will set as 0(C3,C2,C1)
					   // Row as output (PB3, PB2, PB1,PB0 the bit) will set as 1 (R4,R3,R2,R1)

	_delay_ms(1);

	PORTB = 0b11110000; // column pins:Input pins pull up register is activated by setting 1 to PORTC colum
						// Row pins: output pins are set to LOW(0)

	while(PINB == 0b11110000){ // If any button will be pressed then either of the PC6,PC5,PC4 bit (C3, C2, C1) will be 0 and it will come out from the while loop
		// if C1 is pressed : 11100000
		// if C2 is pressed : 11010000
		// if C3 is pressed : 10110000


		keypressed = PINB;  // store the value of which colum is pressed in keypressed variables
	}

	DDRC = 0b01110000;  // column as output
						// Row as input

	_delay_ms(1);
	PORTC = 0b00001111; // pull up register in row input
	_delay_ms(1);


	keypressed |= PINC;   // or the value of ROW and Column pressed when they are set as input

//	printString0("\n");


	switch(keypressed){

	case(0b11101110): //C1|R1
			return('1');
			break;

	case(0b11011110): //C2|R1
			return('2');
			break;

	case(0b10111110): //C3|R1
			return('3');
			break;


	case(0b11101101): //C1|R2
			return('4');
			break;


	case(0b11011101): //C2|R2
			return('5');
			break;

	case(0b10111101): // C3|R2
			return('6');
			break;

	case(0b11101011):    // C1 | R3
			return('7');
			break;

	case(0b11011011):     //C2|R3
			return('8');
			break;

	case(0b10111011):  // C3|R3
			return('9');
			break;

	case(0b11100111):  // C1|R4
			return('*');
			break;

	case(0b11010111):     //C2|R4
			return('0');
			break;

	case(0b10110111):      //C3|R4
			return('#');
			break;

	}


	_delay_ms(100);




}


int main(void){

	UART_Init0();
	uint8_t a =0;

	while(1){
		a = getKey();

		UART_TxChar0(a);
	}




}
