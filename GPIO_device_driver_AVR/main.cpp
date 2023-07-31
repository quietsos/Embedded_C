/*
 * main.cpp
 *
 *  Created on: July 31, 2023
 *      Author: quiet
 */


/*
 	 X = This alphabet(A,B,C,D) telling which port we are using
 	 N = This is define the pin number(1,2,3,4) of the port
 	 Example = DDRB, PORTB


 	 DDRX = Configure the pin mode ( 1 = Output mode, 0 = Input mode)
 	 PORTX = Set the pin status ( 1 = High, 0 = LOW)
 	 PINX = Tell state of the pin as input.


 	 0x = hex(0xff)
 	 0b = binary(11100011)
 	 7 6 5 4 3 2 1 0
    MSB           LSB
 */



#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

#define SET_BIT(byte, bit) (byte |= (1 << bit))
#define CLEAR_BIT(byte, bit) (byte &= ~(1 << bit))
#define IS_SET(byte, bit) ((byte) & (1 << bit))



// Macro variables for Selecting OUTPUT mode  in PinMode() function
#define OUTPUT 1
#define INPUT 0

//Macro variables for equating state in digitalWrite() function

#define HIGH 1
#define LOW 0




#define led PB5


void pinMode(uint8_t pin, volatile *pin_data_direction_register, bool Mode)
{

	if(Mode == OUTPUT){
		*pin_data_direction_register |= (1 << pin);
	}
	else{
		*pin_data_direction_register &= ~(1 << pin);
	}
}


int main(){


	DDRB = 0b11111111;


	while(1){

		PORTB |= (1 << led);
		_delay_ms(1000);
		PORTB |= (0 << led);
		_delay_ms(1000);

	};
};

