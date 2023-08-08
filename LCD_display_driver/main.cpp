/*
 * main.cpp
 *
 *  Created on: Aug 3, 2023
 *      Author: quiet
 */
#include <avr/io.h>
//#include "uarts.h"
#include "lcd.h"


uint8_t data_pins[8];

int main(void){

	int u=1;
	LCD_init();
	_delay_ms(20);
	LCD_string("Hello sohan");
	lcd_setcursor(0,1);
	LCD_string("Sohan");
	_delay_ms(1000);
	lcd_clear();
	_delay_ms(1000);
	LCD_string("Hii bro");
	lcd_printint_num(976);


}

