/*
 * lcd.h
 *
 *  Created on: Aug 5, 2023
 *      Author: quiet
 */
/*
 * ST7006 has two registers:
 * 	1. Instruction Register(IR) => RS = 0 then IR is set
 * 	2. Data Registers(DR) => RS = 1 then DR is set
 *
 * 	When Instruction Register is set we send commands for setting up the LCD
 * 	When Data Registers is set we send data for printing on the LCD
 *
 * 	R/W pins: Configures when to Read/Write
 * 		R/W = 1 set to Read
 * 		R/W = 0 set to write
 * 	note: before writing any command or data Busy Flag should be checked
 *
 *
 * Enable Pin: is used for starting Read or Write operation
 *
 *  note: After equating values to data pins of LCD,
 *  	  Enable pin should be toggled from High to Low for data/command execution.
 *
 *  	  If enable pulse is not their then no command/data will be execuited.
 *  	  Thus LCD_wait is used in Lcd_command() and Lcd_data()
 *
 *
 * The chip has two kind of interface 8 bit and 4 bit,
 * If 8 bit mode has to be used then all D0-7 pin will be used.
 * If 4 bit mode is used then D4-D7 pins is used in which first 4 high bits(D7-4) are used followed by last 4 bits(D0-D3)
 *
 */


#ifndef LCD_H_
#define LCD_H_

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>


// for atmega2560

// RW		Digital Pin-12(PB6)
// RS		Digital Pin-13(PB7)
// Enable	Digital Pin-11(PB5)

#define LCD_command_dir DDRB    // sets the output and input
#define LCD_command_port PORTB  // sets the high and low of pins

// for Arduino atmega2560

#define LCD_data_dir DDRC     // data pins input output mode of (0-7)pins
#define LCD_data_port PORTC  // data pins high and low

#define debug false

#define RW 6  // define Read(1)/Write(0) signal pin
#define RS 7  // Define REgister select (data(1)/command reg(0
#define EN 5  // Define enable pin set to 1 for



// for atmega2560

#define LCD_D0 0 // digital pin -49(PL0)
#define LCD_D1 1 // digital pin -48(PL1)
#define LCD_D2 2 // digital pin -47(PL2)
#define LCD_D3 3 // digital pin -46(PL3)
#define LCD_D4 4 // digital pin -45(PL4)
#define LCD_D5 5 // digital pin -44(PL5)
#define LCD_D6 6 // digital pin -43(PL6)
#define LCD_D7 7 // digital pin -42(PL7)





#define SET_BIT(byte,bit) ((byte) |= (1UL << (bit)))
#define CLEAR_BIT(byte,bit) ((byte) &= ~(1UL << (bit)))
#define IS_SET(byte,bit) (((byte) & (1UL << (bit))) >> (bit))



//Function Set
// Rs RW DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
// 0   0  0   0   1   DL  N   F   -   -
// 8bit (DL=1), 4bit (DL=0)
// 1line (N=0), 2line (N=1)

#define lcd_8bit_1line 0b00110000
#define lcd_8bit_2line 0b00111000
#define lcd_4bit_1line 0b00100000
#define lcd_4bit_2line 0b00101000


// Display ON/OFF control
// RS RW DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
//  0  0  0   0   0   0   1   D   C   B
// DN-display on(D=1), CN-Cursor on(C=1), BN blink on (B=1)
// DF-display off(D=0),CF-Cursor off(C=0), BF blink off(B=1)
#define lcd_DN_CN_BN 0b00001111
#define lcd_DN_CF_BF 0b00001100
#define lcd_DN_CN_BF 0b00001110
#define lcd_DF_CF_BF 0b00001000
#define lcd_DF_CN_BN 0b00001011


// Entry mode set
// RS RW DB7 DB6 DB5 DB4 DB3 DB2  DB1  DB0
//  0  0  0   0   0   0   0   1   I/D  S/H
//CMR -cursor move right(I/D=1), DMF-display move off(S/H=0)
//CML -cursor move left(I/D=0), DMR-display move is performed:right(S/H=1)
//DML -display move is performed: left(S/H=1)

#define lcd_CMR_DMF 0b00000110
#define lcd_CML_DMF 0b00000100
#define lcd_CMR_DMR 0b00000111
#define lcd_CML_DML 0b00000101

#define lcd_clear_all 0b00000001
#define lcd_rhome     0b00000010
#define lcd_SETDDRAMADDR 0x80



uint8_t address_counter=0; //variable to store address counter and busy flag(7th bit) status
/*
 *Enable pin: starts Read/write operation
 *Enable pin is Toggled from high to low for final execution of command/data
 *Enable Rise/Fall time: Will be determined by one Machine Cycle
 *Enable Pulse Width : Will be determined by amount of delay
 *if to change the speed for displaying characters of a string then
 *change second delay(lower the value: fast it print)
 *					 (Higher the value : slower it prints)
 */

void lcd_enable_pulse();

/*
 * 8-bit mode writing function
 * Write data to send to 8 pins of LCD, by sending Data signals to GPIO pins of MCU
 * So GPIO pins are set as OUPTU and then value is equated to them
 * Then for final execution Enable pulse is send
 * if enable pulse is not send then data/command will not executed.
 */

void lcd_write8bit(uint8_t value);

/*
 * 4-bit mode writing function
 * Writes data to be send to 4 pins of LCD, by sending data signals to GPIO pins of MCU
 * So, GPIO pins are set as OUTPUT and then value is equated to them
 * first HIGHER 4 bits are execution enable pulse is send.
 * if enable pulse is not send then data/command will not be executed
 *
 */

void lcd_write4bit(uint8_t value);

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

uint8_t read_BF_adress();


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

int LCD_wait(void);


/*
 * Command write function:
 * RS = 0 for selecting instruction register
 * R/W = 0 for writing data
 * Clear display, display on/off, cursor blink,
 * scroll of text, cursor/display shift all such features are set by
 * selecting instruction Register
 */

void LCD_command(uint8_t cmd);
void lcd_clear();
void lcd_Rethome();
void lcd_setcursor(uint8_t col, uint8_t row);
void LCD_init();

/*
 * Data write fucntion
 * send data to be printed to data pins --data port
 * make the RS pin set to 1
 * set RW pin to low 0 to write to LCD
 * EN pin is set to 1 for finally execution of command then some delay
 * and then again set pin to 0
 */

void LCD_data(uint8_t data);

//for printing integers in form 001, 023, 015
void lcd_printint(int value);

//for printing integers in form 1, 23, 44, 55
void lcd_printint_num(int value);

// send string function
void LCD_string(const char*str);
void lcd_print1line(const char *buff);
void lcd_print2line(const char *buff);
void lcd_bit(unsigned char val);



#include "function.cpp"

#define lcd_ON_
















#endif /* LCD_H_ */
