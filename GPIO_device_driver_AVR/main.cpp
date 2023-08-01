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
//#include "uarts.h"


#define SET_BIT(byte, bit) (byte |= (1 << bit))    // this bit manipulation is used for set the bit to 1
#define CLEAR_BIT(byte, bit) (byte &= ~(1 << bit)) // this bit manipulation is used for clear the bit to 0
#define IS_SET(byte, bit) ((byte) & (1 << bit))    // this bit manipulation is used for check the bit weather is set or clear



// Macro variables for Selecting OUTPUT mode  in PinMode() function
#define OUTPUT 1
#define INPUT 0

//Macro variables for equating state in digitalWrite() function

#define HIGH 1
#define LOW 0


// pin define for actuator connected with
// This pin is the default Arduino uno Onboard led connected.
#define led PB5
#define readSensor PB4
uint8_t data;




//DDRX used for pinMode
void pinMode(uint8_t pin , uint8_t volatile *pin_data_direction_register, uint8_t Mode){

	if(Mode == OUTPUT){
		*pin_data_direction_register |= (1 << pin);
	}
	else if (Mode == INPUT){
		*pin_data_direction_register &= ~(1 << pin);
	}
}


//PORTX used to write pin status high and low
void digitalWrite(uint8_t pin, uint8_t volatile *port_data_pin_register, uint8_t state){
	if(state == HIGH){
		*port_data_pin_register |= (1 << pin);
	}
	else if(state == LOW){
		*port_data_pin_register &= ~(1 << pin);
	}
}


//PINX is used to read pin status
uint8_t digitalRead(uint8_t pin, uint8_t volatile *port_input_pin_register){

	return ((*port_input_pin_register) & (1 << pin));
}


int main(){


//	DDRB = 0b11111111;
//
//
//	while(1){
//
//		PORTB |= (1 << led);
//		_delay_ms(1000);
//		PORTB |= (0 << led);
//		_delay_ms(1000);
//
//	};


//	UART_Init0();
	pinMode(led, &DDRB, OUTPUT);
	pinMode(readSensor, &DDRB, INPUT);

	data = digitalRead(readSensor, &PINB);
//	UART_Printf(data);


	while(1){
//		digitalWrite(led, &PORTB, HIGH);
//		_delay_ms(1000);
//		digitalWrite(led, &PORTB, LOW);
//		_delay_ms(1000);

		if(digitalRead(readSensor, &PINB)){
			digitalWrite(led, &PORTB, HIGH);
		}
		else{
			digitalWrite(led, &PORTB, LOW);
		}

	}


}

