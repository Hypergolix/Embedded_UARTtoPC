/*
* UARTtoPC_ATMega328p.c
*
* Created: 20/07/2021 20:37:55
* Author : Leon J
*
* UART Communication
*/

#define F_CPU 16000000UL

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	unsigned int baudrate = 3;						// 230400 Baud
	// unsigned char data[] = "Sent from ATMega328P  ";
	
	DDRB = 0b11111101;								// Set pin PB1 as input and the rest as outputs
	PORTB = 0b00000000;								//
	DDRC = 0b00000000;
	
	// Set Baud rate
	UBRR0H = (baudrate>>8);
	UBRR0L = (baudrate);
	
	UCSR0C = 0x06;
	UCSR0B = (1<<TXEN0);

	// ADC
	ADMUX = 0b01100000;								// ADC Channel 0. ADLAR = 1. AVCC as reference
	ADCSRA = 0b10000111;							// ?
	
	while (1)
	{
		// ADC
		ADCSRA |= (1 << ADSC);
		while(ADCSRA & (1 << ADSC));
		
		uint8_t ADCdata = ADCH;						// Move ADC data into a variable, necessary?
		
		do{
			while (!( UCSR0A & (1<<UDRE0)));		// Waits for the buffer to be empty
			UDR0 = ((ADCdata % 10) + 48);			// Sends over UART. Can things be processed meanwhile? - REVERSE ORDER
			ADCdata /= 10;
		}while(ADCdata != 0);						// While not 0/there are numbers left to send
		// SERIAL FORMATTING
		// Send line end and carriage return for better formatting
		while (!( UCSR0A & (1<<UDRE0)));
		UDR0 = '\n';
		while (!( UCSR0A & (1<<UDRE0)));
		UDR0 = '\r';
		_delay_ms(50);
	}
}