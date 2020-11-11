/*******************************************************
*** ECE 322
***	Lab 4:
***		Ryan Bride
***	Notes
***  * is used as multiplcation
***  # is used as divide
***  A is add
***  B is subtract
***  C is clear
***  D is enter
*******************************************************/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "IncFile1.h"
#include <string.h>
#include <stdlib.h>

static unsigned char keyout;


unsigned char keypadLookup(unsigned char Col, unsigned char Row) { 
	unsigned char keypadInput = 0x00;
	unsigned char keyInput = (Col + Row);  //assign local to the row and column 4 * 4 vector thing i have created. 
	//Assign the scanned row and column to the hex value corresponding to the proper input from the keypad.
	
	switch(keyInput) {
		case 0x11:
			keypadInput = 0x01;		//input is 1
			break;
		case 0x21:
			keypadInput = 0x02;		//Input is 2
			break;
		case 0x41:
			keypadInput = 0x03;     //Input is 3	
			break;
		case 0x81:
			keypadInput = 0x0A;     //Input is A
			break;
			
		case 0x12:
			keypadInput = 0x04;     //Input is 4
			break;
		case 0x22:
			keypadInput = 0x05;     //Input is 5
			break;
		case 0x42:
			keypadInput = 0x06;     //Input is 6
			break;
		case 0x82:
			keypadInput = 0x0B;		//Input is B
			break;
			
		case 0x14:
			keypadInput = 0x07;		//Input is 7
			break;
		case 0x24:
			keypadInput = 0x08;		//Input is 8 
			break;
		case 0x44:
			keypadInput = 0x09;		//Input is 9 
			break;
		case 0x84:
			keypadInput = 0x0C;		//Input is C
			break;
		
		case 0x18:
			keypadInput = 0x2A;		//Input is *
			break;
		case 0x28:
			keypadInput = 0x00;		//Input is 0
			break;
		case 0x48:
			keypadInput = 0x24;		//Input is #
			break;
		case 0x88:
			keypadInput = 0x0D;		//Input is D
			break;
		
	}	
	return(keypadInput);
}

void init_port(void) {
	/* 1. Set PortB and Upper Nibble of PortC to outputs. Set Lower Nibble of PortC and PortD to Input 
	2. Enable Pull-Resistors on PORTD and lower input Nibble of PortC(i.e the ports outputs 1 
	until connected to a ground or pull down or otherwise externally driven) */
	DDRB = 0xFF;  
	DDRC = 0xF0;  
	DDRD = 0x00;  
	 
	PORTD = 0xFF; 
	PORTC = 0x0F; 
	PORTB = 0x00;
}

ISR(INT0_vect) {
	cli();
	_delay_ms(30); //debouncer
	unsigned char selectionRow = 0x01;
	
	/* Start Parsing Column */
	DDRC = 0x0F;
	PORTC = ~(1<<0); //isolate row based on isr index
	unsigned char selectionCol = PINC & 0xF0; //read inputs from upper 4 bits of PINC
	keyout = keypadLookup(selectionCol, selectionRow);

	/* Reset PortC to Pre Column Parsing State */
	DDRC = 0xF0;
	PORTC = 0x0F;
	
	_delay_ms(100);
	EIFR |= (1<<0); //Turn off interrupt flag to prevent interrupt calling itself
	sei();
}
ISR(INT1_vect) {
	cli();
	_delay_ms(30); //debouncer
	unsigned char selectionRow = 0x02;
	
	/* Start Parsing Column */
	DDRC = 0x0F;
	PORTC = ~(1<<1); //isolate row based on isr index
	unsigned char selectionCol = PINC & 0xF0; //read inputs from upper 4 bits of PINC
	keyout = keypadLookup(selectionCol, selectionRow);
	
	/* Reset PortC to Pre Column Parsing State */
	DDRC = 0xF0;
	PORTC = 0x0F;
	
	_delay_ms(100);
	EIFR |= (1<<1); //Turn off interrupt flag to prevent interrupt calling itself
	sei();
}
ISR(INT2_vect) {
	cli();
	_delay_ms(30); //debouncer
	unsigned char selectionRow = 0x04;
	
	/* Start Parsing Column */
	DDRC = 0x0F;
	PORTC = ~(1<<2); //isolate row based on isr index
	unsigned char selectionCol = PINC & 0xF0; //read inputs from upper 4 bits of PINC
	keyout = keypadLookup(selectionCol, selectionRow);
	
	/* Reset PortC to Pre Column Parsing State */
	DDRC = 0xF0; 
	PORTC = 0x0F; 

	_delay_ms(100);
	EIFR |= (1<<2); //Turn off interrupt flag to prevent interrupt calling itself
	sei();
}
ISR(INT3_vect) {
	cli();
	_delay_ms(30); //debouncer
	unsigned char selectionRow = 0x08;
	
	/* Start Parsing Column */
	DDRC = 0x0F;
	PORTC = ~(1<<2); //isolate row based on isr index
	unsigned char selectionCol = PINC & 0xF0; //read inputs from upper 4 bits of PINC
	keyout = keypadLookup(selectionCol, selectionRow);

	/* Reset PortC to Pre Column Parsing State */
	DDRC = 0xF0;
	PORTC = 0x0F;
	_delay_ms(100);
	EIFR |= (1<<3); //Turn off interrupt flag to prevent interrupt calling itself
	sei();
}

void lcd_draw_val(int val){
	char buffer [5];
	itoa(val, buffer, 10);
	for (uint8_t i=1; i<strlen(buffer); i++){
		BackspaceLCD();
	}
	for (uint8_t i=0; i<strlen(buffer);i++){
		WriteCharacterToLCD(buffer[i]);
	}
}


