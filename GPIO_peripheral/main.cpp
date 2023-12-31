/*
 * main.cpp
 *
 *  Created on: Jul 31, 2023
 *      Author: quiet
 */


#include <avr/io.h>
#include <string.h>
#include <avr/delay.h>


#define led0 PB0
#define led1 PB1
#define led2 PB2
#define led3 PB3
#define led4 PB4
#define led5 PB5
#define led6 PB6
#define led7 PB7



int main(){


//	// basic way of controlling port
	DDRB = 0b00000010;
	PORTB = 0b00000010;
	_delay_ms(1000);
	PORTB = 0b00000000;
	_delay_ms(1000);



	DDRB = 0b11111111;

	while(1){

		// single pin led toggling using bit manipulation.
		PORTB = (1 << led5);
		_delay_ms(1000);
		PORTB = (0 << led5);
		_delay_ms(1000);


		// multiple pin led/actuator toggle using bit manipulation

		PORTB = (1 << led1) | (1 << led2) | (1 << led5) | (1 << led6);
		_delay_ms(1000);
		PORTB = (0 << led1) | (0 << led2) | (0 << led5) | (0 << led6);
		PORTB = (1 << led0) | (1 << led3) | (1 << led4) | (1 << led7);
		_delay_ms(1000);
		PORTB = (0 << led0) | (0 << led3) | (0 << led4) | (0 << led7);


	}
}




