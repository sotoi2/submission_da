Insert initial code here

/*
 * Midterm2.c
 */ 


#include "SparkFun_APDS9960.h"
#include "i2c_master.h"
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define FOSC 16000000
#define UBRREQ FOSC/16/BAUD -1
#define APDS9960_WRITE 0x72
#define APDS9960_READ 0x73
void UART_init (void);
void APDS_init (void);

int uart_placingchar( char c , FILE *stream);
FILE str_uart = FDEV_SETUP_STREAM(uart_placingchar, NULL , _FDEV_SETUP_WRITE);
void readvalue(void);

uint16_t red;
uint16_t green;
uint16_t blue;

int main( void )
{
	UART_init(); // UART values
	APDS_init(); // APDS9960
	i2c_init(); // I2C
	stdout = &str_uart;
	
	red = 0;
	green = 0;
	blue = 0;
	
		_delay_ms(5000);   // for ap info
		printf("AT+CWMODE=3\r\n");
	_delay_ms(2000);// For AT comm
	printf("AT\r\n");
	_delay_ms(5000); // wifi connected 
	printf("AT+CWJAP=\"\",\"\"\r\n");	// wifi settings

	while(1) 
	{
		
		// cloud connection
		_delay_ms(5000);
		printf("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");

		// Connection making
		_delay_ms(5000);
		printf("AT+CIPMUX=0\r\n");
		_delay_ms(5000);
		readvalue();
		printf("AT+CIPSEND=104\r\n");
		printf("GET https://api.thingspeak.com/update?api_key=KZR5N56BUMMM6Q7G&field1=0%05u&field2=%05u&field3=%05u\r\n", red, green, blue); // key
		_delay_ms(3000);
		
	
	}
}

void readvalue(){

	uint8_t redH, redL;
	uint8_t greenH, greenL;
	uint8_t blueH, blueL;
	
	// red
	i2c_readReg(APDS9960_WRITE, APDS9960_RDATAH, &redH, 1);
	i2c_readReg(APDS9960_WRITE, APDS9960_RDATAL, &redL, 1);
	
	// green 
	i2c_readReg(APDS9960_WRITE, APDS9960_GDATAH, &greenH, 1);
	i2c_readReg(APDS9960_WRITE, APDS9960_GDATAL, &greenL, 1);
	
	// blue
	i2c_readReg(APDS9960_WRITE, APDS9960_BDATAH, &blueH, 1);
	i2c_readReg(APDS9960_WRITE, APDS9960_BDATAL, &blueL, 1);
	
	red = (redH << 8) | redL;
	green = (greenH << 8) | greenL;
	blue = (blueH << 8) | blueL;
	
	// limit
	if (red > 255)
	red = 255;
	if (green > 255)
	green = 255;
	if (blue > 255)
	blue = 255;
}

void APDS_init(void){
	uint8_t setup;
	
	i2c_readReg(APDS9960_WRITE, APDS9960_ID, &setup,1);
	if(setup != APDS9960_ID_1) while(1);
	setup = 1 << 1 | 1<<0 | 1<<3 | 1<<4;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_POFFSET_DL, &setup, 1);
	setup = DEFAULT_CONFIG1;
	
	
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_PPULSE, &setup, 1);
	setup = DEFAULT_POFFSET_UR;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_POFFSET_UR, &setup, 1);
	setup = DEFAULT_POFFSET_DL;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_ENABLE, &setup, 1);
	setup = DEFAULT_ATIME;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG1, &setup, 1);
	setup = DEFAULT_PERS;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_PERS, &setup, 1);
	setup = DEFAULT_CONFIG2;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG2, &setup, 1);
	setup = DEFAULT_CONFIG3;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG3, &setup, 1);
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_ATIME, &setup, 1);
	setup = DEFAULT_WTIME;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_WTIME, &setup, 1);
	setup = DEFAULT_PROX_PPULSE;
	
	
}

void USART_placingstring(char *StringPtr)
{
	while ((*StringPtr != '\0')){ //poll for string inputs
		while (!(UCSR0A & (1 << UDRE0))); // continue polling 
		UDR0 = *StringPtr; // UDR0 register taking a value
		StringPtr++; // receiving characters
	}
}

void UART_init(void)
{
	//baud rate
	uint16_t baud_rate = UBRREQ;
	UBRR0H = baud_rate >> 8;
	UBRR0L = baud_rate & 0xFF;
	
	// use a stop bit 
	UCSR0C = (3 <<UCSZ00);
	
	//receiver,  transmitter enable
	UCSR0B = ( 1 <<RXEN0)|( 1 <<TXEN0);
}

int uart_placingchar(char c, FILE *stream)
{
	//poll until empty buffer
	while ( !( UCSR0A & ( 1 <<UDRE0)) );
	//storing data to buffer
	UDR0 = c;
	return 0;
}