void calcval(long int val1, unsigned char oper, long int val2){
	long int comp = 0;
	switch(oper)
	{
		case 0x0A:
			comp = val1 + val2;
		case 0x0B:
			comp = val1 - val2;
		case 0x2A:
			comp = val1*val2;
		case 0x2F:
			comp = val1/val2;		
	}
	char buff[9];
	itoa(comp, buff,10);
	for (uint8_t i=0; i<strlen(buff);i++)
		{
		WriteCharacterToLCD(buff[i]);
		}
	return;
}

int main(void)
{
	/* Enable Global Interrupts and Initialize Ports */
	EICRA = 0x02;		/* Set interrupts to be falling edge triggered */
	EIMSK = 0x0F; /* Enable Int0, Int1, Int2, and Int3 */
	sei();
	init_port();
	initializeLCD();
		
	//static to check for change
	static unsigned char storedKeyout;
	keyout = 0x00;  //initialize keyout to 00
	storedKeyout = 0x00;
	
	enum {IDLE=0, IN1=1, IN1_IDLE=2, OPER=3, OPER_IDLE=4, IN2=5, IN2_IDLE=6, COMPUTE=7} calcState;
	calcState = IDLE;

	//Statics for the Calculator
	static long int val1 = 0;
	static unsigned char oper = 0;   //Default to space
	static long int val2 = 0;  
	static long int comp;
	
	
	while (1) {		
		switch (calcState){
			case IDLE:
				if (storedKeyout != keyout) {
					ClearLCD();
					PORTB = keyout;
					storedKeyout = keyout;
					calcState = IN1;
					}
				break;
				
			case IN1:
				//New Calculation started
				//First reset the calculator if the input is not 1-9
				if (keyout >= 0 && keyout <=9){
					val1 = val1*10 +  keyout;  
					calcState = IN1_IDLE; 
					lcd_draw_val(val1);
					break;		
				}
				//User pressed a non number button on the pad so just ignore the input at this early stage
				else {
					calcState = IDLE;
					break;
				}
				
			case IN1_IDLE:
				//Idle State waiting for either another number to add to 1 or
				//Wait for the value to change again. 
				if (storedKeyout != keyout) {
					PORTB = keyout;
					storedKeyout = keyout;
					//If the new key input is another number add it to the first value used in calculator
					if(keyout >=0 && keyout <=9){
						calcState = IN1;
					}	
					//else if the input is an operator set state to operator
					else if(keyout == 0x0A || keyout == 0x0B || keyout == 0x24 || keyout == 0x2A){
						calcState = OPER;
					} 
					//else if the input is 0x0C or 0x0D clear the calculator 
					else if (keyout == 0x0C || 0x0D){
						val1 = 0; 
						oper = 0;
						val2 = 0;
						calcState = IDLE;			
					} 						
				}
				break;
			
			case OPER:
				switch(keyout)
					{
					case 0x0A:
						//addition
						oper = 0x2B;	//+						
						calcState = OPER_IDLE;
						break;
					case 0x0B:
						//subtraction
						oper = 0x2D;	//-
						calcState = OPER_IDLE;

						break;
					case 0x2A:
						//multiply
						oper = 0x2A;	//*
						calcState = OPER_IDLE;
						break;
					case 0x24:
						//Division
						oper = 0x2f;	// /
						calcState = OPER_IDLE;
						break;
					}
				NewlineLCD();
				WriteCharacterToLCD(oper);
				break;
			
			case OPER_IDLE:
				//Wait for the value to change again.
				if (storedKeyout != keyout) 
					{
					PORTB = keyout;
					storedKeyout = keyout;
					//If the new key input is another number add it to the new second value
					if(keyout >=0 && keyout <=9)
						{
						NewlineLCD();
						calcState = IN2;
						}
					//if the next input is another operator, clear, or enter the screen and reset as none are valid values
					else
						{
						val1 = 0;
						oper = 0;
						val2 = 0;
						calcState = IDLE;
						}
					
					}
				break;
					
			case IN2:  
				val2 = val2*10 + keyout;
				calcState = IN2_IDLE;
				lcd_draw_val(val2);
				break;
				
			case IN2_IDLE:
				//Idle State waiting for either another number to add to to 2 or 0x0d to calculate
				//Wait for the value to change again.
				if (storedKeyout != keyout) 
					{
					PORTB = keyout;
					storedKeyout = keyout;
					//If the new key input is another number add it to the first value used in calculator
					if(keyout >=0 && keyout <=9)
						{
						calcState = IN2;
						}
					else if (keyout == 0x0D)
					{
						calcState = COMPUTE;
					}
					//if the next input is another operator, clear, or enter the screen and reset as none are valid values
					else  if(keyout == 0x0A || keyout == 0x0B || keyout == 0x24 || keyout == 0x2A || keyout == 0x0C)
						{

						val1 = 0;
						oper = 0;
						val2 = 0;
						calcState = IDLE;
						}
					
					}
				break;
				
			case COMPUTE:
				ClearLCD();
				switch(oper)
				{
					case 0x2B:
						comp = val1 + val2;
						break;
					case 0x2D:
						comp = val1 - val2;
						break;
					case 0x2A:
						comp = val1* val2;
						break;
					case 0x2f:
						comp = val1/val2;
						break;
				}
				char buff[9];
				itoa(comp, buff,10);
				for (uint8_t i=0; i<strlen(buff);i++)
				{
					WriteCharacterToLCD(buff[i]);
				}
				PORTB = comp;
				val1 = 0;
				oper = 0;
				val2 = 0;
				calcState = IDLE;
				break;	
			}
			
		}
	return 0;	
}

	
