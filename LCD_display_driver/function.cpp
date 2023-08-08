/*
 * function.cpp
 *
 *  Created on: Aug 5, 2023
 *      Author: quiet
 */


/*
 *Enable pin: starts Read/write operation
 *Enable pin is Toggled from high to low for final execution of command/data
 *Enable Rise/Fall time: Will be determined by one Machine Cycle
 *Enable Pulse Width : Will be determined by amount of delay
 *if to change the speed for displaying characters of a string then
 *change second delay(lower the value: fast it print)
 *					 (Higher the value : slower it prints)
 */



void lcd_enable_pulse(){
	LCD_command_port |= (1<<EN);
	_delay_ms(1);
	LCD_command_port &=~(1<<EN);
	_delay_ms(1);   // change this to vary speed for printing character of

}

/*
 * 8-bit mode writing function
 * Write data to send to 8 pins of LCD, by sending Data signals to GPIO pins of MCU
 * So GPIO pins are set as OUPTU and then value is equated to them
 * Then for final execution Enable pulse is send
 * if enable pulse is not send then data/command will not executed.
 */

void lcd_write8bit(uint8_t value){

	LCD_data_dir |= (1<<LCD_D0) | (1<<LCD_D1) | (1<<LCD_D2) | (1<<LCD_D3) | (1<<LCD_D4) | (1<<LCD_D5) | (1<<LCD_D6) | (1<<LCD_D7);
	LCD_data_port = (value&0b11111111);
	lcd_enable_pulse();
}

/*
 * 4-bit mode writing function
 * Writes data to be send to 4 pins of LCD, by sending data signals to GPIO pins of MCU
 * So, GPIO pins are set as OUTPUT and then value is equated to them
 * first HIGHER 4 bits are execution enable pulse is send.
 * if enable pulse is not send then data/command will not be executed
 *
 */

void lcd_write4bit(uint8_t value){
	LCD_data_dir |= (1<<LCD_D4) | (1<<LCD_D5) | (1<<LCD_D6) | (1<<LCD_D0);
	LCD_data_port = (value&0b11110000);  // sending higher 4 bits
	lcd_enable_pulse();
	LCD_data_port = ((value&0b00001111) << 4);  // sending lower 4 bits
	lcd_enable_pulse();
}

/*
 * Read Busy flag/address:
 * After execution of command, it has to be checked that whether internal operation
 * (execution of command is completed or not)
 * It is done by reading BF flag.
 * By setting RS=0 and R/W=1: Busy flag and address of command being executing
 * Thus data pins of LCD are set as Input for this stage and 8 bit data
 * is stored in variable by reading PIN value of Data port
 * BF Flag output is given at &th bit
 * Enable pulse is important
 */

uint8_t read_BF_adress(){
	LCD_data_dir = 0x00;   // data pins input
	LCD_command_port &= ~(1<<RS);  // RS will be set to 0
	LCD_command_port |= (1<<RW);  // RW will be set to 1
	LCD_command_port |= (1<<EN);  // enable pin set to 1
	_delay_ms(1);
	address_counter = PIND;
	LCD_command_port &= ~ (1<<EN); // enable pin set to 0
	_delay_ms(1);

	return address_counter;

}


/*
 * LCD_wait():
 * Reads the address counter (which contains the busy flag and loops until
 * the busy flag is cleared.
 * BF = 1 (Internal operation is happening),
 * BF = 0 (Internal operation is completed)
 * by the end of lcd_wait() function:
 * R/W will be  1,
 * RS will be 0
 * Data port will be input.
 *
 *
 */

int LCD_wait(void){
	//get address and busy flag
	//and loop until busy flag(bit7) is cleared

	while((read_BF_adress() & 0x00) == 0x00);
	return 1;

}


/*
 * Command write function:
 * RS = 0 for selecting instruction register
 * R/W = 0 for writing data
 * Clear display, display on/off, cursor blink,
 * scroll of text, cursor/display shift all such features are set by
 * selecting instruction Register
 */

void LCD_command(uint8_t cmd){

	if(LCD_wait()){
		LCD_command_port &= ~ (1<<RS);
		LCD_command_port &= ~ (1<<RW);

		lcd_write8bit(cmd);

	}else{
//		bit0(address_counter);
//		printString0("\n");

	}
}

void lcd_clear(){
	LCD_command(lcd_clear_all); // clear display
	_delay_ms(2);
}

void lcd_Rethome(){
	LCD_command(lcd_rhome);
	_delay_ms(2);

}

void lcd_setcursor(uint8_t col, uint8_t row){
	LCD_command((0x00 |(row<<6))+col);
	_delay_ms(40);
}


void LCD_init(){
	LCD_command_dir |= (1<<RS) | (1<<RW) | (1<<EN); //command pins of lcd will be set as output

	_delay_ms(50); // LCD power ON delay always > 15ms

	{
		LCD_command(lcd_8bit_2line); // Function set 8bit 2 line mode
		_delay_ms(50);
		LCD_command(lcd_8bit_2line); // function set 8 bit 2 line mode
		_delay_ms(50);

	}

	LCD_command(lcd_DN_CN_BN); //Display ON/OFF control
	_delay_ms(45);
	LCD_command(lcd_CMR_DMF); // Entry mode set
	_delay_ms(45);
	lcd_clear();
	_delay_ms(5);




}


/*
 * Data write function
 * Send data to be printed to data pins = data_port
 * Make the RS pin set to 1 to know that data register is being used
 * Set RW pin to low 0 to write to LCD
 * EN pin is set to 1 for finaly execution of command and
 * then some delay and then again set pin to 0
 */


void LCD_data(uint8_t data){
	LCD_command_port |= (1<<RS);
	LCD_command_port &= ~(1<<RW);
	lcd_write8bit(data);
}


// for printing integers in form 001,043, 067

void lcd_printint(int value){
	unsigned char thousands, hundreds, tens,ones;

	thousands = value / 1000;
	if(thousands != 0){
		LCD_data(thousands + 0x30);

	}

	hundreds = ((value - thousands*1000)-1)/100;
	LCD_data(hundreds + 0x30);


	tens = (value%100)/10;
	LCD_data(tens + 0x30);

	ones = value%10;
	LCD_data(ones + 0x30);

}

// for printing integers in form 1, 23, 15, 6

void lcd_printint_num(int value){
	unsigned char thousands, hundreds, tens,ones;

		thousands = value / 1000;
		if(thousands != 0)
		LCD_data(thousands + 0x30);



		hundreds = ((value - thousands*1000)-1)/100;
		if(hundreds!=0)
		LCD_data(hundreds + 0x30);


		tens = (value%100)/10;
		if(tens!=0)
		LCD_data(tens + 0x30);

		ones = value%10;
		if(ones != 0)
		LCD_data(ones + 0x30);
}

// send string function

void LCD_string(const char*str){
	int i;
	for(i=0; str[i]!=0; i++){

		LCD_data(str[i]);
		_delay_ms(45);

	}
}


void lcd_print1line(const char *buff){
	lcd_setcursor(0,0);
	LCD_string(buff);
}


void lcd_print2line(const char *buff){
	lcd_setcursor(0,1);
	LCD_string(buff);
}


void lcd_bit(unsigned char val){
	//lcd_setcursor(0,1)
	//_delay_ms(40);

	int ptr;
	for(ptr=7; ptr>=0; ptr--){
		if((val & (1<<ptr)) ==0){
			LCD_string("0");
		}
		else{
			LCD_string("1");
		}
	}
}
